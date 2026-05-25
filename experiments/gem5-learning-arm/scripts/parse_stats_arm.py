#!/usr/bin/env python3
"""ARM gem5 Statistics Parser"""

import os
import sys
import argparse
import csv

def parse_stats_file(stats_file):
    stats = {}
    try:
        with open(stats_file, 'r') as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                parts = line.split()
                if len(parts) >= 2:
                    key = parts[0]
                    value = parts[1]
                    try:
                        if '.' in value:
                            stats[key] = float(value)
                        else:
                            stats[key] = int(value)
                    except ValueError:
                        stats[key] = value
    except FileNotFoundError:
        print(f"Error: {stats_file} not found", file=sys.stderr)
        return None
    return stats

def extract_key_metrics(stats_dict):
    metrics = {}
    key_mapping = {
        'simInsts': ['simInsts', 'system.cpu.simInsts'],
        'simTicks': ['simTicks', 'system.cpu.simTicks'],
        'hostSeconds': ['hostSeconds', 'host_seconds'],
        'ipc': ['simulated_ipc', 'system.cpu.ipc'],
        'dcache_misses': ['system.cpu.dcache.misses'],
        'l2_misses': ['system.l2.misses'],
    }
    
    for metric_name, key_names in key_mapping.items():
        for key in key_names:
            if key in stats_dict:
                metrics[metric_name] = stats_dict[key]
                break
    return metrics

def main():
    parser = argparse.ArgumentParser(description="Parse ARM gem5 statistics")
    parser.add_argument('results_dirs', nargs='+', help="Result directories")
    parser.add_argument('--out', default='summary.csv', help="Output CSV file")
    args = parser.parse_args()

    all_metrics = []
    for result_dir in args.results_dirs:
        stats_file = os.path.join(result_dir, 'stats.txt')
        if not os.path.exists(stats_file):
            print(f"Warning: {stats_file} not found")
            continue
        print(f"Parsing {stats_file}...")
        stats = parse_stats_file(stats_file)
        if stats is None:
            continue
        metrics = extract_key_metrics(stats)
        metrics['result_dir'] = result_dir
        all_metrics.append(metrics)

    if not all_metrics:
        print("Error: No valid stats found", file=sys.stderr)
        sys.exit(1)

    fieldnames = set(['result_dir'])
    for metrics in all_metrics:
        fieldnames.update(metrics.keys())
    fieldnames = sorted(list(fieldnames))

    with open(args.out, 'w', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(all_metrics)

    print(f"Statistics written to {args.out}")

if __name__ == '__main__':
    main()
