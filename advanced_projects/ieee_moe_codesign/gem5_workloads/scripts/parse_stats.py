# parse_stats.py
# Parses gem5 stats.txt output files across cache configurations and formats them into a consolidated CSV.

import os
import re
import argparse
import csv

def parse_stats_file(file_path):
    metrics = {
        "simSeconds": 0.0,
        "simTicks": 0,
        "cpuCycles": 0,
        "dcacheMisses": 0,
        "l2cacheMisses": 0,
        "ipc": 0.0
    }
    
    if not os.path.exists(file_path):
        return metrics

    with open(file_path, "r") as f:
        content = f.read()

    def find_val(name):
        pattern = rf"^\s*{re.escape(name)}\s+([0-9\.e\-+]+)"
        match = re.search(pattern, content, re.MULTILINE)
        return float(match.group(1)) if match else None

    sim_seconds = find_val("simSeconds")
    sim_ticks = find_val("simTicks")
    
    # CPU Cycles
    cpu_cycles_match = re.search(r"system\.cpu\.numCycles\s+(\d+)", content)
    if not cpu_cycles_match:
        cpu_cycles_match = re.search(r"system\.cpu\d+\.numCycles\s+(\d+)", content)
    cpu_cycles = int(cpu_cycles_match.group(1)) if cpu_cycles_match else 0

    # L1 D-Cache Misses
    dcache_misses_match = re.search(r"system\.cpu\.dcache\.overallMisses::total\s+(\d+)", content)
    if not dcache_misses_match:
        dcache_misses_match = re.search(r"system\.cpu\d+\.dcache\.overallMisses::total\s+(\d+)", content)
    dcache_misses = int(dcache_misses_match.group(1)) if dcache_misses_match else 0

    # L2 Cache Misses
    l2cache_misses_match = re.search(r"system\.l2\.overallMisses::total\s+(\d+)", content)
    if not l2cache_misses_match:
        l2cache_misses_match = re.search(r"system\.l2cache\.overallMisses::total\s+(\d+)", content)
    l2cache_misses = int(l2cache_misses_match.group(1)) if l2cache_misses_match else 0

    # IPC
    ipc_match = re.search(r"system\.cpu\.ipc\s+([0-9\.]+)", content)
    if not ipc_match:
        ipc_match = re.search(r"system\.cpu\d+\.ipc\s+([0-9\.]+)", content)
    ipc = float(ipc_match.group(1)) if ipc_match else 0.0

    if sim_seconds: metrics["simSeconds"] = sim_seconds
    if sim_ticks: metrics["simTicks"] = int(sim_ticks)
    if cpu_cycles: metrics["cpuCycles"] = cpu_cycles
    if dcache_misses: metrics["dcacheMisses"] = dcache_misses
    if l2cache_misses: metrics["l2cacheMisses"] = l2cache_misses
    if ipc: metrics["ipc"] = ipc

    return metrics

def main():
    parser = argparse.ArgumentParser(description="Parse gem5 stats files")
    parser.add_argument('--input-dir', type=str, required=True, help="Directory containing gem5 output subfolders")
    parser.add_argument('--output', type=str, required=True, help="Path to write consolidated CSV results")
    args = parser.parse_args()

    results = []
    
    # Expected folder format: l1d_<size>_l2_<size>_<queue_type>
    pattern = re.compile(r"l1d_([0-9a-zA-Z]+)_l2_([0-9a-zA-Z]+)_([a-z_]+)")
    
    for folder in sorted(os.listdir(args.input_dir)):
        match = pattern.match(folder)
        if not match:
            continue
            
        l1d_size = match.group(1)
        l2_size = match.group(2)
        queue_type = match.group(3)
        
        stats_path = os.path.join(args.input_dir, folder, "stats.txt")
        metrics = parse_stats_file(stats_path)
        
        row = {
            "l1d_size": l1d_size,
            "l2_size": l2_size,
            "queue_type": queue_type,
            "sim_seconds": metrics["simSeconds"],
            "sim_ticks": metrics["simTicks"],
            "cpu_cycles": metrics["cpuCycles"],
            "dcache_misses": metrics["dcacheMisses"],
            "l2cache_misses": metrics["l2cacheMisses"],
            "ipc": metrics["ipc"]
        }
        results.append(row)

    # Write consolidated CSV
    fieldnames = ["l1d_size", "l2_size", "queue_type", "sim_seconds", "sim_ticks", "cpu_cycles", "dcache_misses", "l2cache_misses", "ipc"]
    with open(args.output, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for row in results:
            writer.writerow(row)
            
    print(f"Parsed {len(results)} stats folders. Output written to {args.output}")

if __name__ == "__main__":
    main()
