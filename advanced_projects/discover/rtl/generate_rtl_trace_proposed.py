import os
import pandas as pd
import numpy as np
import argparse

def generate_trace(args):
    os.makedirs(args.output_dir, exist_ok=True)
    df = pd.read_csv(args.trace_file).sort_values(by=["request_id", "token_id", "layer_id", "topk_rank"])
    
    output_path = os.path.join(args.output_dir, "sim_trace_proposed.txt")
    print(f"Generating proposed RTL simulation trace from {args.trace_file}...")
    
    # Calculate transition probabilities for confidence filtering
    num_experts = df["expert_id"].max() + 1
    transition_counts = np.zeros((num_experts, num_experts))
    for (req_id, layer_id), group in df.groupby(["request_id", "layer_id"]):
        group = group.sort_values(by=["token_id", "topk_rank"])
        tok_experts = group.groupby("token_id")["expert_id"].apply(list).sort_index()
        tokens = tok_experts.index.tolist()
        for i in range(len(tokens) - 1):
            for e1 in tok_experts[tokens[i]]:
                for e2 in tok_experts[tokens[i+1]]:
                    transition_counts[e1, e2] += 1
    row_sums = transition_counts.sum(axis=1, keepdims=True)
    transition_prob = np.where(row_sums > 0, transition_counts / row_sums, 0.0)
    
    grouped = df.groupby(["request_id", "token_id"])
    steps = list(grouped.groups.keys())
    
    # Format of output line:
    # event_type req_id layer_id expert_id kv_block_id size_bytes deadline confidence criticality
    # event_types: 0 (EXPERT_DEMAND), 1 (EXPERT_PREFETCH), 2 (KV_ACCESS)
    
    with open(output_path, "w") as f:
        for idx, (req_id, tok_id) in enumerate(steps):
            token_group = grouped.get_group((req_id, tok_id))
            
            # 1. Prefetch inject: look at next step's experts
            if idx + 1 < len(steps):
                next_tok_group = grouped.get_group(steps[idx+1])
                for layer_id, layer_group in next_tok_group.groupby("layer_id"):
                    # For each expert accessed next step, check if we should trigger a prefetch
                    curr_exps = token_group[token_group["layer_id"] == layer_id]["expert_id"].unique()
                    next_exps = layer_group["expert_id"].unique()
                    
                    for n_exp in next_exps:
                        # Find transition confidence from current experts
                        confidence = 0.0
                        for c_exp in curr_exps:
                            confidence = max(confidence, transition_prob[c_exp, n_exp])
                        
                        confidence_pct = int(confidence * 100)
                        
                        # event_type=1 (PREFETCH), req_id, layer_id, expert_id, kv_block_id=0
                        # size = 64MB, deadline = 200 cycles, confidence, criticality = 0
                        f.write(f"1 {req_id} {layer_id} {n_exp} 0 67108864 200 {confidence_pct} 0\n")
            
            # 2. Access events for current step
            for layer_id, layer_group in token_group.groupby("layer_id"):
                curr_exps = layer_group["expert_id"].unique()
                
                # KV cache block access: block_id is token_id / 16 (block_size)
                # event_type=2 (KV_ACCESS), req_id, layer_id, expert_id=0, kv_block_id = tok_id / 16
                # size = 8MB, deadline = 25 cycles, confidence = 100, criticality = 1
                kv_block = int(tok_id // 16)
                f.write(f"2 {req_id} {layer_id} 0 {kv_block} 8388608 25 100 1\n")
                
                # Expert weight accesses
                for exp in curr_exps:
                    # event_type=0 (EXPERT_DEMAND), req_id, layer_id, expert_id, kv_block_id=0
                    # size = 64MB, deadline = 100 cycles, confidence = 100, criticality = 0
                    f.write(f"0 {req_id} {layer_id} {exp} 0 67108864 100 100 0\n")
                    
    print(f"Proposed RTL simulation trace written to {output_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--trace_file", type=str, required=True)
    parser.add_argument("--output_dir", type=str, default="/home/a/discover/rtl")
    args = parser.parse_args()
    
    generate_trace(args)
