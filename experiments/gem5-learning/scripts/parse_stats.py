#!/usr/bin/env python3
"""
gem5 統計解析工具

功能：
- 從多個 gem5 結果目錄讀取 stats.txt
- 提取關鍵統計指標
- 輸出 CSV 格式

用法：
  python3 parse_stats.py results/exp1 results/exp2 ... --out summary.csv
"""

import os
import sys
import argparse
import csv
from pathlib import Path

def parse_stats_file(stats_file):
    """
    解析 gem5 stats.txt 檔案
    返回 dict 格式的統計數據
    """
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
        print(f"Error: stats file not found: {stats_file}", file=sys.stderr)
        return None
    
    return stats

def extract_key_metrics(stats_dict):
    """
    從統計數據中提取關鍵指標
    """
    metrics = {}
    
    key_mapping = {
        'simInsts': ['simInsts', 'system.cpu.simInsts', 'simulated_instructions'],
        'simTicks': ['simTicks', 'system.cpu.simTicks', 'simulated_ticks'],
        'hostSeconds': ['hostSeconds', 'host_seconds'],
        'hostTickRate': ['hostTickRate', 'host_tick_rate'],
        'hostInstRate': ['hostInstRate', 'host_inst_rate'],
        'ipc': ['simulated_ipc', 'system.cpu.ipc', 'ipc'],
        'numCycles': ['numCycles', 'system.cpu.numCycles', 'simulated_cycles'],
        'dcache_misses': ['system.cpu.dcache.misses', 'dcache.misses'],
        'dcache_hits': ['system.cpu.dcache.hits', 'dcache.hits'],
        'icache_misses': ['system.cpu.icache.misses', 'icache.misses'],
        'icache_hits': ['system.cpu.icache.hits', 'icache.hits'],
        'l2_misses': ['system.l2.misses', 'l2.misses'],
        'l2_hits': ['system.l2.hits', 'l2.hits'],
    }
    
    for metric_name, key_names in key_mapping.items():
        for key in key_names:
            if key in stats_dict:
                metrics[metric_name] = stats_dict[key]
                break
    
    return metrics

def main():
    parser = argparse.ArgumentParser(
        description="Parse gem5 statistics from multiple results directories"
    )
    parser.add_argument(
        'results_dirs',
        nargs='+',
        help="One or more result directories containing stats.txt"
    )
    parser.add_argument(
        '--out',
        default='summary.csv',
        help="Output CSV file (default: summary.csv)"
    )
    
    args = parser.parse_args()
    
    all_metrics = []
    
    for result_dir in args.results_dirs:
        stats_file = os.path.join(result_dir, 'stats.txt')
        
        if not os.path.exists(stats_file):
            print(f"Warning: {stats_file} not found, skipping {result_dir}")
            continue
        
        print(f"Parsing {stats_file}...")
        stats = parse_stats_file(stats_file)
        
        if stats is None:
            continue
        
        metrics = extract_key_metrics(stats)
        metrics['result_dir'] = result_dir
        
        all_metrics.append(metrics)
    
    if not all_metrics:
        print("Error: No valid stats files found", file=sys.stderr)
        sys.exit(1)
    
    fieldnames = set(['result_dir'])
    for metrics in all_metrics:
        fieldnames.update(metrics.keys())
    fieldnames = sorted(list(fieldnames))
    
    try:
        with open(args.out, 'w', newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(all_metrics)
        
        print(f"Statistics written to {args.out}")
        print(f"\nSummary: Parsed {len(all_metrics)} result(s), {len(fieldnames)} metrics")
        
    except IOError as e:
        print(f"Error writing CSV: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()
