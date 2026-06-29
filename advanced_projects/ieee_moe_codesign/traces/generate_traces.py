# generate_traces.py
# Generates synthetic MoE workload traces with varying expert counts and Zipf skews.
# Uses a bounded Zipf distribution to rigorously support any alpha parameter (including alpha <= 1.0).

import os
import random
import csv

def sample_bounded_zipf(alpha, num_elements):
    """Samples from a bounded Zipf distribution over [0, num_elements - 1]."""
    probs = [1.0 / (i ** alpha) for i in range(1, num_elements + 1)]
    sum_probs = sum(probs)
    probs = [p / sum_probs for p in probs]
    
    cdf = []
    current_sum = 0.0
    for p in probs:
        current_sum += p
        cdf.append(current_sum)
        
    u = random.random()
    for idx, val in enumerate(cdf):
        if u < val:
            return idx
    return num_elements - 1

def generate_trace(num_requests, num_experts, tokens_range, layers, zipf_alpha, arrival_rate, seed):
    random.seed(seed)
    
    trace_data = []
    current_arrival_time = 0.0
    
    for req_id in range(num_requests):
        interval = random.expovariate(arrival_rate)
        current_arrival_time += interval
        
        total_tokens = random.randint(tokens_range[0], tokens_range[1])
        
        # Generate expert demands
        for token_id in range(total_tokens):
            for layer_id in range(layers):
                expert_id = sample_bounded_zipf(zipf_alpha, num_experts)
                
                trace_data.append({
                    "request_id": req_id,
                    "token_id": token_id,
                    "layer": layer_id,
                    "expert_id": int(expert_id),
                    "arrival_time_ms": round(current_arrival_time, 3)
                })
                
    return trace_data

def main():
    os.makedirs("traces", exist_ok=True)
    
    # Parameters matching default config
    num_requests = 15
    tokens_range = (16, 48)
    layers = 6
    arrival_rate = 0.15
    seed = 42

    expert_options = [8, 16, 32, 64]
    zipf_alphas = [0.8, 1.0, 1.2, 1.5, 2.0]
    
    # 1. Generate trace sweeps for expert counts (fixed alpha = 1.2)
    for num_exp in expert_options:
        trace = generate_trace(num_requests, num_exp, tokens_range, layers, 1.2, arrival_rate, seed)
        filename = f"traces/workload_E{num_exp}_alpha1.2.csv"
        with open(filename, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=["request_id", "token_id", "layer", "expert_id", "arrival_time_ms"])
            writer.writeheader()
            writer.writerows(trace)
        print(f"Generated trace file: {filename}")

    # 2. Generate trace sweeps for zipf alpha skews (fixed E = 16)
    for alpha in zipf_alphas:
        # Avoid recreating workload_E16_alpha1.2
        if alpha == 1.2:
            continue
        trace = generate_trace(num_requests, 16, tokens_range, layers, alpha, arrival_rate, seed)
        filename = f"traces/workload_E16_alpha{alpha:.1f}.csv"
        with open(filename, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=["request_id", "token_id", "layer", "expert_id", "arrival_time_ms"])
            writer.writeheader()
            writer.writerows(trace)
        print(f"Generated trace file: {filename}")

if __name__ == "__main__":
    main()
