# test_simulator.py
# Unit tests for the discrete-event MoE inference simulator.

import sys
import os
import unittest

# Ensure src directory is in Python path
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../src'))

from workload_gen import Request, WorkloadGenerator
from hardware_model import SystemConfig, PCIeBus
from cache import GPUExpertCache
from scheduler import MoEScheduler
from decision_engine import DecisionEngine
from simulator import HMCSimulator, Event

class TestMoESimulator(unittest.TestCase):
    def setUp(self):
        # Default config for testing
        self.config = SystemConfig(
            num_experts=8,
            expert_size_gb=1.0,
            pcie_bandwidth_gbs=16.0,
            gpu_cache_capacity=2,
            cpu_expert_latency_ms=4.0,
            gpu_expert_latency_ms=0.2,
            gpu_dense_latency_ms=0.5,
            cpu_orch_latency_ms=0.1,
            cpu_transfer_latency_ms=0.05,
            pcie_dma_channels=1,
            cpu_cores=2
        )

    def test_state_transitions_single_request(self):
        """Test correct state transitions for a single request in an empty cache."""
        req = Request(
            request_id=0,
            arrival_time=0.0,
            total_tokens=1,
            total_layers=1,
            num_experts=8,
            zipf_alpha=1.2
        )
        
        # Override expert demand to a known one (e.g., expert 5)
        req.expert_demands = [[5]]
        
        sim = HMCSimulator(
            gpu_cache_capacity=2,
            pcie_bandwidth_gbs=16.0,
            expert_size_gb=1.0,
            cpu_expert_latency_ms=4.0,
            gpu_expert_latency_ms=0.2,
            gpu_dense_latency_ms=0.5,
            cpu_orch_latency_ms=0.1,
            pcie_dma_channels=1,
            cpu_cores=2,
            scheduling_policy="fcfs",
            offloading_policy="transfer",
            cache_policy="lru",
            sched_epoch=0.0 # Immediate scheduling
        )
        
        results = sim.run([req])
        
        # Assertions
        self.assertEqual(req.status, "FINISHED")
        self.assertEqual(results["total_gpu_hits"], 0)
        self.assertEqual(results["total_gpu_misses"], 1)
        self.assertEqual(results["pcie_transfer_count"], 1)
        # Expected duration: CPU orch (0.1) + GPU dense (0.5) + PCIe wait/transfer (62.5) + GPU expert (0.2) = 63.3
        self.assertAlmostEqual(results["total_sim_time_ms"], 63.3, places=2)

    def test_cache_hit_no_transfer(self):
        """Verify that cache hits do not trigger PCIe transfers."""
        sim = HMCSimulator(
            gpu_cache_capacity=2,
            scheduling_policy="fcfs",
            offloading_policy="transfer",
            cache_policy="lru",
            sched_epoch=0.0
        )
        
        # Manually insert expert 3 into cache
        sim.cache.insert(3, policy="lru", current_time=0.0)
        
        req = Request(0, 0.0, 1, 1, 8)
        req.expert_demands = [[3]]
        
        results = sim.run([req])
        
        self.assertEqual(results["total_gpu_hits"], 1)
        self.assertEqual(results["total_gpu_misses"], 0)
        self.assertEqual(results["pcie_transfer_count"], 0)
        # Expected duration: CPU orch (0.1) + GPU dense (0.5) + GPU expert (0.2) = 0.8
        self.assertAlmostEqual(results["total_sim_time_ms"], 0.8, places=2)

    def test_cpu_offload_no_transfer(self):
        """Verify that CPU offloading does not trigger PCIe transfers."""
        sim = HMCSimulator(
            gpu_cache_capacity=2,
            scheduling_policy="fcfs",
            offloading_policy="offload",
            cache_policy="lru",
            sched_epoch=0.0
        )
        
        req = Request(0, 0.0, 1, 1, 8)
        req.expert_demands = [[3]]
        
        results = sim.run([req])
        
        self.assertEqual(results["total_gpu_hits"], 0)
        self.assertEqual(results["total_gpu_misses"], 1)
        self.assertEqual(results["total_cpu_offloads"], 1)
        self.assertEqual(results["pcie_transfer_count"], 0)
        # Expected duration: CPU orch (0.1) + GPU dense (0.5) + CPU expert (4.0) + CPU transfer back (0.05) = 4.65
        self.assertAlmostEqual(results["total_sim_time_ms"], 4.65, places=2)

    def test_felas_aging(self):
        """Test that F-ELAS starvation prevention triggers properly."""
        # We will manually construct a pending queue and verify F-ELAS prioritizing a starving stream
        scheduler = MoEScheduler(self.config, starvation_threshold=3)
        cache = GPUExpertCache(self.config)
        
        # Insert expert 1 to cache (making it warm)
        cache.insert(1, policy="lru")
        
        # Create requests
        req_warm = Request(0, 0.0, 1, 1, 8)
        req_warm.expert_demands = [[1]] # Needs warm expert 1 (cache hit)
        
        req_cold = Request(1, 0.0, 1, 1, 8)
        req_cold.expert_demands = [[2]] # Needs cold expert 2
        req_cold.waiting_ticks = 3      # Already starving!
        
        # Test normal ELAS first: warm stream should be scheduled first
        elas_order = scheduler.schedule([req_warm, req_cold], cache, policy="elas")
        self.assertEqual(elas_order[0].request_id, req_warm.request_id)
        
        # Test F-ELAS: starving cold stream should be prioritized
        felas_order = scheduler.schedule([req_warm, req_cold], cache, policy="f-elas")
        self.assertEqual(felas_order[0].request_id, req_cold.request_id)

    def test_dcmd_cost_decisions(self):
        """Verify DCMD cost model makes correct offloading decisions based on bus queue delay."""
        # Use a custom config with small expert size to make transfer faster than offload when PCIe is idle
        config = SystemConfig(
            num_experts=8,
            expert_size_gb=0.01,  # 10MB
            pcie_bandwidth_gbs=16.0,
            gpu_cache_capacity=2,
            cpu_expert_latency_ms=4.0,
            gpu_expert_latency_ms=0.2,
            gpu_dense_latency_ms=0.5,
            cpu_orch_latency_ms=0.1,
            cpu_transfer_latency_ms=0.05,
            pcie_dma_channels=1,
            cpu_cores=2
        )
        dec_engine = DecisionEngine(config)
        pcie_bus = PCIeBus(config)
        
        # Case A: Free PCIe Bus -> Should choose TRANSFER
        decision_free = dec_engine.decide(
            expert_id=1,
            num_tokens=1,
            pcie_bus=pcie_bus,
            current_time=0.0,
            cpu_queue_len=0,
            policy="dcmd"
        )
        self.assertEqual(decision_free, "TRANSFER")
        
        # Case B: Heavily congested PCIe Bus -> Should choose OFFLOAD
        # Request multiple dummy transfers to populate queue and cause long pcie wait
        for i in range(2, 22):
            pcie_bus.request_transfer(i, 0.0, lambda: None)
        
        decision_congested = dec_engine.decide(
            expert_id=22,
            num_tokens=1,
            pcie_bus=pcie_bus,
            current_time=0.0,
            cpu_queue_len=0,
            policy="dcmd"
        )
        self.assertEqual(decision_congested, "OFFLOAD")

    def test_counter_consistency(self):
        """Check statistical counters consistency."""
        wg = WorkloadGenerator(
            num_requests=10,
            num_experts=8,
            tokens_range=(5, 15),
            layers=3,
            zipf_alpha=1.1,
            arrival_rate=0.5,
            random_seed=123
        )
        requests = wg.generate()
        
        sim = HMCSimulator(
            gpu_cache_capacity=3,
            scheduling_policy="f-elas",
            offloading_policy="dcmd",
            cache_policy="arc"
        )
        results = sim.run(requests)
        
        total_misses = results["total_gpu_misses"]
        total_hits = results["total_gpu_hits"]
        total_offloads = results["total_cpu_offloads"]
        total_transfers = results["pcie_transfer_count"]
        
        # Check that misses are handled by either offload or transfer
        self.assertEqual(total_misses, total_offloads + total_transfers)
        
        # Check that total tokens processed * total layers equals total hits + total misses
        expected_total_moe = sum(r.total_tokens * r.total_layers for r in requests)
        self.assertEqual(expected_total_moe, total_hits + total_misses)

if __name__ == "__main__":
    unittest.main()
