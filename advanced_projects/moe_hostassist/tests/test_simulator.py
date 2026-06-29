# test_simulator.py
# Unit tests for the MoE-HostAssist SimPy simulator.

import pytest
import sys
import os

# Add simulator folder to python path
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../'))
from simulator.moe_sim import (
    Request, ExpertCacheModel, CachePolicy, EvictionResult,
    HMCSimulator, SchedPolicy, WorkloadGenerator, RequestDispatcher
)

def test_cache_model_lru():
    # Cache capacity of 2, 4 total experts
    cache = ExpertCacheModel(capacity=2, n_experts=4, policy=CachePolicy.LRU)
    
    # Access 0: Miss, cache state {0}
    res, evicted = cache.access(0, timestamp=0.0)
    assert res == EvictionResult.MISS
    assert evicted is None
    
    # Access 1: Miss, cache state {0, 1}
    res, evicted = cache.access(1, timestamp=1.0)
    assert res == EvictionResult.MISS
    assert evicted is None
    
    # Access 0: Hit, cache state {1, 0} (0 is now most recent)
    res, evicted = cache.access(0, timestamp=2.0)
    assert res == EvictionResult.HIT
    assert evicted is None
    
    # Access 2: Miss, cache state {0, 2}, evicted should be 1 (least recent)
    res, evicted = cache.access(2, timestamp=3.0)
    assert res == EvictionResult.MISS
    assert evicted == 1

def test_cache_model_lfu():
    cache = ExpertCacheModel(capacity=2, n_experts=4, policy=CachePolicy.LFU)
    
    # Access 0, 0, 1
    cache.access(0, timestamp=0.0)
    cache.access(0, timestamp=1.0)
    cache.access(1, timestamp=2.0)
    
    # Access 2: Miss, cache has {0, 1}. Frequencies: 0: 2, 1: 1.
    # Evicted should be 1 (lower frequency)
    res, evicted = cache.access(2, timestamp=3.0)
    assert res == EvictionResult.MISS
    assert evicted == 1

def test_cache_model_prediction_guided():
    cache = ExpertCacheModel(capacity=2, n_experts=4, policy=CachePolicy.PREDICTION_GUIDED)
    # Train predictor with sequence 0->1->2->0->1->2
    for exp in [0, 1, 2, 0, 1, 2]:
        cache.access(exp, timestamp=0.0)
        
    assert cache.predictor.predict_next(top_n=1) == [0] # after 2, predicts 0

def test_simulator_basic_run():
    # Submit 3 requests
    requests = WorkloadGenerator.generate_requests(
        num_requests=3,
        n_experts=16,
        topk=2,
        arrival_pattern="concurrent",
        arrival_rate=5.0,
        zipf_alpha=1.2
    )
    
    sim = HMCSimulator(
        gpu_cache_capacity=4,
        n_experts=16,
        pcie_bandwidth_gbs=16.0,
        cpu_cores=4,
        scheduling_policy=SchedPolicy.FIFO,
        cache_policy=CachePolicy.LRU
    )
    
    # Dispatch requests via RequestDispatcher
    dispatcher = RequestDispatcher(sim.env, sim, requests)
        
    # Run the SimPy environment
    sim.env.run(until=100000.0)
    
    assert len(sim.completed_requests) == 3
    for r in sim.completed_requests:
        assert r.finish_time > r.arrival_time
        assert r.ttft > 0.0
        assert r.tpot >= 0.0
        # Check breakdown totals
        total_breakdown = sum(r.breakdown.values())
        assert total_breakdown > 0.0
