#!/usr/bin/env python3
"""
calculate_complexity.py
Calculates complexity and resource footprint estimates for Candidate C vs Candidate E
on the PYNQ-Z2 (XC7Z020) platform.
"""

import pandas as pd

def main():
    # XC7Z020 resources: 53200 LUTs, 106400 FFs, 140 BRAMs, 220 DSPs
    xc7z020_luts = 53200
    xc7z020_brams = 140
    
    # Estimates
    # Candidate C: Smart DMA
    c_luts = 4000
    c_ffs = 5000
    c_brams = 3
    c_dsps = 0
    c_dev_months = 3
    c_debug_months = 2
    
    # Candidate E: Hybrid Sidecar
    e_luts = 12000
    e_ffs = 14000
    e_brams = 20
    e_dsps = 4
    e_dev_months = 6
    e_debug_months = 6
    
    rows = [
        {
            "Architecture": "Candidate C (Smart DMA)",
            "LUT_Estimate": c_luts,
            "LUT_Percent": (c_luts / xc7z020_luts) * 100.0,
            "BRAM_Estimate": c_brams,
            "BRAM_Percent": (c_brams / xc7z020_brams) * 100.0,
            "DSP_Estimate": c_dsps,
            "Dev_Time_Months": c_dev_months,
            "Debug_Time_Months": c_debug_months,
            "Total_Hardware_Months": c_dev_months + c_debug_months,
            "Risk_Multiplier": 1.0
        },
        {
            "Architecture": "Candidate E (Hybrid Sidecar)",
            "LUT_Estimate": e_luts,
            "LUT_Percent": (e_luts / xc7z020_luts) * 100.0,
            "BRAM_Estimate": e_brams,
            "BRAM_Percent": (e_brams / xc7z020_brams) * 100.0,
            "DSP_Estimate": e_dsps,
            "Dev_Time_Months": e_dev_months,
            "Debug_Time_Months": e_debug_months,
            "Total_Hardware_Months": e_dev_months + e_debug_months,
            "Risk_Multiplier": 2.4 # high RISC-V integration risk
        }
    ]
    
    df = pd.DataFrame(rows)
    print("=" * 60)
    print("  Resource and Complexity Estimations for PYNQ-Z2")
    print("=" * 60)
    for idx, r in df.iterrows():
        print(f"{r['Architecture']}:")
        print(f"  LUTs  = {int(r['LUT_Estimate'])} ({r['LUT_Percent']:.1f}%) | BRAMs = {int(r['BRAM_Estimate'])} ({r['BRAM_Percent']:.1f}%)")
        print(f"  Time  = {int(r['Total_Hardware_Months'])} months | Risk Multiplier = {r['Risk_Multiplier']:.1f}x")
    print("=" * 60)
    
    df.to_csv("docs/exploration/complexity_estimates.csv", index=False)

if __name__ == "__main__":
    main()
