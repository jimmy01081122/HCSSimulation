# MAEP Mechanism Validation Summary
## Sprint v5.1 Validation Findings & Synthesis

This document summarizes the quantitative validation findings from the 7 Mechanism Validation sprints (MV1-MV7). All observations are derived from timing-accurate simulator sweeps and trace-level statistical characterizations.

---

## Answers to Validation Questions

### 1. 哪些現象通過驗證？ (What phenomena are validated?)
*   **Memory Bandwidth Bottleneck (P1)**: Confirmed that weight transfer dominates MoE execution latency. The bottleneck ratio $\beta = \frac{t_{\text{transfer}}}{t_{\text{compute}}}$ ranges from **5.49 to 21.97** under DDR5-like (32 B/cycle) bandwidth.
*   **Scheduling-Induced Cache Thrashing Cliff (P2)**: Confirmed that expert-major scheduling groups activations to maximize NPU compute locality, but thrashes LRU cache hit rates to exactly **0.00%** because it extends reuse distances.
*   **Cache × Prefetch Interaction (P3)**: Confirmed that pairing a cache with a prefetcher (Markov-1) recovers cache hits to **59.09%** under Zipfian routing.
*   **Weight-Activation Bandwidth Contention (WABC) (P4)**: Confirmed that sharing off-chip DRAM buses between weight loads and activation reads/writes reduces weight bandwidth, degrading double-buffering speedup.

### 2. 哪些現象只在特定條件成立？ (What phenomena hold only under specific conditions?)
*   **Double-Buffering Speedup**: Holds only under balanced bottleneck ratios ($0.5 \le \beta \le 2.0$). Fails in extreme memory-bound systems ($\beta > 5.0$) or when WABC share exceeds 30%.
*   **Markov Prediction Accuracy**: Holds only when routing entropy is low ($H < 2.0$) and transition probabilities are concentrated.

### 3. 哪些現象被削弱？ (What phenomena are weakened?)
*   **LRU Caching Alone**: The value of standard LRU caches without prefetching is weakened, as they thrash to 0.00% hit rate under expert-major scheduling across all characterized traces.

### 4. 哪些假說需要更多實驗？ (What hypotheses require further experiments?)
*   **Weight-Priority DRAM Arbitration (WPDA)**: The hypothesized WPDA arbiter requires cycle-level simulations to compare its latency benefits against standard Round-Robin and Least-Recently-Granted policies.
*   **Adaptive Batch/Context Sizing**: Requires a systematic runtime adaptation study to verify latency-balancing benefits.

### 5. 核心因素哪些最不重要？ (Which factors are least important?)
*   **Compute Engine size and Local Metadata control cores**: Since compute latency is hidden in the balanced region, scaling compute is of secondary importance.

### 6. 下一輪應驗證什麼？ (What should be validated in the next round?)
*   The hardware area, layout overhead, and lookup timing budget of a Cache Tag CAM array coupled with a Markov-1 prefetcher, and bus arbiter efficiency under WABC.
