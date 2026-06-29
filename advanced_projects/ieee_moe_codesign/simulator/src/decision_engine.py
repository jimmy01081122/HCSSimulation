# decision_engine.py
# Dynamic Cost-Model Decision (DCMD) engine for MoE expert execution offloading.
# Analyzes queues and latency costs to decide between PCIe transfer and CPU offloading.

class DecisionEngine:
    def __init__(self, config):
        self.config = config

    def decide(self, expert_id, num_tokens, pcie_bus, current_time, cpu_queue_len=0, policy="dcmd"):
        """
        Decides whether to TRANSFER the expert to GPU or OFFLOAD execution to CPU.
        Returns "TRANSFER" or "OFFLOAD".
        """
        if policy == "transfer":
            return "TRANSFER"
        elif policy == "offload":
            return "OFFLOAD"
        elif policy == "dcmd":
            # 1. Estimate PCIe transfer latency under bandwidth sharing
            est_pcie_wait = pcie_bus.get_estimated_delay(expert_id, current_time)
            cost_transfer = est_pcie_wait + (num_tokens * self.config.gpu_expert_latency_ms)

            # 2. Estimate CPU execution latency under core pool contention
            # Waiting delay represents queueing delay for available cores
            cpu_queue_delay = (cpu_queue_len / self.config.cpu_cores) * self.config.cpu_expert_latency_ms
            cost_offload = cpu_queue_delay + (num_tokens * self.config.cpu_expert_latency_ms) + self.config.cpu_transfer_latency_ms

            if cost_transfer <= cost_offload:
                return "TRANSFER"
            else:
                return "OFFLOAD"
        
        return "TRANSFER"
