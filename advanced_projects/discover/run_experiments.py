import os
import subprocess
import sys
import json
import pandas as pd

def run_cmd(cmd, description):
    print(f"\n>>> Running {description}...")
    print(f"Command: {cmd}")
    res = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if res.returncode != 0:
        print(f"Error executing command: {res.stderr}")
        return False
    print(res.stdout)
    return True

def parse_yosys_stat(log_file):
    stats = {"cells": 0, "dffs": 0, "gates": 0}
    if not os.path.exists(log_file):
        return stats
    
    with open(log_file, "r") as f:
        in_stat = False
        for line in f:
            if "=== moe_accelerator_frontend ===" in line or "=== Design Statistics ===" in line:
                in_stat = True
            if in_stat:
                if "Number of cells:" in line:
                    try:
                        stats["cells"] = int(line.split()[-1])
                    except:
                        pass
                elif "DFF" in line or "dff" in line:
                    try:
                        stats["dffs"] += int(line.split()[-1])
                    except:
                        pass
                elif "AND" in line or "OR" in line or "XOR" in line or "NOT" in line or "NAND" in line or "NOR" in line or "MUX" in line or "mux" in line:
                    try:
                        stats["gates"] += int(line.split()[-1])
                    except:
                        pass
    return stats

def main():
    print("======================================================================")
    print("             LLM/MoE Co-Design Accelerator Experiment Pipeline         ")
    print("======================================================================")
    
    trace_file = "/home/a/discover/profiling/routing_trace_b1_p128_g128.csv"
    
    # 1. Routing Trace Analysis
    cmd_analysis = f"python3 /home/a/discover/routing/trace_analyzer.py --trace_file {trace_file} --output_dir /home/a/discover/routing"
    if not run_cmd(cmd_analysis, "Routing Trace Analysis"):
        sys.exit(1)
        
    # 2. Expert Cache Simulation
    cmd_expert_cache = f"python3 /home/a/discover/expert_cache/expert_cache_sim.py --trace_file {trace_file} --output_dir /home/a/discover/expert_cache"
    if not run_cmd(cmd_expert_cache, "Expert Cache Simulation"):
        sys.exit(1)
        
    # 3. KV Cache Simulation
    cmd_kv_cache = f"python3 /home/a/discover/kv_cache/kv_cache_sim.py --batch_size 4 --prompt_len 512 --gen_len 128 --output_dir /home/a/discover/kv_cache"
    if not run_cmd(cmd_kv_cache, "KV Cache Simulation"):
        sys.exit(1)
        
    # 4. Joint Scheduler Simulation
    cmd_joint_sched = f"python3 /home/a/discover/joint_scheduler/joint_scheduler_sim.py --trace_file {trace_file} --output_dir /home/a/discover/joint_scheduler"
    if not run_cmd(cmd_joint_sched, "Joint Memory Scheduler Simulation"):
        sys.exit(1)
        
    # 5. Generate RTL trace from routing trace
    cmd_gen_rtl_trace = f"python3 /home/a/discover/rtl/generate_rtl_trace.py --trace_file {trace_file} --output_dir /home/a/discover/rtl"
    if not run_cmd(cmd_gen_rtl_trace, "RTL Trace Translation"):
        sys.exit(1)
        
    # 6. RTL Feasibility: Compile SystemVerilog using iverilog
    print("\n>>> Compiling SystemVerilog with iverilog...")
    compile_cmd = "iverilog -g2012 -o /home/a/discover/rtl/moe_sim /home/a/discover/rtl/moe_accelerator_frontend.sv /home/a/discover/rtl/tb_moe_accelerator.sv"
    print(f"Command: {compile_cmd}")
    compile_res = subprocess.run(compile_cmd, shell=True, capture_output=True, text=True)
    if compile_res.returncode != 0:
        print(f"RTL Compilation Failed:\n{compile_res.stderr}")
        sys.exit(1)
    print("Compilation successful.")
    
    # 7. Run RTL simulation using vvp
    print("\n>>> Running RTL simulation in vvp...")
    sim_cmd = "vvp /home/a/discover/rtl/moe_sim"
    print(f"Command: {sim_cmd}")
    sim_res = subprocess.run(sim_cmd, shell=True, capture_output=True, text=True)
    print(sim_res.stdout)
    if sim_res.returncode != 0:
        print(f"RTL Simulation Failed:\n{sim_res.stderr}")
        sys.exit(1)
        
    # 8. Yosys Synthesis Attempt
    print("\n>>> Running Yosys RTL Synthesis...")
    synth_log = "/home/a/discover/rtl/synth.log"
    synth_cmd = f"yosys -s /home/a/discover/rtl/synth.ys > {synth_log}"
    print(f"Command: {synth_cmd}")
    synth_res = subprocess.run(synth_cmd, shell=True, capture_output=True, text=True)
    
    # Parse stats
    stats = parse_yosys_stat(synth_log)
    print("\n================ RTL SYNTHESIS RESULTS ================")
    print(f"Yosys Synthesis attempted: True")
    print(f"Synthesis status: {'SUCCESS' if synth_res.returncode == 0 else 'FAILED'}")
    print(f"Total logic cells synthesized: {stats['cells']}")
    print(f"Total registers (DFF):        {stats['dffs']}")
    print(f"Total combinational gates:    {stats['gates']}")
    print("========================================================")
    
    # Save synthesis metrics
    with open("/home/a/discover/rtl/synthesis_results.json", "w") as f:
        json.dump({
            "attempted": True,
            "status": "success" if synth_res.returncode == 0 else "failed",
            "cell_count": stats["cells"],
            "register_count": stats["dffs"],
            "gate_count": stats["gates"]
        }, f, indent=4)
        
    print("\n======================================================================")
    print("             Experiment Pipeline Completed Successfully!               ")
    print("======================================================================")

if __name__ == "__main__":
    main()
