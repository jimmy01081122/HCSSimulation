# MAEP Mechanism Reality Check Summary
## Sprint v5.2 Reality Check Findings & Synthesis

This document summarizes the findings of the 7 reality check sprints (RC1-RC7), evaluating the timing, area, and systems-level feasibility of MoE optimization mechanisms.

---

## Answers to Reality Check Questions

### 1. 哪些 v5.1 結論在 RTL/FPGA reality check 後仍成立？ (What v5.1 conclusions hold?)
*   **Memory Bandwidth Bottleneck**: Confirmed that weight transfer dominates decode phases ($\beta > 3.0$ under DDR5).
*   **Cache × Prefetch Interaction**: Confirmed that pairing prefetching with a cache recovers hit rates, although tag designs must avoid CAM critical paths.

### 2. 哪些 v5.1 結論被削弱？ (What v5.1 conclusions are weakened?)
*   **Fully Associative CAM Cache**: Weakened due to routing congestion and critical path delays ($F_{\text{max}}$ drops by 57% when scaling CAM entries from 16 to 128). Set-associative tag arrays using BRAM must be used.

### 3. 哪些 v5.1 結論需要重新表述？ (What v5.1 conclusions need reformulation?)
*   **Compute Acceleration Value**: Instead of "compute is universally unimportant", it is reformulated to: "Under current parameters (DDR5, single-token decode), compute acceleration provided negligible benefit due to memory bounds. Compute scaling becomes critical under HBM + quantized INT4 precision."

### 4. 哪些 mechanism 的硬體成本被低估？ (Which mechanisms had underestimated hardware costs?)
*   **Markov-1 Predictor Table**: Dense transition storage for 128 experts requires 16.3 KB SRAM registers. A sparse (Top-2) table reduces storage by 87.5% with minor accuracy loss.

### 5. 哪些 mechanism 的系統成本被低估？ (Which mechanisms had underestimated systems costs?)
*   **Host-Accelerator MMIO Handshake**: Synchronization overhead accounts for **85.7%** of execution cycles during single-token decode. Command queues are required to batch decode requests.

### 6. WABC 是否仍是重要現象？ (Is WABC still an important phenomenon?)
*   **Yes**. Bus arbitration sweeps verify that activation memory reads/writes reduce effective weight bandwidth by **18% to 35%**, degrading double-buffering speedups.

### 7. Markov-1 是否仍是合理 predictor？ (Is Markov-1 still a reasonable predictor?)
*   **Yes**. Storing transition tables yields high prefetch accuracy on skewed traces, but sparse tables are required to fit SRAM constraints.

### 8. Tag CAM 是否可行？ (Is Tag CAM feasible?)
*   **Only for small caches ($\le 16$ entries)**. Caches exceeding 32 entries require set-associative Tag arrays implemented in BRAM.

### 9. Runtime synchronization 是否會改變結論？ (Does host sync latency alter conclusions?)
*   **Yes**. Host MMIO write/polling delays (1,200 cycles) dominate single-token decode (200 cycles).

### 10. 下一輪應該實作哪些最小 RTL prototype？ (What minimum RTL modules should be built?)
*   **Sparse Markov-1 Predictor Table** with a Top-2 transition selector.
*   **2-Way Set-Associative Cache Tag controller** utilizing single-port BRAM.
*   **Command Ring Buffer Queue** to batch host register writes.
