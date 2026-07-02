#!/usr/bin/env python3
import argparse
import json
import os
import time
import torch
import torch.nn as nn
import numpy as np

def parse_args():
    parser = argparse.ArgumentParser(description="Toy MoE PyTorch Trace Generator")
    parser.add_argument("--num_layers", type=int, default=4, help="Number of layers")
    parser.add_argument("--num_experts", type=int, default=8, help="Number of experts")
    parser.add_argument("--top_k", type=int, default=2, help="Top-k experts to select")
    parser.add_argument("--hidden_size", type=int, default=64, help="Hidden dimension size")
    parser.add_argument("--batch_size", type=int, default=4, help="Batch size")
    parser.add_argument("--seq_len", type=int, default=32, help="Sequence length")
    parser.add_argument("--seed", type=int, default=42, help="Random seed")
    parser.add_argument("--output", type=str, default="traces/toy_moe_E8_T128_K2.jsonl", help="Output file path")
    return parser.parse_args()

class ToyMoE(nn.Module):
    def __init__(self, num_layers, hidden_size, num_experts):
        super().__init__()
        self.num_layers = num_layers
        self.routers = nn.ModuleList([
            nn.Linear(hidden_size, num_experts) for _ in range(num_layers)
        ])
        
    def forward(self, x):
        # x is [num_tokens, hidden_size]
        # Return logits for each layer
        all_logits = []
        current_x = x
        for router in self.routers:
            logits = router(current_x)
            all_logits.append(logits)
            # Add simple dummy residual mapping to represent computation
            current_x = current_x + torch.tanh(logits @ torch.randn(logits.shape[-1], current_x.shape[-1], device=x.device) * 0.05)
        return all_logits

def main():
    args = parse_args()
    torch.manual_seed(args.seed)
    np.random.seed(args.seed)
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")
    
    gpu_name = "N/A"
    if device.type == "cuda":
        gpu_name = torch.cuda.get_device_name(0)
        print(f"GPU Name: {gpu_name}")
        
    num_tokens = args.batch_size * args.seq_len
    print(f"Generating trace for {args.num_layers} layers, {num_tokens} tokens per layer, {args.num_experts} experts...")
    
    # Instantiate toy model
    model = ToyMoE(args.num_layers, args.hidden_size, args.num_experts).to(device)
    model.eval()
    
    # Generate random input
    input_states = torch.randn(num_tokens, args.hidden_size, device=device)
    
    start_time = time.time()
    with torch.no_grad():
        all_logits = model(input_states)
    end_time = time.time()
    elapsed_time = end_time - start_time
    print(f"Time taken to generate logits: {elapsed_time:.6f} seconds")
    
    # Export to jsonl
    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    
    # Expert load counters
    expert_counts = np.zeros(args.num_experts, dtype=int)
    
    with open(args.output, "w") as f:
        for layer_idx, logits in enumerate(all_logits):
            # Compute softmax scores
            scores = torch.softmax(logits, dim=-1).cpu().numpy()
            
            for token_idx in range(num_tokens):
                tok_scores = scores[token_idx]
                topk_idx = np.argsort(tok_scores)[::-1][:args.top_k]
                topk_scores = tok_scores[topk_idx]
                
                # Update stats
                for idx in topk_idx:
                    expert_counts[idx] += 1
                
                line = {
                    "layer": int(layer_idx),
                    "token": int(token_idx),
                    "scores": [float(x) for x in tok_scores],
                    "topk_experts": [int(x) for x in topk_idx],
                    "topk_scores": [float(x) for x in topk_scores]
                }
                f.write(json.dumps(line) + "\n")
                
    print(f"Trace written to: {args.output}")
    print(f"Overall Expert Loads: {expert_counts.tolist()}")
    
    # Return stats for reporting
    stats = {
        "cuda_available": torch.cuda.is_available(),
        "gpu_name": gpu_name,
        "elapsed_time_sec": elapsed_time,
        "loads": expert_counts.tolist(),
        "imbalance_ratio": float((args.num_experts * np.max(expert_counts)) / np.sum(expert_counts))
    }
    
    with open(args.output + ".stats.json", "w") as sf:
        json.dump(stats, sf, indent=2)

if __name__ == "__main__":
    main()
