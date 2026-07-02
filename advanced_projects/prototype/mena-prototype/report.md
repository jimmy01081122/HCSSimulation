# MENA Prototype Initial Investigation Report

This report presents the environment state, trace analysis, simulation statistics, RTL design validation status, and future system integration paths for the **MoE Expert-transfer and NPU Assist Processor (MENA)** prototype.

---

## 1. Environment Check

The workspace environment was verified using `/home/a/prototype/mena-prototype/scripts/check_env.sh`:

*   **Operating System**: Ubuntu 22.04.5 LTS (Jammy Jellyfish) on WSL2
*   **Python Version**: 3.10.12
*   **pip Version**: 22.0.2
*   **Compiler Versions**: gcc 11.4.0, g++ 11.4.0, Make 4.3
*   **Icarus Verilog**: Version 11.0 (Stable) - Installed & functional
*   **Host GPU**: NVIDIA GeForce RTX 3050 (6144 MiB total VRAM)
*   **PyTorch Environment**: CPU-only (`2.12.0+cpu`)
*   **Installed Python Libraries**:
    *   `numpy` (2.2.6)
    *   `pandas` (2.3.3)
    *   `matplotlib` (3.10.9)
    *   `torch` (2.12.0+cpu)

### Tools Status & Environment Integration

> [!NOTE]
> `verilator` and `cmake` are fully integrated and executed via the pre-built `mccpu-dev:latest` Docker container wrapper (`./scripts/run_verilator_docker.sh`). No local host package installation is required.

*   **verilator**: **PASSED** (Executed inside Docker container, Verilator version 5.020)
*   **cmake**: **PASSED** (Executed inside Docker container, CMake version 3.28.3)

---

## 2. Generated Trace Analysis

Four synthetic routing traces and one PyTorch toy router trace were generated. Let's look at their structural differences:

| Trace File Name | Experts (E) | Tokens (T) | Top-K | Max Load | Min Load | Imbalance Ratio | Top Expert | Normalized Entropy |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **synth_uniform_E8_T128_K2** | 8 | 128 | 2 | 146 | 106 | 1.1406 | 3 | 0.9974 |
| **synth_zipf_E8_T128_K2** | 8 | 128 | 2 | 489 | 4 | 3.8203 | 0 | 0.6353 |
| **synth_hot_E16_T512_K2** | 16 | 512 | 2 | 2048 | 0 | 8.0000 | 0 | 0.2500 |
| **synth_layer_shift_E16_T512_K2** | 16 | 512 | 2 | 972 | 1 | 3.7969 | 3 | 0.6734 |
| **toy_moe_E8_T128_K2** | 8 | 128 | 2 | 152 | 95 | 1.1875 | 4 | 0.9956 |

### Observations:
*   **Uniform Trace & Toy MoE Trace** both exhibit extremely high entropy (>0.99) and very low load imbalance (~1.1), meaning that expert routing choices are distributed evenly. This represents typical pre-training MoE routers or well-regularized loads.
*   **Zipf Trace** introduces moderate skew, favoring lower-indexed experts. This simulates realistic LLM MoE architectures where some experts represent general syntactic tasks (e.g. punctuation, common verbs) and are heavily loaded, while others represent niche semantics and are rarely queried.
*   **Hot Expert Trace** isolates access to only 2 experts out of 16. Imbalance ratio rises to the mathematical maximum of 8.0 for K=2.
*   **Layer-Shift Trace** introduces dynamic preference rotations across layers. The imbalance is moderate (3.79), but the active expert working set changes rapidly layer-by-layer.

---

## 3. System Simulation Summary

The event-driven system-level simulation was run across 9 configurations under default parameters:
*   `expert_size_bytes` = 2 MB
*   `bandwidth_bytes_per_cycle` = 64 Bytes/cycle (e.g. 64 GB/s at 1 GHz)
*   `exec_cycles_per_token` = 100 cycles

### Compiled Results Table

| Experiment | Trace File | Experts | Cache Size | Cache Policy | Double Buffering | Hit Rate | Misses | Transfer Cycles | Exec Cycles | Total Cycles | P50 Wait | P95 Wait |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **1. Uniform + Cache None** | uniform_E8 | 8 | 0 | none | False | 0.00% | 32 | 1,000,000.0 | 102,400.0 | 1,102,400.0 | 167,650.0 | 272,500.0 |
| **2. Uniform + LRU (size 4)** | uniform_E8 | 8 | 4 | lru | False | 0.00% | 32 | 1,000,000.0 | 102,400.0 | 1,102,400.0 | 167,650.0 | 272,500.0 |
| **3. Zipf + LRU (size 4)** | zipf_E8 | 8 | 4 | lru | False | 0.00% | 31 | 968,750.0 | 102,400.0 | 1,071,150.0 | 74,600.0 | 175,192.5 |
| **4. Zipf + History (size 4)** | zipf_E8 | 8 | 4 | history | False | 67.74% | 10 | 312,500.0 | 102,400.0 | 414,900.0 | 31,250.0 | 113,850.0 |
| **5. Hot + LRU (size 4)** | hot_E16 | 16 | 4 | lru | False | 75.00% | 2 | 62,500.0 | 409,600.0 | 472,100.0 | 41,225.0 | 113,700.0 |
| **6. Hot + History (size 4)** | hot_E16 | 16 | 4 | history | False | 75.00% | 2 | 62,500.0 | 409,600.0 | 472,100.0 | 41,225.0 | 113,700.0 |
| **7. Zipf + LRU + DB** | zipf_E8 | 8 | 4 | lru | True | 0.00% | 31 | 968,750.0 | 102,400.0 | 969,350.0 | 62,500.0 | 151,562.5 |
| **8. Zipf + History + DB** | zipf_E8 | 8 | 4 | history | True | 67.74% | 10 | 312,500.0 | 102,400.0 | **338,500.0** | 31,250.0 | **93,750.0** |
| **9. Hot + History + DB** | hot_E16 | 16 | 4 | history | True | 75.00% | 2 | 62,500.0 | 409,600.0 | **440,850.0** | 41,225.0 | **82,450.0** |

> [!TIP]
> Under Zipf routing with the History Prefetcher, enabling **Double Buffering (DB)** reduced total execution cycles from **414,900.0** to **338,500.0** (a further **18.4%** latency reduction) by successfully overlapping weight transfers and execution in the background!
> Under Hot expert routing, DB reduced total execution cycles from **472,100.0** to **440,850.0** (a **6.6%** latency reduction).

### Key Simulator Takeaways:
1.  **LRU Caching Bottleneck**: Under uniform and standard Zipf layouts, sequential expert processing causes complete cache thrashing (0% hit rate) because the working set of active experts per layer is larger than the cache size (4).
2.  **Prefetching Power**: The history-based prefetching policy successfully registers sequential transitions and fetches expert weights into the background cache. This hides a substantial portion of the weight-transfer latency.
3.  **Double Buffering Overlap**: Enabling double buffering (DB) allows the transfer of the next expert's weights to run in parallel with the current expert's execution. This hides a significant portion of the transfer overhead, cutting latency on Zipf workloads by 18.4%.
4.  **Active Set Fit**: In models with highly localized expert hotness (e.g. Hot E16), the working set fits completely in SRAM, resulting in a high hit rate (75.0% - bounded only by initial compulsory misses) regardless of the replacement policy. Double Buffering still improves overall execution time by overlapping compulsory misses with NPU compute.

---

## 4. RTL Compilation & Verification Status

*   **Compiler/Linter Used**: Icarus Verilog v11.0 (Stable) & Verilator v5.020 (Docker)
*   **RTL Syntax Status**: **PASSED** (all SystemVerilog files compile successfully, zero syntax errors).
*   **Verilator Testbench Executions**: **PASSED** (Ran via wrapper script `./scripts/run_verilator_docker.sh`)

### Simulation Validation Logs:
1.  **Top-K Selector (`topk_sim`)**:
    *   *Test count*: 1009 test cases executed.
    *   *Results*: **SUCCESS** (All cases matched Python reference models, covering random uniform distributions, identical score corner cases, adjacent indices, and boundary bounds).
2.  **Token Dispatcher (`dispatch_sim`)**:
    *   *Test count*: 5 custom batch scenarios executed.
    *   *Results*: **SUCCESS** (All mapping outputs and queue expert-counts matched reference golden outputs, including hot expert skew and single expert routing).

### RTL Design Details:
*   [topk_selector.sv](file:///home/a/prototype/mena-prototype/rtl/topk_selector.sv): Combines a nested maximum search to return the top-2 scoring experts and their values in a single combinational pass.
*   [token_dispatcher.sv](file:///home/a/prototype/mena-prototype/rtl/token_dispatcher.sv): Employs a finite state machine to scan memory in expert-major order (releasing tokens for expert 0, then expert 1, etc.) in a predictable, clock-aligned fashion.
*   [expert_queue.sv](file:///home/a/prototype/mena-prototype/rtl/expert_queue.sv): Implements a standard FIFO register queue for token routing slots.
*   [mena_top_stub.sv](file:///home/a/prototype/mena-prototype/rtl/mena_top_stub.sv): Integrates the combinational selector directly with the dispatcher write registers to showcase a zero-overhead hardware pipeline.

---

## 5. Next-Step Recommendations

To advance this prototype toward physical FPGA implementation and microarchitectural modeling, we recommend the following 5 tasks:

1.  **Implement Double Buffering in Simulator**: Design and model dual-bank SRAM memory interfaces inside the Python simulator to analyze overlap benefits of memory weight transfers and computation.
2.  **Synthesize RTL on Xilinx Vivado**: Import RTL files into Xilinx Vivado targeting PYNQ-Z2 to verify frequency targets (100MHz) and obtain actual LUT/FF/DSP resource utilisation.
3.  **AXI-lite and AXI-stream Overlay Implementation**: Build the block design in Vivado, wrapping the SystemVerilog design with the AXI IP blocks, and export the `.bit` bitstream.
4.  **Integrate gem5 MMIO and PIO Device**: Create the simple PIO device registration in the gem5 simulator using the register map defined in the demo plan.
5.  **Benchmarking and Real-world Routing Sweeps**: Replay PyTorch router routing traces on the simulator to optimize history window prediction lengths.
