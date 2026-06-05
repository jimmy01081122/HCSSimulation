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
    demand_queue.sort(key=lambda x: (x["cycle"], x["request_id"]))
    
    # Parse hint records
    hint_queue = []
    for h in hint_rows:
        hint_queue.append({
            "hint_cycle": int(h["hint_cycle"]),
            "target_cycle": int(h["target_cycle"]),
            "hint_expert_id": int(h["hint_expert_id"]),
            "hint_score": float(h["hint_score"])
        })
        
    # Reconstruct exactly what trace_to_testbench.py writes to memory
    max_c = 0
    for r in demand_queue:
        max_c = max(max_c, r["cycle"])
    for h in hint_queue:
        max_c = max(max_c, h["hint_cycle"])

    req_mem_valid = {}
    req_mem_expert_id = {}
    for r in demand_queue:
        req_mem_valid[r["cycle"]] = True
        req_mem_expert_id[r["cycle"]] = r["expert_id"]

    hint_mem_valid = {}
    hint_mem_expert_id = {}
    hint_mem_score = {}
    # Process hint rows in order to match overwriting behavior in trace_to_testbench.py
    for h in hint_queue:
        c = h["hint_cycle"]
        hint_mem_valid[c] = True
        hint_mem_expert_id[c] = h["hint_expert_id"]
        hint_mem_score[c] = h["hint_score"]

    # Simulation parameters
    policy = args.policy
    cache_size = args.cache_size
    dma_latency = args.dma_latency
    score_threshold = args.score_threshold
    
    cache_size_effective = 0 if policy == "no_cache" else cache_size
    prefetch_enable = (policy == "routing_aware_next")

    # Simulator State
    cache_tags = [0] * max(1, cache_size_effective)
    cache_valids = [False] * max(1, cache_size_effective)
    cache_prefetched = [False] * max(1, cache_size_effective)
    replacement_ptr = 0

    fifo = []
    fifo_out_has_missed = False

    dma_busy = False
    dma_active_expert_id = -1
    dma_active_is_prefetch = False
    dma_remaining_cycles = 0

    # Pipelined registers matching RTL
    dma_done_reg = False
    dma_done_expert_id_reg = -1
    dma_done_is_prefetch_reg = False

    in_flight_valid = False
    in_flight_expert_id = 0

    # Metrics Counters
    cnt_total_req = 0
    cnt_hit = 0
    cnt_miss = 0
    cnt_prefetch_issued = 0
    cnt_prefetch_hit = 0
    cnt_prefetch_useless = 0
    cnt_prefetch_dropped = 0
    cnt_prefetch_filtered = 0

    # Testbench-like driver state: initialized to 0
    testbench_cycle = 0
    req_valid = False
    req_expert_id = 0
    prefetch_hint_valid = False
    prefetch_hint_expert_id = 0
    prefetch_hint_score = 0.0

    clk_cycle = 0
    
    # Main simulation loop (clock-cycle accurate)
    while clk_cycle < 1000000:
        # 1. Combinational evaluations
        req_ready = len(fifo) < 8
        fifo_empty = len(fifo) == 0
        out_expert_id = fifo[0] if not fifo_empty else 0
        out_valid = not fifo_empty
        
        # Cache lookup
        lookup_hit = False
        lookup_hit_way = -1
        lookup_hit_prefetched = False
        if out_valid and cache_size_effective > 0:
            for w in range(cache_size_effective):
                if cache_valids[w] and cache_tags[w] == out_expert_id:
                    lookup_hit = True
                    lookup_hit_way = w
                    lookup_hit_prefetched = cache_prefetched[w]
                    break
                    
        # Under no_cache policy, FIFO pops when the demand DMA finishes
        if policy == "no_cache":
            fifo_pop = out_valid and dma_done_reg and not dma_done_is_prefetch_reg
        else:
            fifo_pop = out_valid and lookup_hit
        
        # DMA request arbitration (matching RTL top_moe_prefetch_system.v line 190)
        demand_wants_dma = out_valid and not lookup_hit and not (in_flight_valid and in_flight_expert_id == out_expert_id) and not dma_busy
        
        # Prefetch hint evaluation
        hint_in_cache = False
        if prefetch_hint_valid and cache_size_effective > 0:
            for w in range(cache_size_effective):
                if cache_valids[w] and cache_tags[w] == prefetch_hint_expert_id:
                    hint_in_cache = True
                    break
                    
        score_ok = (prefetch_hint_score >= score_threshold) if prefetch_hint_valid else False
        prefetch_wants_to_issue = prefetch_hint_valid and prefetch_enable and score_ok and not hint_in_cache
        hint_in_flight = in_flight_valid and (in_flight_expert_id == prefetch_hint_expert_id)
        
        prefetch_can_issue = prefetch_wants_to_issue and not dma_busy and not hint_in_flight and not demand_wants_dma
        prefetch_dropped = prefetch_wants_to_issue and not prefetch_can_issue
        
        dma_req_valid = demand_wants_dma or prefetch_can_issue
        dma_req_expert_id = out_expert_id if demand_wants_dma else prefetch_hint_expert_id
        dma_req_is_prefetch = False if demand_wants_dma else True
        dma_req_ready = not dma_busy
        
        # 2. Sequential state updates (Posedge clock)
        
        # Cache Fill (using the pipelined dma_done_reg)
        if dma_done_reg and cache_size_effective > 0:
            way = replacement_ptr
            if cache_valids[way]:
                if cache_prefetched[way]:
                    cnt_prefetch_useless += 1
            cache_tags[way] = dma_done_expert_id_reg
            cache_valids[way] = True
            cache_prefetched[way] = dma_done_is_prefetch_reg
            replacement_ptr = (replacement_ptr + 1) % cache_size_effective
            
        # Cache Tag Array Prefetch Bit Clear
        if fifo_pop and lookup_hit_prefetched and cache_size_effective > 0:
            cache_prefetched[lookup_hit_way] = False
            
        # FIFO Update
        fifo_push = req_valid and req_ready
        if fifo_push and fifo_pop:
            fifo.pop(0)
            fifo.append(req_expert_id)
        elif fifo_push:
            fifo.append(req_expert_id)
        elif fifo_pop:
            fifo.pop(0)
            
        # Counters Update
        if fifo_pop:
            if not fifo_out_has_missed:
                cnt_hit += 1
                cnt_total_req += 1
                if lookup_hit_prefetched:
                    cnt_prefetch_hit += 1
            fifo_out_has_missed = False
            
        if dma_req_valid and dma_req_ready:
            if not dma_req_is_prefetch:
                fifo_out_has_missed = True
                cnt_miss += 1
                cnt_total_req += 1
            else:
                cnt_prefetch_issued += 1
                
        if prefetch_hint_valid and prefetch_enable and not score_ok:
            cnt_prefetch_filtered += 1
            
        if prefetch_dropped:
            cnt_prefetch_dropped += 1
            
        # Testbench State update
        next_req_valid = req_valid
        next_req_expert_id = req_expert_id
        next_prefetch_hint_valid = prefetch_hint_valid
        next_prefetch_hint_expert_id = prefetch_hint_expert_id
        next_prefetch_hint_score = prefetch_hint_score
        next_testbench_cycle = testbench_cycle
        
        if req_valid and not req_ready:
            pass
        else:
            if testbench_cycle < 10000:
                next_req_valid = req_mem_valid.get(testbench_cycle, False)
                next_req_expert_id = req_mem_expert_id.get(testbench_cycle, 0)
                next_prefetch_hint_valid = hint_mem_valid.get(testbench_cycle, False)
                next_prefetch_hint_expert_id = hint_mem_expert_id.get(testbench_cycle, 0)
                next_prefetch_hint_score = hint_mem_score.get(testbench_cycle, 0.0)
                next_testbench_cycle = testbench_cycle + 1
            else:
                next_req_valid = False
                next_prefetch_hint_valid = False
                
        req_valid = next_req_valid
        req_expert_id = next_req_expert_id
        prefetch_hint_valid = next_prefetch_hint_valid
        prefetch_hint_expert_id = next_prefetch_hint_expert_id
        prefetch_hint_score = next_prefetch_hint_score
        testbench_cycle = next_testbench_cycle
        
        # In flight register update
        next_in_flight_valid = in_flight_valid
        next_in_flight_expert_id = in_flight_expert_id
        if dma_req_valid and dma_req_ready:
            next_in_flight_valid = True
            next_in_flight_expert_id = dma_req_expert_id
        elif dma_done_reg:
            next_in_flight_valid = False
            next_in_flight_expert_id = 0
            
        # DMA Register pipelining update
        next_dma_done_reg = False
        next_dma_done_expert_id_reg = -1
        next_dma_done_is_prefetch_reg = False
        
        if dma_busy:
            dma_remaining_cycles -= 1
            if dma_remaining_cycles == 0:
                next_dma_done_reg = True
                next_dma_done_expert_id_reg = dma_active_expert_id
                next_dma_done_is_prefetch_reg = dma_active_is_prefetch
                dma_busy = False
                
        if not dma_busy and dma_req_valid and dma_req_ready:
            dma_busy = True
            dma_active_expert_id = dma_req_expert_id
            dma_active_is_prefetch = dma_req_is_prefetch
            dma_remaining_cycles = dma_latency
            
        dma_done_reg = next_dma_done_reg
        dma_done_expert_id_reg = next_dma_done_expert_id_reg
        dma_done_is_prefetch_reg = next_dma_done_is_prefetch_reg
        
        in_flight_valid = next_in_flight_valid
        in_flight_expert_id = next_in_flight_expert_id
        
        clk_cycle += 1
        
        # Termination check matching testbench logic
        if cnt_total_req == len(demand_queue) and len(fifo) == 0 and not dma_busy:
            for _ in range(10):
                clk_cycle += 1
            break

    # Final sanity checks and self-consistency assertions
    assert cnt_hit + cnt_miss == cnt_total_req, f"Counter inconsistency: hit({cnt_hit}) + miss({cnt_miss}) != total({cnt_total_req})"
    assert cnt_total_req == len(demand_queue), f"Processed requests({cnt_total_req}) != trace requests({len(demand_queue)})"

    hit_rate = round(cnt_hit / cnt_total_req, 4) if cnt_total_req > 0 else 0.0

    # Print summary
    print(f"--- Simulation Summary ({policy}) ---")
    print(f"Total Requests:          {cnt_total_req}")
    print(f"Cache Hits:              {cnt_hit}")
    print(f"Cache Misses:            {cnt_miss}")
    print(f"Hit Rate:                {hit_rate * 100:.2f}%")
    print(f"Prefetch Issued Count:   {cnt_prefetch_issued}")
    print(f"Prefetch Useful Count:   {cnt_prefetch_hit}")
    print(f"Prefetch Useless Count:  {cnt_prefetch_useless}")
    print(f"Prefetch Dropped Count:  {cnt_prefetch_dropped}")
    print(f"Prefetch Filtered Count: {cnt_prefetch_filtered}")
    print(f"Estimated Cycles:        {clk_cycle}")
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
            cnt_total_req, cnt_hit, cnt_miss, hit_rate,
            cnt_prefetch_issued, cnt_prefetch_hit, cnt_prefetch_useless,
            cnt_prefetch_dropped, cnt_prefetch_filtered, clk_cycle
        ])
    print(f"Results saved to: {args.output}")

if __name__ == "__main__":
    run_simulation()
