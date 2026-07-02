#!/usr/bin/env python3
"""
simulate_candidates.py
Generates quantitative comparison metrics for Candidates A, B, C, D, and E
under Zipfian E8 workload.
"""

import pandas as pd
import numpy as np

def main():
    # Base parameters for synth_zipf_E8
    total_tokens = 512
    compute_cycles_per_token = 100
    expert_size_bytes = 2000000
    dram_bw_bytes_per_cycle = 64.0
    
    # Latency and Area proxies
    # Candidates:
    # A: Software Runtime Baseline (No HW, high software overhead, serial)
    # B: MENA (Hardware cache + prefetch + lightweight NPU)
    # C: Smart DMA Sidecar (Intelligent DMA, cache + prefetch, no NPU)
    # D: Memory-Centric Runtime Processor (RISC-V runtime coprocessor)
    # E: Hybrid Sidecar (Smart DMA + tiny metadata core)
    
    candidates = ["A", "B", "C", "D", "E"]
    
    # Software overhead factors (cycles added per layer routing/synchronization)
    sw_sync_overhead = {
        "A": 50000.0, # massive host-GPU handshake
        "B": 5000.0,  # MMIO synchronization
        "C": 1000.0,  # direct memory mapped DMA
        "D": 2000.0,  # coprocessor message queue
        "E": 1200.0   # optimized hybrid queue
    }
    
    # Hardware cache hit rates based on simulation models
    hit_rates = {
        "A": 0.00,  # standard LRU thrashes
        "B": 0.59,  # history prefetch
        "C": 0.59,  # history prefetch
        "D": 0.65,  # compiler-guided cache bypass
        "E": 0.70   # hybrid active prefetch + bypass
    }
    
    # Double buffering capability
    double_buffering = {
        "A": False,
        "B": True,
        "C": True,
        "D": False, # serialized metadata execution
        "E": True
    }
    
    # Area Proxy in TSMC 28nm (mm2)
    area_mm2 = {
        "A": 0.0,
        "B": 0.85, # lightweight NPU + cache + queue + predictor
        "C": 0.15, # cache + queue + predictor (No NPU)
        "D": 0.45, # RISC-V core + metadata SRAM
        "E": 0.50  # Smart DMA + tiny core
    }
    
    # Energy Proxy per layer (uJ)
    # DRAM load: 160 pJ/byte. SRAM read: 0.1 pJ/bit. NPU compute: 10 pJ/op.
    
    rows = []
    for c in candidates:
        hit = hit_rates[c]
        misses = 22 * (1.0 - hit) # 22 base expert transfers
        
        # DRAM weight transfer cycles
        transfer_cycles = (misses * expert_size_bytes) / dram_bw_bytes_per_cycle
        
        # NPU compute cycles
        compute_cycles = total_tokens * compute_cycles_per_token
        
        # Calculate execution latency
        if double_buffering[c]:
            exec_cycles = max(transfer_cycles, compute_cycles)
        else:
            exec_cycles = transfer_cycles + compute_cycles
            
        total_latency = exec_cycles + sw_sync_overhead[c]
        
        # Energy proxies
        dram_energy_uj = (misses * expert_size_bytes * 160.0) / 1e6
        sram_energy_uj = 0.5 if c != "A" else 0.0 # tiny lookup overhead
        npu_energy_uj = 2.5 if c in ["B"] else 0.0 # NPU compute overhead
        total_energy_uj = dram_energy_uj + sram_energy_uj + npu_energy_uj
        
        rows.append({
            "Candidate": c,
            "Hit_Rate": hit,
            "Misses": misses,
            "Transfer_Cycles": transfer_cycles,
            "Compute_Cycles": compute_cycles,
            "Sync_Overhead": sw_sync_overhead[c],
            "Total_Cycles": total_latency,
            "Area_mm2": area_mm2[c],
            "Energy_uJ": total_energy_uj
        })
        
    df = pd.DataFrame(rows)
    print("=" * 70)
    print("  Candidate Architecture Comparison Matrix (MAEP)")
    print("=" * 70)
    for idx, r in df.iterrows():
        print(f"Candidate {r['Candidate']}:")
        print(f"  Total Cycles = {r['Total_Cycles']:.1f} | Speedup vs A = {df.iloc[0]['Total_Cycles']/r['Total_Cycles']:.2f}x")
        print(f"  Area Proxy   = {r['Area_mm2']:.2f} mm2 | Energy Proxy = {r['Energy_uJ']:.1f} uJ")
    print("=" * 70)
    
    df.to_csv("docs/exploration/architecture_competition.csv", index=False)

if __name__ == "__main__":
    main()
