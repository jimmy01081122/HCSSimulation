import os
import time
import json
import pandas as pd
import torch
import packaging.version

# Monkey-patch packaging.version.parse to handle 'N/A' version string in gguf package
_orig_parse = packaging.version.parse
def custom_parse(version):
    if version == 'N/A' or not isinstance(version, str):
        return _orig_parse('0.19.0')
    try:
        return _orig_parse(version)
    except Exception:
        return _orig_parse('0.19.0')
packaging.version.parse = custom_parse

from transformers import AutoTokenizer, AutoModelForCausalLM
from datasets import load_dataset

routing_events = []
current_token_step = 0
current_request_id = 0

def make_gate_hook(layer_idx):
    def hook(module, input_states, output_logits):
        if isinstance(output_logits, tuple):
            logits = output_logits[0]
        else:
            logits = output_logits
            
        with torch.no_grad():
            scores = torch.softmax(logits.float(), dim=-1)
            k = 4
            topk_scores, topk_indices = torch.topk(scores, k=k, dim=-1)
            num_tokens = logits.size(0)
            
            for r in range(k):
                exp_id = topk_indices[-1, r].item()
                score = topk_scores[-1, r].item()
                routing_events.append({
                    "request_id": current_request_id,
                    "token_id": current_token_step,
                    "layer_id": layer_idx,
                    "position": num_tokens - 1,
                    "topk_rank": r,
                    "expert_id": exp_id,
                    "router_score": score,
                    "timestamp": time.time()
                })
    return hook

def main():
    global current_token_step, current_request_id, routing_events
    
    print("=========================================================")
    print("   Qwen1.5-MoE-A2.7B GGUF GSM8K Profiling Pipeline       ")
    print("=========================================================")
    
    repo_id = "Kieran2828/Qwen1.5-MoE-A2.7B-Q4_K_M-GGUF"
    gguf_file = "qwen1.5-moe-a2.7b-q4_k_m.gguf"
    
    print(f"Loading tokenizer from {repo_id}...")
    tokenizer = AutoTokenizer.from_pretrained(repo_id, gguf_file=gguf_file, trust_remote_code=True)
    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token
        
    print(f"Loading GGUF model: {gguf_file} on CPU with offloading...")
    t0 = time.time()
    
    offload_dir = "/home/a/discover/profiling/offload_qwen_gguf"
    os.makedirs(offload_dir, exist_ok=True)
    
    # Constrain memory to prevent WSL2 OOM (CPU limit of 10GiB is safe)
    max_mem = {"cpu": "10GiB"}
    
    # Load GGUF model natively as a standard PyTorch module in bfloat16
    model = AutoModelForCausalLM.from_pretrained(
        repo_id,
        gguf_file=gguf_file,
        torch_dtype=torch.bfloat16,
        device_map="auto",
        max_memory=max_mem,
        offload_folder=offload_dir,
        offload_state_dict=True,
        low_cpu_mem_usage=True
    )
    model.eval()
    print(f"Model loaded successfully in {time.time() - t0:.2f} seconds.")
    
    # Register forward hooks on model gate modules
    hook_handles = []
    registered_count = 0
    for name, module in model.named_modules():
        if name.endswith(".mlp.gate"):
            parts = name.split(".")
            try:
                layer_idx = int(parts[parts.index("layers") + 1])
                handle = module.register_forward_hook(make_gate_hook(layer_idx))
                hook_handles.append(handle)
                registered_count += 1
            except Exception as e:
                print(f"Failed to hook layer {name}: {e}")
                
    print(f"Registered forward hooks on {registered_count} MoE router gates.")
    
    print("Loading GSM8K test dataset...")
    dataset = load_dataset("openai/gsm8k", "main", split="test")
    
    samples = dataset.select(range(2))
    results = []
    
    for idx, sample in enumerate(samples):
        current_request_id = idx
        question = sample["question"]
        ref_answer = sample["answer"]
        
        prompt = f"Question: {question}\nAnswer:"
        inputs = tokenizer(prompt, return_tensors="pt")
        input_ids = inputs["input_ids"]
        prompt_len = input_ids.size(1)
        
        print(f"\n--- Running Sample {idx+1}/{len(samples)} ---")
        print(f"Prompt length: {prompt_len} tokens")
        
        # Prefill stage
        current_token_step = 0
        t_prefill_start = time.time()
        with torch.no_grad():
            outputs = model(input_ids)
        t_prefill_end = time.time()
        ttft_ms = (t_prefill_end - t_prefill_start) * 1000.0
        
        # Decode stage
        max_new_tokens = 32
        decode_latencies = []
        generated_tokens = []
        current_ids = input_ids
        
        for step in range(max_new_tokens):
            current_token_step = step + 1
            t_step_start = time.time()
            with torch.no_grad():
                outputs = model(current_ids)
                next_token = torch.argmax(outputs.logits[:, -1, :], dim=-1, keepdim=True)
                current_ids = torch.cat([current_ids, next_token], dim=-1)
            t_step_end = time.time()
            
            decode_latencies.append((t_step_end - t_step_start) * 1000.0)
            generated_tokens.append(next_token.item())
            
            if next_token.item() == tokenizer.eos_token_id:
                break
                
        avg_decode_ms = sum(decode_latencies) / len(decode_latencies)
        throughput = len(generated_tokens) / (sum(decode_latencies) / 1000.0)
        gen_text = tokenizer.decode(generated_tokens, skip_special_tokens=True)
        
        print(f"TTFT: {ttft_ms:.2f} ms")
        print(f"Avg Decode Latency: {avg_decode_ms:.2f} ms/token")
        print(f"Throughput: {throughput:.2f} tokens/s")
        print(f"Generated snippet: {gen_text[:80]}...")
        
        results.append({
            "sample_idx": idx,
            "prompt_len": prompt_len,
            "gen_len": len(generated_tokens),
            "ttft_ms": ttft_ms,
            "avg_decode_latency_ms": avg_decode_ms,
            "tokens_per_second": throughput,
            "generated_text": gen_text,
            "reference_answer": ref_answer
        })
        
    for handle in hook_handles:
        handle.remove()
        
    output_dir = "/home/a/discover/profiling"
    os.makedirs(output_dir, exist_ok=True)
    
    with open(os.path.join(output_dir, "qwen_moe_metrics.json"), "w") as f:
        json.dump(results, f, indent=4)
        
    df_routing = pd.DataFrame(routing_events)
    df_routing.to_csv(os.path.join(output_dir, "qwen_moe_routing_trace.csv"), index=False)
    print(f"\nProfiling completed. Metrics saved to {output_dir}/qwen_moe_metrics.json and real MoE routing trace to {output_dir}/qwen_moe_routing_trace.csv")

if __name__ == "__main__":
    main()
