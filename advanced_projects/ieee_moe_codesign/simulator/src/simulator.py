# simulator.py
# Discrete-event simulation engine for heterogeneous MoE inference co-design.
# Models system state transitions, event queues, and performance metrics.

import heapq
import numpy as np
from hardware_model import SystemConfig, PCIeBus
from cache import GPUExpertCache
from decision_engine import DecisionEngine
from scheduler import MoEScheduler
from workload_gen import WorkloadGenerator

class Event:
    def __init__(self, time, event_type, request=None, data=None):
        self.time = time
        self.event_type = event_type
        self.request = request
        self.data = data
        self.cancelled = False

    def __lt__(self, other):
        return self.time < other.time


class HMCSimulator:
    def __init__(self, 
                 gpu_cache_capacity=4,
                 pcie_bandwidth_gbs=16.0,
                 expert_size_gb=1.0,
                 cpu_expert_latency_ms=4.0,
                 gpu_expert_latency_ms=0.2,
                 gpu_dense_latency_ms=0.5,
                 cpu_orch_latency_ms=0.1,
                 cpu_transfer_latency_ms=0.05,
                 pcie_dma_channels=2,
                 cpu_cores=8,
                 scheduling_policy="elas",
                 offloading_policy="dcmd",
                 cache_policy="lru",
                 starvation_threshold=5,
                 sched_epoch=0.1):
        
        self.config = SystemConfig(
            num_experts=16,
            expert_size_gb=expert_size_gb,
            pcie_bandwidth_gbs=pcie_bandwidth_gbs,
            gpu_cache_capacity=gpu_cache_capacity,
            cpu_expert_latency_ms=cpu_expert_latency_ms,
            gpu_expert_latency_ms=gpu_expert_latency_ms,
            gpu_dense_latency_ms=gpu_dense_latency_ms,
            cpu_orch_latency_ms=cpu_orch_latency_ms,
            cpu_transfer_latency_ms=cpu_transfer_latency_ms,
            pcie_dma_channels=pcie_dma_channels,
            cpu_cores=cpu_cores
        )
        
        self.scheduling_policy = scheduling_policy
        self.offloading_policy = offloading_policy
        self.cache_policy = cache_policy
        self.sched_epoch = sched_epoch

        self.pcie_bus = PCIeBus(self.config)
        self.cache = GPUExpertCache(self.config)
        self.decision_engine = DecisionEngine(self.config)
        self.scheduler = MoEScheduler(self.config, starvation_threshold)

        self.current_time = 0.0
        self.event_queue = []
        
        # State tracking
        self.active_requests = []
        self.completed_requests = []
        self.moe_pending_queue = []
        self.active_cpu_jobs = []
        self.cpu_waiting_queue = []
        
        self.active_pcie_event = None
        
        # Utilization tracking
        self.gpu_active_jobs = 0
        self.last_gpu_change_time = 0.0
        self.gpu_busy_ms = 0.0

        self.last_cpu_change_time = 0.0
        self.cpu_busy_core_ms = 0.0

        # Statistics
        self.stats_gpu_hits = 0
        self.stats_gpu_misses = 0
        self.stats_cpu_offloads = 0
        self.stats_pcie_transfers = 0

    def _update_gpu_utilization(self):
        dt = self.current_time - self.last_gpu_change_time
        if dt > 0:
            if self.gpu_active_jobs > 0:
                self.gpu_busy_ms += dt
        self.last_gpu_change_time = self.current_time

    def _update_cpu_utilization(self):
        dt = self.current_time - self.last_cpu_change_time
        if dt > 0:
            num_active = len(self.active_cpu_jobs)
            self.cpu_busy_core_ms += dt * num_active
        self.last_cpu_change_time = self.current_time

    def schedule_event(self, time, event_type, request=None, data=None):
        event = Event(time, event_type, request, data)
        heapq.heappush(self.event_queue, event)
        return event

    def reschedule_pcie_event(self):
        """Cancels existing PCIe completion event and schedules the next one."""
        if self.active_pcie_event is not None:
            self.active_pcie_event.cancelled = True
            self.active_pcie_event = None

        next_time = self.pcie_bus.get_next_completion_time(self.current_time)
        if next_time is not None:
            self.active_pcie_event = self.schedule_event(next_time, "PCIE_COMPLETION")

    def run(self, requests):
        self.current_time = 0.0
        self.event_queue = []
        self.completed_requests = []
        self.active_requests = requests
        
        # Schedule request arrivals
        for req in requests:
            self.schedule_event(req.arrival_time, "REQUEST_ARRIVAL", req)

        # Schedule periodic scheduling epochs if enabled
        if self.sched_epoch > 0:
            self.schedule_event(self.sched_epoch, "SCHEDULER_EPOCH")

        # Event processing loop
        while self.event_queue:
            if len(self.completed_requests) == len(self.active_requests):
                break

            event = heapq.heappop(self.event_queue)
            if event.cancelled:
                continue

            self.current_time = event.time
            self._process_event(event)

        # Finalize utilization stats at simulation end
        self._update_gpu_utilization()
        self._update_cpu_utilization()

        return self.collect_statistics()

    def _process_event(self, event):
        req = event.request

        if event.event_type == "REQUEST_ARRIVAL":
            req.status = "ORCHESTRATING"
            req.start_processing_time = self.current_time
            self.schedule_event(self.current_time + self.config.cpu_orch_latency_ms, "CPU_ORCH_DONE", req)

        elif event.event_type == "CPU_ORCH_DONE":
            req.total_cpu_time += self.config.cpu_orch_latency_ms
            req.status = "GPU_DENSE"
            self._update_gpu_utilization()
            self.gpu_active_jobs += 1
            self.schedule_event(self.current_time + self.config.gpu_dense_latency_ms, "GPU_DENSE_DONE", req)

        elif event.event_type == "GPU_DENSE_DONE":
            self._update_gpu_utilization()
            self.gpu_active_jobs -= 1
            req.total_gpu_time += self.config.gpu_dense_latency_ms
            req.status = "MOE_PENDING"
            req.current_expert_needed = req.get_current_expert_demand()
            
            self.moe_pending_queue.append(req)
            
            # If immediate scheduling is configured, trigger it now
            if self.sched_epoch <= 0:
                self._dispatch_scheduler()

        elif event.event_type == "SCHEDULER_EPOCH":
            self._dispatch_scheduler()
            self.schedule_event(self.current_time + self.sched_epoch, "SCHEDULER_EPOCH")

        elif event.event_type == "PCIE_COMPLETION":
            self.active_pcie_event = None
            callbacks = self.pcie_bus.complete_next_transfer(self.current_time)
            
            for cb in callbacks:
                cb()
                
            self.reschedule_pcie_event()

        elif event.event_type == "GPU_MOE_DONE":
            self._update_gpu_utilization()
            self.gpu_active_jobs -= 1
            req.total_gpu_time += self.config.gpu_expert_latency_ms
            
            advanced = req.advance_pipeline()
            if not advanced:
                req.finish_time = self.current_time
                self.completed_requests.append(req)
            else:
                self.schedule_event(self.current_time, "REQUEST_ARRIVAL", req)

        elif event.event_type == "CPU_MOE_DONE":
            self._update_cpu_utilization()
            self.active_cpu_jobs.remove(req)
            req.total_cpu_time += self.config.cpu_expert_latency_ms
            
            # Start transfer of results back to main path
            self.schedule_event(self.current_time + self.config.cpu_transfer_latency_ms, "CPU_TRANSFER_DONE", req)

            # Process next CPU job if any
            if self.cpu_waiting_queue:
                next_req = self.cpu_waiting_queue.pop(0)
                self.active_cpu_jobs.append(next_req)
                next_req.status = "CPU_MOE_EXECUTING"
                self.schedule_event(self.current_time + self.config.cpu_expert_latency_ms, "CPU_MOE_DONE", next_req)
            self._update_cpu_utilization()

        elif event.event_type == "CPU_TRANSFER_DONE":
            advanced = req.advance_pipeline()
            if not advanced:
                req.finish_time = self.current_time
                self.completed_requests.append(req)
            else:
                self.schedule_event(self.current_time, "REQUEST_ARRIVAL", req)

    def _dispatch_scheduler(self):
        """Runs the scheduling algorithm and dispatches pending MoE streams."""
        if not self.moe_pending_queue:
            return

        # Sort based on policy
        scheduled_streams = self.scheduler.schedule(self.moe_pending_queue, self.cache, self.scheduling_policy)
        
        remaining_pending = []
        
        for req in scheduled_streams:
            expert_id = req.current_expert_needed
            
            # Case 1: Cache Hit
            if self.cache.is_hit(expert_id):
                self.moe_pending_queue.remove(req)
                req.status = "GPU_MOE_EXECUTING"
                req.waiting_ticks = 0
                self.cache.touch(expert_id, self.current_time)
                req.gpu_hit_count += 1
                self.stats_gpu_hits += 1
                
                self._update_gpu_utilization()
                self.gpu_active_jobs += 1
                self.schedule_event(self.current_time + self.config.gpu_expert_latency_ms, "GPU_MOE_DONE", req)
                
            # Case 2: Cache Miss but already in flight
            elif self.cache.is_in_flight(expert_id):
                self.moe_pending_queue.remove(req)
                req.status = "MOE_TRANSFERRING"
                req.waiting_ticks = 0
                req.gpu_miss_count += 1
                self.stats_gpu_misses += 1
                
                # Register callback for when the existing transfer completes
                def make_callback(target_req):
                    def cb():
                        self.cache.insert(expert_id, self.cache_policy, self.current_time)
                        target_req.status = "GPU_MOE_EXECUTING"
                        self._update_gpu_utilization()
                        self.gpu_active_jobs += 1
                        self.schedule_event(self.current_time + self.config.gpu_expert_latency_ms, "GPU_MOE_DONE", target_req)
                    return cb
                
                self.pcie_bus.request_transfer(expert_id, self.current_time, make_callback(req))
                self.reschedule_pcie_event()

            # Case 3: Cache Miss and not in flight
            else:
                # Get the number of concurrent tokens waiting for this expert
                same_expert_tokens = sum(1 for r in self.moe_pending_queue if r.current_expert_needed == expert_id)
                
                decision = self.decision_engine.decide(
                    expert_id, 
                    same_expert_tokens, 
                    self.pcie_bus, 
                    self.current_time,
                    cpu_queue_len=len(self.cpu_waiting_queue),
                    policy=self.offloading_policy
                )

                if decision == "TRANSFER":
                    # Check if we should delay it to batch or if we can start transfer immediately
                    has_idle_channel = len(self.pcie_bus.active_transfers) < self.config.pcie_dma_channels
                    is_starving = req.waiting_ticks >= self.scheduler.starvation_threshold

                    # Under F-ELAS / ELAS, we hold back transfers if all channels are busy to allow batching,
                    # unless starvation threshold is reached or policy is FCFS.
                    if has_idle_channel or is_starving or self.scheduling_policy == "fcfs":
                        self.moe_pending_queue.remove(req)
                        req.status = "MOE_TRANSFERRING"
                        req.waiting_ticks = 0
                        req.gpu_miss_count += 1
                        self.stats_gpu_misses += 1
                        
                        def make_callback(target_req):
                            def cb():
                                self.cache.insert(expert_id, self.cache_policy, self.current_time)
                                target_req.status = "GPU_MOE_EXECUTING"
                                self._update_gpu_utilization()
                                self.gpu_active_jobs += 1
                                self.schedule_event(self.current_time + self.config.gpu_expert_latency_ms, "GPU_MOE_DONE", target_req)
                            return cb
                        
                        self.cache.start_transfer(expert_id)
                        self.stats_pcie_transfers += 1
                        self.pcie_bus.request_transfer(expert_id, self.current_time, make_callback(req))
                        self.reschedule_pcie_event()
                    else:
                        # Delay dispatching to keep it in the queue for batching
                        remaining_pending.append(req)
                else:
                    # CPU Offload execution
                    self.moe_pending_queue.remove(req)
                    req.status = "CPU_MOE_EXECUTING"
                    req.waiting_ticks = 0
                    req.cpu_offload_count += 1
                    self.stats_cpu_offloads += 1
                    req.gpu_miss_count += 1
                    self.stats_gpu_misses += 1
                    
                    self._update_cpu_utilization()
                    if len(self.active_cpu_jobs) < self.config.cpu_cores:
                        self.active_cpu_jobs.append(req)
                        self.schedule_event(self.current_time + self.config.cpu_expert_latency_ms, "CPU_MOE_DONE", req)
                    else:
                        self.cpu_waiting_queue.append(req)
                    self._update_cpu_utilization()

        # Update waiting ticks for streams that were delayed/bypassed
        for req in self.moe_pending_queue:
            req.waiting_ticks += 1

    def collect_statistics(self):
        total_latencies = [r.finish_time - r.arrival_time for r in self.completed_requests]
        
        avg_latency = np.mean(total_latencies) if total_latencies else 0.0
        max_latency = np.max(total_latencies) if total_latencies else 0.0
        p50_latency = np.percentile(total_latencies, 50) if total_latencies else 0.0
        p99_latency = np.percentile(total_latencies, 99) if total_latencies else 0.0
        
        total_misses = self.stats_gpu_hits + self.stats_gpu_misses
        gpu_hit_rate = (self.stats_gpu_hits / total_misses) if total_misses > 0 else 0.0
        
        total_sim_time = self.current_time
        
        # Utilization calculation
        gpu_utilization = (self.gpu_busy_ms / total_sim_time) if total_sim_time > 0 else 0.0
        cpu_utilization = (self.cpu_busy_core_ms / (self.config.cpu_cores * total_sim_time)) if total_sim_time > 0 else 0.0

        return {
            "scheduling_policy": self.scheduling_policy,
            "offloading_policy": self.offloading_policy,
            "cache_policy": self.cache_policy,
            "total_sim_time_ms": total_sim_time,
            "avg_request_latency_ms": avg_latency,
            "p50_request_latency_ms": p50_latency,
            "p99_request_latency_ms": p99_latency,
            "max_request_latency_ms": max_latency,
            "gpu_hit_rate": gpu_hit_rate,
            "total_gpu_hits": self.stats_gpu_hits,
            "total_gpu_misses": self.stats_gpu_misses,
            "total_cpu_offloads": self.stats_cpu_offloads,
            "pcie_transfer_count": self.stats_pcie_transfers,
            "gpu_utilization": gpu_utilization,
            "cpu_utilization": cpu_utilization
        }
