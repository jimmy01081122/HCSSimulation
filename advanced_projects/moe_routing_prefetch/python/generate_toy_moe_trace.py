#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import argparse
import random
import csv
import torch
import torch.nn as nn
import torch.nn.functional as F

def parse_args():
    parser = argparse.ArgumentParser(description="Generate Toy PyTorch MoE Routing and Hint traces.")
    parser.add_argument("--hidden-dim", type=int, default=128, help="Hidden dimension")
    parser.add_argument("--ffn-dim", type=int, default=512, help="FFN intermediate dimension")
    parser.add_argument("--num-experts", type=int, default=8, help="Number of experts")
    parser.add_argument("--top-k", type=int, default=2, help="top-k routing")
    parser.add_argument("--num-layers", type=int, default=2, help="MoE layer count")
    parser.add_argument("--batch-size", type=int, default=4, help="Batch size")
    parser.add_argument("--seq-len", type=int, default=128, help="Sequence length")
    parser.add_argument("--num-batches", type=int, default=10, help="Number of batches to process")
    parser.add_argument("--device", type=str, default="cpu", help="cpu or cuda")
    parser.add_argument("--seed", type=int, default=42, help="Random seed")
    parser.add_argument("--router-bias-mode", type=str, default="hot_expert", 
                        choices=["uniform", "hot_expert", "temporal", "layer_bias"], 
                        help="Router bias mode")
    parser.add_argument("--router-temperature", type=float, default=1.0, help="Softmax temperature")
    parser.add_argument("--input-locality-strength", type=float, default=0.5, help="Strength of temporal similarity in inputs")
    parser.add_argument("--prefetch-distance", type=int, default=4, help="Prefetch distance for hint cycle")
    parser.add_argument("--output", type=str, required=True, help="Path to output demand trace CSV")
    parser.add_argument("--output-hint", type=str, required=True, help="Path to output hint trace CSV")
    return parser.parse_args()

class ToyExpert(nn.Module):
    def __init__(self, hidden_dim, ffn_dim):
        super().__init__()
        self.w1 = nn.Linear(hidden_dim, ffn_dim)
        self.w2 = nn.Linear(ffn_dim, hidden_dim)
        
    def forward(self, x):
        return self.w2(F.relu(self.w1(x)))

class ToyMoERouter(nn.Module):
    def __init__(self, hidden_dim, num_experts, top_k, layer_id, bias_mode, temperature):
        super().__init__()
        self.gate = nn.Linear(hidden_dim, num_experts)
        self.num_experts = num_experts
        self.top_k = top_k
        self.layer_id = layer_id
        self.bias_mode = bias_mode
        self.temperature = temperature
        
    def forward(self, x, token_ids):
        # x: (N, hidden_dim) where N = batch_size * seq_len
        # token_ids: (N,) global token indices
        logits = self.gate(x) # (N, num_experts)
        
        # Apply bias mode
        if self.bias_mode == "hot_expert":
            # Bias experts 0 and 3
            logits[:, 0] += 10.0
            logits[:, 3] += 10.0
        elif self.bias_mode == "temporal":
            # Shift bias expert every 50 tokens
            phase_len = 50
            for idx in range(x.size(0)):
                tid = token_ids[idx].item()
                bias_expert = (tid // phase_len) % self.num_experts
                logits[idx, bias_expert] += 10.0
        elif self.bias_mode == "layer_bias":
            # Each layer biases a different expert
            bias_expert = self.layer_id % self.num_experts
            logits[:, bias_expert] += 10.0
            
        # Apply temperature
        logits = logits / self.temperature
        
        # Softmax and Top-K
        scores = F.softmax(logits, dim=-1)
        topk_scores, topk_indices = torch.topk(scores, self.top_k, dim=-1)
        
        # Renormalize scores
        topk_scores = topk_scores / topk_scores.sum(dim=-1, keepdim=True)
        
        return topk_scores, topk_indices

class ToyMoELayer(nn.Module):
    def __init__(self, hidden_dim, ffn_dim, num_experts, top_k, layer_id, bias_mode, temperature):
        super().__init__()
        self.router = ToyMoERouter(hidden_dim, num_experts, top_k, layer_id, bias_mode, temperature)
        self.experts = nn.ModuleList([ToyExpert(hidden_dim, ffn_dim) for _ in range(num_experts)])
        self.top_k = top_k
        
    def forward(self, x, token_ids):
        batch_size, seq_len, hidden_dim = x.shape
        x_flat = x.view(-1, hidden_dim)
        tids_flat = token_ids.view(-1)
        
        scores, indices = self.router(x_flat, tids_flat)
        
        out_flat = torch.zeros_like(x_flat)
        
        # For trace collection we only need scores and indices.
        # But to be a functional PyTorch forward, we also run the experts.
        for i in range(self.top_k):
            exp_idx = indices[:, i] # (N,)
            exp_score = scores[:, i].unsqueeze(-1) # (N, 1)
            
            for e_id, expert in enumerate(self.experts):
                mask = (exp_idx == e_id)
                if mask.any():
                    exp_out = expert(x_flat[mask])
                    out_flat[mask] += exp_score[mask] * exp_out
                    
        return out_flat.view(batch_size, seq_len, hidden_dim), scores, indices

class ToyMoEModel(nn.Module):
    def __init__(self, hidden_dim, ffn_dim, num_experts, top_k, num_layers, bias_mode, temperature):
        super().__init__()
        self.layers = nn.ModuleList([
            ToyMoELayer(hidden_dim, ffn_dim, num_experts, top_k, l, bias_mode, temperature)
            for l in range(num_layers)
        ])
        
    def forward(self, x, token_ids):
        routing_data = []
        for layer in self.layers:
            x, scores, indices = layer(x, token_ids)
            routing_data.append((scores, indices))
        return x, routing_data

def generate_locality_inputs(batch_size, seq_len, hidden_dim, strength):
    """
    Generate inputs using AR-1 process to control temporal locality.
    """
    X = torch.zeros(batch_size, seq_len, hidden_dim)
    for b in range(batch_size):
        prev = torch.randn(hidden_dim)
        for t in range(seq_len):
            curr_rand = torch.randn(hidden_dim)
            curr = strength * prev + (1.0 - strength) * curr_rand
            X[b, t] = curr
            prev = curr
    return X

def main():
    args = parse_args()
    
    # Set seed
    random.seed(args.seed)
    torch.manual_seed(args.seed)
    if torch.cuda.is_available():
        torch.cuda.manual_seed_all(args.seed)
        
    device = torch.device(args.device if torch.cuda.is_available() and args.device == "cuda" else "cpu")
    
    # Build model
    model = ToyMoEModel(
        hidden_dim=args.hidden_dim,
        ffn_dim=args.ffn_dim,
        num_experts=args.num_experts,
        top_k=args.top_k,
        num_layers=args.num_layers,
        bias_mode=args.router_bias_mode,
        temperature=args.router_temperature
    ).to(device)
    
    # Check GPU memory or model footprint (< 1GB requirement)
    param_size = 0
    for param in model.parameters():
        param_size += param.nelement() * param.element_size()
    buffer_size = 0
    for buffer in model.buffers():
        buffer_size += buffer.nelement() * buffer.element_size()
    size_all_mb = (param_size + buffer_size) / 1024**2
    print(f"Model Memory Footprint: {size_all_mb:.2f} MB")
    if size_all_mb > 1000:
        print("WARNING: Model size exceeds 1GB limit!", file=sys.stderr)
        
    demand_records = []
    hint_records = []
    
    global_request_id = 0
    token_offset = 0
    
    model.eval()
    with torch.no_grad():
        for b_idx in range(args.num_batches):
            # Generate inputs with temporal correlation
            inputs = generate_locality_inputs(args.batch_size, args.seq_len, args.hidden_dim, args.input_locality_strength).to(device)
            
            # Global token IDs
            token_ids = torch.zeros(args.batch_size, args.seq_len, dtype=torch.long, device=device)
            for b in range(args.batch_size):
                for t in range(args.seq_len):
                    token_ids[b, t] = token_offset + b * args.seq_len + t
            
            outputs, routing_data = model(inputs, token_ids)
            
            # Process routing data
            # routing_data is a list of (scores, indices) for each layer
            # scores: (batch_size * seq_len, top_k)
            # indices: (batch_size * seq_len, top_k)
            for l_idx, (scores, indices) in enumerate(routing_data):
                scores_cpu = scores.cpu()
                indices_cpu = indices.cpu()
                
                N = scores_cpu.size(0)
                for idx in range(N):
                    # Local token and batch indices
                    b = idx // args.seq_len
                    t = idx % args.seq_len
                    global_t = token_offset + b * args.seq_len + t
                    
                    cycle = global_t * args.num_layers + l_idx
                    
                    for k in range(args.top_k):
                        exp_id = indices_cpu[idx, k].item()
                        score = scores_cpu[idx, k].item()
                        
                        rec = {
                            "cycle": cycle,
                            "request_id": global_request_id,
                            "layer_id": l_idx,
                            "token_id": global_t,
                            "topk_rank": k,
                            "expert_id": exp_id,
                            "router_score": round(score, 4),
                            "token_size_bytes": args.hidden_dim * 2, # float16 size
                            "expert_weight_size_bytes": args.hidden_dim * args.ffn_dim * 2 * 2, # MLP weight footprint
                            "source": "toy_pytorch"
                        }
                        demand_records.append(rec)
                        
                        hint_cycle = max(0, cycle - args.prefetch_distance)
                        hint_rec = {
                            "hint_cycle": hint_cycle,
                            "target_cycle": cycle,
                            "layer_id": l_idx,
                            "token_id": global_t,
                            "hint_expert_id": exp_id,
                            "hint_score": round(score, 4),
                            "hint_source": "toy_router_score"
                        }
                        hint_records.append(hint_rec)
                        
                        global_request_id += 1
                        
            token_offset += args.batch_size * args.seq_len
            
    # Write demand trace
    os.makedirs(os.path.dirname(os.path.abspath(args.output)), exist_ok=True)
    with open(args.output, 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=[
            "cycle", "request_id", "layer_id", "token_id", "topk_rank", 
            "expert_id", "router_score", "token_size_bytes", 
            "expert_weight_size_bytes", "source"
        ])
        writer.writeheader()
        writer.writerows(demand_records)
        
    # Write hint trace
    os.makedirs(os.path.dirname(os.path.abspath(args.output_hint)), exist_ok=True)
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
    main()
