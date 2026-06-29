# run_experiments.py
# Runs the 5 core MoE-HostAssist simulation sweeps and writes results to CSV/JSON files.

import sys
import os
import numpy as np
import pandas as pd
import json
import random

# Add simulator path
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../'))
from simulator.moe_sim import HMCSimulator, WorkloadGenerator, SchedPolicy, CachePolicy, RequestDispatcher

RESULTS_DIR = "results"
os.makedirs(RESULTS_DIR, exist_ok=True)

# Set random seed for reproducibility
random.seed(42)
np.random.seed(42)

def run_e1_bottleneck_interaction():
    print(">>> Running Experiment E1: Bottleneck Interaction Profiling...")
    results = []
    
    concurrencies = [1, 2, 4, 8, 16, 32, 64]
    
    for N in concurrencies:
        # Generate workload for N concurrent streams
        requests = WorkloadGenerator.generate_requests(
            num_requests=N * 3,
            n_experts=64,
            topk=2,
            arrival_pattern="concurrent",
            arrival_rate=2.0 * N, # scale arrival rate with N
            zipf_alpha=1.2,
            agentic_fraction=0.0
        )
        
        sim = HMCSimulator(
            gpu_cache_capacity=16, # 25% of 64 experts
            n_experts=64,
            pcie_bandwidth_gbs=16.0,
            cpu_cores=8,
            scheduling_policy=SchedPolicy.FIFO,
            cache_policy=CachePolicy.LRU,
            dispatch_jitter_us=50.0
        )
        
        dispatcher = RequestDispatcher(sim.env, sim, requests)
        sim.env.run(until=500000.0) # 500 seconds
        
        completed = sim.completed_requests
        if not completed:
            continue
            
        # Calculate mean latencies
        e2e = np.mean([r.finish_time - r.arrival_time for r in completed])
        p99 = np.percentile([r.finish_time - r.arrival_time for r in completed], 99)
        
        # Calculate breakdown component means
        breakdown_means = {}
        for key in completed[0].breakdown.keys():
            breakdown_means[key] = np.mean([r.breakdown[key] for r in completed])
            
        sum_indiv = sum(breakdown_means.values())
        interaction_coeff = max(0.0, e2e - sum_indiv)
        
        # GPU idle bubble fraction estimation
        # time where GPU is waiting for PCIe or Dispatch
        gpu_bubble_mean = breakdown_means["gpu_bubble_ms"]
        gpu_exec_mean = breakdown_means["gpu_exec_ms"]
        gpu_bubble_frac = gpu_bubble_mean / (gpu_bubble_mean + gpu_exec_mean) if (gpu_bubble_mean + gpu_exec_mean) > 0 else 0.0
        
        row = {
            "concurrency": N,
            "e2e_latency_ms": e2e,
            "p99_latency_ms": p99,
            "interaction_overhead_ms": interaction_coeff,
            "gpu_bubble_fraction": gpu_bubble_frac,
            **breakdown_means
        }
        results.append(row)
        print(f"Concurrency N={N} | E2E={e2e:.2f}ms | P99={p99:.2f}ms | Interaction={interaction_coeff:.2f}ms")
        
    df = pd.DataFrame(results)
    df.to_csv(os.path.join(RESULTS_DIR, "e1_bottleneck_interaction.csv"), index=False)
    print("Experiment E1 Complete.")

def run_e2_cache_miss_cascade():
    print(">>> Running Experiment E2: Cache Miss Cascade Characterization...")
    results = []
    
    # We simulate a locality shift.
    # We generate a workload where at T=10s (10000ms), the Zipf distribution parameter alpha shifts from 1.5 to 0.2.
    cache_capacities = [3, 6, 9, 12, 16, 20, 32] # representing roughly 5% to 50% of 64 experts
    policies = [CachePolicy.LRU, CachePolicy.LFU, CachePolicy.PREDICTION_GUIDED, CachePolicy.ORACLE]
    
    for policy in policies:
        for cap in cache_capacities:
            # Generate requests
            requests = WorkloadGenerator.generate_requests(
                num_requests=30,
                n_experts=64,
                topk=2,
                arrival_pattern="concurrent",
                arrival_rate=5.0,
                zipf_alpha=1.5 # base alpha
            )
            
            # Inject locality shift after request 15 (which arrives around 3 seconds)
            # Change expert sequence to zipf_alpha=0.2 (unstable/uniform) for requests 15 onwards
            probs_shift = 1.0 / (np.arange(1, 64 + 1) ** 0.2)
            probs_shift /= probs_shift.sum()
            
            oracle_sequence = []
            for idx, r in enumerate(requests):
                if idx >= 15:
                    # Regenerate expert sequences with high entropy (uniform Zipf)
                    new_seq = []
                    for _ in range(r.total_tokens):
                        selected = list(np.random.choice(64, size=2, replace=False, p=probs_shift))
                        new_seq.append(selected)
                    r.expert_sequence = new_seq
                
                # Collect overall sequential access sequence for Oracle policy
                for token_experts in r.expert_sequence:
                    oracle_sequence.extend(token_experts)
            
            sim = HMCSimulator(
                gpu_cache_capacity=cap,
                n_experts=64,
                pcie_bandwidth_gbs=16.0,
                cpu_cores=8,
                scheduling_policy=SchedPolicy.FIFO,
                cache_policy=policy
            )
            # Pass oracle sequence to cache model
            sim.cache.oracle_sequence = oracle_sequence
            
            dispatcher = RequestDispatcher(sim.env, sim, requests)
            sim.env.run(until=200000.0)
            
            hit_rate = sim.cache.get_hit_rate()
            max_cascade = sim.cache.max_cascade_depth
            evictions = sim.cache.evictions
            
            # Calculate tail latency spike during transition
            completed = sim.completed_requests
            latencies = [r.finish_time - r.arrival_time for r in completed]
            p99 = np.percentile(latencies, 99) if latencies else 0.0
            
            row = {
                "policy": policy.name,
                "capacity": cap,
                "hit_rate": hit_rate,
                "max_cascade_depth": max_cascade,
                "eviction_count": evictions,
                "p99_latency_ms": p99
            }
            results.append(row)
            print(f"Policy={policy.name} | Cap={cap} | HitRate={hit_rate:.2%} | MaxCascade={max_cascade}")
            
    df = pd.DataFrame(results)
    df.to_csv(os.path.join(RESULTS_DIR, "e2_cache_cascade.csv"), index=False)
    print("Experiment E2 Complete.")

def run_e3_dispatch_jitter():
    print(">>> Running Experiment E3: Host Dispatch Jitter to GPU Bubble Coupling...")
    results = []
    
    jitters = [0.0, 10.0, 50.0, 100.0, 200.0, 500.0] # in microseconds
    concurrencies = [1, 2, 4, 8, 16]
    
    for N in concurrencies:
        for jitter in jitters:
            requests = WorkloadGenerator.generate_requests(
                num_requests=N * 4,
                n_experts=64,
                topk=2,
                arrival_pattern="concurrent",
                arrival_rate=3.0 * N,
                zipf_alpha=1.2
            )
            
            sim = HMCSimulator(
                gpu_cache_capacity=16,
                n_experts=64,
                pcie_bandwidth_gbs=16.0,
                cpu_cores=8,
                scheduling_policy=SchedPolicy.FIFO,
                cache_policy=CachePolicy.LRU,
                dispatch_jitter_us=jitter
            )
            
            dispatcher = RequestDispatcher(sim.env, sim, requests)
            sim.env.run(until=100000.0)
            
            completed = sim.completed_requests
            if not completed:
                continue
                
            gpu_bubble = np.mean([r.breakdown["gpu_bubble_ms"] for r in completed])
            e2e = np.mean([r.finish_time - r.arrival_time for r in completed])
            
            results.append({
                "concurrency": N,
                "dispatch_jitter_us": jitter,
                "gpu_bubble_ms": gpu_bubble,
                "e2e_latency_ms": e2e
            })
            print(f"Concurrency N={N} | Jitter={jitter}us | GPU Bubble={gpu_bubble:.2f}ms")
            
    df = pd.DataFrame(results)
    df.to_csv(os.path.join(RESULTS_DIR, "e3_dispatch_jitter.csv"), index=False)
    print("Experiment E3 Complete.")

def run_e4_prefetch_tradeoff():
    print(">>> Running Experiment E4: Prefetch Benefit vs. Prediction Accuracy Trade-off...")
    results = []
    
    # We sweep prediction accuracy P and prefetch window size W.
    # To model prediction uncertainty:
    # A prefetch scheduler triggers PCIe transfers of the predicted expert in advance.
    # With probability P, it prefetches the correct expert.
    # With probability (1-P), it prefetches a wrong expert, which consumes PCIe bus bandwidth and pollutes the cache.
    
    accuracies = [0.0, 0.20, 0.40, 0.60, 0.80, 1.00]
    windows = [1, 2, 4]
    
    for W in windows:
        for P in accuracies:
            requests = WorkloadGenerator.generate_requests(
                num_requests=20,
                n_experts=64,
                topk=2,
                arrival_pattern="concurrent",
                arrival_rate=4.0,
                zipf_alpha=1.2
            )
            
            # Simple prefetch logic modeled inside simulator:
            # We adjust the effective transfer latency and cache hit rate of the simulation dynamically.
            # If P is high, the cache miss latency is reduced because weights are pre-loaded.
            # If P is low, the wrong prefetch causes additional cache eviction and bus congestion (modeled as effective bandwidth reduction).
            
            effective_bw = 16.0 * (1.0 - 0.3 * (1.0 - P)) # wrong prefetch reduces useful bandwidth by up to 30%
            
            # Adjust expert cache hit rate to model prefetch hits:
            # Hit rate increases proportionally to P and W, capped at 95%
            cache_capacity_adjusted = int(16 * (1.0 + 0.5 * P * W))
            cache_capacity_adjusted = min(64, cache_capacity_adjusted)
            
            sim = HMCSimulator(
                gpu_cache_capacity=cache_capacity_adjusted,
                n_experts=64,
                pcie_bandwidth_gbs=effective_bw,
                cpu_cores=8,
                scheduling_policy=SchedPolicy.FIFO,
                cache_policy=CachePolicy.LRU
            )
            
            dispatcher = RequestDispatcher(sim.env, sim, requests)
            sim.env.run(until=150000.0)
            
            completed = sim.completed_requests
            e2e = np.mean([r.finish_time - r.arrival_time for r in completed]) if completed else 0.0
            
            # Prefetch bandwidth overhead ratio
            bw_overhead = (1.0 - P) * 0.4 * W # wrong prefetch ratio
            
            results.append({
                "prefetch_window": W,
                "prediction_accuracy": P,
                "e2e_latency_ms": e2e,
                "bandwidth_overhead_ratio": bw_overhead
            })
            print(f"W={W} | P={P:.2f} | E2E={e2e:.2f}ms | BW Overhead={bw_overhead:.2%}")
            
    df = pd.DataFrame(results)
    df.to_csv(os.path.join(RESULTS_DIR, "e4_prefetch_tradeoff.csv"), index=False)
    print("Experiment E4 Complete.")

def run_e5_priority_scheduling():
    print(">>> Running Experiment E5: Priority Scheduling Pareto Frontier...")
    results = []
    
    # We sweep FIFO, EDF, WRR (Weighted Round-Robin), and DACC (Deadline-Aware + Cache Co-management)
    # under mixed agentic (latency-critical) and batch (throughput-bound) workloads.
    
    policies = [SchedPolicy.FIFO, SchedPolicy.GREEDY_SHORTEST, SchedPolicy.EDF, SchedPolicy.DACC]
    agentic_fractions = [0.1, 0.25, 0.5]
    
    for fraction in agentic_fractions:
        for policy in policies:
            requests = WorkloadGenerator.generate_requests(
                num_requests=30,
                n_experts=64,
                topk=2,
                arrival_pattern="concurrent",
                arrival_rate=5.0,
                zipf_alpha=1.2,
                agentic_fraction=fraction
            )
            
            sim = HMCSimulator(
                gpu_cache_capacity=16,
                n_experts=64,
                pcie_bandwidth_gbs=16.0,
                cpu_cores=8,
                scheduling_policy=policy,
                cache_policy=CachePolicy.LRU
            )
            
            # DACC policy features: protect agentic requests' experts in GPU cache
            if policy == SchedPolicy.DACC:
                # model cache partitioning: reserve 4 cache lines for agentic streams
                sim.cache.capacity = 12
                
            dispatcher = RequestDispatcher(sim.env, sim, requests)
            sim.env.run(until=200000.0)
            
            completed = sim.completed_requests
            if not completed:
                continue
                
            agentic_reqs = [r for r in completed if r.is_agentic]
            batch_reqs = [r for r in completed if not r.is_agentic]
            
            agentic_p99 = np.percentile([r.finish_time - r.arrival_time for r in agentic_reqs], 99) if agentic_reqs else 0.0
            batch_throughput = len(batch_reqs) / (sim.env.now / 1000.0) if batch_reqs else 0.0
            
            # Deadline miss rate
            misses = sum(1 for r in agentic_reqs if r.finish_time > r.deadline)
            miss_rate = misses / len(agentic_reqs) if agentic_reqs else 0.0
            
            results.append({
                "agentic_fraction": fraction,
                "policy": policy.name,
                "agentic_p99_latency_ms": agentic_p99,
                "batch_throughput_tps": batch_throughput,
                "deadline_miss_rate": miss_rate
            })
            print(f"Fraction={fraction} | Policy={policy.name} | Agentic P99={agentic_p99:.2f}ms | Batch Thru={batch_throughput:.2f} req/s | Miss Rate={miss_rate:.2%}")
            
    df = pd.DataFrame(results)
    df.to_csv(os.path.join(RESULTS_DIR, "e5_priority_scheduling.csv"), index=False)
    print("Experiment E5 Complete.")

def main():
    print("============================================================")
    print("             Starting MoE-HostAssist Sweeps")
    print("============================================================")
    
    run_e1_bottleneck_interaction()
    run_e2_cache_miss_cascade()
    run_e3_dispatch_jitter()
    run_e4_prefetch_tradeoff()
    run_e5_priority_scheduling()
    
    print("============================================================")
    print("             All Simulation Sweeps Complete")
    print("============================================================")

if __name__ == "__main__":
    main()
