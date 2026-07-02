#!/usr/bin/env python3
import sys

def get_dispatch_order(token_experts, num_experts=8, num_tokens=32):
    # token_experts is a list of lists: [[exp0_0, exp0_1], [exp1_0, exp1_1], ...]
    
    # Calculate counts
    expert_counts = [0] * num_experts
    for t_idx, exps in enumerate(token_experts):
        for e in set(exps): # avoid double counting a token if it maps to the same expert twice
            if e < num_experts:
                expert_counts[e] += 1
                
    # Generate dispatch sequence (expert-major)
    dispatch_sequence = []
    for e in range(num_experts):
        for t_idx in range(num_tokens):
            if e in token_experts[t_idx]:
                dispatch_sequence.append((e, t_idx))
                
    return expert_counts, dispatch_sequence
