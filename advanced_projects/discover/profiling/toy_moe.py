import os
import time
import argparse
import json
import pandas as pd
import torch
import torch.nn as nn

class ToyExpert(nn.Module):
    def __init__(self, d_model, d_ff):
        super().__init__()
        self.w1 = nn.Linear(d_model, d_ff)
        self.w2 = nn.Linear(d_ff, d_model)
        self.act = nn.GELU()
        
    def forward(self, x):
        return self.w2(self.act(self.w1(x)))

class ToyRouter(nn.Module):
    def __init__(self, d_model, num_experts, top_k=2):
        super().__init__()
        self.gate = nn.Linear(d_model, num_experts, bias=False)
        with torch.no_grad():
            nn.init.orthogonal_(self.gate.weight)
        self.top_k = top_k
        
    def forward(self, x):
        logits = self.gate(x)
        scores = torch.softmax(logits, dim=-1)
        topk_scores, topk_indices = torch.topk(scores, self.top_k, dim=-1)
        topk_scores_norm = topk_scores / (topk_scores.sum(dim=-1, keepdim=True) + 1e-6)
        return topk_scores_norm, topk_indices

class ToyMoELayer(nn.Module):
    def __init__(self, d_model, d_ff, num_experts, top_k=2):
        super().__init__()
        self.num_experts = num_experts
        self.top_k = top_k
        self.router = ToyRouter(d_model, num_experts, top_k)
        self.experts = nn.ModuleList([ToyExpert(d_model, d_ff) for _ in range(num_experts)])
        
    def forward(self, x):
        orig_shape = x.shape
        x_flat = x.view(-1, orig_shape[-1]) # [total_tokens, d_model]
        
        scores, indices = self.router(x_flat) # [total_tokens, top_k]
        
        out_flat = torch.zeros_like(x_flat)
        
        # Vectorized expert processing
        for exp_id in range(self.num_experts):
            mask = (indices == exp_id)
            if not mask.any():
                continue
            token_indices, topk_ranks = torch.where(mask)
            exp_in = x_flat[token_indices]
            exp_out = self.experts[exp_id](exp_in)
            
            gate_scores = scores[token_indices, topk_ranks].unsqueeze(-1)
            out_flat.index_add_(0, token_indices, gate_scores * exp_out)
            
        return out_flat.view(orig_shape), (scores, indices)

class ToyMoEModel(nn.Module):
    def __init__(self, vocab_size, d_model, d_ff, num_layers, num_experts, top_k=2):
        super().__init__()
        self.embedding = nn.Embedding(vocab_size, d_model)
        self.layers = nn.ModuleList([
            ToyMoELayer(d_model, d_ff, num_experts, top_k)
            for _ in range(num_layers)
        ])
        self.lm_head = nn.Linear(d_model, vocab_size, bias=False)
        
    def forward(self, input_ids):
        x = self.embedding(input_ids)
        all_layer_routing = []
        for layer in self.layers:
            x, routing = layer(x)
            all_layer_routing.append(routing)
        logits = self.lm_head(x)
        return logits, all_layer_routing

def profile_model(args):
    device = torch.device("cuda" if torch.cuda.is_available() and args.device == "gpu" else "cpu")
    print(f"Using device: {device}")
    
    vocab_size = 1000
    d_model = 256
    d_ff = 512
    num_layers = args.num_layers
    num_experts = args.num_experts
    top_k = args.top_k
    
    model = ToyMoEModel(vocab_size, d_model, d_ff, num_layers, num_experts, top_k).to(device)
    model.eval()
    
    total_params = sum(p.numel() for p in model.parameters())
    print(f"Model parameters: {total_params / 1e6:.2f}M")
    
    batch_size = args.batch_size
    prompt_len = args.prompt_len
    gen_len = args.gen_len
    
    # Generate structured inputs to simulate realistic temporal locality
    base_pattern = torch.randint(10, 100, (1, prompt_len // 4))
    repeated_pattern = base_pattern.repeat(1, 4)
    input_ids = repeated_pattern.repeat(batch_size, 1).to(device)
    
    t0 = time.time()
    with torch.no_grad():
        logits, prefill_routing = model(input_ids)
    prefill_time = time.time() - t0
    ttft_ms = prefill_time * 1000
    
    decode_times = []
    routing_trace = []
    
    current_ids = input_ids
    for step in range(gen_len):
        t_step_start = time.time()
        
        with torch.no_grad():
            logits, step_routing = model(current_ids)
            next_token = torch.argmax(logits[:, -1, :], dim=-1, keepdim=True)
            current_ids = torch.cat([current_ids, next_token], dim=-1)
            
        t_step_end = time.time()
        decode_times.append(t_step_end - t_step_start)
        
        seq_len = current_ids.size(1) - 1
        for batch_idx in range(batch_size):
            for layer_idx in range(num_layers):
                flat_idx = batch_idx * seq_len + (seq_len - 1)
                scores, indices = step_routing[layer_idx]
                for rank in range(top_k):
                    exp_id = indices[flat_idx, rank].item()
                    score = scores[flat_idx, rank].item()
                    routing_trace.append({
                        "request_id": batch_idx,
                        "token_id": step,
                        "layer_id": layer_idx,
                        "position": seq_len,
                        "topk_rank": rank,
                        "expert_id": exp_id,
                        "router_score": score,
                        "timestamp": time.time()
                    })
                    
    avg_decode_ms = sum(decode_times) / len(decode_times) * 1000
    tokens_per_sec = (batch_size * gen_len) / sum(decode_times)
    
    peak_gpu_mb = torch.cuda.max_memory_allocated(device) / (1024 * 1024) if device.type == "cuda" else 0
    
    peak_cpu_mb = 0.0
    try:
        with open("/proc/self/status", "r") as f:
            for line in f:
                if line.startswith("VmRSS:"):
                    peak_cpu_mb = float(line.split()[1]) / 1024.0
                    break
    except Exception:
        pass
        
    print(f"TTFT: {ttft_ms:.2f} ms")
    print(f"Avg Decode Latency: {avg_decode_ms:.2f} ms/token")
    print(f"Throughput: {tokens_per_sec:.2f} tokens/s")
    print(f"Peak GPU Memory: {peak_gpu_mb:.2f} MB")
    print(f"Peak CPU Memory: {peak_cpu_mb:.2f} MB")
    
    os.makedirs(args.output_dir, exist_ok=True)
    
    result_data = {
        "ttft_ms": ttft_ms,
        "avg_decode_latency_ms": avg_decode_ms,
        "tokens_per_second": tokens_per_sec,
        "peak_gpu_memory_mb": peak_gpu_mb,
        "peak_cpu_memory_mb": peak_cpu_mb,
        "total_runtime_s": sum(decode_times) + prefill_time,
        "is_oom": False
    }
    
    with open(os.path.join(args.output_dir, f"metrics_b{batch_size}_p{prompt_len}_g{gen_len}.json"), "w") as f:
        json.dump(result_data, f, indent=4)
        
    df_trace = pd.DataFrame(routing_trace)
    trace_path = os.path.join(args.output_dir, f"routing_trace_b{batch_size}_p{prompt_len}_g{gen_len}.csv")
    df_trace.to_csv(trace_path, index=False)
    print(f"Routing trace saved to {trace_path}")
    
    return result_data

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--batch_size", type=int, default=1)
    parser.add_argument("--prompt_len", type=int, default=128)
    parser.add_argument("--gen_len", type=int, default=128)
    parser.add_argument("--num_layers", type=int, default=6)
    parser.add_argument("--num_experts", type=int, default=8)
    parser.add_argument("--top_k", type=int, default=2)
    parser.add_argument("--device", type=str, default="gpu")
    parser.add_argument("--output_dir", type=str, default="/home/a/discover/profiling")
    args = parser.parse_args()
    
    profile_model(args)
