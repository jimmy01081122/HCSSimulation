#!/usr/bin/env python3
import json
import random
import os
import sys

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from scripts.golden_topk import get_topk

def generate_cases():
    cases = []
    
    # 1. All scores same
    cases.append([10] * 8)
    cases.append([0] * 8)
    cases.append([65535] * 8)
    
    # 2. Max value at index 0
    cases.append([100, 5, 10, 20, 30, 40, 50, 60])
    cases.append([1000, 1000, 10, 20, 30, 40, 50, 60])
    
    # 3. Max value at last index (7)
    cases.append([5, 10, 20, 30, 40, 50, 60, 100])
    
    # 4. top1/top2 adjacent
    cases.append([5, 10, 100, 99, 40, 50, 60, 30])
    cases.append([10, 100, 99, 10, 10, 10, 10, 10])
    
    # 5. score contains 0
    cases.append([0, 5, 0, 10, 0, 15, 0, 20])
    
    # Random 1000 cases
    random.seed(42)
    for _ in range(1000):
        # 16-bit scores
        scores = [random.randint(0, 1000) for _ in range(8)]
        cases.append(scores)
        
    # Format to json format
    test_vectors = []
    for c in cases:
        idx, val = get_topk(c)
        test_vectors.append({
            "scores": c,
            "topk_idx": idx,
            "topk_score": val
        })
        
    out_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../tb/topk_vectors.json"))
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(test_vectors, f, indent=2)
    print(f"Generated {len(test_vectors)} test vectors in {out_path}")
    
    # Generate C++ Header file
    h_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../verilator/topk_vectors.h"))
    os.makedirs(os.path.dirname(h_path), exist_ok=True)
    with open(h_path, "w") as f:
        f.write("// Auto-generated test vectors for topk_selector\n")
        f.write("#ifndef TOPK_VECTORS_H\n")
        f.write("#define TOPK_VECTORS_H\n\n")
        f.write("struct TopkTestCase {\n")
        f.write("    unsigned short scores[8];\n")
        f.write("    unsigned char gold_idx[2];\n")
        f.write("    unsigned short gold_score[2];\n")
        f.write("};\n\n")
        f.write(f"const int NUM_TOPK_TEST_CASES = {len(test_vectors)};\n\n")
        f.write("const TopkTestCase TOPK_TEST_CASES[] = {\n")
        for tv in test_vectors:
            s_str = ", ".join(map(str, tv["scores"]))
            idx_str = ", ".join(map(str, tv["topk_idx"]))
            score_str = ", ".join(map(str, tv["topk_score"]))
            f.write(f"    {{ {{{s_str}}}, {{{idx_str}}}, {{{score_str}}} }},\n")
        f.write("};\n\n")
        f.write("#endif // TOPK_VECTORS_H\n")
    print(f"Generated C++ header test vectors in {h_path}")

if __name__ == "__main__":
    generate_cases()
