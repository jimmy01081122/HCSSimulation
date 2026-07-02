#!/usr/bin/env python3
import unittest
import sys
import os

# Adjust path to import from sim
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from sim.expert_cache import ExpertCache

class TestExpertCache(unittest.TestCase):
    
    def test_simple_lru_correctness(self):
        # Cache size = 3, Policy = LRU
        cache = ExpertCache(cache_size=3, policy='lru', num_experts=8)
        
        # Access 0, 1, 2 (all should miss)
        h, ev, pr, state = cache.request(0)
        self.assertFalse(h)
        self.assertIsNone(ev)
        self.assertEqual(set(state), {0})
        
        h, ev, pr, state = cache.request(1)
        self.assertFalse(h)
        self.assertIsNone(ev)
        self.assertEqual(set(state), {0, 1})
        
        h, ev, pr, state = cache.request(2)
        self.assertFalse(h)
        self.assertIsNone(ev)
        self.assertEqual(set(state), {0, 1, 2})
        
        # Hit on 1, updates recency
        h, ev, pr, state = cache.request(1)
        self.assertTrue(h)
        self.assertIsNone(ev)
        
        # Access 3: Cache is full, should evict LRU.
        # Recency order: 0 (oldest), 2, 1 (newest). So evicts 0.
        h, ev, pr, state = cache.request(3)
        self.assertFalse(h)
        self.assertEqual(ev, 0)
        self.assertEqual(set(state), {1, 2, 3})
        
    def test_lfu_correctness(self):
        # Cache size = 3, Policy = LFU
        cache = ExpertCache(cache_size=3, policy='lfu', num_experts=8)
        
        # Build frequency:
        # Request 0 twice, 1 three times, 2 once
        cache.request(0)
        cache.request(0)
        cache.request(1)
        cache.request(1)
        cache.request(1)
        cache.request(2)
        
        # Currently, cache has {0, 1, 2}. Frequencies: 0: 2, 1: 3, 2: 1.
        # Access 3: Cache is full, should evict LFU which is 2 (frequency 1).
        h, ev, pr, state = cache.request(3)
        self.assertFalse(h)
        self.assertEqual(ev, 2)
        self.assertEqual(set(state), {0, 1, 3})
        
    def test_repeated_hot_expert(self):
        cache = ExpertCache(cache_size=2, policy='lru', num_experts=8)
        
        # Repeatedly request 0 and 1
        for _ in range(10):
            h, ev, pr, state = cache.request(0)
            # After first request, it should be a hit
            if _ > 0:
                self.assertTrue(h)
            h, ev, pr, state = cache.request(1)
            if _ > 0:
                self.assertTrue(h)
                
        self.assertEqual(set(cache.cache), {0, 1})
        
    def test_phase_shift_expert_access_pattern(self):
        # Phase 1: hot set is {0, 1, 2}
        # Phase 2: hot set is {3, 4, 5}
        cache = ExpertCache(cache_size=3, policy='lru', num_experts=8)
        
        # Fill cache with Phase 1
        for x in [0, 1, 2, 0, 1, 2]:
            cache.request(x)
        self.assertEqual(set(cache.cache), {0, 1, 2})
        
        # Phase shift to Phase 2
        # First request to 3 evicts oldest (0)
        h, ev, pr, state = cache.request(3)
        self.assertEqual(ev, 0)
        
        # Request to 4 evicts 1
        h, ev, pr, state = cache.request(4)
        self.assertEqual(ev, 1)
        
        # Request to 5 evicts 2
        h, ev, pr, state = cache.request(5)
        self.assertEqual(ev, 2)
        
        self.assertEqual(set(cache.cache), {3, 4, 5})
        
    def test_cache_size_zero(self):
        cache = ExpertCache(cache_size=0, policy='lru', num_experts=8)
        for x in range(5):
            h, ev, pr, state = cache.request(x)
            self.assertFalse(h)
            self.assertIsNone(ev)
            self.assertEqual(state, [])
            
    def test_cache_size_ge_num_experts(self):
        num_experts = 8
        cache = ExpertCache(cache_size=10, policy='lru', num_experts=num_experts)
        
        # Request all experts
        for x in range(num_experts):
            h, ev, pr, state = cache.request(x)
            self.assertFalse(h) # compulsory miss
            self.assertIsNone(ev)
            
        self.assertEqual(len(cache.cache), num_experts)
        
        # All subsequent requests should hit
        for x in range(num_experts):
            h, ev, pr, state = cache.request(x)
            self.assertTrue(h)
            self.assertIsNone(ev)

if __name__ == '__main__':
    unittest.main()
