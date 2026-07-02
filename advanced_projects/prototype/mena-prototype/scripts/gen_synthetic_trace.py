#!/usr/bin/env python3
import argparse
import json
import os
import numpy as np

def parse_args():
    parser = argparse.ArgumentParser(description="Synthetic MoE Routing Trace Generator")
    parser.add_argument("--num_layers", type=int, default=4, help="Number of MoE layers")
    parser.add_argument("--num_tokens", type=int, default=128, help="Number of tokens per forward pass")
    parser.add_argument("--num_experts", type=int, default=8, help="Total number of experts")
    parser.add_argument("--top_k", type=int, default=2, help="Number of experts to route to per token")
    parser.add_argument("--score_width", type=int, default=16, help="Bit-width of scores for quantization (optional)")
    parser.add_argument("--skew_type", type=str, default="uniform", 
                        choices=["uniform", "zipf", "hot_expert", "layer_shift"],
                        help="Skew type of routing probabilities")
    parser.add_argument("--zipf_alpha", type=float, default=1.5,
                        help="Zipf exponent alpha (default 1.5). Higher = more skewed. Used with --skew_type zipf or layer_shift.")
    parser.add_argument("--seed", type=int, default=42, help="Random seed")
    parser.add_argument("--output", type=str, required=True, help="Output JSONL trace file path")
    return parser.parse_args()

def main():
    args = parse_args()
    np.random.seed(args.seed)
    
    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    
    with open(args.output, "w") as f:
        for layer in range(args.num_layers):
            if args.skew_type == "uniform":
                base_logits = np.zeros(args.num_experts)
            elif args.skew_type == "zipf":
                ranks = np.arange(1, args.num_experts + 1)
                zipf_probs = 1.0 / (ranks ** args.zipf_alpha)
                zipf_probs /= zipf_probs.sum()
                base_logits = np.log(zipf_probs + 1e-9)
            elif args.skew_type == "hot_expert":
                probs = np.zeros(args.num_experts)
                if args.num_experts >= 2:
                    probs[0] = 0.50
                    probs[1] = 0.30
                    remaining = 0.20 / (args.num_experts - 2)
                    probs[2:] = remaining
                else:
                    probs[0] = 1.0
                base_logits = np.log(probs + 1e-9)
            elif args.skew_type == "layer_shift":
                ranks = np.arange(1, args.num_experts + 1)
                zipf_probs = 1.0 / (ranks ** args.zipf_alpha)
                zipf_probs /= zipf_probs.sum()
                # Shift distribution by layer to simulate dynamic expert caching requirements
                shifted_probs = np.roll(zipf_probs, layer)
                base_logits = np.log(shifted_probs + 1e-9)
            else:
                raise ValueError(f"Unknown skew_type: {args.skew_type}")
            
            for token in range(args.num_tokens):
                # Add some random noise to represent router logit variance
                noise = np.random.normal(0, 0.5, size=args.num_experts)
                logits = base_logits + noise
                
                # Softmax to get probabilities (routing scores)
                exp_logits = np.exp(logits - np.max(logits))
                scores = exp_logits / np.sum(exp_logits)
                
                # Optional: quantize scores to simulate hardware effects
                if args.score_width > 0:
                    scale = (1 << (args.score_width - 1)) - 1
                    quantized = np.round(scores * scale) / scale
                    # Renormalize to ensure sum to 1
                    sum_q = quantized.sum()
                    if sum_q > 0:
                        scores = quantized / sum_q
                    else:
                        scores = quantized
                
                # Determine top-k expert selection
                topk_idx = np.argsort(scores)[::-1][:args.top_k].tolist()
                topk_scores = scores[topk_idx].tolist()
                
                line = {
                    "layer": int(layer),
                    "token": int(token),
                    "scores": [float(x) for x in scores],
                    "topk_experts": [int(x) for x in topk_idx],
                    "topk_scores": [float(x) for x in topk_scores]
                }
                f.write(json.dumps(line) + "\n")

    print(f"Generated trace file: {args.output}")

if __name__ == "__main__":
    main()
