import os
import json
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import argparse

class ExpertCacheSimulator:
    def __init__(self, trace_file, capacity_per_layer=2, expert_size_mb=128.0, pcie_bandwidth_gbps=16.0, compute_time_ms=10.0, prefetch_window=1, confidence_threshold=0.15):
        self.df = pd.read_csv(trace_file).sort_values(by=["request_id", "token_id", "layer_id", "topk_rank"])
        self.capacity = capacity_per_layer
        self.expert_size_mb = expert_size_mb
        self.bandwidth_gbps = pcie_bandwidth_gbps
        self.compute_time_ms = compute_time_ms
        self.prefetch_window = prefetch_window
        self.confidence_threshold = confidence_threshold
        
        self.unique_layers = sorted(self.df["layer_id"].unique())
        self.num_layers = len(self.unique_layers)
        self.num_experts = self.df["expert_id"].nunique()
        
        # Calculate transfer latency for one expert: Size (MB) / Bandwidth (GB/s) = ms
        self.transfer_latency_ms = self.expert_size_mb / self.bandwidth_gbps
        
    def run_policy(self, name):
        # Cache states per layer
        caches = {l: [] for l in self.unique_layers} # list of expert_ids in cache, ordered LRU
        expert_ready_time = {} # (layer_id, expert_id) -> float
        
        t_global = 0.0
        t_bus_available = 0.0
        
        # Metrics
        demand_access_count = 0
        demand_hit_count = 0
        demand_miss_count = 0
        prefetch_request_count = 0
        useful_prefetch_count = 0
        useless_prefetch_count = 0
        eviction_count = 0
        
        # Precompute future accesses for Oracle prefetching
        future_accesses = {l: [] for l in self.unique_layers}
        for idx, row in self.df.iterrows():
            future_accesses[row["layer_id"]].append((row["token_id"], row["expert_id"]))
            
        # Group by step
        grouped = self.df.groupby(["request_id", "token_id"])
        steps = list(grouped.groups.keys())
        
        # Precompute transition matrix for temporal_window_prefetch
        transition_counts = np.zeros((self.num_experts + 128, self.num_experts + 128))
        for (req_id, layer_id), group in self.df.groupby(["request_id", "layer_id"]):
            group = group.sort_values(by=["token_id", "topk_rank"])
            tok_experts = group.groupby("token_id")["expert_id"].apply(list).sort_index()
            tokens = tok_experts.index.tolist()
            for i in range(len(tokens) - 1):
                for e1 in tok_experts[tokens[i]]:
                    for e2 in tok_experts[tokens[i+1]]:
                        if e1 < transition_counts.shape[0] and e2 < transition_counts.shape[1]:
                            transition_counts[e1, e2] += 1
        row_sums = transition_counts.sum(axis=1, keepdims=True)
        transition_prob = np.where(row_sums > 0, transition_counts / row_sums, 0.0)
        
        # Precompute cross-layer affinity for layer_temporal_affinity_cache
        affinity_counts = {l: np.zeros((self.num_experts + 128, self.num_experts + 128)) for l in self.unique_layers[:-1]}
        for (req_id, tok_id), group in self.df.groupby(["request_id", "token_id"]):
            layer_exps = group.groupby("layer_id")["expert_id"].apply(list)
            for i in range(self.num_layers - 1):
                l = self.unique_layers[i]
                l_next = self.unique_layers[i+1]
                if l in layer_exps and l_next in layer_exps:
                    for e_curr in layer_exps[l]:
                        for e_next in layer_exps[l_next]:
                            if e_curr < affinity_counts[l].shape[0] and e_next < affinity_counts[l].shape[1]:
                                affinity_counts[l][e_curr, e_next] += 1
        affinity_prob = {}
        for i in range(self.num_layers - 1):
            l = self.unique_layers[i]
            row_sums = affinity_counts[l].sum(axis=1, keepdims=True)
            affinity_prob[l] = np.where(row_sums > 0, affinity_counts[l] / row_sums, 0.0)
            
        # Capacity distribution per layer
        layer_capacities = {l: self.capacity for l in self.unique_layers}
        if name == "layer_aware_lru":
            exp_freqs = self.df.groupby(["layer_id", "expert_id"]).size().unstack(fill_value=0)
            layer_entropies = {}
            for l in self.unique_layers:
                if l in exp_freqs.index:
                    freq = exp_freqs.loc[l].values
                    p = freq / (freq.sum() + 1e-9)
                    entropy = -np.sum(p * np.log2(p + 1e-9))
                    layer_entropies[l] = entropy
                else:
                    layer_entropies[l] = 0.0
            total_budget = self.capacity * self.num_layers
            inv_entropies = [1.0 / (layer_entropies[l] + 0.1) for l in self.unique_layers]
            sum_inv = sum(inv_entropies)
            caps = [int(np.round((inv / sum_inv) * total_budget)) for inv in inv_entropies]
            for idx_l in range(self.num_layers):
                caps[idx_l] = max(1, caps[idx_l])
            while sum(caps) > total_budget:
                max_idx = np.argmax(caps)
                caps[max_idx] -= 1
            while sum(caps) < total_budget:
                min_idx = np.argmin(caps)
                caps[min_idx] += 1
            for idx_l, l in enumerate(self.unique_layers):
                layer_capacities[l] = caps[idx_l]
                
        # Cache for static hot expert policy (never evicted)
        static_hot_caches = {}
        if name == "static_hot_expert":
            for l in self.unique_layers:
                layer_df = self.df[self.df["layer_id"] == l]
                top_exps = layer_df["expert_id"].value_counts().head(self.capacity).index.tolist()
                static_hot_caches[l] = set(top_exps)
                
        # Compute time per layer
        t_comp_layer = self.compute_time_ms / self.num_layers
        
        # Simulation Loop
        for idx, (req_id, tok_id) in enumerate(steps):
            token_group = grouped.get_group((req_id, tok_id))
            
            # Identify next step's actual experts
            next_experts = {}
            if idx + 1 < len(steps):
                next_tok_group = grouped.get_group(steps[idx+1])
                for l_id, g in next_tok_group.groupby("layer_id"):
                    next_experts[l_id] = set(g["expert_id"].unique())
                    
            # Process layers sequentially
            for i, layer_id in enumerate(self.unique_layers):
                if layer_id not in token_group["layer_id"].values:
                    continue
                    
                layer_group = token_group[token_group["layer_id"] == layer_id]
                req_experts = layer_group["expert_id"].unique()
                
                cache = caches[layer_id]
                cap = layer_capacities[layer_id]
                
                t_comp_start_target = t_global
                max_expert_ready_time = t_comp_start_target
                
                # Update future accesses list for Oracle
                while future_accesses[layer_id] and future_accesses[layer_id][0][0] <= tok_id:
                    future_accesses[layer_id].pop(0)
                    
                for exp in req_experts:
                    demand_access_count += 1
                    
                    if name == "no_cache":
                        in_cache = False
                    elif name == "static_hot_expert":
                        in_cache = (exp in static_hot_caches[layer_id])
                    else:
                        in_cache = (exp in cache)
                        
                    if in_cache:
                        demand_hit_count += 1
                        r_time = expert_ready_time.get((layer_id, exp), 0.0)
                        max_expert_ready_time = max(max_expert_ready_time, r_time)
                        
                        if name not in ["no_cache", "static_hot_expert"]:
                            cache.remove(exp)
                            cache.append(exp)
                    else:
                        demand_miss_count += 1
                        # Load from PCIe bus
                        start_transfer = max(t_global, t_bus_available)
                        end_transfer = start_transfer + self.transfer_latency_ms
                        t_bus_available = end_transfer
                        expert_ready_time[(layer_id, exp)] = end_transfer
                        max_expert_ready_time = max(max_expert_ready_time, end_transfer)
                        
                        if name not in ["no_cache", "static_hot_expert"]:
                            if len(cache) >= cap:
                                victim = cache.pop(0)
                                expert_ready_time.pop((layer_id, victim), None)
                                eviction_count += 1
                            cache.append(exp)
                            
                t_comp_actual_start = max(t_global, max_expert_ready_time)
                t_comp_end = t_comp_actual_start + t_comp_layer
                t_global = t_comp_end
                
                # Prefetching triggers
                if name == "oracle_prefetch":
                    if layer_id in next_experts:
                        for n_exp in next_experts[layer_id]:
                            if n_exp not in cache:
                                prefetch_request_count += 1
                                useful_prefetch_count += 1
                                
                                start_transfer = max(t_global, t_bus_available)
                                end_transfer = start_transfer + self.transfer_latency_ms
                                t_bus_available = end_transfer
                                expert_ready_time[(layer_id, n_exp)] = end_transfer
                                
                                if len(cache) >= cap:
                                    furthest_dist = -1
                                    victim = None
                                    for c_exp in cache:
                                        next_idx = len(future_accesses[layer_id])
                                        for f_i, (f_tok, f_exp) in enumerate(future_accesses[layer_id]):
                                            if f_tok > tok_id and f_exp == c_exp:
                                                next_idx = f_i
                                                break
                                        if next_idx > furthest_dist:
                                            furthest_dist = next_idx
                                            victim = c_exp
                                    cache.remove(victim)
                                    expert_ready_time.pop((layer_id, victim), None)
                                    eviction_count += 1
                                cache.append(n_exp)
                                
                elif name == "temporal_window_prefetch":
                    if layer_id in next_experts:
                        for curr_exp in req_experts:
                            if curr_exp < transition_prob.shape[0]:
                                prob = transition_prob[curr_exp]
                                candidates = np.where(prob >= self.confidence_threshold)[0]
                                for cand in candidates:
                                    if cand not in cache:
                                        prefetch_request_count += 1
                                        is_useful = (cand in next_experts[layer_id])
                                        if is_useful:
                                            useful_prefetch_count += 1
                                        else:
                                            useless_prefetch_count += 1
                                            
                                        start_transfer = max(t_global, t_bus_available)
                                        end_transfer = start_transfer + self.transfer_latency_ms
                                        t_bus_available = end_transfer
                                        expert_ready_time[(layer_id, cand)] = end_transfer
                                        
                                        if len(cache) >= cap:
                                            victim = cache.pop(0)
                                            expert_ready_time.pop((layer_id, victim), None)
                                            eviction_count += 1
                                        cache.append(cand)
                                        
                elif name == "layer_temporal_affinity_cache":
                    if i + 1 < self.num_layers:
                        next_layer_id = self.unique_layers[i+1]
                        if next_layer_id in next_experts:
                            next_cache = caches[next_layer_id]
                            next_cap = layer_capacities[next_layer_id]
                            actual_next_exps = next_experts[next_layer_id]
                            
                            for curr_exp in req_experts:
                                if curr_exp < affinity_prob[layer_id].shape[0]:
                                    prob = affinity_prob[layer_id][curr_exp]
                                    candidates = np.where(prob >= self.confidence_threshold)[0]
                                    for cand in candidates:
                                        if cand not in next_cache:
                                            prefetch_request_count += 1
                                            is_useful = (cand in actual_next_exps)
                                            if is_useful:
                                                useful_prefetch_count += 1
                                            else:
                                                useless_prefetch_count += 1
                                                
                                            start_transfer = max(t_global, t_bus_available)
                                            end_transfer = start_transfer + self.transfer_latency_ms
                                            t_bus_available = end_transfer
                                            expert_ready_time[(next_layer_id, cand)] = end_transfer
                                            
                                            if len(next_cache) >= next_cap:
                                                victim = next_cache.pop(0)
                                                expert_ready_time.pop((next_layer_id, victim), None)
                                                eviction_count += 1
                                            next_cache.append(cand)
                                            
        total_compute_ms = len(steps) * self.compute_time_ms
        total_latency_ms = t_global
        total_stall_ms = max(0.0, total_latency_ms - total_compute_ms)
        
        transferred_mb = (demand_miss_count + prefetch_request_count) * self.expert_size_mb
        demand_transfer_mb = demand_miss_count * self.expert_size_mb
        prefetch_transfer_mb = prefetch_request_count * self.expert_size_mb
        
        non_overlap_latency_ms = total_compute_ms + (demand_miss_count + prefetch_request_count) * self.transfer_latency_ms
        
        total_transfer_time_ms = transferred_mb / self.bandwidth_gbps
        bandwidth_utilization = total_transfer_time_ms / total_latency_ms if total_latency_ms > 0 else 0.0
        bandwidth_utilization = min(1.0, bandwidth_utilization)
        
        hit_rate = demand_hit_count / demand_access_count if demand_access_count > 0 else 0.0
        
        return {
            "hit_rate": float(hit_rate),
            "traffic_mb": float(transferred_mb),
            "demand_traffic_mb": float(demand_transfer_mb),
            "prefetch_traffic_mb": float(prefetch_transfer_mb),
            "exposed_stall_ms": float(total_stall_ms),
            "total_latency_ms": float(total_latency_ms),
            "speedup": float(non_overlap_latency_ms / total_latency_ms) if total_latency_ms > 0 else 1.0,
            "bandwidth_utilization": float(bandwidth_utilization),
            "prefetch_request_count": int(prefetch_request_count),
            "useful_prefetch_count": int(useful_prefetch_count),
            "useless_prefetch_count": int(useless_prefetch_count)
        }

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--trace_file", type=str, required=True)
    parser.add_argument("--capacity", type=int, default=8)
    parser.add_argument("--expert_size", type=float, default=64.0)
    parser.add_argument("--pcie_bandwidth", type=float, default=16.0)
    parser.add_argument("--compute_time", type=float, default=10.0)
    parser.add_argument("--confidence_threshold", type=float, default=0.03)
    parser.add_argument("--output_file", type=str, default="/home/a/discover/expert_cache/expert_cache_results.json")
    args = parser.parse_args()
    
    sim = ExpertCacheSimulator(
        trace_file=args.trace_file,
        capacity_per_layer=args.capacity,
        expert_size_mb=args.expert_size,
        pcie_bandwidth_gbps=args.pcie_bandwidth,
        compute_time_ms=args.compute_time,
        confidence_threshold=args.confidence_threshold
    )
    
    policies = [
        "no_cache",
        "lru",
        "lfu",
        "static_hot_expert",
        "oracle_prefetch",
        "layer_aware_lru",
        "temporal_window_prefetch",
        "layer_temporal_affinity_cache"
    ]
    
    results = {}
    for p in policies:
        results[p] = sim.run_policy(p)
        
    baseline_latency = results["no_cache"]["total_latency_ms"]
    for p in policies:
        results[p]["speedup_vs_nocache"] = float(baseline_latency / results[p]["total_latency_ms"])
        
    print("\n================== Running Expert Cache Simulation ==================")
    print(f"Trace: {args.trace_file}")
    print(f"Capacity per layer: {args.capacity}, Expert Size: {args.expert_size} MB")
    print(f"Confidence Threshold: {args.confidence_threshold}")
    print(f"{'Policy':<30} | {'Hit Rate':<10} | {'Traffic (MB)':<12} | {'Stall (ms)':<10} | {'Total Lat (ms)':<14} | {'Speedup':<8}")
    print("-" * 95)
    for p in policies:
        r = results[p]
        print(f"{p:<30} | {r['hit_rate']*100:8.2f}% | {r['traffic_mb']:12.1f} | {r['exposed_stall_ms']:10.1f} | {r['total_latency_ms']:14.1f} | {r['speedup_vs_nocache']:7.2f}x")
        
    with open(args.output_file, "w") as f:
        json.dump(results, f, indent=4)
    print(f"\nResults saved to {args.output_file}")

if __name__ == "__main__":
    main()
