#!/usr/bin/env python3
"""
calculate_predictor_cost.py
Quantifies the hardware area, SRAM capacity, lookup latency, and energy consumption
of Markov-1 and Markov-2 predictors across various expert counts.
"""

import pandas as pd
import numpy as np

def main():
    expert_counts = [8, 16, 64, 256]
    bit_width = 8 # 8-bit saturating counters

    # TSMC 28nm SRAM area estimate: ~0.12 um2 per bit cell + 30% peripheral overhead
    um2_per_bit = 0.12 * 1.3
    
    # 28nm SRAM read energy: ~0.1 pJ/bit
    pj_per_bit = 0.1
    
    # Off-chip DRAM read energy: ~20 pJ/bit (160 pJ/byte)
    dram_pj_per_byte = 160.0
    expert_size_bytes = 2000000 # 2MB

    rows = []
    for E in expert_counts:
        # Markov-1
        m1_entries = E * E
        m1_bits = m1_entries * bit_width
        m1_sram_kb = m1_bits / 8192
        m1_area_mm2 = (m1_bits * um2_per_bit) / 1e6
        m1_read_energy_nj = (m1_bits * pj_per_bit) / 1000.0 # energy to read one row
        
        # Markov-2
        m2_entries = E * E * E
        m2_bits = m2_entries * bit_width
        m2_sram_kb = m2_bits / 8192
        m2_area_mm2 = (m2_bits * um2_per_bit) / 1e6
        m2_read_energy_nj = (m2_bits * pj_per_bit) / 1000.0
        
        rows.append({
            "Expert_Count": E,
            "M1_SRAM_KB": m1_sram_kb,
            "M1_Area_mm2": m1_area_mm2,
            "M1_Energy_nJ": m1_read_energy_nj,
            "M2_SRAM_KB": m2_sram_kb,
            "M2_Area_mm2": m2_area_mm2,
            "M2_Energy_nJ": m2_read_energy_nj
        })

    df = pd.DataFrame(rows)
    print("=" * 60)
    print("  Markov Predictor Hardware Cost Analysis (TSMC 28nm)")
    print("=" * 60)
    for idx, r in df.iterrows():
        print(f"Experts: {int(r['Expert_Count'])}")
        print(f"  Markov-1: SRAM = {r['M1_SRAM_KB']:.3f} KB | Area = {r['M1_Area_mm2']:.6f} mm2 | Energy = {r['M1_Energy_nJ']:.4f} nJ")
        print(f"  Markov-2: SRAM = {r['M2_SRAM_KB']:.3f} KB | Area = {r['M2_Area_mm2']:.6f} mm2 | Energy = {r['M2_Energy_nJ']:.4f} nJ")
    print("-" * 60)
    
    dram_load_energy_mj = (expert_size_bytes * dram_pj_per_byte) / 1e9
    print(f"Off-chip DRAM load energy for ONE 2MB expert: {dram_load_energy_mj:.6f} mJ")
    print("=" * 60)
    
    df.to_csv("docs/exploration/predictor_costs.csv", index=False)

if __name__ == "__main__":
    main()
