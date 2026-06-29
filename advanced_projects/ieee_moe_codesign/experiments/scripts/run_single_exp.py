# run_single_exp.py
# Runs a single parameter sweep experiment based on a YAML configuration file.
# Outputs CSV results containing latency, hit rate, and utilization metrics.

import os
import sys
import yaml
import argparse
import copy
import csv

# Add src to Python path
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../simulator/src'))

from simulator import HMCSimulator
from workload_gen import WorkloadGenerator

def main():
    parser = argparse.ArgumentParser(description="Run single MoE sweep experiment")
    parser.add_argument('--config', type=str, required=True, help="Path to experiment YAML configuration")
    args = parser.parse_args()
    
    if not os.path.exists(args.config):
        print(f"Error: Configuration file {args.config} not found.")
        sys.exit(1)
        
    with open(args.config, 'r') as f:
        config_data = yaml.safe_load(f)
        
    exp_name = config_data.get("experiment_name", "sweep")
    base_config = config_data.get("base_config", {})
    sweep_var = config_data.get("sweep_variable")
    sweep_vals = config_data.get("sweep_values", [])
    strategies = config_data.get("strategies", [])
    output_file = config_data.get("output_file", "results.csv")
    
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    
    print(f"Running sweep: {exp_name} (sweeping {sweep_var} over {sweep_vals})")
    
    results = []
    
    for val in sweep_vals:
        # 1. Update config parameters
        current_config = copy.deepcopy(base_config)
        current_config[sweep_var] = val
        
        # 2. Resolve or generate workload
        # For zipf_alpha, we load the pre-generated trace file to ensure reproducibility
        if sweep_var == "zipf_alpha":
            num_exp = current_config.get("num_experts", 16)
            trace_path = f"traces/workload_E{num_exp}_alpha{val:.1f}.csv"
            # If 1.0, float formatting could make it alpha1.0.csv
            requests = WorkloadGenerator.load_trace_file(trace_path)
        # For concurrency, we generate the workload dynamically based on the request count
        elif sweep_var == "num_requests":
            wg = WorkloadGenerator(
                num_requests=val,
                num_experts=current_config.get("num_experts", 16),
                tokens_range=current_config.get("tokens_range", (16, 48)),
                layers=current_config.get("layers", 6),
                zipf_alpha=current_config.get("zipf_alpha", 1.2),
                arrival_rate=current_config.get("arrival_rate", 0.15),
                random_seed=current_config.get("random_seed", 42)
            )
            requests = wg.generate()
        else:
            # For non-workload sweeps, load the default trace
            num_exp = current_config.get("num_experts", 16)
            trace_path = f"traces/workload_E{num_exp}_alpha1.2.csv"
            requests = WorkloadGenerator.load_trace_file(trace_path)
            
        for strategy in strategies:
            # Create a deep copy of requests so each strategy runs on identical workloads
            reqs = copy.deepcopy(requests)
            
            # Map configuration dictionary to simulator constructor
            sim = HMCSimulator(
                gpu_cache_capacity=current_config.get("gpu_cache_capacity", 4),
                pcie_bandwidth_gbs=current_config.get("pcie_bandwidth_gbs", 16.0),
                expert_size_gb=current_config.get("expert_size_gb", 1.0),
                cpu_expert_latency_ms=current_config.get("cpu_expert_latency_ms", 4.0),
                gpu_expert_latency_ms=current_config.get("gpu_expert_latency_ms", 0.2),
                gpu_dense_latency_ms=current_config.get("gpu_dense_latency_ms", 0.5),
                cpu_orch_latency_ms=current_config.get("cpu_orch_latency_ms", 0.1),
                cpu_transfer_latency_ms=current_config.get("cpu_transfer_latency_ms", 0.05),
                pcie_dma_channels=current_config.get("pcie_dma_channels", 2),
                cpu_cores=current_config.get("cpu_cores", 8),
                scheduling_policy=strategy.get("sched"),
                offloading_policy=strategy.get("offload"),
                cache_policy=current_config.get("cache_policy", "lru"),
                starvation_threshold=current_config.get("starvation_threshold", 5),
                sched_epoch=current_config.get("sched_epoch", 0.1)
            )
            
            res = sim.run(reqs)
            
            # Record result row
            row = {
                "sweep_value": val,
                "strategy": strategy.get("name"),
                "total_sim_time_ms": res["total_sim_time_ms"],
                "avg_request_latency_ms": res["avg_request_latency_ms"],
                "p50_request_latency_ms": res["p50_request_latency_ms"],
                "p99_request_latency_ms": res["p99_request_latency_ms"],
                "max_request_latency_ms": res["max_request_latency_ms"],
                "gpu_hit_rate": res["gpu_hit_rate"],
                "total_gpu_hits": res["total_gpu_hits"],
                "total_gpu_misses": res["total_gpu_misses"],
                "total_cpu_offloads": res["total_cpu_offloads"],
                "pcie_transfer_count": res["pcie_transfer_count"],
                "gpu_utilization": res["gpu_utilization"],
                "cpu_utilization": res["cpu_utilization"]
            }
            results.append(row)
            
            print(f"Val={val:<4} | Sched={strategy.get('sched'):<6} | Off={strategy.get('offload'):<8} | SimTime={res['total_sim_time_ms']:.1f}ms | HitRate={res['gpu_hit_rate']*100:.1f}%")

    # Save to CSV
    if results:
        fieldnames = results[0].keys()
        with open(output_file, 'w', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(results)
        print(f"Sweep results successfully written to {output_file}\n")

if __name__ == "__main__":
    main()
