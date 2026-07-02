#include <iostream>
#include <memory>
#include "Vtopk_selector.h"
#include "verilated.h"
#include "topk_vectors.h"

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto top = std::make_unique<Vtopk_selector>();

    std::cout << "Starting Verilator testbench for topk_selector..." << std::endl;
    std::cout << "Running " << NUM_TOPK_TEST_CASES << " test cases." << std::endl;

    int errors = 0;

    for (int tc = 0; tc < NUM_TOPK_TEST_CASES; ++tc) {
        const auto& test_case = TOPK_TEST_CASES[tc];

        // Pack 8 scores of 16-bit into the 128-bit scores_in signal.
        // Verilator represents wide signals (>64 bits) as arrays of 32-bit words.
        // scores_in is 128 bits, which fits in 4 words (4 * 32 = 128).
        top->scores_in[0] = (static_cast<uint32_t>(test_case.scores[1]) << 16) | test_case.scores[0];
        top->scores_in[1] = (static_cast<uint32_t>(test_case.scores[3]) << 16) | test_case.scores[2];
        top->scores_in[2] = (static_cast<uint32_t>(test_case.scores[5]) << 16) | test_case.scores[4];
        top->scores_in[3] = (static_cast<uint32_t>(test_case.scores[7]) << 16) | test_case.scores[6];

        // Evaluate model
        top->eval();

        // Unpack outputs
        // topk_idx is 6 bits (2 * 3-bit idx): topk_idx[2:0] is idx0, topk_idx[5:3] is idx1
        uint32_t got_idx0 = top->topk_idx & 0x7;
        uint32_t got_idx1 = (top->topk_idx >> 3) & 0x7;

        // topk_score is 32 bits (2 * 16-bit score): topk_score[15:0] is score0, topk_score[31:16] is score1
        uint32_t got_score0 = top->topk_score & 0xFFFF;
        uint32_t got_score1 = (top->topk_score >> 16) & 0xFFFF;

        // Compare with golden model
        bool match = (got_idx0 == test_case.gold_idx[0]) &&
                     (got_idx1 == test_case.gold_idx[1]) &&
                     (got_score0 == test_case.gold_score[0]) &&
                     (got_score1 == test_case.gold_score[1]);

        if (!match) {
            errors++;
            if (errors <= 10) { // Limit error printout
                std::cerr << "Mismatch in test case " << tc << ":" << std::endl;
                std::cerr << "  Scores: ";
                for (int i = 0; i < 8; ++i) std::cerr << test_case.scores[i] << " ";
                std::cerr << std::endl;
                std::cerr << "  Expected: idx=[" << (int)test_case.gold_idx[0] << ", " << (int)test_case.gold_idx[1] 
                          << "], score=[" << test_case.gold_score[0] << ", " << test_case.gold_score[1] << "]" << std::endl;
                std::cerr << "  Got:      idx=[" << got_idx0 << ", " << got_idx1 
                          << "], score=[" << got_score0 << ", " << got_score1 << "]" << std::endl;
            }
        }
    }

    if (errors == 0) {
        std::cout << "SUCCESS: All " << NUM_TOPK_TEST_CASES << " topk_selector test cases passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "FAILURE: topk_selector test failed with " << errors << " errors." << std::endl;
        return -1;
    }
}
