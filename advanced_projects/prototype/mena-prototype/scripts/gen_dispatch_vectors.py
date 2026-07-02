#!/usr/bin/env python3
import json
import random
import os
import sys

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from scripts.golden_dispatch import get_dispatch_order

def main():
    test_cases = []
    
    # 1. All tokens route to same expert (expert 3)
    c1 = [[3, 3] for _ in range(32)]
    test_cases.append(("All tokens route to expert 3", c1))
    
    # 2. Hot expert routing (only experts 0 and 1)
    random.seed(42)
    c2 = []
    for _ in range(32):
        c2.append([random.choice([0, 1]), random.choice([0, 1])])
    test_cases.append(("Hot expert routing (only 0 and 1)", c2))
    
    # 3. Empty expert queue (expert 2 and 5 are completely empty, others random)
    c3 = []
    for _ in range(32):
        e1 = random.choice([0, 1, 3, 4, 6, 7])
        e2 = random.choice([0, 1, 3, 4, 6, 7])
        c3.append([e1, e2])
    test_cases.append(("Empty expert queues (2 and 5 empty)", c3))
    
    # 4. Standard synthetic random routing (all 8 experts)
    c4 = []
    for _ in range(32):
        c4.append([random.randint(0, 7), random.randint(0, 7)])
    test_cases.append(("Synthetic random routing", c4))
    
    # 5. Token dispatched to two different experts for sure
    c5 = []
    for i in range(32):
        # ensure exp1 != exp2
        e1 = i % 8
        e2 = (i + 1) % 8
        c5.append([e1, e2])
    test_cases.append(("Token routed to two different experts", c5))

    # Process and build vector lists
    vectors_json = []
    for name, token_experts in test_cases:
        counts, sequence = get_dispatch_order(token_experts)
        vectors_json.append({
            "name": name,
            "token_experts": token_experts,
            "expert_counts": counts,
            "dispatch_sequence": [{"expert_id": s[0], "token_id": s[1]} for s in sequence]
        })
        
    out_json = os.path.abspath(os.path.join(os.path.dirname(__file__), "../tb/dispatch_vectors.json"))
    os.makedirs(os.path.dirname(out_json), exist_ok=True)
    with open(out_json, "w") as f:
        json.dump(vectors_json, f, indent=2)
    print(f"Generated {len(vectors_json)} dispatch test cases in {out_json}")
    
    # Generate C++ header
    out_h = os.path.abspath(os.path.join(os.path.dirname(__file__), "../verilator/dispatch_vectors.h"))
    with open(out_h, "w") as f:
        f.write("// Auto-generated test vectors for token_dispatcher\n")
        f.write("#ifndef DISPATCH_VECTORS_H\n")
        f.write("#define DISPATCH_VECTORS_H\n\n")
        
        f.write("struct DispatchStep {\n")
        f.write("    unsigned char expert_id;\n")
        f.write("    unsigned char token_id;\n")
        f.write("};\n\n")
        
        f.write("struct DispatchTestCase {\n")
        f.write("    const char* name;\n")
        f.write("    unsigned char token_experts[32][2];\n")
        f.write("    unsigned char gold_counts[8];\n")
        f.write("    int num_dispatches;\n")
        f.write("    DispatchStep gold_steps[64];\n")
        f.write("};\n\n")
        
        f.write(f"const int NUM_DISPATCH_TEST_CASES = {len(vectors_json)};\n\n")
        f.write("const DispatchTestCase DISPATCH_TEST_CASES[] = {\n")
        
        for case in vectors_json:
            f.write("    {\n")
            f.write(f'        "{case["name"]}",\n')
            
            # token_experts array
            f.write("        {\n")
            for pair in case["token_experts"]:
                f.write(f"            {{{pair[0]}, {pair[1]}}},\n")
            f.write("        },\n")
            
            # gold_counts
            counts_str = ", ".join(map(str, case["expert_counts"]))
            f.write(f"        {{{counts_str}}},\n")
            
            # num_dispatches
            f.write(f"        {len(case['dispatch_sequence'])},\n")
            
            # gold_steps
            f.write("        {\n")
            for i, step in enumerate(case["dispatch_sequence"]):
                f.write(f"            {{{step['expert_id']}, {step['token_id']}}},\n")
            # Fill remaining with 0
            for i in range(len(case["dispatch_sequence"]), 64):
                f.write("            {0, 0},\n")
            f.write("        }\n")
            f.write("    },\n")
            
        f.write("};\n\n")
        f.write("#endif // DISPATCH_VECTORS_H\n")
    print(f"Generated C++ header dispatch vectors in {out_h}")

if __name__ == "__main__":
    main()
