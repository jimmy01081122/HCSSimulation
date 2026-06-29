class DecisionEngine:
    def __init__(self, config):
        self.config = config

    def decide(self, expert_id, num_tokens, pcie_bus, cpu_queue_len=0, policy="dcmd"):
        """
        決定對於 missed expert_id，要使用 TRANSFER 還是 OFFLOAD。
        回傳值: "TRANSFER" 或 "OFFLOAD"
        """
        if policy == "transfer":
            return "TRANSFER"
        elif policy == "offload":
            return "OFFLOAD"
        elif policy == "dcmd":
            # 1. 估計 PCIe 傳輸與排隊成本
            est_pcie_wait = pcie_bus.get_estimated_delay(expert_id)
            cost_transfer = est_pcie_wait + (num_tokens * self.config.gpu_expert_latency_ms)

            # 2. 估計 CPU 計算成本 (考慮核心數限制下的排隊爭用)
            cpu_queue_delay = (cpu_queue_len / self.config.cpu_cores) * self.config.cpu_expert_latency_ms
            cost_offload = cpu_queue_delay + (num_tokens * self.config.cpu_expert_latency_ms) + self.config.cpu_transfer_latency_ms

            if cost_transfer < cost_offload:
                return "TRANSFER"
            else:
                return "OFFLOAD"
        else:
            return "TRANSFER"
