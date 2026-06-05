#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import argparse
import random
import math
import csv

def parse_args():
    parser = argparse.ArgumentParser(description="Generate synthetic MoE routing and hint traces.")
    parser.add_argument("--num-experts", type=int, default=8, help="Number of experts")
    parser.add_argument("--top-k", type=int, default=1, help="Top-k experts per token")
    parser.add_argument("--num-layers", type=int, default=1, help="Number of MoE layers")
    parser.add_argument("--num-tokens", type=int, default=64, help="Number of tokens")
    parser.add_argument("--distribution", type=str, default="zipf", choices=["uniform", "zipf", "phase_shift"], help="Distribution type")
    parser.add_argument("--zipf-s", type=float, default=1.2, help="Zipf parameter s")
    parser.add_argument("--phase-length", type=int, default=200, help="Phase length for phase_shift")
    parser.add_argument("--prefetch-distance", type=int, default=4, help="Prefetch distance")
    parser.add_argument("--output", type=str, required=True, help="Path to output demand trace CSV")
    parser.add_argument("--output-hint", type=str, required=True, help="Path to output hint trace CSV")
    parser.add_argument("--seed", type=int, default=42, help="Random seed")
    parser.add_argument("--hint-source", type=str, default="synthetic_scripted_hint", choices=["synthetic_scripted_hint", "synthetic_future_offset"], help="Hint source label")
    return parser.parse_args()

def sample_zipf(n, s):
    # Pure Python Zipf sampling
    weights = [1.0 / math.pow(i + 1, s) for i in range(n)]
    total = sum(weights)
    probs = [w / total for w in weights]
    
    # Cumulative probabilities
    cum_probs = []
    current = 0.0
    for p in probs:
        current += p
        cum_probs.append(current)
    
    r = random.random()
    for idx, cp in enumerate(cum_probs):
        if r <= cp:
            return idx
    return n - 1

def generate_traces():
    args = parse_args()
    random.seed(args.seed)
    
    num_experts = args.num_experts
    top_k = args.top_k
    num_layers = args.num_layers
    num_tokens = args.num_tokens
    dist = args.distribution
    zipf_s = args.zipf_s
    phase_len = args.phase_length
    prefetch_dist = args.prefetch_distance
    
    # Output arrays
    demand_records = []
    hint_records = []
    
    global_request_id = 0
    
    # For phase_shift
    phase_hot_experts = []
    num_phases = (num_tokens + phase_len - 1) // phase_len
    for p in range(num_phases):
        # Pick 2 hot experts for this phase
        hot = random.sample(range(num_experts), min(2, num_experts))
        phase_hot_experts.append(hot)
        
    for t in range(num_tokens):
        # We process tokens layer by layer.
        # In a real model, a token goes through Layer 0, then Layer 1, etc.
        # So we can increment the virtual cycle by 1 for each token step, or layer step.
        # Let's say each token takes 1 cycle per layer.
        # cycle = token_id * num_layers + layer_id
        for l in range(num_layers):
            cycle = t * num_layers + l
            
            # Select top-k experts
            selected_experts = []
            
            if dist == "uniform":
                selected_experts = random.sample(range(num_experts), top_k)
            elif dist == "zipf":
                # Sample top_k distinct experts using Zipf weights
                candidates = list(range(num_experts))
                # Compute Zipf weights
                weights = [1.0 / math.pow(i + 1, zipf_s) for i in candidates]
                total_w = sum(weights)
                probs = [w / total_w for w in weights]
                
                # Sample without replacement
                while len(selected_experts) < top_k:
                    r = random.random()
                    curr = 0.0
                    for idx, p in enumerate(probs):
                        curr += p
                        if r <= curr:
                            exp_id = candidates[idx]
                            if exp_id not in selected_experts:
                                selected_experts.append(exp_id)
                            break
            elif dist == "phase_shift":
                phase_idx = t // phase_len
                hot = phase_hot_experts[phase_idx]
                
                # Sample top_k experts. Hot experts have 80% prob, cold have 20%
                while len(selected_experts) < top_k:
                    if random.random() < 0.8 and len(hot) > 0:
                        exp_id = random.choice(hot)
                    else:
                        exp_id = random.choice(range(num_experts))
                    if exp_id not in selected_experts:
                        selected_experts.append(exp_id)
            
            # Generate router scores (softmax-like, sorted descending)
            raw_scores = [random.random() for _ in range(top_k)]
            raw_scores.sort(reverse=True)
            sum_raw = sum(raw_scores) + 0.1 # Leave some score for other experts
            scores = [round(s / sum_raw, 4) for s in raw_scores]
            
            # Create demand records
            for rank, exp_id in enumerate(selected_experts):
                rec = {
                    "cycle": cycle,
                    "request_id": global_request_id,
                    "layer_id": l,
                    "token_id": t,
                    "topk_rank": rank,
                    "expert_id": exp_id,
                    "router_score": scores[rank],
                    "token_size_bytes": 256,
                    "expert_weight_size_bytes": 262144,
                    "source": "synthetic"
                }
                demand_records.append(rec)
                
                # Prefetch Hint generation:
                # We need to simulate that router sends hint prefetch_dist cycles early.
                # If target_cycle = cycle, hint_cycle = target_cycle - prefetch_dist.
                # If hint_cycle < 0, we can clamp it to 0 or drop.
                # Here we clamp hint_cycle to 0 as it's the start of execution.
                hint_cycle = max(0, cycle - prefetch_dist)
                
                hint_rec = {
                    "hint_cycle": hint_cycle,
                    "target_cycle": cycle,
                    "layer_id": l,
                    "token_id": t,
                    "hint_expert_id": exp_id,
                    "hint_score": scores[rank],
                    "hint_source": args.hint_source
                }
                hint_records.append(hint_rec)
                
                global_request_id += 1
                
    # Warn if using future offset
    if args.hint_source == "synthetic_future_offset":
        print("WARNING: synthetic_future_offset is generated from future targets. It MUST NOT be used for actual deployable performance claims.", file=sys.stderr)
        
    # Ensure directories exist
    os.makedirs(os.path.dirname(os.path.abspath(args.output)), exist_ok=True)
    os.makedirs(os.path.dirname(os.path.abspath(args.output_hint)), exist_ok=True)
    
    # Write demand trace
    with open(args.output, 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=[
            "cycle", "request_id", "layer_id", "token_id", "topk_rank", 
            "expert_id", "router_score", "token_size_bytes", 
            "expert_weight_size_bytes", "source"
        ])
        writer.writeheader()
        writer.writerows(demand_records)
        
    # Write hint trace
    with open(args.output_hint, 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=[
            "hint_cycle", "target_cycle", "layer_id", "token_id", 
            "hint_expert_id", "hint_score", "hint_source"
        ])
        writer.writeheader()
        writer.writerows(hint_records)
        
    print(f"Generated {len(demand_records)} demand requests and {len(hint_records)} prefetch hints.")
    print(f"Demand trace saved to: {args.output}")
    print(f"Hint trace saved to: {args.output_hint}")

if __name__ == "__main__":
    generate_traces()
