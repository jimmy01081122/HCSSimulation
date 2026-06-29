# hardware_model.py
# System hardware modeling for heterogeneous MoE inference co-design.
# Models multi-channel PCIe bandwidth sharing and system configuration.

import math

class SystemConfig:
    def __init__(self, 
                 num_experts=16,
                 expert_size_gb=1.0,
                 pcie_bandwidth_gbs=16.0,
                 gpu_cache_capacity=4,
                 cpu_expert_latency_ms=4.0,
                 gpu_expert_latency_ms=0.2,
                 gpu_dense_latency_ms=0.5,
                 cpu_orch_latency_ms=0.1,
                 cpu_transfer_latency_ms=0.05,
                 pcie_dma_channels=2,
                 cpu_cores=8):
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
        
        # Base PCIe transfer latency for a single expert in isolation
        self.base_transfer_latency_ms = (self.expert_size_gb / self.pcie_bandwidth_gbs) * 1000.0


class PCIeTransfer:
    def __init__(self, expert_id, total_bytes_gb, start_time):
        self.expert_id = expert_id
        self.total_bytes_gb = total_bytes_gb
        self.remaining_bytes_gb = total_bytes_gb
        self.start_time = start_time
        self.last_update_time = start_time
        self.callbacks = []


class PCIeBus:
    def __init__(self, config):
        self.config = config
        self.active_transfers = []  # List of PCIeTransfer
        self.queue = []             # List of PCIeTransfer
        self.last_update_time = 0.0
        
        # Statistics
        self.stats_total_transfer_time = 0.0
        self.stats_transfer_count = 0

    def update_progress(self, current_time):
        """Updates the progress of all active transfers up to current_time."""
        dt = current_time - self.last_update_time
        if dt <= 0:
            self.last_update_time = current_time
            return

        num_active = len(self.active_transfers)
        if num_active > 0:
            # Bandwidth sharing: total bandwidth is shared equally among active transfers
            shared_bw_gb_ms = (self.config.pcie_bandwidth_gbs / num_active) * 0.001
            for transfer in self.active_transfers:
                transfer.remaining_bytes_gb -= dt * shared_bw_gb_ms
                if transfer.remaining_bytes_gb < 0.0:
                    transfer.remaining_bytes_gb = 0.0
            self.stats_total_transfer_time += dt

        self.last_update_time = current_time

    def request_transfer(self, expert_id, current_time, callback):
        """Requests an expert transfer. Returns True if started immediately, False if queued."""
        self.update_progress(current_time)
        
        # Check if already active
        for transfer in self.active_transfers:
            if transfer.expert_id == expert_id:
                transfer.callbacks.append(callback)
                return True

        # Check if already queued
        for transfer in self.queue:
            if transfer.expert_id == expert_id:
                transfer.callbacks.append(callback)
                return False

        new_transfer = PCIeTransfer(expert_id, self.config.expert_size_gb, current_time)
        new_transfer.callbacks.append(callback)

        if len(self.active_transfers) < self.config.pcie_dma_channels:
            self.active_transfers.append(new_transfer)
            self.stats_transfer_count += 1
            return True
        else:
            self.queue.append(new_transfer)
            return False

    def get_next_completion_time(self, current_time):
        """Returns the absolute time of the next transfer completion, or None."""
        self.update_progress(current_time)
        num_active = len(self.active_transfers)
        if num_active == 0:
            return None

        shared_bw_gb_ms = (self.config.pcie_bandwidth_gbs / num_active) * 0.001
        min_time_to_completion = float('inf')
        for transfer in self.active_transfers:
            time_left = transfer.remaining_bytes_gb / shared_bw_gb_ms
            if time_left < min_time_to_completion:
                min_time_to_completion = time_left

        return current_time + min_time_to_completion

    def complete_next_transfer(self, current_time):
        """Completes the transfer(s) that finished at current_time. Returns list of callbacks."""
        self.update_progress(current_time)
        
        finished = []
        remaining = []
        for transfer in self.active_transfers:
            # Use small epsilon to prevent floating point inaccuracies
            if transfer.remaining_bytes_gb <= 1e-9:
                finished.append(transfer)
            else:
                remaining.append(transfer)

        self.active_transfers = remaining

        callbacks_to_trigger = []
        for transfer in finished:
            callbacks_to_trigger.extend(transfer.callbacks)

        # Fill active transfers from queue
        while len(self.active_transfers) < self.config.pcie_dma_channels and self.queue:
            next_transfer = self.queue.pop(0)
            next_transfer.start_time = current_time
            next_transfer.last_update_time = current_time
            self.active_transfers.append(next_transfer)
            self.stats_transfer_count += 1

        return callbacks_to_trigger

    def get_estimated_delay(self, expert_id, current_time):
        """Estimates the completion delay of an expert_id from current_time."""
        self.update_progress(current_time)
        num_active = len(self.active_transfers)
        
        # If already active
        for transfer in self.active_transfers:
            if transfer.expert_id == expert_id:
                # Under current share
                shared_bw_gb_ms = (self.config.pcie_bandwidth_gbs / num_active) * 0.001
                return transfer.remaining_bytes_gb / shared_bw_gb_ms

        # Position in queue
        queue_idx = -1
        for idx, transfer in enumerate(self.queue):
            if transfer.expert_id == expert_id:
                queue_idx = idx
                break

        if queue_idx == -1:
            queue_idx = len(self.queue)

        # Estimate remaining time of active transfers
        if num_active == 0:
            return self.config.base_transfer_latency_ms

        shared_bw_gb_ms = (self.config.pcie_bandwidth_gbs / num_active) * 0.001
        avg_active_remain = sum(t.remaining_bytes_gb for t in self.active_transfers) / num_active / shared_bw_gb_ms
        
        # Queue delay calculation assuming full occupancy
        queue_delay = (queue_idx / self.config.pcie_dma_channels) * self.config.base_transfer_latency_ms
        return avg_active_remain + queue_delay
