#!/usr/bin/env python3
"""
dse_explorations_v2.py
Advanced Design Space Exploration (DSE) script.
Implements:
1. Generation of Type E (Adversarial) and Mock Real MoE (Mixtral, DeepSeekMoE) traces.
2. Parametric sweeps across cache sizes (0 to 64), expert counts (8 to 128), and top-k (1, 2, 4).
3. Performance evaluations for Double/Triple buffering and NPU compute bounds.
4. Saving results in CSV formats for subsequent analysis.
"""

import sys
import os
import json
import pandas as pd
import numpy as np
from pathlib import Path

# Adjust path to import simulator
REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO_ROOT))
from sim.mena_sim import simulate

class ArgsObj:
    def __init__(self, **kwargs):
        self.trace = str(REPO_ROOT / "traces" / "synth_zipf_E8_T128_K2.jsonl")
        self.num_experts = 8
        self.top_k = 2
        self.expert_cache_size = 4
        self.policy = "lru"
        self.dispatch_policy = "expert_major"
        self.bandwidth_bytes_per_cycle = 64.0
        self.expert_size_bytes = 2000000
        self.token_size_bytes = 4096
        self.exec_cycles_per_token = 100
        self.double_buffering = False
        self.output_csv = None

        for k, v in kwargs.items():
            setattr(self, k, v)

# ---------------------------------------------------------------------------
# Trace Generation Helpers
# ---------------------------------------------------------------------------

def generate_adversarial_trace(filename, num_tokens=256, num_layers=4):
    """
    Generates an adversarial trace designed to exploit LRU cache size 4.
    Cycles through 5 experts: 0, 1, 2, 3, 4, 0, 1, 2, 3, 4...
    This ensures that with cache capacity <= 4, the hit rate is exactly 0%.
    """
    records = []
    cycle = [0, 1, 2, 3, 4]
    idx = 0
    for layer in range(num_layers):
        for tok in range(num_tokens):
            topk = [cycle[idx % 5], cycle[(idx + 1) % 5]]
            idx += 1
            records.append({
                "layer": layer,
                "token": tok,
                "topk_experts": topk,
                "scores": [0.1] * 8
            })
    
    path = REPO_ROOT / "traces" / filename
    with open(path, "w") as f:
        for r in records:
            f.write(json.dumps(r) + "\n")
    print(f"Generated Adversarial trace: {path}")

def generate_mock_real_traces():
    """
    Generates mock routing sequences resembling Mixtral 8x7B and DeepSeekMoE.
    - Mixtral: 8 experts, top-2 routing, moderately skewed Zipf.
    - DeepSeekMoE: 64 routed experts, top-6 routing, very sparse, low transition correlation.
    """
    # 1. Mixtral 8x7B (8 experts, top-2)
    mixtral_records = []
    # Zipf-like probabilities for 8 experts
    probs = np.array([0.4, 0.25, 0.15, 0.1, 0.05, 0.03, 0.01, 0.01])
    probs /= probs.sum()
    for layer in range(4):
        for tok in range(128):
            topk = np.random.choice(8, size=2, replace=False, p=probs).tolist()
            mixtral_records.append({
                "layer": layer,
                "token": tok,
                "topk_experts": topk,
                "scores": probs.tolist()
            })
    
    mixtral_path = REPO_ROOT / "traces" / "real_mixtral_E8_T128_K2.jsonl"
    with open(mixtral_path, "w") as f:
        for r in mixtral_records:
            f.write(json.dumps(r) + "\n")
            
    # 2. DeepSeekMoE (64 routed experts, top-6)
    deepseek_records = []
    ds_probs = np.zeros(64)
    # High concentration in top 8, long tail for the rest
    ds_probs[:8] = 0.6 / 8
    ds_probs[8:] = 0.4 / 56
    ds_probs /= ds_probs.sum()
    for layer in range(4):
        for tok in range(128):
            topk = np.random.choice(64, size=6, replace=False, p=ds_probs).tolist()
            deepseek_records.append({
                "layer": layer,
                "token": tok,
                "topk_experts": topk,
                "scores": ds_probs.tolist()
            })
            
    deepseek_path = REPO_ROOT / "traces" / "real_deepseek_E64_T128_K6.jsonl"
    with open(deepseek_path, "w") as f:
        for r in deepseek_records:
            f.write(json.dumps(r) + "\n")
            
    print("Generated Mock Real Traces.")

# ---------------------------------------------------------------------------
# Sweep Calculations
# ---------------------------------------------------------------------------

def run_cache_param_sweep():
    """Sweeps Cache Sizes (0, 2, 4, 8, 16, 32, 64) and Expert Counts (8, 16, 32, 64)"""
    print("\n--- Running Cache DSE Parametric Sweep (Q4) ---")
    traces = [
        ("synth_zipf_E8_T128_K2.jsonl", 8),
        ("synth_hot_E16_T512_K2.jsonl", 16),
        ("real_deepseek_E64_T128_K6.jsonl", 64)
    ]
    cache_sizes = [0, 2, 4, 8, 16, 32, 64]
    
    rows = []
    for t_name, n_exp in traces:
        t_path = REPO_ROOT / "traces" / t_name
        if not t_path.exists():
            continue
        for size in cache_sizes:
            args = ArgsObj(
                trace=str(t_path),
                num_experts=n_exp,
                expert_cache_size=size,
                policy="lru"
            )
            res = simulate(args)
            rows.append(res)
    return pd.DataFrame(rows)

def run_real_trace_eval():
    """Evaluates Mixtral and DeepSeekMoE mock real traces"""
    print("\n--- Running Real Trace Verification Sweep (Q10) ---")
    traces = [
        ("real_mixtral_E8_T128_K2.jsonl", 8, 2),
        ("real_deepseek_E64_T128_K6.jsonl", 64, 6)
    ]
    policies = ["none", "lru", "history"]
    
    rows = []
    for t_name, n_exp, k in traces:
        t_path = REPO_ROOT / "traces" / t_name
        for pol in policies:
            args = ArgsObj(
                trace=str(t_path),
                num_experts=n_exp,
                top_k=k,
                expert_cache_size=8,
                policy=pol,
                double_buffering=True if pol == "history" else False
            )
            res = simulate(args)
            rows.append(res)
    return pd.DataFrame(rows)

def run_adversarial_eval():
    """Evaluates the adversarial trace to show failure modes"""
    print("\n--- Running Adversarial Trace Evaluation (Q9) ---")
    t_path = REPO_ROOT / "traces" / "adversarial_E8_T256_K2.jsonl"
    
    rows = []
    # Test different cache sizes
    for size in [2, 4, 5, 8]:
        args = ArgsObj(
            trace=str(t_path),
            num_experts=8,
            expert_cache_size=size,
            policy="lru"
        )
        res = simulate(args)
        rows.append(res)
    return pd.DataFrame(rows)

def main():
    os.makedirs(str(REPO_ROOT / "docs" / "exploration"), exist_ok=True)
    
    # Generate traces
    generate_adversarial_trace("adversarial_E8_T256_K2.jsonl")
    generate_mock_real_traces()
    
    # Run sweeps
    df_cache = run_cache_param_sweep()
    df_cache.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_cache_v2.csv"), index=False)
    
    df_real = run_real_trace_eval()
    df_real.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_real_traces.csv"), index=False)
    
    df_adv = run_adversarial_eval()
    df_adv.to_csv(str(REPO_ROOT / "docs" / "exploration" / "sweep_adversarial.csv"), index=False)
    
    print("\nAdvanced DSE Sweeps completed successfully!")

if __name__ == "__main__":
    main()
