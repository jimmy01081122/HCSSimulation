# moe_sim.py
# Discrete Event Simulator (DES) for MoE-HostAssist based on SimPy.
# Evaluates software-hardware co-design trade-offs in Mixture-of-Experts serving.

import simpy
import numpy as np
import pandas as pd
import random
import json
import os
from collections import deque, defaultdict
from enum import Enum

class CachePolicy(Enum):
    LRU = 1
    LFU = 2
    ARC = 3
    ORACLE = 4
    PREDICTION_GUIDED = 5

class SchedPolicy(Enum):
    FIFO = 1
    ROUND_ROBIN = 2
    GREEDY_SHORTEST = 3
    EDF = 4
    DACC = 5

class EvictionResult(Enum):
    HIT = 1
    MISS = 2

class Request:
    def __init__(self, request_id, arrival_time, prompt_tokens, gen_tokens, expert_sequence, is_agentic=False, deadline=None):
        self.request_id = request_id
        self.arrival_time = arrival_time
        self.prompt_tokens = prompt_tokens
        self.gen_tokens = gen_tokens
        self.total_tokens = prompt_tokens + gen_tokens
        self.expert_sequence = expert_sequence  # list of list of expert_ids per token
        self.is_agentic = is_agentic
        self.deadline = deadline if deadline is not None else (arrival_time + 500.0) # default deadline
        
        self.current_token_idx = 0
        self.start_time = None
        self.finish_time = None
        self.ttft = 0.0
        self.tpot = 0.0
        
        # Latency breakdown in milliseconds
        self.breakdown = {
            "host_scheduling_ms": 0.0,
            "expert_cache_miss_ms": 0.0,
            "activation_transfer_ms": 0.0,
            "queue_wait_ms": 0.0,
            "gpu_bubble_ms": 0.0,
            "cpu_stall_ms": 0.0,
            "gpu_exec_ms": 0.0,
            "cpu_exec_ms": 0.0
        }
        self.cache_events = [] # logs of cache hit/miss per token

class MarkovPredictor:
    """Order-3 Markov Chain predictor for expert accesses."""
    def __init__(self, n_experts):
        self.n_experts = n_experts
        self.history = deque(maxlen=3)
        self.transition_counts = defaultdict(lambda: defaultdict(int))
        
    def record(self, expert_id):
        if len(self.history) == 3:
            state = tuple(self.history)
            self.transition_counts[state][expert_id] += 1
        self.history.append(expert_id)
        
    def predict_next(self, top_n=2):
        if len(self.history) < 3:
            return []
        state = tuple(self.history)
        predictions = sorted(self.transition_counts[state].items(), key=lambda x: x[1], reverse=True)
        return [p[0] for p in predictions[:top_n]]

class ExpertCacheModel:
    def __init__(self, capacity, n_experts, policy=CachePolicy.LRU, oracle_sequence=None):
        self.capacity = capacity
        self.n_experts = n_experts
        self.policy = policy
        self.oracle_sequence = oracle_sequence or []
        
        # Cache state representation
        self.cache = set()
        self.recency = []  # used for LRU (most recent at end)
        self.frequency = defaultdict(int) # used for LFU
        
        # ARC (Adaptive Replacement Cache) parameters
        self.t1 = []
        self.t2 = []
        self.b1 = []
        self.b2 = []
        self.p = 0.0
        
        # Predictor for PREDICTION_GUIDED
        self.predictor = MarkovPredictor(n_experts)
        
        # Metrics tracking
        self.hits = 0
        self.misses = 0
        self.evictions = 0
        self.consecutive_misses = 0
        self.max_cascade_depth = 0

    def access(self, expert_id, timestamp, future_index=0) -> tuple:
        # Record frequency and notify Markov predictor
        self.frequency[expert_id] += 1
        self.predictor.record(expert_id)
        
        if expert_id in self.cache:
            self.hits += 1
            self.consecutive_misses = 0
            self._update_state_on_hit(expert_id)
            return (EvictionResult.HIT, None)
            
        self.misses += 1
        self.consecutive_misses += 1
        if self.consecutive_misses > self.max_cascade_depth:
            self.max_cascade_depth = self.consecutive_misses
            
        evicted = None
        if len(self.cache) >= self.capacity:
            evicted = self._evict(future_index)
            if evicted is not None:
                self.cache.remove(evicted)
                self.evictions += 1
                
        self.cache.add(expert_id)
        self._update_state_on_miss(expert_id)
        return (EvictionResult.MISS, evicted)

    def _update_state_on_hit(self, expert_id):
        if self.policy == CachePolicy.LRU or self.policy == CachePolicy.PREDICTION_GUIDED:
            if expert_id in self.recency:
                self.recency.remove(expert_id)
            self.recency.append(expert_id)
        elif self.policy == CachePolicy.ARC:
            if expert_id in self.t1:
                self.t1.remove(expert_id)
                self.t2.append(expert_id)
            elif expert_id in self.t2:
                self.t2.remove(expert_id)
                self.t2.append(expert_id)

    def _update_state_on_miss(self, expert_id):
        if self.policy == CachePolicy.LRU or self.policy == CachePolicy.PREDICTION_GUIDED:
            self.recency.append(expert_id)
        elif self.policy == CachePolicy.ARC:
            if expert_id in self.b1:
                # Adapt parameter p
                delta = 1.0 if len(self.b1) >= len(self.b2) else len(self.b2) / len(self.b1)
                self.p = min(self.p + delta, self.capacity)
                self.b1.remove(expert_id)
                self.t2.append(expert_id)
            elif expert_id in self.b2:
                # Adapt parameter p
                delta = 1.0 if len(self.b2) >= len(self.b1) else len(self.b1) / len(self.b2)
                self.p = max(self.p - delta, 0)
                self.b2.remove(expert_id)
                self.t2.append(expert_id)
            else:
                self.t1.append(expert_id)

    def _evict(self, future_index=0):
        if not self.cache:
            return None
            
        if self.policy == CachePolicy.LRU:
            for exp in self.recency:
                if exp in self.cache:
                    self.recency.remove(exp)
                    return exp
            return list(self.cache)[0]
            
        elif self.policy == CachePolicy.LFU:
            min_freq = float('inf')
            evict_cand = None
            for exp in self.cache:
                if self.frequency[exp] < min_freq:
                    min_freq = self.frequency[exp]
                    evict_cand = exp
            return evict_cand
            
        elif self.policy == CachePolicy.ORACLE:
            # Look ahead in oracle_sequence to find the expert accessed furthest in the future
            furthest_dist = -1
            evict_cand = None
            for exp in self.cache:
                try:
                    dist = self.oracle_sequence[future_index:].index(exp)
                except ValueError:
                    dist = float('inf')
                if dist > furthest_dist:
                    furthest_dist = dist
                    evict_cand = exp
            return evict_cand
            
        elif self.policy == CachePolicy.PREDICTION_GUIDED:
            # Score cache items: score = recency + Markov prediction weight
            predicted = self.predictor.predict_next(top_n=3)
            # Evict something not in the predicted set if possible
            candidates = [exp for exp in self.cache if exp not in predicted]
            if candidates:
                # LRU among non-predicted candidates
                for exp in self.recency:
                    if exp in candidates:
                        self.recency.remove(exp)
                        return exp
                return candidates[0]
            else:
                # If all are predicted, fall back to LRU
                for exp in self.recency:
                    if exp in self.cache:
                        self.recency.remove(exp)
                        return exp
                return list(self.cache)[0]
                
        elif self.policy == CachePolicy.ARC:
            # Simple ARC eviction logic
            if len(self.t1) >= self.p:
                evict_cand = self.t1.pop(0)
                self.b1.append(evict_cand)
            else:
                evict_cand = self.t2.pop(0)
                self.b2.append(evict_cand)
            return evict_cand
            
        return list(self.cache)[0]

    def get_hit_rate(self) -> float:
        total = self.hits + self.misses
        return self.hits / total if total > 0 else 0.0

class HMCSimulator:
    """Discrete Event Simulator for Mixture-of-Experts scheduling."""
    def __init__(self, 
                 gpu_cache_capacity=8,
                 n_experts=64,
                 pcie_bandwidth_gbs=16.0,
                 pcie_init_overhead_us=20.0,
                 cpu_cores=8,
                 gpu_sm_count=80,
                 scheduling_policy=SchedPolicy.FIFO,
                 cache_policy=CachePolicy.LRU,
                 cpu_expert_latency_ms=10.0,
                 gpu_expert_latency_ms=1.5,
                 gpu_kernel_launch_overhead_us=15.0,
                 dispatch_jitter_us=50.0,
                 expert_size_mb=40.0, # expert weight size in MB
                 elas_threshold_ms=100.0,
                 starvation_threshold=5,
                 env=None):
                 
        self.env = env if env is not None else simpy.Environment()
        self.gpu_cache_capacity = gpu_cache_capacity
        self.n_experts = n_experts
        self.pcie_bandwidth_gbs = pcie_bandwidth_gbs
        self.pcie_init_overhead_us = pcie_init_overhead_us
        self.cpu_cores_count = cpu_cores
        self.gpu_sm_count = gpu_sm_count
        self.scheduling_policy = scheduling_policy
        self.cache_policy = cache_policy
        self.cpu_expert_latency_ms = cpu_expert_latency_ms
        self.gpu_expert_latency_ms = gpu_expert_latency_ms
        self.gpu_kernel_launch_overhead_us = gpu_kernel_launch_overhead_us
        self.dispatch_jitter_us = dispatch_jitter_us
        self.expert_size_mb = expert_size_mb
        self.elas_threshold_ms = elas_threshold_ms
        self.starvation_threshold = starvation_threshold
        
        # Resources
        self.cpu_cores = simpy.Resource(self.env, capacity=cpu_cores)
        self.pcie_bus = simpy.Resource(self.env, capacity=1) # serialized transfers
        self.gpu_sm = simpy.Container(self.env, capacity=gpu_sm_count, init=gpu_sm_count)
        
        # Initialize Cache Model
        self.cache = ExpertCacheModel(gpu_cache_capacity, n_experts, cache_policy)
        
        # Request queueing state
        self.ready_queue = []
        self.completed_requests = []
        
        # Profiling stats
        self.gpu_active_time = 0.0
        self.total_sim_time = 0.0
        self.dispatch_latencies = []
        
        # Monitor active streams for GPU contention
        self.active_streams = 0
        self.env.process(self._scheduler_loop())

    def submit_request(self, request):
        self.ready_queue.append(request)
        
    def _get_gpu_contention_slowdown(self) -> float:
        """Models multi-stream SM sharing slowdown factor."""
        N = self.active_streams
        if N <= 1:
            return 1.0
        elif N <= 4:
            # linear slowdown
            return 1.0 + 0.15 * (N - 1)
        else:
            # super-linear slowdown (contention spikes)
            return 1.45 + 0.35 * (N - 4)

    def _scheduler_loop(self):
        while True:
            if not self.ready_queue:
                yield self.env.timeout(0.1) # poll delay
                continue
                
            # Select request based on scheduling policy
            if self.scheduling_policy == SchedPolicy.FIFO:
                req = self.ready_queue.pop(0)
            elif self.scheduling_policy == SchedPolicy.GREEDY_SHORTEST:
                # Shortest remaining generation tokens first
                self.ready_queue.sort(key=lambda r: r.gen_tokens - r.current_token_idx)
                req = self.ready_queue.pop(0)
            elif self.scheduling_policy == SchedPolicy.EDF or self.scheduling_policy == SchedPolicy.DACC:
                # Earliest Deadline First
                self.ready_queue.sort(key=lambda r: r.deadline)
                req = self.ready_queue.pop(0)
            else:
                req = self.ready_queue.pop(0) # fallback
                
            # Process request execution in a SimPy process
            self.env.process(self._process_request(req))

    def _process_request(self, req):
        self.active_streams += 1
        req.start_time = self.env.now
        
        # 1. Host dispatch latency & jitter
        jitter = random.normalvariate(self.dispatch_jitter_us / 1000.0, (self.dispatch_jitter_us / 2.0) / 1000.0)
        jitter = max(0.001, jitter)
        yield self.env.timeout(jitter)
        req.breakdown["host_scheduling_ms"] += jitter
        
        # Execution loop for each token
        for tok_idx in range(req.total_tokens):
            req.current_token_idx = tok_idx
            
            # Identify active experts for this token
            experts = req.expert_sequence[tok_idx] if tok_idx < len(req.expert_sequence) else req.expert_sequence[-1]
            
            token_hit_all = True
            token_cache_events = []
            
            for exp_id in experts:
                future_idx = tok_idx * len(experts) # used for Oracle look-ahead
                res, evicted = self.cache.access(exp_id, self.env.now, future_idx)
                token_cache_events.append(res.name)
                
                if res == EvictionResult.HIT:
                    # Execute on GPU
                    slowdown = self._get_gpu_contention_slowdown()
                    gpu_time = self.gpu_expert_latency_ms * slowdown + (self.gpu_kernel_launch_overhead_us / 1000.0)
                    
                    # Lock GPU SM resource
                    req_sm = 4 # assume 4 SMs per expert kernel
                    t_start_wait = self.env.now
                    yield self.gpu_sm.get(req_sm)
                    req.breakdown["queue_wait_ms"] += (self.env.now - t_start_wait)
                    
                    t_start_exec = self.env.now
                    yield self.env.timeout(gpu_time)
                    req.breakdown["gpu_exec_ms"] += (self.env.now - t_start_exec)
                    yield self.gpu_sm.put(req_sm)
                else:
                    token_hit_all = False
                    # Cache miss: choose load or CPU fallback
                    is_cpu_fallback = False
                    
                    # Elastic-DCMD decision logic
                    if self.scheduling_policy == SchedPolicy.DACC or self.scheduling_policy == SchedPolicy.EDF:
                        # Priority-aware decision: check queue congestion or remaining time
                        remaining_time = req.deadline - self.env.now
                        if remaining_time < self.elas_threshold_ms:
                            is_cpu_fallback = True
                            
                    # CPU core fallback check
                    if is_cpu_fallback:
                        # Lock host CPU core and execute
                        with self.cpu_cores.request() as cpu_req:
                            t_start_wait = self.env.now
                            yield cpu_req
                            req.breakdown["cpu_stall_ms"] += (self.env.now - t_start_wait)
                            
                            t_start_exec = self.env.now
                            yield self.env.timeout(self.cpu_expert_latency_ms)
                            req.breakdown["cpu_exec_ms"] += (self.env.now - t_start_exec)
                    else:
                        # standard load: transfer over PCIe
                        req.breakdown["expert_cache_miss_ms"] += 1.0 # record miss overhead
                        
                        transfer_time = (self.expert_size_mb / (self.pcie_bandwidth_gbs * 1024.0 / 8.0)) * 1000.0 + (self.pcie_init_overhead_us / 1000.0)
                        
                        with self.pcie_bus.request() as pcie_req:
                            t_start_wait = self.env.now
                            yield pcie_req
                            req.breakdown["gpu_bubble_ms"] += (self.env.now - t_start_wait) # waiting for PCIe transfer slot
                            
                            t_start_trans = self.env.now
                            yield self.env.timeout(transfer_time)
                            req.breakdown["activation_transfer_ms"] += (self.env.now - t_start_trans)
                            
                        # Run expert on GPU after transfer completes
                        slowdown = self._get_gpu_contention_slowdown()
                        gpu_time = self.gpu_expert_latency_ms * slowdown + (self.gpu_kernel_launch_overhead_us / 1000.0)
                        
                        req_sm = 4
                        t_start_wait = self.env.now
                        yield self.gpu_sm.get(req_sm)
                        req.breakdown["queue_wait_ms"] += (self.env.now - t_start_wait)
                        
                        t_start_exec = self.env.now
                        yield self.env.timeout(gpu_time)
                        req.breakdown["gpu_exec_ms"] += (self.env.now - t_start_exec)
                        yield self.gpu_sm.put(req_sm)
            
            req.cache_events.append(token_cache_events)
            if tok_idx == 0:
                req.ttft = self.env.now - req.arrival_time
                
        req.finish_time = self.env.now
        req.tpot = (req.finish_time - req.arrival_time - req.ttft) / max(1, req.gen_tokens)
        
        self.completed_requests.append(req)
        self.active_streams -= 1

class RequestDispatcher:
    """Manages request arrival timelines, submitting requests to the simulator at their arrival times."""
    def __init__(self, env, simulator, requests):
        self.env = env
        self.simulator = simulator
        self.requests = requests
        self.env.process(self._arrival_loop())
        
    def _arrival_loop(self):
        # Sort requests by arrival time
        self.requests.sort(key=lambda r: r.arrival_time)
        
        last_time = 0.0
        for req in self.requests:
            delay = req.arrival_time - last_time
            if delay > 0:
                yield self.env.timeout(delay)
            last_time = req.arrival_time
            
            # Submit to simulator
            self.simulator.submit_request(req)

class WorkloadGenerator:
    """Helper to generate request traces based on specified workload parameters."""
    @staticmethod
    def generate_requests(num_requests=10, 
                          n_experts=64, 
                          topk=2, 
                          arrival_pattern="concurrent", 
                          arrival_rate=1.0, 
                          zipf_alpha=1.0, 
                          agentic_fraction=0.2):
        
        requests = []
        random.seed(42)
        np.random.seed(42)
        
        # Pre-generate expert popularity Zipf distribution
        probs = 1.0 / (np.arange(1, n_experts + 1) ** zipf_alpha)
        probs /= probs.sum()
        
        current_time = 0.0
        for i in range(num_requests):
            # 1. Arrival time (convert seconds to milliseconds)
            if arrival_pattern == "single":
                current_time += (1.0 / arrival_rate) * 1000.0
            elif arrival_pattern == "concurrent":
                current_time += (random.expovariate(arrival_rate)) * 1000.0
            elif arrival_pattern == "bursty":
                # On-off bursty model: high arrival rate, then idle
                if i % 5 == 0:
                    current_time += (random.uniform(2.0, 5.0)) * 1000.0
                else:
                    current_time += (random.uniform(0.01, 0.1)) * 1000.0
            else:
                current_time += (random.expovariate(arrival_rate)) * 1000.0
                
            prompt_len = random.randint(16, 64)
            gen_len = random.randint(32, 128)
            total_len = prompt_len + gen_len
            
            # Generate synthetic expert sequences (with locality)
            expert_sequence = []
            for _ in range(total_len):
                # Sample top-k experts
                selected = list(np.random.choice(n_experts, size=topk, replace=False, p=probs))
                expert_sequence.append(selected)
                
            is_agentic = random.random() < agentic_fraction
            deadline = current_time + (50.0 if is_agentic else 500.0) # agentic requests have strict deadline
            
            req = Request(
                request_id=i,
                arrival_time=current_time,
                prompt_tokens=prompt_len,
                gen_tokens=gen_len,
                expert_sequence=expert_sequence,
                is_agentic=is_agentic,
                deadline=deadline
            )
            requests.append(req)
            
        return requests
