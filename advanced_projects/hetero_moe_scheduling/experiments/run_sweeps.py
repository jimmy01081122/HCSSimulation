import os
import sys
import copy
import csv
import random
import numpy as np

# 確保可以 import 我們的模擬器
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))

from simulator.hardware_model import SystemConfig
from simulator.run_hmcs import HMCSimulator, generate_workload

def run_experiment_sweeps():
    print("=== 開始 Design Space Exploration (DSE) 參數掃描 ===")
    
    # 建立 reports 目錄
    os.makedirs("/work/advanced_projects/hetero_moe_scheduling/reports", exist_ok=True)
    csv_file_path = "/work/advanced_projects/hetero_moe_scheduling/reports/sweep_results.csv"

    # 基礎參數配置
    base_config = {
        "num_experts": 16,
        "expert_size_gb": 1.0,
        "pcie_bandwidth_gbs": 16.0,
        "gpu_cache_capacity": 4,
        "cpu_expert_latency_ms": 4.0,
        "gpu_expert_latency_ms": 0.2,
        "gpu_dense_latency_ms": 0.5,
        "cpu_orch_latency_ms": 0.1
    }

    # 隨機生成一個基礎工作負載並進行深拷貝 (Deepcopy)，確保每個實驗使用完全相同的輸入
    random.seed(42)
    np.random.seed(42)
    workload_size = 15
    base_workload = generate_workload(
        num_requests=workload_size,
        arrival_rate=0.15,
        num_experts=base_config["num_experts"],
        tokens_range=(16, 48),
        layers=6,
        zipf_alpha=1.2
    )

    # 定義要掃描的變量與範圍
    cache_capacities = [2, 4, 6, 8, 12, 16]
    pcie_bandwidths = [8.0, 16.0, 32.0, 64.0]
    
    # 我們要測試的 5 種軟硬體調度策略組合 (新增論文優化方案)
    strategies = [
        ("fcfs", "transfer", "FCFS_TRANSFER"),
        ("elas", "transfer", "ELAS_TRANSFER"),
        ("elas", "offload", "ELAS_OFFLOAD"),
        ("elas", "dcmd", "ELAS_DCMD_CO_DESIGN"),
        ("f-elas", "dcmd", "F_ELAS_DCMD_OPT")
    ]

    results = []

    # 1. 掃描 Cache Capacity
    print("\n--- 1. 掃描 GPU Expert Cache 容量 ---")
    for cap in cache_capacities:
        for sched, offload, name in strategies:
            # 建立 configuration
            cfg = SystemConfig(
                num_experts=base_config["num_experts"],
                expert_size_gb=base_config["expert_size_gb"],
                pcie_bandwidth_gbs=base_config["pcie_bandwidth_gbs"],
                gpu_cache_capacity=cap,
                cpu_expert_latency_ms=base_config["cpu_expert_latency_ms"],
                gpu_expert_latency_ms=base_config["gpu_expert_latency_ms"],
                gpu_dense_latency_ms=base_config["gpu_dense_latency_ms"],
                cpu_orch_latency_ms=base_config["cpu_orch_latency_ms"]
            )
            # 複製 workload
            reqs = copy.deepcopy(base_workload)
            sim = HMCSimulator(cfg, reqs, scheduling_policy=sched, offloading_policy=offload, cache_policy="lru")
            res = sim.run(dt=0.1)
            
            res_row = {
                "sweep_type": "cache_capacity",
                "cache_capacity": cap,
                "pcie_bandwidth_gbs": base_config["pcie_bandwidth_gbs"],
                "strategy": name,
                "total_sim_time_ms": res["total_sim_time_ms"],
                "avg_request_latency_ms": res["avg_request_latency_ms"],
                "gpu_hit_rate": res["gpu_hit_rate"],
                "cpu_offloads": res["total_cpu_offloads"],
                "pcie_transfers": res["pcie_transfer_count"],
                "total_pcie_wait_time_ms": res["total_pcie_wait_time_ms"]
            }
            results.append(res_row)
            print(f"Cap={cap:<2} | Strategy={name:<22} | SimTime={res['total_sim_time_ms']:.1f}ms | HitRate={res['gpu_hit_rate']*100:.1f}% | Offloads={res['total_cpu_offloads']}")

    # 2. 掃描 PCIe Bandwidth
    print("\n--- 2. 掃描 PCIe 傳輸頻寬 ---")
    for bw in pcie_bandwidths:
        for sched, offload, name in strategies:
            cfg = SystemConfig(
                num_experts=base_config["num_experts"],
                expert_size_gb=base_config["expert_size_gb"],
                pcie_bandwidth_gbs=bw,
                gpu_cache_capacity=base_config["gpu_cache_capacity"],
                cpu_expert_latency_ms=base_config["cpu_expert_latency_ms"],
                gpu_expert_latency_ms=base_config["gpu_expert_latency_ms"],
                gpu_dense_latency_ms=base_config["gpu_dense_latency_ms"],
                cpu_orch_latency_ms=base_config["cpu_orch_latency_ms"]
            )
            reqs = copy.deepcopy(base_workload)
            sim = HMCSimulator(cfg, reqs, scheduling_policy=sched, offloading_policy=offload, cache_policy="lru")
            res = sim.run(dt=0.1)
            
            res_row = {
                "sweep_type": "pcie_bandwidth",
                "cache_capacity": base_config["gpu_cache_capacity"],
                "pcie_bandwidth_gbs": bw,
                "strategy": name,
                "total_sim_time_ms": res["total_sim_time_ms"],
                "avg_request_latency_ms": res["avg_request_latency_ms"],
                "gpu_hit_rate": res["gpu_hit_rate"],
                "cpu_offloads": res["total_cpu_offloads"],
                "pcie_transfers": res["pcie_transfer_count"],
                "total_pcie_wait_time_ms": res["total_pcie_wait_time_ms"]
            }
            results.append(res_row)
            print(f"BW={bw:<4} | Strategy={name:<22} | SimTime={res['total_sim_time_ms']:.1f}ms | HitRate={res['gpu_hit_rate']*100:.1f}% | Offloads={res['total_cpu_offloads']}")

    # 寫入 CSV 檔案
    headers = ["sweep_type", "cache_capacity", "pcie_bandwidth_gbs", "strategy", "total_sim_time_ms", "avg_request_latency_ms", "gpu_hit_rate", "cpu_offloads", "pcie_transfers", "total_pcie_wait_time_ms"]
    with open(csv_file_path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=headers)
        writer.writeheader()
        writer.writerows(results)
    
    print(f"\n[SUCCESS] 掃描實驗完成！結果已寫入至 {csv_file_path}")

if __name__ == "__main__":
    run_experiment_sweeps()
