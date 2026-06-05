#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import argparse
import csv

def parse_args():
    parser = argparse.ArgumentParser(description="MoE Expert Cache and Prefetch Simulator.")
    parser.add_argument("--trace", type=str, required=True, help="Path to demand trace CSV")
    parser.add_argument("--hint-trace", type=str, required=True, help="Path to hint trace CSV")
    parser.add_argument("--policy", type=str, default="routing_aware_next", choices=["no_cache", "fifo", "routing_aware_next"], help="Cache replacement/prefetch policy")
    parser.add_argument("--cache-size", type=int, default=4, help="Cache size (number of expert ways)")
    parser.add_argument("--dma-latency", type=int, default=10, help="DMA latency in cycles")
    parser.add_argument("--score-threshold", type=float, default=0.1, help="Score threshold for prefetch decision")
    parser.add_argument("--output", type=str, required=True, help="Path to output statistics CSV")
    return parser.parse_args()

def load_csv(path):
    records = []
    if not os.path.exists(path):
        return records
    with open(path, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            records.append(row)
    return records

def run_simulation():
    args = parse_args()
    
    demand_rows = load_csv(args.trace)
    hint_rows = load_csv(args.hint_trace)
    
    # Parse demand records
    demand_queue = []
    for r in demand_rows:
        demand_queue.append({
            "cycle": int(r["cycle"]),
            "request_id": int(r["request_id"]),
            "expert_id": int(r["expert_id"]),
            "router_score": float(r["router_score"])
        })
    # Sort just in case
    demand_queue.sort(key=lambda x: (x["cycle"], x["request_id"]))
    
    # Parse hint records
    hint_by_cycle = {}
    for h in hint_rows:
        hc = int(h["hint_cycle"])
        hint_rec = {
            "hint_cycle": hc,
            "target_cycle": int(h["target_cycle"]),
            "hint_expert_id": int(h["hint_expert_id"]),
            "hint_score": float(h["hint_score"])
        }
        if hc not in hint_by_cycle:
            hint_by_cycle[hc] = []
        hint_by_cycle[hc].append(hint_rec)
        
    # Simulator State
    cache = set()
    fifo_queue = [] # Oldest loaded experts at the front
    prefetched = {} # expert_id -> bool
    accessed_after_prefetch = {} # expert_id -> bool
    
    # DMA State (Single channel, fixed latency)
    dma_in_flight = False
    dma_expert_id = -1
    dma_is_prefetch = False
    dma_remaining_cycles = 0
    
    # Metrics
    total_requests = 0
    cache_hits = 0
    cache_misses = 0
    prefetch_issued_count = 0
    prefetch_useful_count = 0
    prefetch_useless_count = 0
    prefetch_dropped_count = 0
    prefetch_filtered_count = 0
    
    policy = args.policy
    cache_size = args.cache_size
    dma_latency = args.dma_latency
    score_threshold = args.score_threshold
    
    c = 0
    demand_idx = 0
    
    # Main simulation loop
    while demand_idx < len(demand_queue):
        # 1. DMA Update
        if dma_in_flight:
            dma_remaining_cycles -= 1
            if dma_remaining_cycles == 0:
                # DMA finishes and triggers cache fill
                if dma_expert_id not in cache:
                    if len(cache) >= cache_size:
                        # Eviction (FIFO replacement)
                        victim = fifo_queue.pop(0)
                        cache.remove(victim)
                        if prefetched.get(victim, False) and not accessed_after_prefetch.get(victim, False):
                            prefetch_useless_count += 1
                            
                    cache.add(dma_expert_id)
                    fifo_queue.append(dma_expert_id)
                    prefetched[dma_expert_id] = dma_is_prefetch
                    accessed_after_prefetch[dma_expert_id] = False
                dma_in_flight = False
                
        # 2. Demand Request Processing
        # Check if there is an active demand request at this cycle
        demand_miss_wants_dma = False
        if demand_idx < len(demand_queue) and c >= demand_queue[demand_idx]["cycle"]:
            req = demand_queue[demand_idx]
            exp_id = req["expert_id"]
            total_requests += 1
            
            if policy == "no_cache":
                # no_cache policy: always miss, starts a DMA every request (sequentialized)
                if not dma_in_flight:
                    dma_in_flight = True
                    dma_expert_id = exp_id
                    dma_is_prefetch = False
                    dma_remaining_cycles = dma_latency
                    cache_misses += 1
                else:
                    # Wait for DMA to complete
                    pass
                # Check completion inside loop
                if dma_in_flight and dma_expert_id == exp_id and dma_remaining_cycles == 1:
                    # Complete on the next cycle, so we advance demand_idx next cycle
                    pass
                elif not dma_in_flight:
                    # Satisfied immediately if latency was 0 (or finished)
                    demand_idx += 1
            else:
                if exp_id in cache:
                    # Hit!
                    cache_hits += 1
                    if prefetched.get(exp_id, False) and not accessed_after_prefetch.get(exp_id, False):
                        prefetch_useful_count += 1
                        accessed_after_prefetch[exp_id] = True
                    demand_idx += 1
                else:
                    # Miss!
                    # Check if currently being fetched by in-flight DMA
                    if dma_in_flight and dma_expert_id == exp_id:
                        # Stalling, waiting for this DMA to finish
                        pass
                    else:
                        # Must start a demand miss DMA
                        if not dma_in_flight:
                            dma_in_flight = True
                            dma_expert_id = exp_id
                            dma_is_prefetch = False
                            dma_remaining_cycles = dma_latency
                            cache_misses += 1
                        else:
                            # DMA is busy with another expert, must stall
                            demand_miss_wants_dma = True
                            
        # 3. Prefetch Hint Processing
        if policy == "routing_aware_next" and c in hint_by_cycle:
            for hint in hint_by_cycle[c]:
                h_exp = hint["hint_expert_id"]
                h_score = hint["hint_score"]
                
                if h_score < score_threshold:
                    prefetch_filtered_count += 1
                    continue
                    
                if h_exp in cache:
                    # Already in cache, skip
                    continue
                    
                # Check duplicate prefetch or busy conditions
                in_flight = (dma_in_flight and dma_expert_id == h_exp)
                
                # Check if demand miss wants to start DMA at this cycle
                # (demand miss has priority over prefetch)
                if dma_in_flight or in_flight or demand_miss_wants_dma:
                    prefetch_dropped_count += 1
                else:
                    # Start prefetch DMA!
                    dma_in_flight = True
                    dma_expert_id = h_exp
                    dma_is_prefetch = True
                    dma_remaining_cycles = dma_latency
                    prefetch_issued_count += 1
                    
        c += 1
        # Safety check to prevent infinite loops
        if c > 1000000:
            print("ERROR: Simulation timeout (possible infinite loop).", file=sys.stderr)
            break
            
    estimated_cycles = c
    hit_rate = round(cache_hits / total_requests, 4) if total_requests > 0 else 0.0
    
    # Print summary
    print(f"--- Simulation Summary ({policy}) ---")
    print(f"Total Requests:          {total_requests}")
    print(f"Cache Hits:              {cache_hits}")
    print(f"Cache Misses:            {cache_misses}")
    print(f"Hit Rate:                {hit_rate * 100:.2f}%")
    print(f"Prefetch Issued Count:   {prefetch_issued_count}")
    print(f"Prefetch Useful Count:   {prefetch_useful_count}")
    print(f"Prefetch Useless Count:  {prefetch_useless_count}")
    print(f"Prefetch Dropped Count:  {prefetch_dropped_count}")
    print(f"Prefetch Filtered Count: {prefetch_filtered_count}")
    print(f"Estimated Cycles:        {estimated_cycles}")
    print("-------------------------------------")
    
    # Save output
    os.makedirs(os.path.dirname(os.path.abspath(args.output)), exist_ok=True)
    with open(args.output, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow([
            "total_requests", "cache_hits", "cache_misses", "hit_rate",
            "prefetch_issued_count", "prefetch_useful_count", "prefetch_useless_count",
            "prefetch_dropped_count", "prefetch_filtered_count", "estimated_cycles"
        ])
        writer.writerow([
            total_requests, cache_hits, cache_misses, hit_rate,
            prefetch_issued_count, prefetch_useful_count, prefetch_useless_count,
            prefetch_dropped_count, prefetch_filtered_count, estimated_cycles
        ])
    print(f"Results saved to: {args.output}")

if __name__ == "__main__":
    run_simulation()
