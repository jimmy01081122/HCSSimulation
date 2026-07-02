#!/usr/bin/env python3
import argparse
import json
import os
import sys
import pandas as pd
import numpy as np

# Adjust path to import ExpertCache
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from sim.expert_cache import ExpertCache

def parse_args():
    parser = argparse.ArgumentParser(description="MENA MoE System Simulator")
    parser.add_argument("--trace", type=str, required=True, help="Path to routing trace JSONL")
    parser.add_argument("--num_experts", type=int, default=8, help="Total number of experts")
    parser.add_argument("--top_k", type=int, default=2, help="Top-k selection count")
    parser.add_argument("--expert_cache_size", type=int, default=4, help="Cache capacity in experts")
    parser.add_argument("--policy", type=str, default="lru", choices=["none", "lru", "lfu", "history"], help="Cache replacement policy")
    parser.add_argument("--dispatch_policy", type=str, default="expert_major", 
                        choices=["token_order", "expert_major", "longest_queue_first", "hot_expert_first"], 
                        help="Token dispatch policy")
    parser.add_argument("--bandwidth_bytes_per_cycle", type=float, default=64.0, help="DRAM memory bandwidth bytes/cycle")
    parser.add_argument("--expert_size_bytes", type=int, default=2000000, help="Expert weight size in bytes")
    parser.add_argument("--token_size_bytes", type=int, default=4096, help="Token activation size in bytes")
    parser.add_argument("--exec_cycles_per_token", type=int, default=100, help="Execution cycles per token on NPU")
    parser.add_argument("--double_buffering", action="store_true", help="Enable double buffering (overlap weight transfer and execution)")
    parser.add_argument("--output_csv", type=str, default=None, help="Append summary to this CSV file")
    return parser.parse_args()

def load_trace(trace_path):
    trace_by_layer = {}
    total_tokens = 0
    with open(trace_path, 'r') as f:
        for line in f:
            data = json.loads(line)
            layer = data["layer"]
            token = data["token"]
            topk = data["topk_experts"]
            
            if layer not in trace_by_layer:
                trace_by_layer[layer] = []
            trace_by_layer[layer].append({
                "token": token,
                "topk_experts": topk
            })
            if layer == 0:
                total_tokens += 1
    return trace_by_layer, total_tokens

def simulate(args):
    trace_by_layer, tokens_per_layer = load_trace(args.trace)
    num_layers = len(trace_by_layer)
    
    # Initialize cache
    cache = ExpertCache(cache_size=args.expert_cache_size, policy=args.policy, num_experts=args.num_experts)
    
    transfer_cycles_per_miss = args.expert_size_bytes / args.bandwidth_bytes_per_cycle
    
    total_hits = 0
    total_misses = 0
    total_transfer_cycles = 0.0
    total_exec_cycles = 0.0
    current_time_cycles = 0.0
    
    # Queue depth trackers
    queue_depths = []
    token_wait_times = []
    
    # Process layer by layer (coordination boundary)
    for layer_idx in sorted(trace_by_layer.keys()):
        tokens = trace_by_layer[layer_idx]
        
        # Build expert queues for this layer
        expert_queues = {i: [] for i in range(args.num_experts)}
        for t_info in tokens:
            t_id = t_info["token"]
            for exp in t_info["topk_experts"]:
                expert_queues[exp].append(t_id)
                
        # Record queue depths for active queues
        for exp, q in expert_queues.items():
            if len(q) > 0:
                queue_depths.append(len(q))
                
        # Determine the order in which we will process the experts
        if args.dispatch_policy == "token_order":
            # Process in the order of token arrival.
            # We map token arrival to the experts they need, and execute those experts in order of token appearance.
            expert_order = []
            seen = set()
            for t_info in tokens:
                for exp in t_info["topk_experts"]:
                    if exp not in seen and len(expert_queues[exp]) > 0:
                        seen.add(exp)
                        expert_order.append(exp)
                        
        elif args.dispatch_policy == "expert_major":
            # Simple numeric expert order (0, 1, 2...)
            expert_order = [i for i in range(args.num_experts) if len(expert_queues[i]) > 0]
            
        elif args.dispatch_policy == "longest_queue_first":
            # Sort experts by descending queue length
            active_experts = [i for i in range(args.num_experts) if len(expert_queues[i]) > 0]
            expert_order = sorted(active_experts, key=lambda x: len(expert_queues[x]), reverse=True)
            
        elif args.dispatch_policy == "hot_expert_first":
            # Sort by descending queue length (which represents hotness in this layer)
            active_experts = [i for i in range(args.num_experts) if len(expert_queues[i]) > 0]
            expert_order = sorted(active_experts, key=lambda x: len(expert_queues[x]), reverse=True)
            
        else:
            expert_order = [i for i in range(args.num_experts) if len(expert_queues[i]) > 0]
            
        # Process each expert in the chosen order
        layer_start_cycles = current_time_cycles
        
        if args.double_buffering:
            npu_ready_time = current_time_cycles
            dma_ready_time = current_time_cycles
            
            for exp in expert_order:
                q = expert_queues[exp]
                num_tokens_in_q = len(q)
                if num_tokens_in_q == 0:
                    continue
                    
                # Request expert from cache
                hit, evicted, prefetched, state = cache.request(exp)
                
                # If hit, 0 transfer delay. If miss, we pay transfer_cycles_per_miss
                if args.expert_cache_size > 0:
                    if hit:
                        total_hits += 1
                        transfer_time = 0.0
                    else:
                        total_misses += 1
                        transfer_time = transfer_cycles_per_miss
                        total_transfer_cycles += transfer_cycles_per_miss
                else:
                    total_misses += 1
                    transfer_time = transfer_cycles_per_miss
                    total_transfer_cycles += transfer_cycles_per_miss
                    
                # Double buffering timeline
                transfer_start = dma_ready_time
                transfer_finish = transfer_start + transfer_time
                dma_ready_time = transfer_finish
                
                exec_start = max(npu_ready_time, transfer_finish)
                exec_time = num_tokens_in_q * args.exec_cycles_per_token
                exec_finish = exec_start + exec_time
                npu_ready_time = exec_finish
                
                # Record token wait times
                expert_start_time = exec_start
                for token_id in q:
                    wait_time = expert_start_time - layer_start_cycles
                    token_wait_times.append(wait_time)
                    
                total_exec_cycles += exec_time
                
            # The layer ends when NPU completes execution of all experts
            current_time_cycles = npu_ready_time
            
        else:
            # Single Buffering (Original serialized logic)
            for exp in expert_order:
                q = expert_queues[exp]
                num_tokens_in_q = len(q)
                if num_tokens_in_q == 0:
                    continue
                    
                # Request expert from cache
                hit, evicted, prefetched, state = cache.request(exp)
                
                # If hit, 0 transfer delay. If miss, we pay transfer_cycles_per_miss
                if args.expert_cache_size > 0:
                    if hit:
                        total_hits += 1
                    else:
                        total_misses += 1
                        current_time_cycles += transfer_cycles_per_miss
                        total_transfer_cycles += transfer_cycles_per_miss
                else:
                    # Cache size 0: always miss, always transfer
                    total_misses += 1
                    current_time_cycles += transfer_cycles_per_miss
                    total_transfer_cycles += transfer_cycles_per_miss
                    
                # Processing time for the tokens in the queue
                expert_start_time = current_time_cycles
                
                for token_id in q:
                    wait_time = expert_start_time - layer_start_cycles
                    token_wait_times.append(wait_time)
                    
                exec_time = num_tokens_in_q * args.exec_cycles_per_token
                current_time_cycles += exec_time
                total_exec_cycles += exec_time
            
    # Calculate statistics
    total_tokens_all = sum(len(trace_by_layer[l]) for l in trace_by_layer)
    total_requests = total_hits + total_misses
    hit_rate = total_hits / total_requests if total_requests > 0 else 0.0
    
    max_q_depth = max(queue_depths) if queue_depths else 0
    avg_q_depth = np.mean(queue_depths) if queue_depths else 0.0
    
    p50_wait = np.percentile(token_wait_times, 50) if token_wait_times else 0.0
    p95_wait = np.percentile(token_wait_times, 95) if token_wait_times else 0.0
    
    # Output to stdout
    print(f"Simulation completed for: {args.trace}")
    print(f"  Total tokens processed: {total_tokens_all}")
    print(f"  Total dispatch groups (layers): {num_layers}")
    print(f"  Max queue depth: {max_q_depth}")
    print(f"  Average queue depth: {avg_q_depth:.2f}")
    print(f"  Expert cache hit rate: {hit_rate:.4%}")
    print(f"  Expert cache misses: {total_misses}")
    print(f"  Estimated transfer bytes: {total_misses * args.expert_size_bytes:,} bytes")
    print(f"  Estimated transfer cycles: {total_transfer_cycles:.1f}")
    print(f"  Estimated dispatch cycles (computation): {total_exec_cycles:.1f}")
    print(f"  Estimated total cycles: {current_time_cycles:.1f}")
    print(f"  P50 queue wait time: {p50_wait:.1f} cycles")
    print(f"  P95 queue wait time: {p95_wait:.1f} cycles")
    print("-" * 50)
    
    summary_data = {
        "Trace": os.path.basename(args.trace),
        "Num_Experts": args.num_experts,
        "Cache_Size": args.expert_cache_size,
        "Cache_Policy": args.policy,
        "Dispatch_Policy": args.dispatch_policy,
        "Double_Buffering": args.double_buffering,
        "Hit_Rate": hit_rate,
        "Miss_Count": total_misses,
        "Transfer_Cycles": total_transfer_cycles,
        "Exec_Cycles": total_exec_cycles,
        "Total_Cycles": current_time_cycles,
        "P50_Wait": p50_wait,
        "P95_Wait": p95_wait,
        "Max_Queue_Depth": max_q_depth,
        "Avg_Queue_Depth": avg_q_depth
    }
    
    if args.output_csv:
        os.makedirs(os.path.dirname(args.output_csv), exist_ok=True)
        df = pd.DataFrame([summary_data])
        if os.path.exists(args.output_csv):
            df.to_csv(args.output_csv, mode='a', header=False, index=False)
        else:
            df.to_csv(args.output_csv, index=False)
            
    return summary_data

if __name__ == "__main__":
    main_args = parse_args()
    simulate(main_args)
