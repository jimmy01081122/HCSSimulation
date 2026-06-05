#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import argparse
import json
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def parse_args():
    parser = argparse.ArgumentParser(description="Analyze MoE routing trace and generate statistics and plots.")
    parser.add_argument("--trace", type=str, required=True, help="Path to input demand trace CSV")
    parser.add_argument("--output-dir", type=str, default="results/", help="Output directory")
    return parser.parse_args()

def load_trace(csv_path) -> pd.DataFrame:
    """Loads the trace CSV file."""
    if not os.path.exists(csv_path):
        raise FileNotFoundError(f"Trace file not found: {csv_path}")
    return pd.read_csv(csv_path)

def compute_expert_frequency(df) -> pd.Series:
    """Computes the activation frequency of each expert."""
    return df['expert_id'].value_counts()

def compute_reuse_distance(df) -> dict:
    """
    Computes the reuse distance for each expert.
    The reuse distance is defined as the difference in request_id (or index)
    between consecutive accesses to the same expert.
    """
    last_seen = {}
    reuse_distances = {}
    
    # Use request_id if available, otherwise use row index
    for idx, row in df.iterrows():
        exp_id = int(row['expert_id'])
        req_id = int(row['request_id']) if 'request_id' in df.columns else idx
        
        if exp_id in last_seen:
            dist = req_id - last_seen[exp_id]
            if exp_id not in reuse_distances:
                reuse_distances[exp_id] = []
            reuse_distances[exp_id].append(dist)
        last_seen[exp_id] = req_id
        
    return reuse_distances

def plot_expert_frequency(freqs: pd.Series, output_path: str):
    """Plots the expert activation frequency bar chart."""
    plt.figure(figsize=(10, 5))
    # Sort index so experts 0, 1, 2... are ordered
    freqs_sorted = freqs.sort_index()
    freqs_sorted.plot(kind='bar', color='royalblue', edgecolor='black')
    plt.title("Expert Activation Frequency")
    plt.xlabel("Expert ID")
    plt.ylabel("Activation Count")
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    plt.close()

def plot_activation_heatmap(df: pd.DataFrame, output_path: str):
    """Plots the expert activation heatmap over cycles."""
    if df.empty:
        return
    max_cycle = int(df['cycle'].max()) if 'cycle' in df.columns else 100
    max_expert = int(df['expert_id'].max()) if 'expert_id' in df.columns else 8
    
    grid = np.zeros((max_expert + 1, max_cycle + 1))
    
    for _, row in df.iterrows():
        c = int(row['cycle'])
        e = int(row['expert_id'])
        score = float(row['router_score']) if 'router_score' in row else 1.0
        if e <= max_expert and c <= max_cycle:
            grid[e, c] = score
        
    plt.figure(figsize=(12, 6))
    plt.imshow(grid, aspect='auto', cmap='YlOrRd', origin='lower')
    plt.colorbar(label='Router Score / Activation')
    plt.title("Expert Activation Heatmap Over Cycles")
    plt.xlabel("Cycle")
    plt.ylabel("Expert ID")
    plt.yticks(range(max_expert + 1))
    plt.grid(True, which='both', color='gray', linestyle='-', linewidth=0.5, alpha=0.3)
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    plt.close()

def export_statistics(stats_dict: dict, output_path: str):
    """Exports the statistics as a JSON file."""
    # Convert numpy types to native Python types
    clean_stats = {}
    for k, v in stats_dict.items():
        if isinstance(v, dict):
            clean_stats[k] = {int(x): int(y) for x, y in v.items()}
        elif isinstance(v, (np.integer, np.floating)):
            clean_stats[k] = v.item()
        else:
            clean_stats[k] = v
            
    with open(output_path, 'w', encoding='utf-8') as f:
        json.dump(clean_stats, f, indent=4, ensure_ascii=False)

def analyze():
    args = parse_args()
    os.makedirs(args.output_dir, exist_ok=True)
    
    df = load_trace(args.trace)
    
    freqs = compute_expert_frequency(df)
    reuse_dists = compute_reuse_distance(df)
    
    all_dists = []
    for dists in reuse_dists.values():
        all_dists.extend(dists)
        
    avg_reuse_dist = float(np.mean(all_dists)) if len(all_dists) > 0 else -1.0
    median_reuse_dist = float(np.median(all_dists)) if len(all_dists) > 0 else -1.0
    
    freq_values = freqs.values
    if len(freq_values) > 1:
        skew_metric = float(np.std(freq_values) / np.mean(freq_values))
    else:
        skew_metric = 0.0
        
    stats = {
        "total_requests": int(len(df)),
        "unique_experts": int(df['expert_id'].nunique()),
        "expert_popularity": freqs.to_dict(),
        "average_reuse_distance": avg_reuse_dist,
        "median_reuse_distance": median_reuse_dist,
        "popularity_skew_std_over_mean": skew_metric,
        "temporal_locality_count": len(all_dists)
    }
    
    plot_expert_frequency(freqs, os.path.join(args.output_dir, "expert_frequency.png"))
    plot_activation_heatmap(df, os.path.join(args.output_dir, "activation_heatmap.png"))
    
    plt.figure(figsize=(10, 5))
    if len(all_dists) > 0:
        plt.hist(all_dists, bins=min(30, max(5, len(set(all_dists)))), color='mediumseagreen', edgecolor='black', alpha=0.7)
    else:
        plt.text(0.5, 0.5, "No reuse occurrences", ha='center', va='center')
    plt.title("Reuse Distance Distribution")
    plt.xlabel("Reuse Distance (Requests)")
    plt.ylabel("Count")
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig(os.path.join(args.output_dir, "reuse_distance_histogram.png"), dpi=150)
    plt.close()
    
    export_statistics(stats, os.path.join(args.output_dir, "trace_statistics.json"))
    print(f"Analysis complete. Results saved in {args.output_dir}")

if __name__ == "__main__":
    analyze()
