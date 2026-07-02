#!/usr/bin/env python3
"""
dse_explorations_v3.py
Advanced DSE simulation script for Q13 (Scaling Study) and Q14 (Future Memory Systems).
Outputs:
1. docs/exploration/sweep_scaling.csv
2. docs/exploration/sweep_memory_systems.csv
"""

import sys
import os
import pandas as pd
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

def run_scaling_study():
    """Sweeps Batch Size equivalents by scaling exec_cycles_per_token or trace token counts"""
    print("\n--- Running Scaling Study Sweep (Q13) ---")
    # Sweep batch multiplier representing concurrent token batch scaling
    batches = [1, 4, 8, 16, 32, 64]
    
    rows = []
    for b in batches:
        # Scale exec cycles per token to represent batch-compute scaling per expert queue
        args = ArgsObj(
            trace=str(REPO_ROOT / "traces" / "synth_zipf_E8_T128_K2.jsonl"),
            exec_cycles_per_token=100 * b,
            double_buffering=True,
            policy="history",
            expert_cache_size=4
        )
        res = simulate(args)
        # Store batch size explicitly for documentation
        res["Batch_Size"] = b
        rows.append(res)
    return pd.DataFrame(rows)

def run_memory_systems_study():
    """Sweeps future memory architectures (LPDDR, DDR5, HBM, CXL)"""
    print("\n--- Running Memory Systems Study Sweep (Q14) ---")
    memory_configs = [
        ("LPDDR-like", 16.0),
        ("DDR5-like", 32.0),
        ("HBM-like", 128.0),
        ("CXL-like", 32.0) # Latency penalty will be modeled in bottleneck analysis
    ]
    db_options = [False, True]
    
    rows = []
    for name, bw in memory_configs:
        for db in db_options:
            args = ArgsObj(
                trace=str(REPO_ROOT / "traces" / "synth_zipf_E8_T128_K2.jsonl"),
                bandwidth_bytes_per_cycle=bw,
                double_buffering=db,
                policy="history",
                expert_cache_size=4
            )
            res = simulate(args)
            res["Memory_System"] = name
            rows.append(res)
    return pd.DataFrame(rows)

def main():
    os.makedirs(str(REPO_ROOT / "docs" / "exploration"), exist_ok=True)
    
    df_scaling = run_scaling_study()
    df_scaling.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_scaling.csv"), index=False)
    
    df_mem = run_memory_systems_study()
    df_mem.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_memory_systems.csv"), index=False)
    
    print("\nScaling & Memory DSE sweeps successfully completed!")

if __name__ == "__main__":
    main()
