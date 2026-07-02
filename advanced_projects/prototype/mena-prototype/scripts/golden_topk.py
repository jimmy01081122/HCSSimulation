#!/usr/bin/env python3
import sys
import json

def get_topk(scores, k=2):
    # Implements the exact same search logic as topk_selector.sv
    # Find max 1
    max1_val = scores[0]
    max1_idx = 0
    for i in range(1, len(scores)):
        if scores[i] > max1_val:
            max1_val = scores[i]
            max1_idx = i
            
    # Find max 2
    if max1_idx == 0:
        max2_val = scores[1]
        max2_idx = 1
    else:
        max2_val = scores[0]
        max2_idx = 0
        
    for i in range(len(scores)):
        if i != max1_idx:
            if scores[i] > max2_val:
                max2_val = scores[i]
                max2_idx = i
                
    return [max1_idx, max2_idx], [max1_val, max2_val]

if __name__ == "__main__":
    # Test golden model itself
    test_scores = [10, 5, 20, 8, 3, 22, 15, 22]
    idx, val = get_topk(test_scores)
    print(f"Scores: {test_scores}")
    print(f"Top-2 Indices: {idx}, Values: {val}")
