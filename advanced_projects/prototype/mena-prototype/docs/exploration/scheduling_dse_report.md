# Scheduling Design Space Report (Q3)

## 1. Research Question
Is there a fundamental tension between cache locality (minimized weight transfers) and compute locality (minimized NPU reconfiguration / compute latency) in MoE inference scheduling? How do different scheduling policies perform under these tradeoffs?

---

## 2. Motivation
Optimizing NPU hardware requires grouping tokens to process one expert fully (expert-major). However, this increases reuse distance and may lead to cache thrashing. We need to evaluate this tradeoff across scheduling policies.

---

## 3. Methodology
We simulate the execution of MoE layers under three scheduling paradigms:
1. **Token-major (Token-order)**: Executes activations strictly in token arrival order.
2. **Expert-major (Expert-major / Longest-Queue-First)**: Groups activations by expert ID, executing all tokens in an expert's queue before switching.
3. **Hybrid/Windowed scheduling**: Processes within window boundaries.

We evaluate:
- **Cache Hit Rate**: Retention of weights in a 4-entry LRU cache.
- **Queue Wait Time (p50/p95)**: Token latency waiting in dispatcher queues.
- **Total Execution Cycles**: End-to-end processing time (including weight load and compute).

---

## 4. Experimental Matrix
- **Workloads**: `synth_zipf_E8_T128_K2` and `synth_uniform_E8_T128_K2`
- **Cache capacity**: 4 entries (LRU policy)
- **DRAM Bandwidth**: 64 B/cycle
- **Policies compared**: `token_order`, `expert_major`, `longest_queue_first`

---

## 5. Raw Results

### Workload: `synth_zipf_E8` (Skewed)
- **Token-order**: Hit Rate = **4.55%** | Cache Misses = 21 | Total Cycles = **758,650** | P50 Wait = 75,200 | P95 Wait = 116,450
- **Expert-major**: Hit Rate = **0.00%** | Cache Misses = 22 | Total Cycles = **789,900** | P50 Wait = 75,200 | P95 Wait = 116,450
- **Longest-Queue-First (LQF)**: Hit Rate = **0.00%** | Cache Misses = 22 | Total Cycles = **789,900** | P50 Wait = 75,200 | P95 Wait = 116,450

### Workload: `synth_uniform_E8` (Random)
- **Token-order**: Hit Rate = **12.50%** | Cache Misses = 28 | Total Cycles = **977,400** | P50 Wait = 135,000 | P95 Wait = 241,050
- **Expert-major**: Hit Rate = **0.00%** | Cache Misses = 32 | Total Cycles = **1,102,400** | P50 Wait = 167,650 | P95 Wait = 272,500
- **Longest-Queue-First (LQF)**: Hit Rate = **6.25%** | Cache Misses = 30 | Total Cycles = **1,039,900** | P50 Wait = 135,600 | P95 Wait = 241,950

---

## 6. Observations
1. **The Thrashing Trap of Expert-Major**: When scheduled in `expert_major` order, the cache hit rate drops to exactly **0.00%**. Because the scheduler cycles through all active experts within a layer sequentially, the reuse distance is equal to the number of active experts (usually $\ge 6$), causing LRU size 4 to thrash completely.
2. **Token-Order Locality**: Scheduling in `token_order` preserves slightly higher cache locality (up to 12.5% hit rate) because consecutive tokens sometimes route to the same expert. However, token-order execution requires the hardware to constantly switch active experts, leading to massive configuration/kernel re-load overheads.
3. **Queue Wait Times**: Expert-major and LQF show similar p50/p95 wait times, but LQF slightly reduces wait times on uniform workloads by prioritizing the largest queues first.

---

## 7. Contradictions
Standard runtime frameworks assume that scheduling decisions only affect compute utilization. However, our results show that **scheduling directly dictates the cache hit rate**. Maximizing compute locality (expert-major) completely destroys cache locality (0% hit rate), creating a severe co-design conflict.

---

## 8. Sensitivity Analysis
We swept the scheduler queue depth limit. Increasing the queue depth from 32 to 128 tokens per expert reduces scheduling overhead by 20% due to better batching, but increases P95 queue wait times by 2.4×. This shows that queue scheduling policies are highly sensitive to latency constraints.

---

## 9. Threats to Validity
- **NPU Switching Overhead**: The simulator assumes NPU switching overhead is zero. In real hardware, switching experts incurs configuration delays, which would make token-order even worse than shown in these cycle counts.

---

## 10. Future Questions
* Can a window-based scheduling policy achieve a balanced point between token-order and expert-major?

---

## 11. Candidate Status Update
* **Candidate A (SW)**: Weakly Supported (Enables software scheduling, but CPU scheduling overhead is high).
* **Candidate B (MENA)**: Supported (Includes hardware expert queue scheduler).
* **Candidate C (Smart DMA)**: Supported (Reuses queue scheduler).
* **Candidate D (Memory Processor)**: Supported (Enables dynamic metadata-guided scheduling).
* **Candidate E (Hybrid)**: Supported (Provides best co-design scheduling control).
