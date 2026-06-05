#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import csv
import os

def parse_args():
    parser = argparse.ArgumentParser(description="Convert demand and hint CSV traces to Verilog memory hex files.")
    parser.add_argument("--trace", type=str, required=True, help="Path to demand trace CSV")
    parser.add_argument("--hint-trace", type=str, required=True, help="Path to hint trace CSV")
    parser.add_argument("--output-req", type=str, required=True, help="Path to output demand stimulus hex")
    parser.add_argument("--output-hint", type=str, required=True, help="Path to output hint stimulus hex")
    return parser.parse_args()

def load_csv(path):
    records = []
    if not os.path.exists(path):
        return records
    with open(path, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            records.append(row)
    return records

def convert():
    args = parse_args()
    
    demand_rows = load_csv(args.trace)
    hint_rows = load_csv(args.hint_trace)
    
    if not demand_rows:
        print("Error: Empty or missing demand trace.")
        return
        
    # Determine max cycle
    max_c = 0
    for r in demand_rows:
        max_c = max(max_c, int(r["cycle"]))
    for h in hint_rows:
        max_c = max(max_c, int(h["hint_cycle"]))
        
    # We will generate hex memory files of size max_c + 1
    # Each entry is a 32-bit hex word
    # For req: 
    # Bit 31: valid
    # Bits 7-0: expert_id
    req_mem = [0] * (max_c + 1)
    for r in demand_rows:
        c = int(r["cycle"])
        exp_id = int(r["expert_id"])
        # Format as 32-bit value: 0x80000000 | exp_id
        val = 0x80000000 | (exp_id & 0xFF)
        req_mem[c] = val
        
    # For hint:
    # Bit 31: valid
    # Bits 15-8: score in UQ0.8 fixed point (0-255)
    # Bits 7-0: hint_expert_id
    hint_mem = [0] * (max_c + 1)
    for h in hint_rows:
        c = int(h["hint_cycle"])
        exp_id = int(h["hint_expert_id"])
        score = float(h["hint_score"])
        score_fp = int(round(score * 255.0)) & 0xFF
        val = 0x80000000 | (score_fp << 8) | (exp_id & 0xFF)
        hint_mem[c] = val
        
    # Write output req
    os.makedirs(os.path.dirname(os.path.abspath(args.output_req)), exist_ok=True)
    with open(args.output_req, 'w') as f:
        for val in req_mem:
            f.write(f"{val:08X}\n")
            
    # Write output hint
    os.makedirs(os.path.dirname(os.path.abspath(args.output_hint)), exist_ok=True)
    with open(args.output_hint, 'w') as f:
        for val in hint_mem:
            f.write(f"{val:08X}\n")
            
    print(f"Stimulus files successfully written with {max_c + 1} cycles:")
    print(f"  Req stimulus:  {args.output_req}")
    print(f"  Hint stimulus: {args.output_hint}")

if __name__ == "__main__":
    convert()
