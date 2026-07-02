# MAEP Architecture Discovery Synthesis
## Comprehensive MoE Architecture Space Synthesis (Framework v4.0)

This document presents the final synthesis of the Mixture-of-Experts Architecture Exploration Platform (MAEP) based on the 13 quantitative design space exploration reports, incorporating the newly discovered factor **Weight-Activation Bandwidth Contention (WABC)**.

---

## Answers to Synthesis Questions

### 1. 哪些現象被強力支持？ (What phenomena are strongly supported?)
* **Memory Bandwidth Bottleneck**: Weight transfer dominates MoE execution latency.
  * *Why*: Expert weight size (2MB) relative to high routing frequency creates massive transfer volumes that exceed DRAM bandwidth capacity.
  * *When holds*: Under low VRAM bandwidth (e.g., LPDDR, DDR5, < 64 B/cycle).
  * *When fails*: High-bandwidth memory (e.g., HBM3, > 256 B/cycle) where NPU computation cycles dominate.
* **Compute Latency Hiding**: NPU compute time can be overlapped behind transfer latency under double buffering.
  * *Why*: When weight transfer is slower than NPU processing, double buffering overlaps the transfer in the background of execution.
  * *When holds*: When bottleneck ratio $\beta = \frac{t_{\text{transfer}}}{t_{\text{compute}}} \ge 1.0$.
  * *When fails*: Under compute-heavy workloads where NPU cycles exceed transfer cycles, or under extreme memory bus contention.
* **Prefetch Gating**: Gating prefetching at a $0.7$ confidence threshold prevents transfer inflation on random workloads.
  * *Why*: Inaccurate prefetching loads unused experts, wasting memory bandwidth and stalling demand transfers. Confidence gating filters low-probability fetches.
  * *When holds*: Under Zipfian skew and low entropy.
  * *When fails*: High entropy random routing where transition probabilities are uniform.

### 2. 哪些現象被部分支持？ (What phenomena are partially supported?)
* **Double Buffering Speedup**: Latency hiding by overlapping transfer and compute.
  * *Why*: Hides transfer latency by scheduling the next expert weight load while the current one is executed on the NPU.
  * *When holds*: Balanced ratio $0.5 \le \beta \le 2.0$.
  * *When fails*: Extreme memory-bound systems ($\beta > 10$) where transfer dominates, or when shared DRAM contention (WABC) slows down weight loading.

### 3. 哪些現象仍不確定？ (What phenomena are still uncertain?)
* **Hardware Tag Lookup Latency**: Exact gate-level delays of fully associative CAM cache tag lookups in SystemVerilog.
  * *Why*: Requires physical synthesis and gate-level netlist simulation.
  * *When holds*: Large tag arrays exceeding 16 entries.
  * *When fails*: Small tag arrays with negligible lookup times.

### 4. 哪些假設被否定？ (What assumptions are rejected?)
* **Matrix Multiplication Acceleration Value**: Rejects the assumption that matrix multiplication acceleration is the primary performance driver.
  * *Why*: Accelerating NPU compute cycles yields 0% system speedup when weight transfer is the bottleneck.
  * *When holds*: Memory-bound region ($\beta \ge 1.0$).
  * *When fails*: High batch sizes where system shifts to compute-bound.
* **Ideal Weight Bandwidth**: Rejects the assumption that weight loading has exclusive DRAM access.
  * *Why*: Shared off-chip memory bus causes WABC conflicts between weight loading and activation transfers.
  * *When holds*: Shared memory bus architectures.
  * *When fails*: Multi-channel systems with dedicated channels.
* **Stand-alone LRU Caching**: Rejects the assumption that standard LRU cache alone is effective.
  * *Why*: Expert-major scheduling cycles through experts, thrashing caches.
  * *When holds*: Without prefetching support.
  * *When fails*: Token-order processing or prefetch-supported caches.

### 5. 哪些因素最重要？ (Which factors are most important?)
* **Memory Transfer Control (Cache, Prefetch, Double Buffering)**.
  * *Why*: They target the primary memory bandwidth bottleneck directly.
  * *When holds*: $\beta > 1.0$.
  * *When fails*: $\beta < 1.0$.

### 6. 哪些因素最不重要？ (Which factors are least important?)
* **Compute Engine size and Local Metadata control cores**.
  * *Why*: Compute latency is hidden, and metadata processing cycles are negligible.
  * *When holds*: $\beta \ge 1.0$.
  * *When fails*: $\beta < 0.5$.

### 7. 哪些因素具有交互作用？ (Which factors exhibit interaction effects?)
* **Cache × Prefetch ($C \times P$)**: Strong positive interaction.
  * *Why*: Prefetch loads weights before execution, cache holds them. Without cache, prefetch has nowhere to store; without prefetch, cache thrashes.
  * *When holds*: Zipf workloads.
  * *When fails*: Random workloads.
* **WABC × Cache**: Positive interaction.
  * *Why*: Cache hits reduce DRAM transfer volume, which mitigates shared-bus conflicts (WABC).
  * *When holds*: Small cache size relative to active set.
  * *When fails*: Infinite cache size.

### 8. 哪些因素存在衝突？ (Which factors exhibit conflicts?)
* **Compute Locality vs. Cache Locality**:
  * *Why*: Expert-major scheduling groups tokens by expert to maximize compute locality but thrashes cache by increasing reuse distance.
  * *When holds*: Cache size is smaller than the active expert set.
  * *When fails*: Cache size is larger than active expert set.
* **Weight vs. Activation Bandwidth (WABC)**:
  * *Why*: Shared DRAM bus usage creates arbitration delay.
  * *When holds*: Shared-bus memory architectures.
  * *When fails*: Separated memory buses.

### 9. 哪些因素存在適用邊界？ (What are the applicable boundaries?)
* **Double Buffering**: Balanced bottleneck ratio ($0.5 \le \beta \le 2.0$).
* **Prefetching**: Low routing entropy ($H < 2.0$) and confidence threshold ($P \ge 0.7$).

### 10. 哪些因素存在失效邊界？ (What are the failure boundaries?)
* **Double Buffering**: Fails when batch size $B \ge 4$ (shifting to compute-bound) or when WABC share exceeds $30\%$.
* **Prefetching**: Fails when routing entropy is high ($H \ge 2.5$), causing up to 125% transfer bloat.

### 11. 是否出現新的架構模式？ (Did a new architecture pattern emerge?)
* **No**. While a combination of mechanisms (Cache, Prefetch, Double Buffer, Queue Scheduler) provides significant speedup, labels like "SMTG" (Smart Memory Transfer Gateway), "Gateway", "Sidecar", "Runtime Engine", or "Metadata Processor" are strictly temporary analytical tags. None of these configurations are considered proven architecture patterns or research conclusions until verified across more workloads, traces, and parameters. No configuration is declared "optimal", "best", "recommended", or a "winner".

### 12. 是否需要新增新的探索方向？ (Is it necessary to add a new exploration direction?)
* **Yes. Weight-Priority DRAM Arbitration**: Exploring hardware bus arbiters to resolve the newly discovered **WABC** conflict.

### 13. 下一輪最值得探索的問題是什麼？ (What is the most important question to explore in the next round?)
* The physical timing closure and layout optimization of the Markov-1 predictor and tag array on the PYNQ-Z2 FPGA board, and the integration of a **Weight-Priority DRAM Arbiter (WPDA)**.
