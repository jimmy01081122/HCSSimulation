#include <iostream>
#include <memory>
#include <cassert>
#include "Vexpert_cache_ctrl.h"
#include "verilated.h"

// Helper function to tick clock with input propagation
void tick(Vexpert_cache_ctrl* top) {
    top->eval(); // Propagate input changes
    top->clk = 1;
    top->eval(); // Trigger clock rising edge
    top->clk = 0;
    top->eval(); // Re-evaluate combinational paths
}

// Reset helper
void reset_top(Vexpert_cache_ctrl* top) {
    top->rst_n = 0;
    top->lookup_valid = 0;
    top->lookup_expert = 0;
    top->prefetch_valid = 0;
    top->prefetch_expert = 0;
    top->mem_req_ready = 0;
    top->mem_fill_valid = 0;
    top->mem_fill_expert = 0;
    top->eval();
    tick(top);
    top->rst_n = 1;
    tick(top);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto top = std::make_unique<Vexpert_cache_ctrl>();

    std::cout << "======================================================" << std::endl;
    std::cout << " Expert Weight Cache Controller Testbench (Verilator)" << std::endl;
    std::cout << "======================================================" << std::endl;

    // ----------------------------------------------------
    // Scenario 1: Cold misses & demand load requests
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 1: Cold Misses & Allocation ---" << std::endl;
    reset_top(top.get());

    // Request expert 3 (Cold miss)
    top->lookup_valid = 1;
    top->lookup_expert = 3;
    top->eval();
    
    assert(top->lookup_hit == 0 && "Cold lookup of expert 3 should miss!");
    assert(top->lookup_ready == 1 && "Should be ready in READY state");
    
    tick(top.get()); // ST_READY -> ST_MISS_REQ
    top->lookup_valid = 0;
    top->eval();
    
    assert(top->mem_req_valid == 1 && "DRAM request should be valid on miss!");
    assert(top->mem_req_expert == 3 && "DRAM request should ask for expert 3");
    assert(top->mem_req_is_pf == 0 && "Demand load should not be marked as prefetch");
    std::cout << "  PASS: Cold miss on expert 3 correctly generated DRAM demand load" << std::endl;

    // DRAM accepts request
    top->mem_req_ready = 1;
    tick(top.get()); // ST_MISS_REQ -> ST_MISS_WAIT
    top->mem_req_ready = 0;

    // DRAM completes load after 3 cycles
    tick(top.get());
    tick(top.get());
    
    top->mem_fill_valid = 1;
    top->mem_fill_expert = 3;
    tick(top.get()); // ST_MISS_WAIT -> ST_READY
    top->mem_fill_valid = 0;
    top->eval();

    assert(top->lookup_ready == 1 && "Should return to READY state");

    // Lookup expert 3 again (should HIT!)
    top->lookup_valid = 1;
    top->lookup_expert = 3;
    top->eval();
    assert(top->lookup_hit == 1 && "Second lookup of expert 3 should hit!");
    tick(top.get());
    top->lookup_valid = 0;
    std::cout << "  PASS: Second lookup hit verified" << std::endl;

    // ----------------------------------------------------
    // Scenario 2: Sequential allocation to fill cache
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 2: Cache Filling ---" << std::endl;
    // Cache has expert 3 in it. Let's load 0, 1, 2.
    int fill_experts[3] = {0, 1, 2};
    for (int idx = 0; idx < 3; idx++) {
        int exp = fill_experts[idx];
        top->lookup_valid = 1;
        top->lookup_expert = exp;
        tick(top.get()); // ST_READY -> ST_MISS_REQ
        top->lookup_valid = 0;
        
        top->mem_req_ready = 1;
        tick(top.get()); // ST_MISS_REQ -> ST_MISS_WAIT
        top->mem_req_ready = 0;
        
        top->mem_fill_valid = 1;
        top->mem_fill_expert = exp;
        tick(top.get()); // ST_MISS_WAIT -> ST_READY
        top->mem_fill_valid = 0;
    }

    // Now lookups to 0, 1, 2, 3 should all HIT!
    for (int exp : {3, 0, 1, 2}) {
        top->lookup_valid = 1;
        top->lookup_expert = exp;
        top->eval();
        assert(top->lookup_hit == 1 && "Cache should be fully populated and hit on all");
        tick(top.get());
        top->lookup_valid = 0;
    }
    std::cout << "  PASS: Cache successfully filled and hit rate verified" << std::endl;

    // ----------------------------------------------------
    // Scenario 3: LRU Eviction Handling
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 3: LRU Eviction ---" << std::endl;
    // Cache has [3, 0, 1, 2] in it.
    // Let's touch 3, 0, and 1 to make them recently used:
    for (int exp : {3, 0, 1}) {
        top->lookup_valid = 1;
        top->lookup_expert = exp;
        tick(top.get());
        top->lookup_valid = 0;
    }
    // Now the age ordering should be:
    // 1 (MRU, age 0), 0 (age 1), 3 (age 2), 2 (LRU, age 3)
    // If we request expert 7, it should evict expert 2.
    top->lookup_valid = 1;
    top->lookup_expert = 7;
    tick(top.get()); // ST_READY -> ST_MISS_REQ
    top->lookup_valid = 0;

    top->mem_req_ready = 1;
    tick(top.get()); // ST_MISS_REQ -> ST_MISS_WAIT
    top->mem_req_ready = 0;

    top->mem_fill_valid = 1;
    top->mem_fill_expert = 7;
    tick(top.get()); // ST_MISS_WAIT -> ST_READY
    top->mem_fill_valid = 0;

    // Expert 7 should now HIT!
    top->lookup_valid = 1;
    top->lookup_expert = 7;
    top->eval();
    assert(top->lookup_hit == 1 && "Expert 7 should be resident");
    tick(top.get());
    top->lookup_valid = 0;

    // Expert 2 (which was evicted) should now MISS!
    top->lookup_valid = 1;
    top->lookup_expert = 2;
    top->eval();
    assert(top->lookup_hit == 0 && "Expert 2 should have been evicted!");
    top->lookup_valid = 0; // Clear valid!
    std::cout << "  PASS: Eviction target correct (evicted oldest expert 2)" << std::endl;

    // ----------------------------------------------------
    // Scenario 4: Prefetch Request
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 4: Prefetch Trigger ---" << std::endl;
    // Prefetch expert 5
    top->prefetch_valid = 1;
    top->prefetch_expert = 5;
    tick(top.get()); // ST_READY -> ST_PF_REQ
    top->prefetch_valid = 0;

    assert(top->mem_req_valid == 1 && "DRAM prefetch request expected!");
    assert(top->mem_req_expert == 5 && "Asking for prefetch target 5");
    assert(top->mem_req_is_pf == 1 && "mem_req_is_pf must be set to 1");
    std::cout << "  PASS: Prefetch miss correctly triggered DRAM prefetch request" << std::endl;

    top->mem_req_ready = 1;
    tick(top.get()); // ST_PF_REQ -> ST_PF_WAIT
    top->mem_req_ready = 0;

    top->mem_fill_valid = 1;
    top->mem_fill_expert = 5;
    tick(top.get()); // ST_PF_WAIT -> ST_READY
    top->mem_fill_valid = 0;

    // Expert 5 should now HIT!
    top->lookup_valid = 1;
    top->lookup_expert = 5;
    top->eval();
    assert(top->lookup_hit == 1 && "Prefetched expert 5 should now HIT!");
    tick(top.get());
    top->lookup_valid = 0;
    std::cout << "  PASS: Prefetched expert hit verified" << std::endl;

    // ----------------------------------------------------
    // Scenario 5: Reset Mid-Operation
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 5: Reset Mid-Operation ---" << std::endl;
    top->lookup_valid = 1;
    top->lookup_expert = 6;
    tick(top.get()); // ST_READY -> ST_MISS_REQ
    top->lookup_valid = 0;

    top->rst_n = 0;
    tick(top.get());
    assert(top->lookup_ready == 1 && "Should return to ST_READY immediately on reset");
    std::cout << "  PASS: State machine correctly reset mid-operation" << std::endl;

    std::cout << "\n======================================================" << std::endl;
    std::cout << " SUCCESS: All expert_cache_ctrl scenarios passed!" << std::endl;
    std::cout << "======================================================" << std::endl;

    return 0;
}
