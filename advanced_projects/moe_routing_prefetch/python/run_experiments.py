#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import subprocess
import csv
import argparse
import shutil
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# Directories
PYTHON_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.dirname(PYTHON_DIR)

def parse_args():
    parser = argparse.ArgumentParser(description="Run sweeps and sensitivity experiments.")
    parser.add_argument("--trace", type=str, default=None, help="Optional input demand trace CSV")
    parser.add_argument("--hint-trace", type=str, default=None, help="Optional input prefetch hint trace CSV")
    parser.add_argument("--output-dir", type=str, default="results/", help="Output directory for plots")
    return parser.parse_args()

def main():
    args = parse_args()
    
    # Resolve absolute paths
    results_dir = os.path.abspath(args.output_dir)
    os.makedirs(results_dir, exist_ok=True)
    
    traces_dir = os.path.join(PROJECT_DIR, "traces")
    os.makedirs(traces_dir, exist_ok=True)
    
    exp_demand_csv = os.path.join(traces_dir, "exp_demand.csv")
    exp_hint_csv = os.path.join(traces_dir, "exp_hint.csv")
    exp_sim_out_csv = os.path.join(results_dir, "exp_sim_out.csv")
    
    # Helper to setup traces
    def setup_traces(num_tokens=500, dist="zipf", zipf_s=1.2, prefetch_dist=4, seed=42):
        if args.trace and args.hint_trace:
            # If user provided traces, copy them to our temp trace locations
            shutil.copyfile(args.trace, exp_demand_csv)
            shutil.copyfile(args.hint_trace, exp_hint_csv)
        else:
            # Generate synthetic traces
            cmd = [
                "python3", os.path.join(PYTHON_DIR, "generate_synthetic_moe_trace.py"),
                "--num-experts", "8",
                "--top-k", "1",
                "--num-layers", "1",
                "--num-tokens", str(num_tokens),
                "--distribution", dist,
                "--zipf-s", str(zipf_s),
                "--prefetch-distance", str(prefetch_dist),
                "--output", exp_demand_csv,
                "--output-hint", exp_hint_csv,
                "--seed", str(seed)
            ]
            subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    def run_cache_sim(policy, cache_size=4, dma_latency=10, score_threshold=0.1, replacement_policy="lru"):
        cmd = [
            "python3", os.path.join(PYTHON_DIR, "expert_cache_sim.py"),
            "--trace", exp_demand_csv,
            "--hint-trace", exp_hint_csv,
            "--policy", policy,
            "--replacement-policy", replacement_policy,
            "--cache-size", str(cache_size),
            "--dma-latency", str(dma_latency),
            "--score-threshold", str(score_threshold),
            "--output", exp_sim_out_csv
        ]
        subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        
        # Read output results
        with open(exp_sim_out_csv, 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                return {
                    "total_requests": int(row["total_requests"]),
                    "cache_hits": int(row["cache_hits"]),
                    "cache_misses": int(row["cache_misses"]),
                    "hit_rate": float(row["hit_rate"]),
                    "prefetch_issued_count": int(row["prefetch_issued_count"]),
                    "prefetch_useful_count": int(row["prefetch_useful_count"]),
                    "prefetch_useless_count": int(row["prefetch_useless_count"]),
                    "prefetch_dropped_count": int(row["prefetch_dropped_count"]),
                    "prefetch_filtered_count": int(row["prefetch_filtered_count"]),
                    "estimated_cycles": int(row["estimated_cycles"])
                }

    # 1. Cache Size Sweep
    print("Running Cache Size Sweep...")
    setup_traces(num_tokens=500, dist="zipf", zipf_s=1.2, prefetch_dist=4)
    cache_sizes = [1, 2, 3, 4, 5, 6, 7, 8]
    results = {
        "no_cache": [], "static_hot_expert": [], "fifo": [], "lru": [],
        "routing_aware_fifo": [], "routing_aware_lru": [], "oracle": []
    }
    for cs in cache_sizes:
        results["no_cache"].append(run_cache_sim("no_cache", cache_size=cs)["hit_rate"])
        results["static_hot_expert"].append(run_cache_sim("static_hot_expert", cache_size=cs)["hit_rate"])
        results["fifo"].append(run_cache_sim("fifo", cache_size=cs)["hit_rate"])
        results["lru"].append(run_cache_sim("lru", cache_size=cs)["hit_rate"])
        results["routing_aware_fifo"].append(run_cache_sim("routing_aware_next", cache_size=cs, replacement_policy="fifo")["hit_rate"])
        results["routing_aware_lru"].append(run_cache_sim("routing_aware_next", cache_size=cs, replacement_policy="lru")["hit_rate"])
        results["oracle"].append(run_cache_sim("oracle_future_window", cache_size=cs, replacement_policy="lru")["hit_rate"])
        
    plt.figure(figsize=(10, 6))
    plt.plot(cache_sizes, results["no_cache"], label="No Cache", marker='o', linestyle='--')
    plt.plot(cache_sizes, results["static_hot_expert"], label="Static Hot Expert", marker='s')
    plt.plot(cache_sizes, results["fifo"], label="FIFO Cache", marker='^')
    plt.plot(cache_sizes, results["lru"], label="LRU Cache", marker='v')
    plt.plot(cache_sizes, results["routing_aware_fifo"], label="Routing-Aware Prefetch (FIFO)", marker='x')
    plt.plot(cache_sizes, results["routing_aware_lru"], label="Routing-Aware Prefetch (LRU)", marker='d')
    plt.plot(cache_sizes, results["oracle"], label="Oracle Upper Bound (OPT)", marker='*')
    plt.title("Cache Size Sweep (Zipf s=1.2, Num Experts=8)")
    plt.xlabel("Cache Size (Ways)")
    plt.ylabel("Cache Hit Rate")
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, "sweep_cache_size.png"), dpi=150)
    plt.close()

    # 2. Prefetch Distance Sweep
    print("Running Prefetch Distance Sweep...")
    distances = [1, 2, 4, 8, 12, 16, 20]
    hit_rates, useful, useless, dropped = [], [], [], []
    for d in distances:
        setup_traces(num_tokens=500, dist="zipf", zipf_s=1.2, prefetch_dist=d)
        res = run_cache_sim("routing_aware_next", cache_size=4, replacement_policy="lru", dma_latency=10)
        hit_rates.append(res["hit_rate"])
        useful.append(res["prefetch_useful_count"])
        useless.append(res["prefetch_useless_count"])
        dropped.append(res["prefetch_dropped_count"])
        
    fig, ax1 = plt.subplots(figsize=(10, 6))
    color = 'tab:red'
    ax1.set_xlabel('Prefetch Distance (Cycles)')
    ax1.set_ylabel('Cache Hit Rate', color=color)
    ax1.plot(distances, hit_rates, color=color, marker='o', label="Hit Rate")
    ax1.tick_params(axis='y', labelcolor=color)
    ax1.grid(True, linestyle='--', alpha=0.5)
    
    ax2 = ax1.twinx()
    ax2.set_ylabel('Prefetch Counts', color='black')
    ax2.plot(distances, useful, color='tab:blue', marker='s', label="Useful Prefetch")
    ax2.plot(distances, useless, color='tab:orange', marker='^', label="Useless Prefetch")
    ax2.plot(distances, dropped, color='tab:gray', marker='x', label="Dropped Prefetch")
    ax2.tick_params(axis='y', labelcolor='black')
    
    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax1.legend(lines1 + lines2, labels1 + labels2, loc='upper left')
    plt.title("Prefetch Distance Sweep (Cache Size=4, DMA Latency=10)")
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, "sweep_prefetch_distance.png"), dpi=150)
    plt.close()

    # 3. Score Threshold Sweep
    print("Running Score Threshold Sweep...")
    setup_traces(num_tokens=500, dist="zipf", zipf_s=1.2, prefetch_dist=4)
    thresholds = [0.0, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8]
    hit_rates, useful, useless, filtered = [], [], [], []
    for th in thresholds:
        res = run_cache_sim("routing_aware_next", cache_size=4, replacement_policy="lru", score_threshold=th)
        hit_rates.append(res["hit_rate"])
        useful.append(res["prefetch_useful_count"])
        useless.append(res["prefetch_useless_count"])
        filtered.append(res["prefetch_filtered_count"])
        
    fig, ax1 = plt.subplots(figsize=(10, 6))
    color = 'tab:red'
    ax1.set_xlabel('Score Threshold')
    ax1.set_ylabel('Cache Hit Rate', color=color)
    ax1.plot(thresholds, hit_rates, color=color, marker='o', label="Hit Rate")
    ax1.tick_params(axis='y', labelcolor=color)
    ax1.grid(True, linestyle='--', alpha=0.5)
    
    ax2 = ax1.twinx()
    ax2.set_ylabel('Prefetch Counts', color='black')
    ax2.plot(thresholds, useful, color='tab:blue', marker='s', label="Useful Prefetch")
    ax2.plot(thresholds, useless, color='tab:orange', marker='^', label="Useless Prefetch")
    ax2.plot(thresholds, filtered, color='tab:purple', marker='x', label="Filtered Prefetch")
    ax2.tick_params(axis='y', labelcolor='black')
    
    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax1.legend(lines1 + lines2, labels1 + labels2, loc='upper right')
    plt.title("Router Score Threshold Sweep (Cache Size=4, DMA Latency=10)")
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, "sweep_score_threshold.png"), dpi=150)
    plt.close()

    # 4. Memory Latency Sweep
    print("Running Memory Latency Sweep...")
    setup_traces(num_tokens=500, dist="zipf", zipf_s=1.2, prefetch_dist=4)
    latencies = [2, 5, 10, 15, 20, 30, 40, 50]
    results_lat = {
        "fifo": [], "lru": [], "routing_aware_lru": [], "oracle": []
    }
    for lat in latencies:
        results_lat["fifo"].append(run_cache_sim("fifo", cache_size=4, dma_latency=lat)["estimated_cycles"])
        results_lat["lru"].append(run_cache_sim("lru", cache_size=4, dma_latency=lat)["estimated_cycles"])
        results_lat["routing_aware_lru"].append(run_cache_sim("routing_aware_next", cache_size=4, dma_latency=lat, replacement_policy="lru")["estimated_cycles"])
        results_lat["oracle"].append(run_cache_sim("oracle_future_window", cache_size=4, dma_latency=lat, replacement_policy="lru")["estimated_cycles"])
        
    plt.figure(figsize=(10, 6))
    plt.plot(latencies, results_lat["fifo"], label="FIFO Cache", marker='o')
    plt.plot(latencies, results_lat["lru"], label="LRU Cache", marker='s')
    plt.plot(latencies, results_lat["routing_aware_lru"], label="Routing-Aware Prefetch (LRU)", marker='^')
    plt.plot(latencies, results_lat["oracle"], label="Oracle Upper Bound (OPT)", marker='*')
    plt.title("Memory Latency Sweep (Cache Size=4)")
    plt.xlabel("DMA Latency (Cycles)")
    plt.ylabel("Estimated Total Cycles")
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, "sweep_memory_latency.png"), dpi=150)
    plt.close()

    # Save summary stats of sweeps as CSV in the output directory
    summary_path = os.path.join(results_dir, "experiments_summary.csv")
    with open(summary_path, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Experiment", "Status"])
        writer.writerow(["Cache Size Sweep", "Completed"])
        writer.writerow(["Prefetch Distance Sweep", "Completed"])
        writer.writerow(["Score Threshold Sweep", "Completed"])
        writer.writerow(["Memory Latency Sweep", "Completed"])
        
    # Cleanup temp csv files
    for path in [exp_demand_csv, exp_hint_csv, exp_sim_out_csv]:
        if os.path.exists(path):
            os.remove(path)
            
    print(f"=== All Experiments Completed. Results saved in: {results_dir} ===")

if __name__ == "__main__":
    main()
