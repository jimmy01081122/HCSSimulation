#!/usr/bin/env python3
import json
import sys
import numpy as np

def analyze_trace(filepath):
    print(f"Analyzing: {filepath}")
    
    total_tokens = 0
    layer_expert_counts = {}
    overall_expert_counts = {}
    
    with open(filepath, 'r') as f:
        for line in f:
            data = json.loads(line)
            layer = data["layer"]
            topk = data["topk_experts"]
            
            if layer not in layer_expert_counts:
                layer_expert_counts[layer] = {}
                
            for exp in topk:
                layer_expert_counts[layer][exp] = layer_expert_counts[layer].get(exp, 0) + 1
                overall_expert_counts[exp] = overall_expert_counts.get(exp, 0) + 1
            
            total_tokens += 1
            
    # Find num_experts
    # We can infer it from the scores array length of the last read line, or from keys
    with open(filepath, 'r') as f:
        first_line = json.loads(f.readline())
        num_experts = len(first_line["scores"])
        top_k = len(first_line["topk_experts"])
        
    loads = [overall_expert_counts.get(i, 0) for i in range(num_experts)]
    total_selections = sum(loads)
    
    if total_selections == 0:
        print("No selections found.")
        return
        
    max_load = max(loads)
    min_load = min(loads)
    avg_load = total_selections / num_experts
    imbalance_ratio = (num_experts * max_load) / total_selections if total_selections > 0 else 0
    
    # Entropy of selection probabilities
    p = np.array(loads) / total_selections
    p = p[p > 0]
    entropy = -np.sum(p * np.log2(p))
    max_entropy = np.log2(num_experts)
    normalized_entropy = entropy / max_entropy if max_entropy > 0 else 1.0
    
    top_expert = np.argmax(loads)
    
    print(f"  Num experts: {num_experts}")
    print(f"  Top-k: {top_k}")
    print(f"  Load distribution: {loads}")
    print(f"  Max expert load: {max_load}")
    print(f"  Min expert load: {min_load}")
    print(f"  Imbalance ratio (E * max_load / sum_load): {imbalance_ratio:.4f}")
    print(f"  Top expert ID: {top_expert} (load={max_load})")
    print(f"  Entropy (Normalized): {entropy:.4f} ({normalized_entropy:.4f})")
    print("-" * 40)
    
    return {
        "num_experts": num_experts,
        "top_k": top_k,
        "loads": loads,
        "max_load": max_load,
        "min_load": min_load,
        "imbalance_ratio": imbalance_ratio,
        "top_expert": int(top_expert),
        "entropy": entropy,
        "normalized_entropy": normalized_entropy
    }

if __name__ == "__main__":
    if len(sys.argv) > 1:
        for fp in sys.argv[1:]:
            analyze_trace(fp)
    else:
        print("Usage: analyze_traces.py <trace1.jsonl> [trace2.jsonl ...]")
