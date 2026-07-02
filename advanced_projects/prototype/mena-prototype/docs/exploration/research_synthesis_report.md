# MAEP Research Synthesis Report
## Comprehensive MoE Architecture Exploration Synthesis

This document presents the final synthesis of the Mixture-of-Experts Architecture Exploration Platform (MAEP) based on the 16 quantitative design space exploration reports.

---

## 1. Candidate Architecture Status Update

* **Candidate A (Software Runtime)**: **Strongly Rejected**. Software-based execution cannot resolve the off-chip DRAM weight transfer bottleneck and introduces high synchronization overhead (15,000 cycles).
* **Candidate B (MENA Accelerator)**: **Weakly Rejected**. While the Cache and Prefetcher are validated, the custom NPU compute engine and routing assist logic are redundant, adding significant area ($0.85 mm^2$) without cycle latency benefits.
* **Candidate C (Smart DMA Sidecar)**: **Supported**. A highly cost-effective, low-area ($0.15 mm^2$) architecture that achieves a **2.79× speedup** by focusing entirely on the memory transfer bottleneck.
* **Candidate D (Memory-Centric Runtime Processor)**: **Weakly Supported**. The RISC-V control core provides high programmability but introduces a timing closure risk and local memory area overhead ($0.45 mm^2$).
* **Candidate E (Hybrid Sidecar)**: **Weakly Supported**. Achieves the best overall latency speedup (**3.80×**) but is complex to verify and prototype within tight academic timelines.

---

## 2. Answers to Synthesis Questions

### 1. 哪些結論被強力支持？ (What conclusions are strongly supported?)
* **Memory Bandwidth Bottleneck**: The weight transfer subsystem dominates MoE execution latency.
* **Compute Latency Hiding**: NPU compute time is completely hidden behind transfer latency under double buffering.
* **Prefetch Gating**: Gating prefetching at a $0.7$ confidence threshold is mandatory to prevent transfer inflation on random workloads.

### 2. 哪些結論被部分支持？ (What conclusions are partially supported?)
* **Double Buffering Speedup**: Highly effective in balanced HBM environments (1.42× speedup), but marginal under extreme memory-bound systems.

### 3. 哪些結論仍不確定？ (What conclusions are still uncertain?)
* **RTL Tag Array Latency**: The exact timing and lookup delay of fully associative CAM cache tags in SystemVerilog.

### 4. 哪些假設被否定？ (What assumptions are rejected?)
* **Compute Acceleration Value**: Accelerating NPU compute cycles yields 0% system speedup under double-buffered scheduling.
* **Static LRU Cache**: Thrashes to 0.00% hit rate under expert-major scheduling unless aided by prefetching.

### 5. 哪些元件最重要？ (Which components are most important?)
* **Expert Cache tags, Confidence-gated Prefetcher, Double-buffer Manager, and Queue Scheduler** (forming the core transfer gateway).

### 6. 哪些元件最不重要？ (Which components are least important?)
* **NPU GEMM compute array and local RISC-V control core**.

### 7. 哪些互動最重要？ (Which interactions are most important?)
* **Cache × Prefetch ($C 	imes P$)**: Strongest constructive interaction, resolving the cache thrashing cliff.

### 8. 是否發現新的架構方向？ (Is a new architecture direction discovered?)
* **Yes. The "Smart Memory Transfer Gateway" (SMTG)** configuration (C+P+DB+S) was discovered as the optimal, minimal hardware gateway.

### 9. 是否需要新增新的 Candidate？ (Is it necessary to add a new Candidate?)
* **Yes. Candidate F (SMTG)** has been introduced as the primary research target, representing the minimal viable subset of Candidate C.

### 10. 目前最值得繼續探索的研究問題是什麼？ (What is the most important research question to explore next?)
* The physical timing closure and layout optimization of the Markov-1 predictor and tag array on the PYNQ-Z2 FPGA board.
