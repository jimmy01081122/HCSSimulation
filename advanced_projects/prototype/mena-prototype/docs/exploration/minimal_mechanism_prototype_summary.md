# MAEP Minimal Mechanism Prototype Summary
## Sprint v5.3 Prototype Findings & Synthesis

This document compiles the hardware prototyping results from the 6 minimal mechanism sprints (MP1-MP6).

---

## Answers to Prototyping Questions

### 1. Sparse Markov-1 是否仍可行？ (Is Sparse Markov-1 still feasible?)
*   **Yes**. Top-2 sparse table storage reduces register overhead by **87.5%** (down to 2 KB) for 128 experts, while introducing only **2.8%** prediction accuracy loss and maintaining $F_{\text{max}} = 310$ MHz.

### 2. Set-associative tag 是否能取代 CAM？ (Can set-associative tags replace CAM?)
*   **Yes**. A 2-way set-associative BRAM tag array increases $F_{\text{max}}$ from 128 MHz to 263 MHz (+105% timing speedup) for a 64-entry cache, with only **1.8%** cache hit rate loss compared to fully associative CAM.

### 3. Command ring buffer 是否能解決 MMIO overhead？ (Can command ring buffer solve MMIO overhead?)
*   **Yes**. A polling-driven command ring buffer reduces host sync cycles from 1,200 to **85 cycles** (a **92.9% reduction**), preventing accelerator idle states during decode loops.

### 4. WABC arbitration 是否值得做更深？ (Is WABC arbitration worth exploring further?)
*   **Yes**. A QoS Threshold arbitration policy achieves **90.8%** double-buffering speedup while avoiding the activation queue starvation risk associated with static Weight-Priority arbitration.

### 5. Quantization 是否削弱 memory bottleneck 結論？ (Does quantization weaken the memory bottleneck conclusion?)
*   **No, under DDR5**. Even with INT4 precision, the decode loop remains memory-bound ($\beta = 3.36$). However, under HBM + INT4 precision, the system shifts to a compute-bound state ($\beta = 0.84$), making compute scaling critical.

### 6. Prefill 與 decode 是否需要分開建模？ (Do prefill and decode phases need separate modeling?)
*   **Yes**. Prefill is compute-bound ($\beta = 0.25$) with high entropy (2.55) and requires no prefetching. Decode is memory-bound ($\beta = 3.36$) with lower entropy (1.83), where prefetching is highly effective.

### 7. 哪些 v5.2 結論仍成立？ (What v5.2 conclusions hold?)
*   **Memory Bandwidth Dominance**: Remains the primary decode bottleneck.
*   **CAM Caching Limits**: Caches larger than 16 entries cannot use fully associative CAM due to timing paths.

### 8. 哪些 v5.2 結論被削弱？ (What v5.2 conclusions are weakened?)
*   **Weight-Priority DRAM Arbitration (WPDA)**: Reclassified due to critical starvation risks on token activation queues under long sequences.

### 9. 下一輪最值得做的最小實體 FPGA demo 是什麼？ (What minimum physical FPGA demo should be built next?)
*   A **2-Way Set-Associative BRAM Cache Tag controller** integrated with a **Top-2 Sparse Markov-1 Prefetcher** and a **QoS Threshold memory bus arbiter**.
