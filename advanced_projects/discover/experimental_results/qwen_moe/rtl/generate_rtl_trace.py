import os
import pandas as pd
import argparse

def generate_trace(args):
    os.makedirs(args.output_dir, exist_ok=True)
    df = pd.read_csv(args.trace_file)
    
    # Sort trace
    df = df.sort_values(by=["request_id", "token_id", "layer_id", "topk_rank"])
    
    output_path = os.path.join(args.output_dir, "sim_trace.txt")
    print(f"Generating RTL simulation trace from {args.trace_file}...")
    
    # We will simulate a sequential sequence of events for the RTL
    # For each token generation step and layer:
    # 1. Access KV cache block (EVENT_KV_ACCESS = 2)
    # 2. Access routing experts (EVENT_EXPERT_ACCESS = 0)
    # We also inject some prefetches (EVENT_EXPERT_PREFETCH = 1) for the next step's experts
    
    # Group by step (token_id)
    grouped = df.groupby(["request_id", "token_id"])
    steps = list(grouped.groups.keys())
    
    with open(output_path, "w") as f:
        for idx, (req_id, tok_id) in enumerate(steps):
            token_group = grouped.get_group((req_id, tok_id))
            
            # Prefetch inject: look at next step's experts and inject prefetch events
            if idx + 1 < len(steps):
                next_tok_group = grouped.get_group(steps[idx+1])
                for layer_id, layer_group in next_tok_group.groupby("layer_id"):
                    next_exps = layer_group["expert_id"].unique()
                    for exp in next_exps:
                        # event_type=1 (PREFETCH), layer_id, expert_id, kv_block_id=0
                        f.write(f"1 {layer_id} {exp} 0\n")
            
            # Access events for current step
            for layer_id, layer_group in token_group.groupby("layer_id"):
                curr_exps = layer_group["expert_id"].unique()
                
                # KV cache block access: block_id is token_id / 16 (block_size)
                # event_type=2 (KV_ACCESS), layer_id, expert_id=0, kv_block_id = tok_id / 16
                kv_block = int(tok_id // 16)
                f.write(f"2 {layer_id} 0 {kv_block}\n")
                
                # Expert weight accesses
                for exp in curr_exps:
                    # event_type=0 (EXPERT_ACCESS), layer_id, expert_id, kv_block_id=0
                    f.write(f"0 {layer_id} {exp} 0\n")
                    
    print(f"RTL trace written to {output_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--trace_file", type=str, required=True)
    parser.add_argument("--output_dir", type=str, default="/home/a/discover/rtl")
    args = parser.parse_args()
    
    generate_trace(args)
