# MoE Mechanism Knowledge Base (Research Exploration v5.0)
## Comprehensive MoE Systems Architecture Discovery Study

This document compiles the empirical and theoretical findings regarding the systems-level mechanisms governing Mixture-of-Experts (MoE) LLM inference accelerators. All observations are derived from timing-accurate simulations and trace-level statistical characterizations of synthetic zipfian, uniform, adversarial, and real-world LLM traces (Mixtral 8x7B, DeepSeek-MoE).

---

## Answers to Synthesis Questions

### 1. 已確認現象 (Confirmed Phenomena)
*   **Memory Bandwidth Bottleneck (P1)**: Weight transfer dominates MoE execution latency.
    *   *Why*: Expert weight size (2MB) relative to high routing frequency creates massive transfer volumes that exceed DRAM bandwidth capacity.
    *   *Evidence*: Sweeping memory bandwidth shows that at LPDDR/DDR5 bandwidths (<64 B/cycle), the bottleneck ratio $\beta = \frac{t_{\text{transfer}}}{t_{\text{compute}}}$ ranges from **5.49 to 21.97**, meaning weight transfer accounts for 85% to 95% of execution latency.
*   **Scheduling-Induced Cache Thrashing Cliff (P2)**: Expert-major scheduling groups token activations to maximize NPU compute locality, but thrashes standard LRU cache hit rates to exactly **0.00%**.
    *   *Why*: Expert-major scheduling processes activations expert-by-expert. This increases the temporal reuse distance of subsequent experts beyond the cache size.
    *   *Evidence*: LRU Cache size 4 under Zipf E8 routing yields 0.00% hit rate, matching no-cache latency.
*   **Cache × Prefetch Interaction (P3)**: Cache hit rates can only recover under expert-major scheduling if paired with active prefetching.
    *   *Why*: The prefetcher predicts sequential transitions and fetches weights into the background cache before execution queries.
    *   *Evidence*: Pairing cache size 4 with a history prefetcher raises the hit rate from 0% to **59.09%** under Zipf E8 workloads, cutting latency by 51.3%.
*   **Weight-Activation Bandwidth Contention (WABC) (P4)**: Shared-bus off-chip memory systems suffer from bandwidth contention between weight loading and token activations.
    *   *Why*: Weight loads and token activation read/writes share the same off-chip memory bus, causing bus arbitration conflicts.
    *   *Evidence*: When activation memory bandwidth share reaches 20%, double-buffering speedup drops to 1.09x. When it exceeds 30%, double buffering results in a performance loss due to bus arbitration stalls.

### 2. 部分確認現象 (Partially Confirmed Phenomena)
*   **Double Buffering Overlap Speedup**:
    *   *Why*: Double buffering schedules the next expert weight load while the current one is executed on the NPU, hiding transfer latency.
    *   *Status*: Effective under balanced memory-compute bandwidth configurations ($0.5 \le \beta \le 2.0$), yielding up to **1.42× speedup**. However, it is marginal under extreme memory-bound systems ($\beta > 10.0$) or high memory bus contention (WABC share >30%).
*   **Transition Predictability in Real Traces**:
    *   *Why*: Consecutive token expert selections exhibit transitional skew, allowing Markov predictors to capture transition probabilities.
    *   *Status*: Verified on Mixtral E8 (median reuse distance = 2.0, avg entropy = 2.39 bits). However, on larger configurations like DeepSeek E64, reuse distance stretches (median RD = 14.0, avg entropy = 5.16 bits), which reduces transition predictability for small history tables.

### 3. 未解釋與待驗證問題 (Unresolved & Validation Questions)
*   **WABC Generalization Study (MV1)**: Is WABC a universal bottleneck in MoE systems? Does WABC occur on real traces like Mixtral, DeepSeek-MoE, and Qwen-MoE? Does it persist across different memory architectures (DDR5, HBM, CXL)?
*   **Entropy-Prefetch Quantitative Relationship (MV2)**: What is the quantitative mathematical relationship between routing entropy $H$ and prefetch accuracy? Can accuracy be modeled as a function of entropy, i.e., $\text{Accuracy} = f(H)$?
*   **Cache Capacity vs. Reuse Distance Predictive Model (MV3)**: Is there a general predictive model relating cache size and temporal reuse distance (RD) distribution (e.g., $p_{50}$ and $p_{95}$) to cache hit rate?
*   **Locality Conflict Generalization (MV4)**: Is the conflict between Compute Locality and Cache Locality a universal property of MoE, or does it only occur under specific routing distributions?
*   **Routing Preference Phase Shifts**: What causes temporal routing patterns to transition between prompt prefill and token generation phases?
*   **Bus Arbitration Delay Non-Linearity**: What are the dynamic queuing delays under shared DDR5 buses, and how do they scale with WABC activation share?

### 4. 被否定與修正假設 (Rejected & Qualified Hypotheses)
*   **NPU GEMM Compute Acceleration Value under Evaluated Conditions**:
    *   *Observation*: Under the currently evaluated experimental configurations (specific trace sizes, transfer parameters, and compute models), accelerating matrix multiplication (NPU GEMM cycles) provided negligible system-level benefit.
    *   *Why*: Under current parameters, the system is memory-bound ($\beta \ge 1.0$). Compute cycles are entirely hidden behind memory transfers under double-buffered scheduling.
    *   *Caveat*: This does not prove compute acceleration is universally unimportant; it is dependent on our current parameters.
*   **LRU Cache Stand-Alone Value**: Rejects the assumption that standard LRU caches are naturally effective for MoE.
    *   *Why*: Thrashing cliffs reduce hit rates to 0% unless prefetching actively populates the cache in the background.
*   **DRAM Bandwidth Isolation**: Rejects the assumption that weight loading has exclusive off-chip bandwidth access.
    *   *Why*: High-context token generation triggers massive activation transfers that conflict with weight loads on shared buses.

### 5. 關鍵機制 (Key Mechanisms)
*   **Confidence-Gated Prefetching**: Gating prefetch requests with a transition confidence threshold **$P \ge 0.7$**.
    *   *Why*: Prevents prefetching low-probability expert candidates, which bloats DRAM transfers.
*   **Dynamic Double-Buffer Handshaking**: Synchronizing weight load phases with NPU compute start signals to ensure weight transfers are completed or scheduled prior to compute execution.

### 6. 次要機制 (Secondary Mechanisms)
*   **Hardware Queue Scheduling Logic**: Queue status logic (LQF) handles routing slots but contributes minimally to cycle reductions (<5%) compared to memory transfer optimization.
*   **RISC-V Coprocessor Metadata Management**: Programmatic routing overhead consumes negligible execution cycles (<2% of total cycles).

### 7. 新增候選與假說 (Emerging Hypotheses / Solutions)
*   **Weight-Priority DRAM Arbitration (WPDA) (Hypothesized Solution for WABC)**:
    *   *Concept*: Dynamic priority bus controller that prioritizes weight loading requests over activation writebacks during double-buffering overlap phases.
    *   *Status*: Hypothesized solution; requires comparative evaluation against other arbitration policies (e.g., Round-Robin, Least-Recently-Granted) to verify benefit.
*   **Adaptive Batch/Context Sizing**:
    *   *Concept*: Dynamically altering the batch size or context length to balance compute and weight loading times, keeping $\beta \approx 1.0$.
    *   *Status*: Hypothesized solution; requires a systematic Batch Control and Runtime Adaptation Study to confirm effectiveness.
*   **Bandwidth-Aware Prefetch Inhibitor**:
    *   *Concept*: A hardware control loop that dynamically disables prefetching when DRAM bus queuing latency exceeds a set threshold.
    *   *Status*: Hypothesized solution; requires validation under varying bus noise levels.

### 8. 強交互作用 (Strong Interactions)
*   **Cache × Prefetch ($C \times P$)**: Strong positive interaction.
    *   *Why*: Prefetch loads weights before execution, cache holds them. Without cache, prefetch has nowhere to store; without prefetch, cache thrashes under expert-major scheduling.
*   **WABC × Cache**: Positive interaction.
    *   *Why*: Cache hits reduce the total DRAM transfer volume, directly decreasing weight-activation shared-bus conflicts.

### 9. 弱交互作用 (Weak Interactions)
*   **Compute Engine × Metadata Processor ($CE \times MP$)**:
    *   *Why*: Accelerating GEMM arrays has zero coupling with RISC-V metadata processing cycles.

### 10. 適用邊界 (Applicable Boundaries)
*   **Confidence-Gated Prefetching**: Low-entropy skewed workloads ($H < 2.0$) and transition confidence threshold $P \ge 0.7$.
*   **Double Buffering**: Balanced bottleneck ratios ($0.5 \le \beta \le 2.0$).
*   **WPDA Arbiter**: Shared-bus memory architectures where activation memory bandwidth share is active.

### 11. 失效邊界 (Failure Boundaries)
*   **Confidence-Gated Prefetching**: Fails under high-entropy random routing ($H \ge 2.5$), causing up to **125% transfer inflation (bloat)**, saturating the bus and stalling demand transfers.
*   **Double Buffering**: Fails when batch size $B \ge 4$ (compute-bound) or when WABC share exceeds 30%, introducing buffer overhead without latency savings.
*   **Standard Expert Caching**: Fails on large expert counts (e.g. DeepSeek E64) where the median reuse distance (14.0) exceeds cache capacity.

### 12. 未知因素 (Unknown Factors)
*   **Quantization Interactions**: How FP8, INT4 weight-only, or activation quantization dynamically scale $\beta$.
*   **Semantic Routing Skew**: How dynamic changes in input query semantics change routing entropy during long context generation.

### 13. 最值得深入研究的問題 (Most Promising Future Work)
*   **Mechanism Validation MV1-MV4**: Quantitative evaluation of WABC generalization, Entropy-Prefetch Accuracy correlation, Cache Capacity vs. Reuse Distance modeling, and Locality Conflict generality.
*   **Physical Layout & Timing Analysis of Tag CAM Arrays**: Verifying the area and lookup timing budget of a 64-entry fully associative Tag CAM array on FPGA.
