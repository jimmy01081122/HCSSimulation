#include <iostream>
#include <memory>
#include <cassert>
#include "Vmena_top.h"
#include "verilated.h"

// Helper function to tick clock with input propagation
void tick(Vmena_top* top) {
    top->eval(); // Propagate input changes
    top->clk = 1;
    top->eval(); // Trigger clock rising edge
    top->clk = 0;
    top->eval(); // Re-evaluate combinational paths
}

// Reset helper
void reset_top(Vmena_top* top) {
    top->rst_n = 0;
    top->reg_write_valid = 0;
    top->reg_addr = 0;
    top->reg_write_data = 0;
    top->mem_req_ready = 0;
    top->mem_fill_valid = 0;
    top->mem_fill_expert = 0;
    top->token_wr_en = 0;
    top->token_wr_id = 0;
    top->token_wr_experts = 0;
    top->eval();
    tick(top);
    top->rst_n = 1;
    tick(top);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto top = std::make_unique<Vmena_top>();

    std::cout << "======================================================" << std::endl;
    std::cout << " MENA Top-Level Subsystem Integration Testbench" << std::endl;
    std::cout << "======================================================" << std::endl;

    reset_top(top.get());

    // 1. Load tokens into the dispatcher
    // We load 4 tokens:
    // Token 0: experts [0, 1]
    // Token 1: experts [1, 2]
    // Token 2: experts [0, 2]
    // Token 3: experts [1, 3]
    std::cout << "Loading token queues..." << std::endl;
    int token_experts[4][2] = {
        {0, 1},
        {1, 2},
        {0, 2},
        {1, 3}
    };

    for (int t = 0; t < 4; ++t) {
        top->token_wr_en = 1;
        top->token_wr_id = t;
        // Pack top-k experts (top_k=2, idx_width=3)
        top->token_wr_experts = (token_experts[t][1] << 3) | token_experts[t][0];
        tick(top.get());
    }
    top->token_wr_en = 0;
    tick(top.get());

    // 2. Start accelerator via MMIO write (CTRL = 1)
    std::cout << "Writing CTRL = 1 to start accelerator..." << std::endl;
    top->reg_write_valid = 1;
    top->reg_addr = 0x00;
    top->reg_write_data = 1; // START bit
    tick(top.get());
    top->reg_write_valid = 0;
    tick(top.get());

    // Verify STATUS is BUSY (bit 1 of STATUS = 1)
    top->reg_addr = 0x04;
    top->eval();
    std::cout << "STATUS Register: 0x" << std::hex << top->reg_read_data << std::dec << std::endl;
    assert((top->reg_read_data & 0x02) && "STATUS should read BUSY");

    // 3. Process lookups and handle DRAM transfers
    // We run simulation loop
    int cycles = 0;
    int timeout = 500;
    bool done = false;

    std::cout << "Running execution loop..." << std::endl;
    while (cycles < timeout) {
        // If there's a memory request, assert ready and complete it in next cycle
        if (top->mem_req_valid) {
            top->mem_req_ready = 1;
            int req_exp = top->mem_req_expert;
            tick(top.get());
            top->mem_req_ready = 0;

            // Mock 2-cycle DRAM delay
            tick(top.get());
            tick(top.get());

            // DRAM fills data
            top->mem_fill_valid = 1;
            top->mem_fill_expert = req_exp;
            tick(top.get());
            top->mem_fill_valid = 0;
        } else {
            tick(top.get());
        }

        // Read STATUS register
        top->reg_addr = 0x04;
        top->eval();
        if (top->reg_read_data & 0x01) {
            // DONE bit set
            done = true;
            break;
        }

        cycles++;
    }

    assert(done && "Simulation timeout before done!");
    std::cout << "\nSimulation finished successfully!" << std::endl;

    // 4. Print performance counter registers
    top->reg_addr = 0x08; top->eval(); uint32_t hits = top->reg_read_data;
    top->reg_addr = 0x0C; top->eval(); uint32_t misses = top->reg_read_data;
    top->reg_addr = 0x10; top->eval(); uint32_t dma_cycles = top->reg_read_data;
    top->reg_addr = 0x14; top->eval(); uint32_t total_cycles = top->reg_read_data;

    std::cout << "\n==========================================" << std::endl;
    std::cout << " Performance Counters Output:" << std::endl;
    std::cout << "   CACHE_HIT   : " << hits << " hits" << std::endl;
    std::cout << "   CACHE_MISS  : " << misses << " misses" << std::endl;
    std::cout << "   DMA_CYCLES  : " << dma_cycles << " cycles" << std::endl;
    std::cout << "   TOTAL_CYCLES: " << total_cycles << " cycles" << std::endl;
    std::cout << "==========================================" << std::endl;

    std::cout << "\nSUCCESS: mena_top integration verified!" << std::endl;
    return 0;
}
