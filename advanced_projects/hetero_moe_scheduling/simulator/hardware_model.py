import math

class SystemConfig:
    def __init__(self, 
                 num_experts=64,
                 expert_size_gb=2.0,      # 每個 expert 的權重大小 (GB)
                 pcie_bandwidth_gbs=16.0, # PCIe 頻寬 (GB/s)，如 PCIe 4.0 x16
                 gpu_cache_capacity=8,   # GPU Cache 能容納的 expert 數量
                 cpu_expert_latency_ms=5.0,# CPU 執行單個 expert token 的延遲 (ms)
                 gpu_expert_latency_ms=0.2,# GPU 執行單個 expert token 的延遲 (ms)
                 gpu_dense_latency_ms=0.5, # GPU 執行單個 token dense layers 的延遲 (ms)
                 cpu_orch_latency_ms=0.1,  # CPU 處理單個 request step 的 scheduling/queueing 延遲 (ms)
                 cpu_transfer_latency_ms=0.05, # CPU 執行時, token 傳輸回 CPU 的延遲 (ms)
                 pcie_dma_channels=2,     # 支援的並行 DMA 通道數 (論文優化)
                 cpu_cores=8              # 用於執行 missing experts 的 CPU 核心數 (論文優化)
                 ):
        self.num_experts = num_experts
        self.expert_size_gb = expert_size_gb
        self.pcie_bandwidth_gbs = pcie_bandwidth_gbs
        self.gpu_cache_capacity = gpu_cache_capacity
        self.cpu_expert_latency_ms = cpu_expert_latency_ms
        self.gpu_expert_latency_ms = gpu_expert_latency_ms
        self.gpu_dense_latency_ms = gpu_dense_latency_ms
        self.cpu_orch_latency_ms = cpu_orch_latency_ms
        self.cpu_transfer_latency_ms = cpu_transfer_latency_ms
        self.pcie_dma_channels = pcie_dma_channels
        self.cpu_cores = cpu_cores

        # 基礎 PCIe 傳輸一個 expert 的延遲 (ms)
        self.base_transfer_latency_ms = (self.expert_size_gb / self.pcie_bandwidth_gbs) * 1000.0


class PCIeBus:
    def __init__(self, config):
        self.config = config
        self.current_transfers = [] # List of dict: {"expert_id": int, "remaining_time": float, "callbacks": list}
        self.queue = []             # List of dict: {"expert_id": int, "callbacks": list}
        self.stats_total_transfer_time = 0.0
        self.stats_transfer_count = 0

    def request_transfer(self, expert_id, callback):
        # 檢查是否已經在傳輸或在佇列中
        for t in self.current_transfers:
            if t["expert_id"] == expert_id:
                t["callbacks"].append(callback)
                return
        for q in self.queue:
            if q["expert_id"] == expert_id:
                q["callbacks"].append(callback)
                return
        
        new_request = {"expert_id": expert_id, "callbacks": [callback]}
        # 檢查是否有空閒的 DMA 通道
        if len(self.current_transfers) < self.config.pcie_dma_channels:
            self.current_transfers.append({
                "expert_id": expert_id,
                "remaining_time": self.config.base_transfer_latency_ms,
                "callbacks": [callback]
            })
            self.stats_transfer_count += 1
        else:
            self.queue.append(new_request)

    def tick(self, dt):
        """推進模擬時間 dt (ms)。若有多個並行傳輸，它們會均分 PCIe 總頻寬。"""
        if not self.current_transfers:
            return

        num_active = len(self.current_transfers)
        # 均分頻寬：每個傳輸獲得的實際傳輸量進度為 dt / num_active
        effective_dt = dt / num_active
        self.stats_total_transfer_time += dt

        finished_indices = []
        for idx, t in enumerate(self.current_transfers):
            t["remaining_time"] -= effective_dt
            if t["remaining_time"] <= 0.0:
                finished_indices.append(idx)

        # 逆序移除完成的傳輸，以防索引混亂
        for idx in sorted(finished_indices, reverse=True):
            t = self.current_transfers.pop(idx)
            for cb in t["callbacks"]:
                cb(t["expert_id"])

        # 從佇列中補充新的傳輸直到通道滿
        while len(self.current_transfers) < self.config.pcie_dma_channels and self.queue:
            next_req = self.queue.pop(0)
            self.current_transfers.append({
                "expert_id": next_req["expert_id"],
                "remaining_time": self.config.base_transfer_latency_ms,
                "callbacks": next_req["callbacks"]
            })
            self.stats_transfer_count += 1

    def get_estimated_delay(self, expert_id):
        """估計載入該 expert 需要等待的總時間 (ms)，考慮通道並行與頻寬均分"""
        # 如果已經在傳輸中
        for t in self.current_transfers:
            if t["expert_id"] == expert_id:
                return t["remaining_time"] * len(self.current_transfers)
        
        # 粗略估算：排隊延遲
        num_active = len(self.current_transfers)
        if num_active < self.config.pcie_dma_channels:
            return self.config.base_transfer_latency_ms

        # 佇列中的位置估算
        queue_idx = 0
        found = False
        for idx, q in enumerate(self.queue):
            if q["expert_id"] == expert_id:
                queue_idx = idx
                found = True
                break

        if not found:
            queue_idx = len(self.queue)

        # 排隊延遲 = (當前運作的平均剩餘時間 + 佇列中在此之前的傳輸時間)
        avg_active_remain = sum(t["remaining_time"] for t in self.current_transfers) / num_active if num_active > 0 else 0
        return (avg_active_remain + (queue_idx / self.config.pcie_dma_channels) * self.config.base_transfer_latency_ms) * self.config.pcie_dma_channels

