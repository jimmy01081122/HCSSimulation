import os
import json
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import argparse

class KVCacheSimulator:
    def __init__(self, num_requests=4, prompt_len=512, gen_len=128, num_layers=32, num_heads=8, head_dim=128, block_size=16, memory_budget_mb=256.0):
        self.num_requests = num_requests
        self.prompt_len = prompt_len
        self.gen_len = gen_len
        self.num_layers = num_layers
        self.num_heads = num_heads
        self.head_dim = head_dim
        self.block_size = block_size
        self.memory_budget_mb = memory_budget_mb
        
    def get_token_kv_size(self, precision_bits=16):
        # KV size per token = 2 (K and V) * num_layers * num_heads * head_dim * (precision_bits / 8) bytes
        return 2 * self.num_layers * self.num_heads * self.head_dim * (precision_bits / 8.0)
        
    def run_policy(self, name, precision_bits=16, eviction_policy="none", sliding_window_size=None, expert_guided=False):
        token_size_bytes = self.get_token_kv_size(precision_bits)
        token_size_mb = token_size_bytes / (1024.0 * 1024.0)
        
        # Track memory footprint and traffic
        memory_history = []
        total_traffic_mb = 0.0
        eviction_count = 0
        recomputation_count = 0
        
        # We model block-based memory
        block_size_mb = token_size_mb * self.block_size
        max_blocks = int(self.memory_budget_mb / block_size_mb)
        if max_blocks == 0:
            max_blocks = 1 # At least one block
            
        # Active requests, each has a list of allocated blocks
        # Each block is a dict: {"req_id": r, "block_idx": b, "last_accessed": t, "importance": imp}
        allocated_blocks = []
        
        # Prefill stage
        # Each request gets prompt_len / block_size blocks
        initial_blocks_per_req = int(np.ceil(self.prompt_len / self.block_size))
        
        t_global = 0
        for r in range(self.num_requests):
            for b in range(initial_blocks_per_req):
                # Generate importance value for expert-guided cache
                # Let's say earlier blocks have lower importance, but some blocks (e.g. system prompt) are hot
                importance = 1.0 if b == 0 else np.random.uniform(0.1, 0.9)
                allocated_blocks.append({
                    "req_id": r,
                    "block_idx": b,
                    "last_accessed": t_global,
                    "importance": importance
                })
                t_global += 1
                total_traffic_mb += block_size_mb # write traffic
                
        # Check for prefill OOM/eviction
        while len(allocated_blocks) > max_blocks:
            eviction_count += 1
            if eviction_policy == "none":
                # Out of memory! In simulation, we just force eviction but flag high risk/oom
                allocated_blocks.pop(0)
            elif eviction_policy == "lru":
                # Evict least recently accessed
                allocated_blocks.sort(key=lambda x: x["last_accessed"])
                allocated_blocks.pop(0)
            elif eviction_policy == "lfu" or expert_guided:
                # Evict least important block
                allocated_blocks.sort(key=lambda x: x["importance"])
                allocated_blocks.pop(0)
            elif eviction_policy == "sliding_window":
                # Evict oldest block
                allocated_blocks.sort(key=lambda x: (x["req_id"], x["block_idx"]))
                allocated_blocks.pop(0)
                
        # Generation stage (token-by-token)
        for step in range(self.gen_len):
            t_global += 1
            
            # Read all active blocks for attention computation
            # The number of blocks needed at step is ceil((prompt_len + step) / block_size)
            blocks_needed_per_req = int(np.ceil((self.prompt_len + step) / self.block_size))
            
            for r in range(self.num_requests):
                # Check if we need to allocate a new block (cross block boundary)
                if (self.prompt_len + step) % self.block_size == 0:
                    new_block_idx = blocks_needed_per_req - 1
                    importance = 1.0 if new_block_idx == 0 else np.random.uniform(0.1, 0.9)
                    
                    # Evict if full
                    if len(allocated_blocks) >= max_blocks:
                        eviction_count += 1
                        if eviction_policy == "none":
                            allocated_blocks.pop(0)
                        elif eviction_policy == "lru":
                            allocated_blocks.sort(key=lambda x: x["last_accessed"])
                            allocated_blocks.pop(0)
                        elif eviction_policy == "lfu" or expert_guided:
                            allocated_blocks.sort(key=lambda x: x["importance"])
                            allocated_blocks.pop(0)
                        elif eviction_policy == "sliding_window":
                            # Evict blocks outside the window
                            allocated_blocks.sort(key=lambda x: (x["req_id"], x["block_idx"]))
                            allocated_blocks.pop(0)
                            
                    allocated_blocks.append({
                        "req_id": r,
                        "block_idx": new_block_idx,
                        "last_accessed": t_global,
                        "importance": importance
                    })
                    total_traffic_mb += block_size_mb # write traffic
                    
                # Read traffic: we must read all currently cached blocks needed for this request
                req_blocks = [b for b in allocated_blocks if b["req_id"] == r]
                
                # Check if any required blocks are NOT in cache (meaning they were evicted)
                # Under sliding window, we simply don't read them (attention quality degradation)
                # Under LRU/LFU/none, we must recompute or load them (causing recomputation count)
                active_block_indices = {b["block_idx"] for b in req_blocks}
                for b_idx in range(blocks_needed_per_req):
                    if b_idx not in active_block_indices:
                        if eviction_policy == "sliding_window":
                            # In sliding window, we don't reload. We just compute with what we have.
                            # So no recomputation count, but approximation risk increases.
                            pass
                        else:
                            # Recompute or load block
                            recomputation_count += 1
                            total_traffic_mb += block_size_mb # load/recomputation transfer traffic
                    else:
                        # Block is in cache, read it
                        total_traffic_mb += block_size_mb
                        # Update last accessed
                        for b in req_blocks:
                            if b["block_idx"] == b_idx:
                                b["last_accessed"] = t_global
                                
            # Record current footprint
            current_footprint = len(allocated_blocks) * block_size_mb
            memory_history.append(current_footprint)
            
        peak_memory_mb = max(memory_history)
        avg_memory_mb = np.mean(memory_history)
        
        # Calculate compression ratio compared to full_kv at the end of generation
        full_kv_size_end_mb = self.num_requests * (self.prompt_len + self.gen_len) * self.get_token_kv_size(16) / (1024.0 * 1024.0)
        compression_ratio = full_kv_size_end_mb / peak_memory_mb
        
        # Estimate latency
        # Compute time + memory access time + recomputation/stall time
        # Let's say DRAM bandwidth is 500 GB/s. Transferring 1 MB takes 1 / 500,000 s = 2 us.
        dram_latency_ms = total_traffic_mb / 500.0 * 1000.0
        compute_ms = self.num_requests * self.gen_len * 1.5 # 1.5ms per token compute
        recompute_latency_ms = recomputation_count * 5.0 # 5ms per block recomputation
        estimated_latency_ms = compute_ms + dram_latency_ms + recompute_latency_ms
        
        # Determine risk
        if eviction_policy == "sliding_window":
            risk_score = 0.8
            risk_level = "High"
        elif precision_bits == 4:
            risk_score = 0.6
            risk_level = "Medium"
        elif precision_bits == 8:
            risk_score = 0.2
            risk_level = "Low"
        elif expert_guided:
            risk_score = 0.15
            risk_level = "Low"
        else:
            risk_score = 0.0 if eviction_count == 0 else 0.4
            risk_level = "Low" if risk_score < 0.2 else "Medium"
            
        return {
            "kv_memory_mb": float(peak_memory_mb),
            "kv_traffic_mb_per_token": float(total_traffic_mb / (self.num_requests * self.gen_len)),
            "compression_ratio": float(compression_ratio),
            "eviction_count": int(eviction_count),
            "recomputation_count": int(recomputation_count),
            "estimated_latency_ms": float(estimated_latency_ms),
            "speedup_vs_full_kv": 0.0, # calculated in runner
            "approximation_risk_score": float(risk_score),
            "quality_risk_level": risk_level
        }

def run_simulation(args):
    os.makedirs(args.output_dir, exist_ok=True)
    sim = KVCacheSimulator(
        num_requests=args.batch_size,
        prompt_len=args.prompt_len,
        gen_len=args.gen_len,
        num_layers=args.num_layers,
        num_heads=args.num_heads,
        head_dim=args.head_dim,
        block_size=args.block_size,
        memory_budget_mb=args.memory_budget
    )
    
    print("\n================== Running KV Cache Simulation ==================")
    print(f"Batch Size (Requests): {args.batch_size}")
    print(f"Prompt Length: {args.prompt_len}, Gen Length: {args.gen_len}")
    print(f"Model: {args.num_layers} Layers, {args.num_heads} KV Heads, {args.head_dim} Head Dim")
    print(f"Block Size: {args.block_size} tokens")
    print(f"Memory Budget: {args.memory_budget} MB")
    
    policies = {
        "full_kv": sim.run_policy("full_kv", precision_bits=16, eviction_policy="none"),
        "paged_kv": sim.run_policy("paged_kv", precision_bits=16, eviction_policy="lru"),
        "sliding_window": sim.run_policy("sliding_window", precision_bits=16, eviction_policy="sliding_window"),
        "uniform_int8_kv": sim.run_policy("uniform_int8_kv", precision_bits=8, eviction_policy="lru"),
        "uniform_int4_kv": sim.run_policy("uniform_int4_kv", precision_bits=4, eviction_policy="lru"),
        "recency_eviction": sim.run_policy("recency_eviction", precision_bits=16, eviction_policy="lru"),
        
        # Proposed policies
        "expert_guided_kv_priority": sim.run_policy("expert_guided_kv_priority", precision_bits=16, eviction_policy="lfu", expert_guided=True),
        "attention_locality_proxy": sim.run_policy("attention_locality_proxy", precision_bits=8, eviction_policy="lru"),
        "hybrid_recency_expert_policy": sim.run_policy("hybrid_recency_expert_policy", precision_bits=8, eviction_policy="lfu", expert_guided=True)
    }
    
    full_kv_lat = policies["full_kv"]["estimated_latency_ms"]
    for name, metrics in policies.items():
        metrics["speedup_vs_full_kv"] = float(full_kv_lat / metrics["estimated_latency_ms"])
        
    print("\nResults:")
    print(f"{'Policy':<30} | {'Peak Mem (MB)':<13} | {'Traffic/Tok':<11} | {'Evicts':<6} | {'Recomps':<7} | {'Latency (ms)':<12} | {'Speedup':<8} | {'Risk':<6}")
    print("-" * 115)
    for name, m in policies.items():
        print(f"{name:<30} | {m['kv_memory_mb']:13.1f} | {m['kv_traffic_mb_per_token']:11.2f} | {m['eviction_count']:6d} | {m['recomputation_count']:7d} | {m['estimated_latency_ms']:12.1f} | {m['speedup_vs_full_kv']:7.2f}x | {m['quality_risk_level']:<6}")
        
    results_json = os.path.join(args.output_dir, "kv_cache_results.json")
    with open(results_json, "w") as f:
        json.dump(policies, f, indent=4)
        
    df_res = pd.DataFrame.from_dict(policies, orient="index")
    results_csv = os.path.join(args.output_dir, "kv_cache_results.csv")
    df_res.to_csv(results_csv)
    print(f"\nResults saved to {results_json} and {results_csv}")
    
    # Plot Speedup and Peak Memory
    names = list(policies.keys())
    mem_usages = [p["kv_memory_mb"] for p in policies.values()]
    speedups = [p["speedup_vs_full_kv"] for p in policies.values()]
    
    fig, ax1 = plt.subplots(figsize=(10, 5))
    
    color = 'tab:blue'
    ax1.set_xlabel('KV Cache Policy')
    ax1.set_ylabel('Peak Memory (MB)', color=color)
    bars = ax1.bar(names, mem_usages, color=color, alpha=0.6, width=0.4, label='Peak Memory')
    ax1.tick_params(axis='y', labelcolor=color)
    plt.xticks(rotation=30, ha='right')
    
    # Add horizontal line for budget
    ax1.axhline(y=args.memory_budget, color='black', linestyle='--', alpha=0.7, label='Budget Limit')
    
    ax2 = ax1.twinx()
    color = 'tab:red'
    ax2.set_ylabel('Speedup (vs Full KV)', color=color)
    ax2.plot(names, speedups, color=color, marker='o', linewidth=2, label='Speedup')
    ax2.tick_params(axis='y', labelcolor=color)
    
    plt.title("KV Cache Policies: Peak Memory & Speedup Comparison")
    fig.tight_layout()
    plots_dir = os.path.join(args.output_dir, "plots")
    os.makedirs(plots_dir, exist_ok=True)
    plt.savefig(os.path.join(plots_dir, "policy_comparison.png"))
    plt.close()
    print(f"Comparison plot saved to {os.path.join(plots_dir, 'policy_comparison.png')}")
    
    return policies

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--batch_size", type=int, default=4)
    parser.add_argument("--prompt_len", type=int, default=512)
    parser.add_argument("--gen_len", type=int, default=128)
    parser.add_argument("--num_layers", type=int, default=32)
    parser.add_argument("--num_heads", type=int, default=8)
    parser.add_argument("--head_dim", type=int, default=128)
    parser.add_argument("--block_size", type=int, default=16)
    parser.add_argument("--memory_budget", type=float, default=256.0)
    parser.add_argument("--output_dir", type=str, default="/home/a/discover/kv_cache")
    args = parser.parse_args()
    
    run_simulation(args)
