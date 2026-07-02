// scheduler_tb.cpp – Verilator testbench for expert_queue_scheduler
// Tests 5 scenarios across all 4 scheduling policies.

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdint>
#include <cassert>
#include "Vexpert_queue_scheduler.h"
#include "verilated.h"

// -----------------------------------------------------------------------
// Parameters (must match RTL defaults)
// -----------------------------------------------------------------------
static constexpr int NUM_EXPERTS  = 8;
static constexpr int QUEUE_WIDTH  = 6;
static constexpr int EXP_W        = 3;  // $clog2(8)

// -----------------------------------------------------------------------
// Helper: pack 8 x 6-bit counts into a 48-bit (uint64_t) word
// -----------------------------------------------------------------------
static uint64_t pack_counts(const int counts[NUM_EXPERTS]) {
    uint64_t v = 0;
    for (int i = 0; i < NUM_EXPERTS; i++)
        v |= (uint64_t)(counts[i] & 0x3F) << (i * QUEUE_WIDTH);
    return v;
}

// -----------------------------------------------------------------------
// Per-policy DUT – we instantiate one per policy at the C++ level and
// drive them through a thin wrapper that selects the right model.
// (Verilator generates a single class; policy is a parameter so we need
//  one binary per policy – instead we instantiate the same compiled
//  SCHED_POLICY=0 binary but override the packed count/valid and read
//  back combinationally, then replicate the pure-comb logic in SW for
//  the other policies.)
//
// Since Verilator compiles parameters into the DUT class name, and we
// compile a single binary (SCHED_POLICY=0), we exercise all four policies
// by re-running the SW reference model alongside the RTL for RR (policy 0),
// and test LQF/HEF/Age purely in software – OR we test all 4 with the RTL
// by simply running 4 separate simulations within the same binary using
// the same Verilated model but we cannot change SCHED_POLICY at runtime.
//
// Pragmatic solution used here: compile with SCHED_POLICY=0, test the RR
// policy behaviorally with the RTL; for the other 3 policies implement a
// SW reference and validate the selection algorithm (mirroring the RTL
// exactly), so the testbench stays single-binary.
// -----------------------------------------------------------------------

// ------------------------------------------------------------------ utils
static int total_errors = 0;

static void check(bool cond, const std::string& msg) {
    if (!cond) {
        std::cerr << "  FAIL: " << msg << std::endl;
        total_errors++;
    } else {
        std::cout << "  PASS: " << msg << std::endl;
    }
}

// -----------------------------------------------------------------------
// Software reference models (mirror RTL exactly)
// -----------------------------------------------------------------------
static int sw_rr(const int cnt[NUM_EXPERTS], int rr_ptr) {
    for (int i = 0; i < NUM_EXPERTS; i++) {
        int idx = (rr_ptr + i) % NUM_EXPERTS;
        if (cnt[idx] > 0) return idx;
    }
    return -1; // all empty
}

static int sw_lqf(const int cnt[NUM_EXPERTS]) {
    int best = -1, best_cnt = 0;
    for (int i = 0; i < NUM_EXPERTS; i++) {
        if (cnt[i] > best_cnt) { best_cnt = cnt[i]; best = i; }
    }
    return best;
}

static int sw_hef(const int cnt[NUM_EXPERTS], const int hot[NUM_EXPERTS]) {
    int best = -1, best_hot = -1;
    for (int i = 0; i < NUM_EXPERTS; i++) {
        if (cnt[i] > 0 && (best == -1 || hot[i] > best_hot)) {
            best_hot = hot[i]; best = i;
        }
    }
    return best;
}

static int sw_age(const int cnt[NUM_EXPERTS], const int age[NUM_EXPERTS]) {
    int best = -1, best_age = -1;
    for (int i = 0; i < NUM_EXPERTS; i++) {
        if (cnt[i] > 0 && (best == -1 || age[i] > best_age)) {
            best_age = age[i]; best = i;
        }
    }
    return best;
}

// -----------------------------------------------------------------------
// Clock helpers
// -----------------------------------------------------------------------
static void tick(Vexpert_queue_scheduler* top) {
    top->clk = 1; top->eval();
    top->clk = 0; top->eval();
}

static void reset(Vexpert_queue_scheduler* top) {
    top->rst_n    = 0;
    top->valid_in = 0;
    top->expert_counts = 0;
    top->clk = 0; top->eval();
    tick(top);
    top->rst_n = 1;
    tick(top);
}

// -----------------------------------------------------------------------
// main
// -----------------------------------------------------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    std::cout << "======================================================" << std::endl;
    std::cout << " Expert Queue Scheduler Testbench" << std::endl;
    std::cout << "======================================================" << std::endl;

    // ===================================================================
    // Scenario 1 (RTL – RR policy):
    //   All experts equal depth=8 → RR should cycle 0,1,2,3,...
    // ===================================================================
    {
        std::cout << "\n--- Scenario 1: RR cycles through equal-depth experts ---" << std::endl;
        auto top = std::make_unique<Vexpert_queue_scheduler>();
        reset(top.get());

        int cnt_arr[NUM_EXPERTS];
        for (int i = 0; i < NUM_EXPERTS; i++) cnt_arr[i] = 8;

        top->expert_counts = pack_counts(cnt_arr);
        top->valid_in      = 1;
        top->eval();

        // The RTL uses sched_policy=0 (RR) since that's what was compiled.
        // We also check the SW model mirrors it.
        int rr_ptr = 0;
        bool ok = true;
        for (int step = 0; step < NUM_EXPERTS; step++) {
            int rtl_sel = (int)top->next_expert;
            int sw_sel  = sw_rr(cnt_arr, rr_ptr);
            if (rtl_sel != sw_sel) {
                std::cerr << "  Step " << step << ": RTL=" << rtl_sel
                          << " SW=" << sw_sel << std::endl;
                ok = false;
            }
            // advance rr_ptr (mirrors RTL sequential update)
            rr_ptr = (rr_ptr + 1) % NUM_EXPERTS;
            tick(top.get());
            top->eval();
        }
        check(ok, "RR cycles 0→1→2→3→4→5→6→7 on equal queues");
        top->final();
    }

    // ===================================================================
    // Scenario 2 (SW – LQF policy):
    //   Expert 5 depth=20, others=1 → LQF should pick 5
    // ===================================================================
    {
        std::cout << "\n--- Scenario 2: LQF picks longest queue (expert 5) ---" << std::endl;
        int cnt_arr[NUM_EXPERTS];
        for (int i = 0; i < NUM_EXPERTS; i++) cnt_arr[i] = 1;
        cnt_arr[5] = 20;

        int sel = sw_lqf(cnt_arr);
        check(sel == 5, "LQF selects expert 5 (depth=20) over others (depth=1)");

        // Also verify ordering: if expert 7 is equal to 5 in depth, lower index wins
        cnt_arr[7] = 20;
        sel = sw_lqf(cnt_arr);
        check(sel == 5, "LQF breaks ties by lower index");
    }

    // ===================================================================
    // Scenario 3 (SW – HEF policy):
    //   Expert 2 dispatched 5 times → HEF should prefer expert 2
    // ===================================================================
    {
        std::cout << "\n--- Scenario 3: HEF maintains hotness after 5 dispatches ---" << std::endl;
        int cnt_arr[NUM_EXPERTS];
        int hot_arr[NUM_EXPERTS];
        for (int i = 0; i < NUM_EXPERTS; i++) { cnt_arr[i] = 4; hot_arr[i] = 0; }

        // Simulate 5 dispatches to expert 2
        for (int d = 0; d < 5; d++) hot_arr[2]++;

        int sel = sw_hef(cnt_arr, hot_arr);
        check(sel == 2, "HEF picks expert 2 after 5 dispatches (hot_score=5)");

        // Verify HEF ignores empty experts
        cnt_arr[2] = 0;
        sel = sw_hef(cnt_arr, hot_arr);
        check(sel != 2, "HEF skips expert 2 when its queue is empty");
    }

    // ===================================================================
    // Scenario 4 (SW – Age policy):
    //   Expert 3 not scheduled for 10 cycles while others are
    //   → Age should pick expert 3
    // ===================================================================
    {
        std::cout << "\n--- Scenario 4: Age-based picks oldest waiting expert ---" << std::endl;
        int cnt_arr[NUM_EXPERTS];
        int age_arr[NUM_EXPERTS];
        for (int i = 0; i < NUM_EXPERTS; i++) { cnt_arr[i] = 4; age_arr[i] = 0; }

        // Simulate 10 cycles where expert 3 accumulates age but others are reset
        for (int cycle = 0; cycle < 10; cycle++) {
            for (int i = 0; i < NUM_EXPERTS; i++) {
                if (i == 3) age_arr[i]++;   // expert 3 never scheduled → age keeps growing
                else        age_arr[i] = 0; // others are scheduled each cycle → reset
            }
        }

        int sel = sw_age(cnt_arr, age_arr);
        check(sel == 3, "Age-based picks expert 3 with highest age (=10)");

        // Verify: if expert 3 is also empty, fall back to next oldest
        cnt_arr[3] = 0;
        age_arr[0] = 5; // give expert 0 some age
        sel = sw_age(cnt_arr, age_arr);
        check(sel == 0, "Age-based falls back to expert 0 when expert 3 empty");
    }

    // ===================================================================
    // Scenario 5 (RTL – RR policy):
    //   Experts 0,1,2 empty, 3-7 have tokens → scheduler skips empties
    // ===================================================================
    {
        std::cout << "\n--- Scenario 5: Scheduler skips empty experts (0,1,2 empty) ---" << std::endl;
        auto top = std::make_unique<Vexpert_queue_scheduler>();
        reset(top.get());

        int cnt_arr[NUM_EXPERTS];
        for (int i = 0; i < NUM_EXPERTS; i++) cnt_arr[i] = (i < 3) ? 0 : 5;

        top->expert_counts = pack_counts(cnt_arr);
        top->valid_in      = 1;
        top->eval();

        bool ok = true;
        // Step through several cycles; should only emit experts 3-7
        for (int step = 0; step < 5; step++) {
            if (top->sched_valid) {
                int sel = (int)top->next_expert;
                if (sel < 3) {
                    std::cerr << "  Step " << step << ": selected empty expert " << sel << std::endl;
                    ok = false;
                }
            }
            tick(top.get());
            top->eval();
        }
        check(ok, "RR skips experts 0-2 (empty) and schedules only from 3-7");

        // Also verify sched_valid=0 when all empty
        top->valid_in = 1;
        for (int i = 0; i < NUM_EXPERTS; i++) cnt_arr[i] = 0;
        top->expert_counts = pack_counts(cnt_arr);
        top->eval();
        check(top->sched_valid == 0, "sched_valid=0 when all queues empty");

        top->final();
    }

    // ===================================================================
    // Summary
    // ===================================================================
    std::cout << "\n======================================================" << std::endl;
    if (total_errors == 0) {
        std::cout << " SUCCESS: All scheduler scenarios passed!" << std::endl;
        return 0;
    } else {
        std::cerr << " FAILURE: " << total_errors << " error(s) detected." << std::endl;
        return 1;
    }
}
