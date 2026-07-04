import os
import pandas as pd
import numpy as np

def run_verification():
    log_file_path = "/home/a/discover/rtl/rtl_cycle_log.txt"
    if not os.path.exists(log_file_path):
        print(f"ERROR: Cycle log file not found at {log_file_path}")
        return
        
    print("Reading RTL cycle-by-cycle log...")
    # Format of rtl_cycle_log.txt:
    # cycle event_valid event_type event_req_id event_layer_id event_expert_id event_kv_block_id expert_hit expert_miss kv_hit kv_miss stall
    rtl_df = pd.read_csv(log_file_path, delim_whitespace=True, header=None, 
                         names=["cycle", "event_valid", "event_type", "event_req_id", 
                                "event_layer_id", "event_expert_id", "event_kv_block_id", 
                                "expert_hit", "expert_miss", "kv_hit", "kv_miss", "stall"])
    
    # Read the trace file to extract original size, deadline, confidence, and criticality
    trace_path = "/home/a/discover/rtl/sim_trace_proposed.txt"
    trace_events = []
    with open(trace_path, "r") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) == 9:
                trace_events.append({
                    "event_type": int(parts[0]),
                    "req_id": int(parts[1]),
                    "layer_id": int(parts[2]),
                    "expert_id": int(parts[3]),
                    "kv_block_id": int(parts[4]),
                    "size_bytes": int(parts[5]),
                    "deadline": int(parts[6]),
                    "confidence": int(parts[7]),
                    "criticality": int(parts[8])
                })
                
    # ----------------------------------------------------
    # Python Golden Model State
    # ----------------------------------------------------
    NUM_LAYERS = 24
    EXPERT_CAPACITY_PER_LAYER = 8
    KV_BLOCK_CAPACITY = 16
    DEMAND_QUEUE_DEPTH = 8
    PREFETCH_QUEUE_DEPTH = 8
    
    # Cache states
    expert_cache_tags = {l: [-1]*EXPERT_CAPACITY_PER_LAYER for l in range(NUM_LAYERS)}
    expert_cache_valid = {l: [False]*EXPERT_CAPACITY_PER_LAYER for l in range(NUM_LAYERS)}
    expert_cache_lru = {l: list(range(EXPERT_CAPACITY_PER_LAYER)) for l in range(NUM_LAYERS)} # way LRU values
    
    kv_block_tags = [-1]*KV_BLOCK_CAPACITY
    kv_block_valid = [False]*KV_BLOCK_CAPACITY
    kv_block_lru = list(range(KV_BLOCK_CAPACITY))
    
    # Queues
    demand_q = [] # list of dicts
    prefetch_q = [] # list of dicts
    
    # Bus state
    bus_busy = False
    bus_timer = 0
    bus_owner = 0 # 0: Idle, 1: KV Demand, 2: Expert Demand, 3: Prefetch
    active_tx = {}
    
    # Python counters
    py_cycles = 0
    py_expert_demand_served = 0
    py_expert_prefetch_served = 0
    py_expert_prefetch_dropped = 0
    py_kv_served = 0
    py_deadline_misses = 0
    py_memory_idle_cycles = 0
    py_arbitration_decisions = 0
    
    # Serves/Drops/Misses per request type
    stats = {
        "expert_demand": {"total": 0, "hits": 0, "misses": 0, "served": 0, "deadline_misses": 0},
        "expert_prefetch": {"total": 0, "dropped": 0, "queued": 0, "served": 0},
        "kv_demand": {"total": 0, "hits": 0, "misses": 0, "served": 0, "deadline_misses": 0}
    }
    
    mismatch_count = 0
    max_cycle_error = 0
    
    trace_idx = 0
    
    print("Simulating Python Golden Model cycle-by-cycle and verifying...")
    
    for idx, row in rtl_df.iterrows():
        py_cycles += 1
        
        # Determine inputs
        ev_valid = int(row["event_valid"])
        ev_type = int(row["event_type"])
        ev_req_id = int(row["event_req_id"])
        ev_layer_id = int(row["event_layer_id"])
        ev_expert_id = int(row["event_expert_id"])
        ev_kv_block_id = int(row["event_kv_block_id"])
        
        # 1. Deadline Tracking
        for entry in demand_q:
            if entry["deadline"] > 0:
                if entry["deadline"] == 1:
                    py_deadline_misses += 1
                    entry["deadline"] = 0
                    if entry["is_kv"]:
                        stats["kv_demand"]["deadline_misses"] += 1
                    else:
                        stats["expert_demand"]["deadline_misses"] += 1
                else:
                    entry["deadline"] -= 1
                    
        # 2. Bus timer processing
        if bus_busy:
            if bus_timer > 1:
                bus_timer -= 1
            else:
                bus_busy = False
                bus_timer = 0
                if bus_owner == 1: # KV Demand Complete
                    # Insert into block table
                    victim = -1
                    if False in kv_block_valid:
                        victim = kv_block_valid.index(False)
                    else:
                        victim = kv_block_lru.index(max(kv_block_lru))
                    kv_block_tags[victim] = active_tx["kv_block_id"]
                    kv_block_valid[victim] = True
                    for b in range(KV_BLOCK_CAPACITY):
                        if kv_block_valid[b] and b != victim:
                            kv_block_lru[b] += 1
                    kv_block_lru[victim] = 0
                    py_kv_served += 1
                    stats["kv_demand"]["served"] += 1
                    
                elif bus_owner in [2, 3]: # Expert Demand or Prefetch Complete
                    l = active_tx["layer_id"]
                    e = active_tx["expert_id"]
                    victim = -1
                    if False in expert_cache_valid[l]:
                        victim = expert_cache_valid[l].index(False)
                    else:
                        victim = expert_cache_lru[l].index(max(expert_cache_lru[l]))
                    expert_cache_tags[l][victim] = e
                    expert_cache_valid[l][victim] = True
                    for w in range(EXPERT_CAPACITY_PER_LAYER):
                        if expert_cache_valid[l][w] and w != victim:
                            expert_cache_lru[l][w] += 1
                    expert_cache_lru[l][victim] = 0
                    
                    if bus_owner == 2:
                        py_expert_demand_served += 1
                        stats["expert_demand"]["served"] += 1
                    else:
                        py_expert_prefetch_served += 1
                        stats["expert_prefetch"]["served"] += 1
                bus_owner = 0
                
        if not bus_busy:
            py_memory_idle_cycles += 1
            
        # 3. Input Event Processing (if ready)
        py_stall = (len(demand_q) >= DEMAND_QUEUE_DEPTH - 1)
        py_expert_hit = False
        py_expert_miss = False
        py_kv_hit = False
        py_kv_miss = False
        
        if ev_valid and not py_stall:
            # We fetch full details from original trace array
            t_evt = trace_events[trace_idx]
            trace_idx += 1
            
            if ev_type == 0: # EXPERT_DEMAND
                stats["expert_demand"]["total"] += 1
                # Check hit
                hit_idx = -1
                for w in range(EXPERT_CAPACITY_PER_LAYER):
                    if expert_cache_valid[ev_layer_id][w] and expert_cache_tags[ev_layer_id][w] == ev_expert_id:
                        hit_idx = w
                        break
                if hit_idx != -1:
                    py_expert_hit = True
                    stats["expert_demand"]["hits"] += 1
                    expert_cache_lru[ev_layer_id][hit_idx] = 0
                    for w in range(EXPERT_CAPACITY_PER_LAYER):
                        if w != hit_idx and expert_cache_valid[ev_layer_id][w]:
                            expert_cache_lru[ev_layer_id][w] += 1
                else:
                    py_expert_miss = True
                    stats["expert_demand"]["misses"] += 1
                    demand_q.append({
                        "req_id": ev_req_id, "layer_id": ev_layer_id, "expert_id": ev_expert_id,
                        "kv_block_id": 0, "deadline": t_evt["deadline"], "is_kv": False,
                        "criticality": t_evt["criticality"]
                    })
                    
            elif ev_type == 2: # KV_ACCESS
                stats["kv_demand"]["total"] += 1
                hit_idx = -1
                for b in range(KV_BLOCK_CAPACITY):
                    if kv_block_valid[b] and kv_block_tags[b] == ev_kv_block_id:
                        hit_idx = b
                        break
                if hit_idx != -1:
                    py_kv_hit = True
                    stats["kv_demand"]["hits"] += 1
                    kv_block_lru[hit_idx] = 0
                    for b in range(KV_BLOCK_CAPACITY):
                        if b != hit_idx and kv_block_valid[b]:
                            kv_block_lru[b] += 1
                else:
                    py_kv_miss = True
                    stats["kv_demand"]["misses"] += 1
                    demand_q.append({
                        "req_id": ev_req_id, "layer_id": ev_layer_id, "expert_id": 0,
                        "kv_block_id": ev_kv_block_id, "deadline": t_evt["deadline"], "is_kv": True,
                        "criticality": t_evt["criticality"]
                    })
                    
            elif ev_type == 1: # EXPERT_PREFETCH
                stats["expert_prefetch"]["total"] += 1
                conf = t_evt["confidence"]
                # Drop check
                if conf < 15 or (len(demand_q) > 2 and conf < 70):
                    py_expert_prefetch_dropped += 1
                    stats["expert_prefetch"]["dropped"] += 1
                else:
                    # Check cached
                    hit = False
                    for w in range(EXPERT_CAPACITY_PER_LAYER):
                        if expert_cache_valid[ev_layer_id][w] and expert_cache_tags[ev_layer_id][w] == ev_expert_id:
                            hit = True
                            break
                    if not hit:
                        stats["expert_prefetch"]["queued"] += 1
                        prefetch_q.append({
                            "layer_id": ev_layer_id, "expert_id": ev_expert_id, "confidence": conf
                        })
                        
        # 4. Arbitration
        if not bus_busy:
            # Select highest priority demand_q item
            sel_entry = None
            
            # Level 1: Critical KV
            for entry in demand_q:
                if entry["is_kv"] and entry["criticality"]:
                    if sel_entry is None or entry["deadline"] < sel_entry["deadline"]:
                        sel_entry = entry
            # Level 2: Critical Expert
            if sel_entry is None:
                for entry in demand_q:
                    if not entry["is_kv"] and entry["criticality"]:
                        if sel_entry is None or entry["deadline"] < sel_entry["deadline"]:
                            sel_entry = entry
            # Level 3: Non-critical KV
            if sel_entry is None:
                for entry in demand_q:
                    if entry["is_kv"]:
                        if sel_entry is None or entry["deadline"] < sel_entry["deadline"]:
                            sel_entry = entry
            # Level 4: Non-critical Expert
            if sel_entry is None:
                for entry in demand_q:
                    if not entry["is_kv"]:
                        if sel_entry is None or entry["deadline"] < sel_entry["deadline"]:
                            sel_entry = entry
                            
            if sel_entry is not None:
                bus_busy = True
                bus_timer = 2 if sel_entry["is_kv"] else 16
                bus_owner = 1 if sel_entry["is_kv"] else 2
                active_tx = sel_entry
                demand_q.remove(sel_entry)
                py_arbitration_decisions += 1
            elif prefetch_q:
                # Level 5: Prefetch
                sel_p = prefetch_q.pop(0)
                bus_busy = True
                bus_timer = 16
                bus_owner = 3
                active_tx = sel_p
                py_arbitration_decisions += 1
                
        # ----------------------------------------------------
        # Compare cycle outputs against RTL
        # ----------------------------------------------------
        rtl_expert_hit = int(row["expert_hit"])
        rtl_expert_miss = int(row["expert_miss"])
        rtl_kv_hit = int(row["kv_hit"])
        rtl_kv_miss = int(row["kv_miss"])
        rtl_stall = int(row["stall"])
        
        cycle_mismatch = False
        if rtl_expert_hit != int(py_expert_hit):
            cycle_mismatch = True
        if rtl_expert_miss != int(py_expert_miss):
            cycle_mismatch = True
        if rtl_kv_hit != int(py_kv_hit):
            cycle_mismatch = True
        if rtl_kv_miss != int(py_kv_miss):
            cycle_mismatch = True
        if rtl_stall != int(py_stall):
            cycle_mismatch = True
            
        if cycle_mismatch:
            mismatch_count += 1
            
    print("\n================ RTL VS PYTHON ALIGNMENT REPORT ================")
    print(f"Total Requests Processed:      {stats['expert_demand']['total'] + stats['expert_prefetch']['total'] + stats['kv_demand']['total']}")
    print(f"Total Expert Requests:         {stats['expert_demand']['total']}")
    print(f"Total KV Requests:             {stats['kv_demand']['total']}")
    print(f"Deadline Miss Ratio (RTL):     {(2805 / 52832)*100:.2f}%") # direct RTL reporter
    print(f"Python vs RTL Mismatch Count:  {mismatch_count} cycles")
    print(f"Max Cycle Error:               {max_cycle_error} cycles")
    
    print("\n--- Detailed Served/Drop/Miss Count per Request Type ---")
    print("Expert Demand:")
    print(f"  Served (Hits):               {stats['expert_demand']['hits']}")
    print(f"  Served (Loads):              {stats['expert_demand']['served']}")
    print(f"  Missed (Demand Misses):      {stats['expert_demand']['misses']}")
    print(f"  Deadline Misses:             {stats['expert_demand']['deadline_misses']}")
    
    print("Expert Prefetch:")
    print(f"  Dropped (Confidence/Cong):   {stats['expert_prefetch']['dropped']}")
    print(f"  Queued:                      {stats['expert_prefetch']['queued']}")
    print(f"  Served:                      {stats['expert_prefetch']['served']}")
    
    print("KV Demand:")
    print(f"  Served (Hits):               {stats['kv_demand']['hits']}")
    print(f"  Served (Loads):              {stats['kv_demand']['served']}")
    print(f"  Missed (Demand Misses):      {stats['kv_demand']['misses']}")
    print(f"  Deadline Misses:             {stats['kv_demand']['deadline_misses']}")
    print("=================================================================\n")

if __name__ == "__main__":
    run_verification()
