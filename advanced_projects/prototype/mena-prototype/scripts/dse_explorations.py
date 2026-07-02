#!/usr/bin/env python3
"""
dse_explorations.py
Runs automated parameter sweeps for MoE Design Space Exploration (DSE).
Saves results to a structured CSV file for analysis.
"""

import sys
import os
import json
import pandas as pd
import numpy as np
from pathlib import Path

# Adjust path to import simulator
REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO_ROOT))
from sim.mena_sim import simulate

class ArgsObj:
    def __init__(self, **kwargs):
        self.trace = str(REPO_ROOT / "traces" / "synth_zipf_E8_T128_K2.jsonl")
        self.num_experts = 8
        self.top_k = 2
        self.expert_cache_size = 4
        self.policy = "lru"
        self.dispatch_policy = "expert_major"
        self.bandwidth_bytes_per_cycle = 64.0
        self.expert_size_bytes = 2000000
        self.token_size_bytes = 4096
        self.exec_cycles_per_token = 100
        self.double_buffering = False
        self.output_csv = None

        for k, v in kwargs.items():
            setattr(self, k, v)

def run_scheduling_sweep():
    print("\n--- Running Scheduling Tradeoff Sweep (Q3) ---")
    traces = ["synth_zipf_E8_T128_K2.jsonl", "synth_uniform_E8_T128_K2.jsonl"]
    policies = ["token_order", "expert_major", "longest_queue_first"]
    
    rows = []
    for t in traces:
        for p in policies:
            args = ArgsObj(
                trace=str(REPO_ROOT / "traces" / t),
                dispatch_policy=p,
                policy="lru"
            )
            res = simulate(args)
            rows.append(res)
    return pd.DataFrame(rows)

def run_cache_sweep():
    print("\n--- Running Cache Capacity Sweep (Q4) ---")
    traces = ["synth_zipf_E8_T128_K2.jsonl", "synth_hot_E16_T512_K2.jsonl"]
    sizes = [0, 2, 4, 8, 16]
    
    rows = []
    for t in traces:
        num_exp = 16 if "E16" in t else 8
        for s in sizes:
            args = ArgsObj(
                trace=str(REPO_ROOT / "traces" / t),
                num_experts=num_exp,
                expert_cache_size=s,
                policy="lru"
            )
            res = simulate(args)
            rows.append(res)
    return pd.DataFrame(rows)

def run_double_buffering_sweep():
    print("\n--- Running Double Buffering Sweep (Q6) ---")
    # Sweep DRAM bandwidths and expert sizes to find compute- vs memory-bound regions
    bandwidths = [16.0, 64.0, 128.0]
    expert_sizes = [1000000, 2000000, 4000000]
    db_options = [False, True]
    
    rows = []
    for bw in bandwidths:
        for size in expert_sizes:
            for db in db_options:
                args = ArgsObj(
                    trace=str(REPO_ROOT / "traces" / "synth_zipf_E8_T128_K2.jsonl"),
                    bandwidth_bytes_per_cycle=bw,
                    expert_size_bytes=size,
                    double_buffering=db,
                    policy="history"
                )
                res = simulate(args)
                rows.append(res)
    return pd.DataFrame(rows)

def run_npu_sweep():
    print("\n--- Running NPU Value Sweep (Q7) ---")
    # Sweep compute latency representing small, large, or no NPU
    exec_latencies = [0, 50, 100, 200, 500]
    db_options = [False, True]
    
    rows = []
    for lat in exec_latencies:
        for db in db_options:
            args = ArgsObj(
                trace=str(REPO_ROOT / "traces" / "synth_zipf_E8_T128_K2.jsonl"),
                exec_cycles_per_token=lat,
                double_buffering=db,
                policy="history"
            )
            res = simulate(args)
            rows.append(res)
    return pd.DataFrame(rows)

def main():
    os.makedirs(str(REPO_ROOT / "docs" / "exploration"), exist_ok=True)
    
    df_sched = run_scheduling_sweep()
    df_sched.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_scheduling.csv"), index=False)
    
    df_cache = run_cache_sweep()
    df_cache.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_cache.csv"), index=False)
    
    df_db = run_double_buffering_sweep()
    df_db.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_double_buffering.csv"), index=False)
    
    df_npu = run_npu_sweep()
    df_npu.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_npu.csv"), index=False)
    
    print("\nAll DSE Sweeps successfully completed!")
    print("Sweep files saved in: docs/exploration/")

if __name__ == "__main__":
    main()
