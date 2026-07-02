#!/usr/bin/env python3
import os
import sys
import subprocess
import pandas as pd

def main():
    sim_script = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "sim", "mena_sim.py"))
    output_csv = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "sim", "results_summary.csv"))
    output_md = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "sim", "results_summary.md"))
    
    # Remove existing CSV if it exists to start fresh
    if os.path.exists(output_csv):
        os.remove(output_csv)
        
    experiments = [
        # 1. uniform trace + cache none
        {
            "name": "1. Uniform + Cache None",
            "trace": "traces/synth_uniform_E8_T128_K2.jsonl",
            "num_experts": 8,
            "cache_size": 0,
            "policy": "none",
            "dispatch": "expert_major",
            "double_buffering": False
        },
        # 2. uniform trace + LRU
        {
            "name": "2. Uniform + LRU (size 4)",
            "trace": "traces/synth_uniform_E8_T128_K2.jsonl",
            "num_experts": 8,
            "cache_size": 4,
            "policy": "lru",
            "dispatch": "expert_major",
            "double_buffering": False
        },
        # 3. zipf trace + LRU
        {
            "name": "3. Zipf + LRU (size 4)",
            "trace": "traces/synth_zipf_E8_T128_K2.jsonl",
            "num_experts": 8,
            "cache_size": 4,
            "policy": "lru",
            "dispatch": "expert_major",
            "double_buffering": False
        },
        # 4. zipf trace + history
        {
            "name": "4. Zipf + History (size 4)",
            "trace": "traces/synth_zipf_E8_T128_K2.jsonl",
            "num_experts": 8,
            "cache_size": 4,
            "policy": "history",
            "dispatch": "expert_major",
            "double_buffering": False
        },
        # 5. hot trace + LRU
        {
            "name": "5. Hot + LRU (size 4)",
            "trace": "traces/synth_hot_E16_T512_K2.jsonl",
            "num_experts": 16,
            "cache_size": 4,
            "policy": "lru",
            "dispatch": "expert_major",
            "double_buffering": False
        },
        # 6. hot trace + history
        {
            "name": "6. Hot + History (size 4)",
            "trace": "traces/synth_hot_E16_T512_K2.jsonl",
            "num_experts": 16,
            "cache_size": 4,
            "policy": "history",
            "dispatch": "expert_major",
            "double_buffering": False
        },
        # 7. zipf trace + LRU + double buffering
        {
            "name": "7. Zipf + LRU + DB",
            "trace": "traces/synth_zipf_E8_T128_K2.jsonl",
            "num_experts": 8,
            "cache_size": 4,
            "policy": "lru",
            "dispatch": "expert_major",
            "double_buffering": True
        },
        # 8. zipf trace + history + double buffering
        {
            "name": "8. Zipf + History + DB",
            "trace": "traces/synth_zipf_E8_T128_K2.jsonl",
            "num_experts": 8,
            "cache_size": 4,
            "policy": "history",
            "dispatch": "expert_major",
            "double_buffering": True
        },
        # 9. hot trace + history + double buffering
        {
            "name": "9. Hot + History + DB",
            "trace": "traces/synth_hot_E16_T512_K2.jsonl",
            "num_experts": 16,
            "cache_size": 4,
            "policy": "history",
            "dispatch": "expert_major",
            "double_buffering": True
        }
    ]
    
    print("Running system simulation experiments...")
    
    for exp in experiments:
        trace_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", exp["trace"]))
        cmd = [
            sys.executable, sim_script,
            "--trace", trace_path,
            "--num_experts", str(exp["num_experts"]),
            "--expert_cache_size", str(exp["cache_size"]),
            "--policy", exp["policy"],
            "--dispatch_policy", exp["dispatch"],
            "--output_csv", output_csv
        ]
        if exp["double_buffering"]:
            cmd.append("--double_buffering")
        print(f"Running: {' '.join(cmd)}")
        subprocess.run(cmd, check=True)
        
    # Read the summary and produce Markdown table
    if os.path.exists(output_csv):
        df = pd.read_csv(output_csv)
        # Add Experiment Name for clarity
        exp_names = [exp["name"] for exp in experiments]
        df.insert(0, "Experiment", exp_names)
        df.to_csv(output_csv, index=False)
        
        # Build markdown summary
        md_content = "# System Simulation Results Summary\n\n"
        md_content += "| Experiment | Trace | Experts | Cache Size | Cache Policy | Double Buffering | Hit Rate | Misses | Transfer Cycles | Exec Cycles | Total Cycles | P50 Wait | P95 Wait |\n"
        md_content += "| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |\n"
        
        for _, row in df.iterrows():
            md_content += f"| {row['Experiment']} | {row['Trace']} | {row['Num_Experts']} | {row['Cache_Size']} | {row['Cache_Policy']} | {row['Double_Buffering']} | {row['Hit_Rate']:.2%} | {row['Miss_Count']} | {row['Transfer_Cycles']:.1f} | {row['Exec_Cycles']:.1f} | {row['Total_Cycles']:.1f} | {row['P50_Wait']:.1f} | {row['P95_Wait']:.1f} |\n"
            
        with open(output_md, "w") as f:
            f.write(md_content)
            
        print("Summary markdown generated successfully.")
    else:
        print("Error: results_summary.csv was not generated.")

if __name__ == "__main__":
    main()
