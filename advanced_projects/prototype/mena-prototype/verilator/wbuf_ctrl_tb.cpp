#include <iostream>
#include <memory>
#include <cassert>
#include "Vweight_buffer_ctrl.h"
#include "verilated.h"

// Helper function to tick clock with input propagation
void tick(Vweight_buffer_ctrl* top) {
    top->eval(); // Propagate input changes
    top->clk = 1;
    top->eval(); // Trigger clock rising edge
    top->clk = 0;
    top->eval(); // Re-evaluate combinational paths
}

// Reset helper
void reset_top(Vweight_buffer_ctrl* top) {
    top->rst_n = 0;
    top->dma_start = 0;
    top->shadow_done = 0;
    top->exec_start = 0;
    top->exec_done = 0;
    tick(top);
    top->rst_n = 1;
    tick(top);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto top = std::make_unique<Vweight_buffer_ctrl>();

    std::cout << "======================================================" << std::endl;
    std::cout << " Weight Buffer Controller (Double Buffering) Testbench" << std::endl;
    std::cout << "======================================================" << std::endl;

    // ----------------------------------------------------
    // Scenario 1: First load & initial stall
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 1: First load and stall handling ---" << std::endl;
    reset_top(top.get());

    // NPU wants to start executing, but buffers are empty. Should stall.
    top->exec_start = 1;
    top->eval();
    assert(top->stall == 1 && "NPU should stall when active buffer is empty!");
    std::cout << "  PASS: stall=1 detected when active is empty" << std::endl;
    
    // DMA completes the first load into shadow buffer
    top->exec_start = 0;
    top->shadow_done = 1;
    tick(top.get()); // state_reg becomes ST_SWAP
    top->shadow_done = 0;
    
    // Now state is ST_SWAP, so swap_event should be active
    top->eval();
    assert(top->swap_event == 1 && "swap_event expected on first swap!");
    tick(top.get()); // ST_SWAP -> ST_RUNNING
    
    assert(top->active_valid == 1 && "Active buffer should be valid after swap!");
    assert(top->shadow_valid == 0 && "Shadow buffer should be empty after swap!");
    assert(top->active_sel == 1 && "Buffer selection should have swapped to BufB (1)!");
    std::cout << "  PASS: First swap successful. active_sel=" << (int)top->active_sel 
              << ", active_valid=" << (int)top->active_valid << std::endl;

    // ----------------------------------------------------
    // Scenario 2: Normal pipelined operation
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 2: Normal pipelined operation (Overlap) ---" << std::endl;
    // DMA starts loading next expert (into BufA, dma_sel=0)
    top->dma_start = 1;
    tick(top.get());
    top->dma_start = 0;

    // NPU starts executing BufB (active_sel=1)
    top->exec_start = 1;
    tick(top.get());
    top->exec_start = 0;
    
    assert(top->stall == 0 && "NPU should NOT stall; active buffer is valid");
    std::cout << "  PASS: NPU execution started without stall" << std::endl;

    // DMA finishes loading shadow buffer (BufA)
    top->shadow_done = 1;
    tick(top.get());
    top->shadow_done = 0;
    assert(top->shadow_valid == 1 && "Shadow buffer should be valid");

    // NPU finishes execution
    top->exec_done = 1;
    tick(top.get()); // state_reg becomes ST_SWAP
    top->exec_done = 0;

    // Next cycle (state_reg == ST_SWAP) should trigger swap_event
    top->eval();
    assert(top->swap_event == 1 && "swap_event pulse expected!");
    tick(top.get()); // ST_SWAP -> ST_RUNNING

    assert(top->active_sel == 0 && "Buffer selection should toggle back to BufA (0)!");
    assert(top->active_valid == 1 && "New active buffer should be valid");
    assert(top->shadow_valid == 0 && "New shadow buffer should be empty");
    std::cout << "  PASS: Normal swap completed successfully" << std::endl;

    // ----------------------------------------------------
    // Scenario 3: DMA slower than NPU (NPU stalls)
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 3: DMA slower than NPU (NPU stalls) ---" << std::endl;
    // NPU starts executing BufA
    top->exec_start = 1;
    tick(top.get());
    top->exec_start = 0;

    // NPU completes execution quickly
    top->exec_done = 1;
    tick(top.get());
    top->exec_done = 0;

    // But DMA hasn't finished yet (shadow_valid = 0)
    tick(top.get());
    assert(top->swap_event == 0 && "Should not swap if shadow is not ready!");
    
    // NPU tries to start next expert, but gets stalled
    top->exec_start = 1;
    top->eval();
    assert(top->stall == 1 && "NPU should stall waiting for DMA!");
    std::cout << "  PASS: NPU stalled waiting for slow DMA" << std::endl;

    // DMA finally finishes
    top->exec_start = 0;
    top->shadow_done = 1;
    tick(top.get()); // state_reg becomes ST_SWAP
    top->shadow_done = 0;

    // Transition to SWAP
    top->eval();
    assert(top->swap_event == 1 && "Swap should trigger once DMA completes");
    tick(top.get()); // ST_SWAP -> ST_RUNNING
    assert(top->active_sel == 1 && "active_sel toggled");
    std::cout << "  PASS: Slow DMA completed, swap triggered" << std::endl;

    // ----------------------------------------------------
    // Scenario 4: DMA faster than NPU
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 4: DMA faster than NPU ---" << std::endl;
    // DMA finishes first
    top->shadow_done = 1;
    tick(top.get());
    top->shadow_done = 0;
    assert(top->shadow_valid == 1 && "DMA finished early, shadow_valid=1");

    // NPU finishes execution later
    top->exec_done = 1;
    tick(top.get()); // state_reg becomes ST_SWAP
    top->exec_done = 0;

    // Swap should happen immediately in the next cycle
    top->eval();
    assert(top->swap_event == 1 && "Immediate swap expected");
    tick(top.get()); // ST_SWAP -> ST_RUNNING
    std::cout << "  PASS: Fast DMA handled, immediate swap on NPU completion" << std::endl;

    // ----------------------------------------------------
    // Scenario 5: Reset mid-operation
    // ----------------------------------------------------
    std::cout << "\n--- Scenario 5: Reset mid-operation ---" << std::endl;
    top->shadow_done = 1;
    tick(top.get());
    top->shadow_done = 0;
    
    // Assert reset
    top->rst_n = 0;
    tick(top.get());
    
    assert(top->active_sel == 0 && "Reset should clear active_sel");
    assert(top->active_valid == 0 && "Reset should clear active_valid");
    assert(top->shadow_valid == 0 && "Reset should clear shadow_valid");
    std::cout << "  PASS: Clean reset mid-operation verified" << std::endl;

    std::cout << "\n======================================================" << std::endl;
    std::cout << " SUCCESS: All weight_buffer_ctrl scenarios passed!" << std::endl;
    std::cout << "======================================================" << std::endl;

    return 0;
}
