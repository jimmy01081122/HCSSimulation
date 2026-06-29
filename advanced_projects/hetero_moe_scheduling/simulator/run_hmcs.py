import sys
import os
import random
import numpy as np
import yaml

# 確保可以 import 同目錄下的 module
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from hardware_model import SystemConfig, PCIeBus
from cache import GPUExpertCache
from decision_engine import DecisionEngine
from scheduler import MoEScheduler

class Request:
    def __init__(self, request_id, arrival_time, total_tokens, total_layers, num_experts, zipf_alpha=1.2):
        self.request_id = request_id
        self.arrival_time = arrival_time
        self.total_tokens = total_tokens
        self.total_layers = total_layers
        self.current_token_idx = 0
        self.current_layer = 0
        
        # 狀態機: "QUEUED", "ORCHESTRATING", "GPU_DENSE", "MOE_PENDING", "MOE_TRANSFERRING", "CPU_MOE_EXECUTING", "GPU_MOE_EXECUTING", "FINISHED"
        self.status = "QUEUED"
        
        # 用於追蹤當前步驟剩餘的時間
        self.remaining_step_time = 0.0
        self.current_expert_needed = None
        
        # 預先生成此 Request 所有 token 各層需要的 expert 需求
        # 使用 Zipf 分布模擬專家選擇的局部性
        self.expert_demands = []
        for _ in range(total_tokens):
            token_demands = []
            for _ in range(total_layers):
                val = np.random.zipf(zipf_alpha)
                expert_id = (val - 1) % num_experts
                token_demands.append(int(expert_id))
            self.expert_demands.append(token_demands)

        # 效能統計
        self.start_processing_time = None
        self.finish_time = None
        self.total_gpu_time = 0.0
        self.total_cpu_time = 0.0
        self.total_pcie_wait_time = 0.0
        self.cpu_offload_count = 0
        self.gpu_hit_count = 0
        self.gpu_miss_count = 0
        self.waiting_ticks = 0 # 飢餓調度計數

    def get_current_expert_demand(self):
        if self.current_token_idx < self.total_tokens and self.current_layer < self.total_layers:
            return self.expert_demands[self.current_token_idx][self.current_layer]
        return None

    def advance_pipeline(self):
        """
        前進到下一個階段。
        """
        if self.current_layer < self.total_layers - 1:
            self.current_layer += 1
            self.status = "ORCHESTRATING"
            self.remaining_step_time = 0.0
        else:
            if self.current_token_idx < self.total_tokens - 1:
                self.current_token_idx += 1
                self.current_layer = 0
                self.status = "ORCHESTRATING"
                self.remaining_step_time = 0.0
            else:
                self.status = "FINISHED"


class HMCSimulator:
    def __init__(self, config, requests, scheduling_policy="elas", offloading_policy="dcmd", cache_policy="lru"):
        self.config = config
        self.requests = requests
        self.scheduling_policy = scheduling_policy
        self.offloading_policy = offloading_policy
        self.cache_policy = cache_policy

        self.pcie_bus = PCIeBus(config)
        self.cache = GPUExpertCache(config)
        self.decision_engine = DecisionEngine(config)
        self.scheduler = MoEScheduler(config)

        self.current_time = 0.0 # ms
        self.active_queue = []  # 當前正在處理的 requests
        self.completed_requests = []
        self.cpu_exec_queue = [] # CPU 執行佇列 (論文優化，模擬 CPU 核心數限制下的排隊)

        # 系統級統計暫存器
        self.stats_total_requests = len(requests)
        self.stats_cpu_offloads = 0
        self.stats_gpu_hits = 0
        self.stats_gpu_misses = 0
        self.stats_total_transfer_count = 0

    def is_finished(self):
        return len(self.completed_requests) == self.stats_total_requests

    def run(self, dt=0.1):
        """
        運行模擬，直到所有 request 完成。
        dt 為模擬步長 (ms)。
        """
        pending_requests = sorted(self.requests, key=lambda r: r.arrival_time)

        while not self.is_finished():
            # 1. 檢查是否有新 request 抵達並加入佇列
            while pending_requests and pending_requests[0].arrival_time <= self.current_time:
                req = pending_requests.pop(0)
                req.status = "QUEUED"
                req.waiting_ticks = 0
                self.active_queue.append(req)

            # 2. 驅動 PCIe 傳輸
            self.pcie_bus.tick(dt)

            # 3. 處理 CPU Orchestration (例如 scheduling / tool call / queueing overhead)
            for req in self.active_queue:
                if req.status == "QUEUED":
                    req.status = "ORCHESTRATING"
                    req.remaining_step_time = self.config.cpu_orch_latency_ms
                    if req.start_processing_time is None:
                        req.start_processing_time = self.current_time
                
                if req.status == "ORCHESTRATING":
                    req.remaining_step_time -= dt
                    req.total_cpu_time += dt
                    if req.remaining_step_time <= 0.0:
                        req.status = "GPU_DENSE"
                        req.remaining_step_time = self.config.gpu_dense_latency_ms

            # 4. 處理 GPU Dense Layers 的執行
            for req in self.active_queue:
                if req.status == "GPU_DENSE":
                    req.remaining_step_time -= dt
                    req.total_gpu_time += dt
                    if req.remaining_step_time <= 0.0:
                        req.status = "MOE_PENDING"
                        req.current_expert_needed = req.get_current_expert_demand()

            # 5. 處理 MoE 階段：調度與決策
            moe_pending_streams = [r for r in self.active_queue if r.status == "MOE_PENDING"]
            if moe_pending_streams:
                scheduled_streams = self.scheduler.schedule(moe_pending_streams, self.cache, self.scheduling_policy)
                
                for req in scheduled_streams:
                    expert_id = req.current_expert_needed
                    
                    if self.cache.is_hit(expert_id):
                        req.status = "GPU_MOE_EXECUTING"
                        req.remaining_step_time = self.config.gpu_expert_latency_ms
                        req.waiting_ticks = 0
                        self.cache.touch(expert_id)
                        req.gpu_hit_count += 1
                        self.stats_gpu_hits += 1
                    else:
                        req.gpu_miss_count += 1
                        self.stats_gpu_misses += 1
                        
                        same_expert_tokens = sum(1 for r in moe_pending_streams if r.current_expert_needed == expert_id)
                        
                        # 決策時考慮當前 CPU 佇列長度，精準建模
                        decision = self.decision_engine.decide(
                            expert_id, 
                            same_expert_tokens, 
                            self.pcie_bus, 
                            cpu_queue_len=len(self.cpu_exec_queue), 
                            policy=self.offloading_policy
                        )
                        
                        if decision == "TRANSFER":
                            req.status = "MOE_TRANSFERRING"
                            req.waiting_ticks = 0
                            
                            def make_callback(target_req):
                                def callback(exp_id):
                                    evicted = self.cache.insert(exp_id, self.cache_policy)
                                    target_req.status = "GPU_MOE_EXECUTING"
                                    target_req.remaining_step_time = self.config.gpu_expert_latency_ms
                                    target_req.waiting_ticks = 0
                                return callback
                            
                            if not self.cache.is_in_flight(expert_id):
                                self.cache.start_transfer(expert_id)
                                self.stats_total_transfer_count += 1
                            
                            self.pcie_bus.request_transfer(expert_id, make_callback(req))
                        
                        else:
                            # 選擇 CPU 執行，加入 CPU 執行佇列
                            req.status = "CPU_MOE_EXECUTING"
                            req.remaining_step_time = self.config.cpu_expert_latency_ms
                            req.waiting_ticks = 0
                            req.cpu_offload_count += 1
                            self.stats_cpu_offloads += 1
                            if req not in self.cpu_exec_queue:
                                self.cpu_exec_queue.append(req)

            # 6. 處理 CPU / GPU Expert 計算的執行 (考慮並行度限制)
            # GPU 計算 (假設高度並行)
            for req in self.active_queue:
                if req.status == "GPU_MOE_EXECUTING":
                    req.remaining_step_time -= dt
                    req.total_gpu_time += dt
                    if req.remaining_step_time <= 0.0:
                        req.advance_pipeline()
                elif req.status == "MOE_TRANSFERRING":
                    req.total_pcie_wait_time += dt

            # CPU 計算 (模擬有限核心數 config.cpu_cores 並行處理)
            self.cpu_exec_queue = [r for r in self.cpu_exec_queue if r.status == "CPU_MOE_EXECUTING"]
            active_cpu_jobs = self.cpu_exec_queue[:self.config.cpu_cores]
            for req in active_cpu_jobs:
                req.remaining_step_time -= dt
                req.total_cpu_time += dt
                if req.remaining_step_time <= 0.0:
                    req.advance_pipeline()

            # 7. 清理並移除已完成的 Requests
            new_active_queue = []
            for req in self.active_queue:
                if req.status == "FINISHED":
                    req.finish_time = self.current_time
                    self.completed_requests.append(req)
                else:
                    new_active_queue.append(req)
            self.active_queue = new_active_queue

            # 8. 推進時間
            self.current_time += dt

        # 模擬結束，統計整體指標
        results = self.collect_statistics()
        return results

    def collect_statistics(self):
        total_latencies = [r.finish_time - r.arrival_time for r in self.completed_requests]
        avg_latency = np.mean(total_latencies) if total_latencies else 0.0
        max_latency = np.max(total_latencies) if total_latencies else 0.0
        
        gpu_hit_rate = (self.stats_gpu_hits / (self.stats_gpu_hits + self.stats_gpu_misses)) if (self.stats_gpu_hits + self.stats_gpu_misses) > 0 else 0.0
        
        total_cpu_exec_time = sum(r.total_cpu_time for r in self.completed_requests)
        total_gpu_exec_time = sum(r.total_gpu_time for r in self.completed_requests)
        total_pcie_wait_time = sum(r.total_pcie_wait_time for r in self.completed_requests)
        
        return {
            "scheduling_policy": self.scheduling_policy,
            "offloading_policy": self.offloading_policy,
            "cache_policy": self.cache_policy,
            "total_sim_time_ms": self.current_time,
            "avg_request_latency_ms": avg_latency,
            "max_request_latency_ms": max_latency,
            "gpu_hit_rate": gpu_hit_rate,
            "total_gpu_hits": self.stats_gpu_hits,
            "total_gpu_misses": self.stats_gpu_misses,
            "total_cpu_offloads": self.stats_cpu_offloads,
            "pcie_transfer_count": self.stats_total_transfer_count,
            "total_cpu_exec_time_ms": total_cpu_exec_time,
            "total_gpu_exec_time_ms": total_gpu_exec_time,
            "total_pcie_wait_time_ms": total_pcie_wait_time
        }


def generate_workload(num_requests=20, arrival_rate=0.2, num_experts=64, tokens_range=(32, 128), layers=8, zipf_alpha=1.2):
    requests = []
    current_arrival_time = 0.0
    for i in range(num_requests):
        interval = random.expovariate(arrival_rate)
        current_arrival_time += interval
        
        total_tokens = random.randint(tokens_range[0], tokens_range[1])
        req = Request(
            request_id=i,
            arrival_time=current_arrival_time,
            total_tokens=total_tokens,
            total_layers=layers,
            num_experts=num_experts,
            zipf_alpha=zipf_alpha
        )
        requests.append(req)
    return requests


if __name__ == "__main__":
    config = SystemConfig(
        num_experts=16,
        expert_size_gb=1.0,
        pcie_bandwidth_gbs=16.0, # Gen4 x16
        gpu_cache_capacity=4,
        cpu_expert_latency_ms=4.0,
        gpu_expert_latency_ms=0.2,
        gpu_dense_latency_ms=0.5,
        cpu_orch_latency_ms=0.1
    )

    print("--- 產生工作負載 ---")
    random.seed(42)
    np.random.seed(42)
    requests_pool = generate_workload(num_requests=10, arrival_rate=0.1, num_experts=16, tokens_range=(16, 32), layers=4, zipf_alpha=1.2)
    
    import copy
    
    policies = [
        ("fcfs", "transfer"), 
        ("elas", "transfer"), 
        ("elas", "offload"),  
        ("elas", "dcmd"),
        ("f-elas", "dcmd") # 我們的論文改良方案
    ]
    
    for sched, offload in policies:
        reqs = copy.deepcopy(requests_pool)
        sim = HMCSimulator(config, reqs, scheduling_policy=sched, offloading_policy=offload, cache_policy="lru")
        res = sim.run(dt=0.1)
        print(f"\n策略組: Scheduler={sched.upper()}, Offloader={offload.upper()}")
        print(f"  總執行時間: {res['total_sim_time_ms']:.2f} ms")
        print(f"  平均 Request 延遲: {res['avg_request_latency_ms']:.2f} ms")
        print(f"  GPU Expert Cache 命中率: {res['gpu_hit_rate']*100:.2f}% (Hits={res['total_gpu_hits']}, Misses={res['total_gpu_misses']})")
        print(f"  CPU Offload 次數: {res['total_cpu_offloads']}")
        print(f"  PCIe 權重傳輸次數: {res['pcie_transfer_count']}")
        print(f"  總 PCIe 等待時間: {res['total_pcie_wait_time_ms']:.2f} ms")
