#include <iostream>
#include <memory>
#include <vector>
#include "Vtoken_dispatcher.h"
#include "verilated.h"
#include "dispatch_vectors.h"

// Helper function to toggle clock
void tick(Vtoken_dispatcher* top) {
    top->clk = 1;
    top->eval();
    top->clk = 0;
    top->eval();
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto top = std::make_unique<Vtoken_dispatcher>();

    std::cout << "Starting Verilator testbench for token_dispatcher..." << std::endl;
    std::cout << "Running " << NUM_DISPATCH_TEST_CASES << " test cases." << std::endl;

    int total_errors = 0;

    for (int tc = 0; tc < NUM_DISPATCH_TEST_CASES; ++tc) {
        const auto& test_case = DISPATCH_TEST_CASES[tc];
        std::cout << "Test Case " << tc << ": " << test_case.name << "..." << std::endl;

        // 1. Reset
        top->rst_n = 0;
        top->clk = 0;
        top->wr_en = 0;
        top->start_dispatch = 0;
        top->eval();
        
        tick(top.get());
        top->rst_n = 1;
        tick(top.get());

        // 2. Load tokens
        for (int t = 0; t < 32; ++t) {
            top->wr_en = 1;
            top->wr_token_id = t;
            // Pack top-k experts (top_k=2, idx_width=3).
            // wr_expert_ids[2:0] is expert0, wr_expert_ids[5:3] is expert1.
            top->wr_expert_ids = (test_case.token_experts[t][1] << 3) | test_case.token_experts[t][0];
            tick(top.get());
        }
        top->wr_en = 0;
        tick(top.get());

        // 3. Verify counts
        // expert_count is a 48-bit signal (8 experts * 6-bit count)
        // Verilator represents it as a 64-bit integer (vluint64_t or unsigned long long)
        uint64_t got_counts_packed = top->expert_count;
        bool count_ok = true;
        for (int e = 0; e < 8; ++e) {
            unsigned char got_count = (got_counts_packed >> (e * 6)) & 0x3F;
            if (got_count != test_case.gold_counts[e]) {
                std::cerr << "  Count mismatch for expert " << e << ": expected " << (int)test_case.gold_counts[e] 
                          << ", got " << (int)got_count << std::endl;
                count_ok = false;
                total_errors++;
            }
        }
        if (count_ok) {
            std::cout << "  Expert counts match golden values." << std::endl;
        }

        // 4. Start dispatch
        top->start_dispatch = 1;
        tick(top.get());
        top->start_dispatch = 0;

        // 5. Monitor dispatch output
        std::vector<DispatchStep> got_steps;
        int timeout_cycles = 1000;
        int cycles = 0;
        bool done = false;

        while (cycles < timeout_cycles) {
            if (top->dispatch_valid) {
                DispatchStep step;
                step.expert_id = top->dispatch_expert_id;
                step.token_id = top->dispatch_token_id;
                got_steps.push_back(step);
            }
            if (top->dispatch_done) {
                done = true;
                break;
            }
            tick(top.get());
            cycles++;
        }

        if (!done) {
            std::cerr << "  ERROR: Timeout waiting for dispatch_done." << std::endl;
            total_errors++;
            continue;
        }

        // 6. Compare dispatch sequence
        bool seq_ok = true;
        if (got_steps.size() != static_cast<size_t>(test_case.num_dispatches)) {
            std::cerr << "  Sequence size mismatch: expected " << test_case.num_dispatches 
                      << " dispatches, got " << got_steps.size() << std::endl;
            seq_ok = false;
            total_errors++;
        } else {
            for (size_t s = 0; s < got_steps.size(); ++s) {
                if (got_steps[s].expert_id != test_case.gold_steps[s].expert_id ||
                    got_steps[s].token_id != test_case.gold_steps[s].token_id) {
                    std::cerr << "  Step " << s << " mismatch: expected expert=" << (int)test_case.gold_steps[s].expert_id
                              << ", token=" << (int)test_case.gold_steps[s].token_id
                              << " | got expert=" << (int)got_steps[s].expert_id
                              << ", token=" << (int)got_steps[s].token_id << std::endl;
                    seq_ok = false;
                    total_errors++;
                }
            }
        }

        if (seq_ok && count_ok) {
            std::cout << "  SUCCESS: Test Case " << tc << " passed!" << std::endl;
        } else {
            std::cerr << "  FAILURE: Test Case " << tc << " failed." << std::endl;
        }
    }

    if (total_errors == 0) {
        std::cout << "SUCCESS: All token_dispatcher test cases passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "FAILURE: token_dispatcher test failed with " << total_errors << " total errors." << std::endl;
        return -1;
    }
}
