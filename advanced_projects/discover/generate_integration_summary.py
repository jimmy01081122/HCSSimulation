import json
import yaml

def generate_summary():
    # Read expert cache results
    with open("/home/a/discover/expert_cache/expert_cache_results_qwen.json", "r") as f:
        cache_data = json.load(f)
        
    # Read joint scheduler results
    with open("/home/a/discover/joint_scheduler_qwen/joint_scheduler_results.json", "r") as f:
        sched_data = json.load(f)
        
    summary = {
        "routing_locality": {
            "gini_coefficient": 0.0692,
            "median_reuse_distance_tokens": 3.0,
            "consecutive_token_overlap_percent": 16.42,
            "hot_expert_ratio_80_20": 0.7667
        },
        "expert_cache_simulation": {
            "no_cache": {
                "hit_rate": cache_data["no_cache"]["hit_rate"],
                "traffic_mb": cache_data["no_cache"]["traffic_mb"],
                "exposed_stall_ms": cache_data["no_cache"]["exposed_stall_ms"],
                "speedup": cache_data["no_cache"]["speedup_vs_nocache"]
            },
            "lru": {
                "hit_rate": cache_data["lru"]["hit_rate"],
                "traffic_mb": cache_data["lru"]["traffic_mb"],
                "exposed_stall_ms": cache_data["lru"]["exposed_stall_ms"],
                "speedup": cache_data["lru"]["speedup_vs_nocache"]
            },
            "static_hot_expert": {
                "hit_rate": cache_data["static_hot_expert"]["hit_rate"],
                "traffic_mb": cache_data["static_hot_expert"]["traffic_mb"],
                "exposed_stall_ms": cache_data["static_hot_expert"]["exposed_stall_ms"],
                "speedup": cache_data["static_hot_expert"]["speedup_vs_nocache"]
            },
            "oracle_prefetch": {
                "hit_rate": cache_data["oracle_prefetch"]["hit_rate"],
                "traffic_mb": cache_data["oracle_prefetch"]["traffic_mb"],
                "exposed_stall_ms": cache_data["oracle_prefetch"]["exposed_stall_ms"],
                "speedup": cache_data["oracle_prefetch"]["speedup_vs_nocache"]
            },
            "layer_temporal_affinity_cache": {
                "hit_rate": cache_data["layer_temporal_affinity_cache"]["hit_rate"],
                "traffic_mb": cache_data["layer_temporal_affinity_cache"]["traffic_mb"],
                "exposed_stall_ms": cache_data["layer_temporal_affinity_cache"]["exposed_stall_ms"],
                "speedup": cache_data["layer_temporal_affinity_cache"]["speedup_vs_nocache"]
            }
        },
        "kv_cache_phi2_calibration": {
            "full_kv": {
                "peak_memory_mb": 255.0,
                "latency_ms": 250508.0,
                "speedup": 1.00,
                "risk": "Medium"
            },
            "paged_kv": {
                "peak_memory_mb": 255.0,
                "latency_ms": 250848.0,
                "speedup": 1.00,
                "risk": "Medium"
            },
            "sliding_window": {
                "peak_memory_mb": 255.0,
                "latency_ms": 65328.0,
                "speedup": 3.83,
                "risk": "High"
            },
            "uniform_int4_kv": {
                "peak_memory_mb": 200.0,
                "latency_ms": 50208.0,
                "speedup": 4.99,
                "risk": "Medium"
            },
            "hybrid_recency_expert_policy": {
                "peak_memory_mb": 255.0,
                "latency_ms": 124288.0,
                "speedup": 2.02,
                "risk": "Low"
            }
        },
        "joint_scheduler_ablation": {
            "expert_first": {
                "latency_ms": sched_data["expert_first"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["expert_first"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["expert_first"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["expert_first"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["expert_first"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["expert_first"]["deadline_miss_ratio"],
                "speedup": sched_data["expert_first"]["speedup_vs_expert_first"]
            },
            "kv_first": {
                "latency_ms": sched_data["kv_first"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["kv_first"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["kv_first"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["kv_first"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["kv_first"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["kv_first"]["deadline_miss_ratio"],
                "speedup": sched_data["kv_first"]["speedup_vs_expert_first"]
            },
            "round_robin": {
                "latency_ms": sched_data["round_robin"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["round_robin"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["round_robin"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["round_robin"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["round_robin"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["round_robin"]["deadline_miss_ratio"],
                "speedup": sched_data["round_robin"]["speedup_vs_expert_first"]
            },
            "bandwidth_partition_static": {
                "latency_ms": sched_data["bandwidth_partition_static"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["bandwidth_partition_static"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["bandwidth_partition_static"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["bandwidth_partition_static"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["bandwidth_partition_static"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["bandwidth_partition_static"]["deadline_miss_ratio"],
                "speedup": sched_data["bandwidth_partition_static"]["speedup_vs_expert_first"]
            },
            "criticality_only": {
                "latency_ms": sched_data["criticality_only"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["criticality_only"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["criticality_only"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["criticality_only"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["criticality_only"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["criticality_only"]["deadline_miss_ratio"],
                "speedup": sched_data["criticality_only"]["speedup_vs_expert_first"]
            },
            "confidence_only": {
                "latency_ms": sched_data["confidence_only"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["confidence_only"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["confidence_only"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["confidence_only"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["confidence_only"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["confidence_only"]["deadline_miss_ratio"],
                "speedup": sched_data["confidence_only"]["speedup_vs_expert_first"]
            },
            "deadline_only": {
                "latency_ms": sched_data["deadline_only"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["deadline_only"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["deadline_only"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["deadline_only"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["deadline_only"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["deadline_only"]["deadline_miss_ratio"],
                "speedup": sched_data["deadline_only"]["speedup_vs_expert_first"]
            },
            "no_prefetch_drop": {
                "latency_ms": sched_data["no_prefetch_drop"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["no_prefetch_drop"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["no_prefetch_drop"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["no_prefetch_drop"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["no_prefetch_drop"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["no_prefetch_drop"]["deadline_miss_ratio"],
                "speedup": sched_data["no_prefetch_drop"]["speedup_vs_expert_first"]
            },
            "no_overlap": {
                "latency_ms": sched_data["no_overlap"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["no_overlap"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["no_overlap"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["no_overlap"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["no_overlap"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["no_overlap"]["deadline_miss_ratio"],
                "speedup": sched_data["no_overlap"]["speedup_vs_expert_first"]
            },
            "full_proposed": {
                "latency_ms": sched_data["full_proposed"]["total_latency_ms"],
                "expert_stall_cycles": sched_data["full_proposed"]["expert_stall_cycles"],
                "kv_stall_cycles": sched_data["full_proposed"]["kv_stall_cycles"],
                "bandwidth_utilization": sched_data["full_proposed"]["bandwidth_utilization"],
                "offered_load_ratio": sched_data["full_proposed"]["offered_load_ratio"],
                "deadline_miss_ratio": sched_data["full_proposed"]["deadline_miss_ratio"],
                "speedup": sched_data["full_proposed"]["speedup_vs_expert_first"]
            }
        },
        "rtl_python_verification_alignment": {
            "total_requests": 11734,
            "total_expert_requests": 4232,
            "total_kv_requests": 1285,
            "deadline_miss_ratio_rtl": "5.31%",
            "python_vs_rtl_mismatch_cycles": 26159,
            "max_cycle_error": 0,
            "expert_demand": {
                "served_hits": 957,
                "served_loads": 3274,
                "misses": 3275,
                "deadline_misses": 1122
            },
            "expert_prefetch": {
                "dropped_due_to_congestion_confidence": 2764,
                "queued": 2911,
                "served": 21
            },
            "kv_demand": {
                "served_hits": 1280,
                "served_loads": 5,
                "misses": 5,
                "deadline_misses": 0
            },
            "synthesis": {
                "logic_cells": 68600,
                "flip_flops": 3405,
                "memory_bits": 0
            }
        }
    }
    
    with open("/home/a/discover/thesis_integration_summary.yaml", "w") as f:
        yaml.dump(summary, f, default_flow_style=False, sort_keys=False)
    print("Structured yaml summary saved successfully.")

if __name__ == "__main__":
    generate_summary()
