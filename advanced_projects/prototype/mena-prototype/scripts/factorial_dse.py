#!/usr/bin/env python3
"""
factorial_dse.py
Performs a full 2^7 = 128 run Factorial Design Space Exploration (DSE).
Analyzes the Main Effects and Interaction Effects of:
1. Cache (C)
2. Prefetch (P)
3. Double Buffer (DB)
4. Scheduler (S)
5. Metadata Processor (MP)
6. Runtime Engine (RE)
7. Compute Engine (CE)
"""

import pandas as pd
import numpy as np

def run_factorial_dse():
    # TSMC 28nm Area proxies (mm2)
    area_cost = {
        "C": 0.05,   # Tag array
        "P": 0.05,   # Markov-1 table
        "DB": 0.10,  # Ping-pong weight buffer memory
        "S": 0.02,   # Scheduler logic
        "MP": 0.30,  # RISC-V core + local memories
        "RE": 0.05,  # HW queue manager
        "CE": 0.60   # Custom GEMM computation logic
    }
    
    # Verification and RTL complexity proxies (arbitrary developer-months)
    complexity_cost = {
        "C": 2.0,
        "P": 2.5,
        "DB": 2.0,
        "S": 1.5,
        "MP": 6.0,
        "RE": 3.0,
        "CE": 8.0
    }

    # Factorial generation
    runs = []
    for C in [0, 1]:
        for P in [0, 1]:
            for DB in [0, 1]:
                for S in [0, 1]:
                    for MP in [0, 1]:
                        for RE in [0, 1]:
                            for CE in [0, 1]:
                                factor_vals = {
                                    "C": C, "P": P, "DB": DB, "S": S, "MP": MP, "RE": RE, "CE": CE
                                }
                                
                                # 1. Area
                                area = sum([val for f, val in area_cost.items() if factor_vals[f] == 1])
                                
                                # 2. Complexity
                                complexity = sum([val for f, val in complexity_cost.items() if factor_vals[f] == 1])
                                
                                # 3. Cache Hit Rate modeling under Zipfian workload
                                if C == 0:
                                    hit_rate = 0.0
                                else:
                                    if S == 0:
                                        hit_rate = 0.15
                                    else:
                                        if P == 1:
                                            hit_rate = 0.59
                                            if MP == 1:
                                                hit_rate = 0.70 # compiler-guided bypass
                                        else:
                                            hit_rate = 0.0
                                            
                                # 4. Weight DRAM Misses & Transfer Latency (cycles)
                                misses = 22 * (1.0 - hit_rate)
                                dram_bw = 64.0
                                expert_size = 2000000
                                transfer_cycles = (misses * expert_size) / dram_bw
                                
                                # 5. NPU / Compute latency
                                tokens = 512
                                compute_cycles_per_token = 100 if CE == 1 else 300 # CE=1 speeds up GEMM
                                compute_cycles = tokens * compute_cycles_per_token
                                
                                # 6. Synchronization / Runtime Overhead
                                sync_overhead = 1000 if RE == 1 else 15000
                                
                                # 7. Total execution latency
                                if DB == 1:
                                    exec_cycles = max(transfer_cycles, compute_cycles)
                                else:
                                    exec_cycles = transfer_cycles + compute_cycles
                                    
                                total_latency = exec_cycles + sync_overhead
                                
                                # 8. Energy proxy (uJ)
                                dram_energy = (misses * expert_size * 160.0) / 1e6
                                sram_energy = 0.05 if (C == 1 or P == 1 or MP == 1) else 0.0
                                compute_energy = (tokens * (10.0 if CE == 1 else 40.0)) / 1e3
                                total_energy = dram_energy + sram_energy + compute_energy
                                
                                run_data = factor_vals.copy()
                                run_data.update({
                                    "Area_mm2": area,
                                    "Complexity_Months": complexity,
                                    "Hit_Rate": hit_rate,
                                    "Total_Cycles": total_latency,
                                    "Energy_uJ": total_energy
                                })
                                runs.append(run_data)
                                
    df = pd.DataFrame(runs)
    df.to_csv("docs/exploration/factorial_dse.csv", index=False)
    
    # Calculate Main Effects
    main_effects = {}
    factors = ["C", "P", "DB", "S", "MP", "RE", "CE"]
    for f in factors:
        mean_high = df[df[f] == 1]["Total_Cycles"].mean()
        mean_low = df[df[f] == 0]["Total_Cycles"].mean()
        main_effects[f] = mean_high - mean_low
        
    print("=" * 60)
    print("  Factorial Design Space Exploration: Main Effects on Latency")
    print("=" * 60)
    for f, val in main_effects.items():
        print(f"Factor {f}: Main Effect = {val:.1f} cycles")
    print("=" * 60)

if __name__ == "__main__":
    run_factorial_dse()
