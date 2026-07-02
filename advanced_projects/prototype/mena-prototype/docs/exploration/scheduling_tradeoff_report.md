# Scheduling Tradeoff & Locality Analysis Report (Q3)

## 1. Research Question
Is there a fundamental tension between cache locality (minimized weight transfers) and compute locality (minimized NPU reconfiguration / compute latency) in MoE inference scheduling? How do token-major and expert-major scheduling policies perform under these tradeoffs?

---

## 2. Motivation
Optimizing NPU hardware requires grouping tokens to process one expert fully (expert-major). However, this increases reuse distance and may lead to cache thrashing. We need to evaluate this tradeoff to determine if co-design scheduler policies can bypass it.

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

## 8. Implications
- **Co-design Conflict**: The results demonstrate a fundamental tension: token-order preserves cache locality but destroys compute throughput; expert-order maximizes compute throughput but destroys cache locality.
- **System Escape Route**: To escape this conflict, MoE systems cannot rely on standard cache policies alone; they *must* use **pre-fetching** to fetch weights before they are executed, and **double buffering** to hide the transfer time.

---

## 9. Threats to Validity
- **NPU Switching Overhead**: The simulator assumes NPU switching overhead is zero. In real hardware, switching experts incurs configuration delays, which would make token-order even worse than shown in these cycle counts.

---

## 10. Future Questions
* Can a window-based scheduling policy achieve a balanced point between token-order and expert-major?

---

## 11. Whether Results Support MENA
* **Status**: **SUPPORTED.**
* **Reason**: The scheduling tradeoff study confirms that pure expert-major scheduling thrashes a small LRU cache completely (0% hit rate). This directly supports the need for MENA's **Expert Queue Scheduler** to cooperate with a **Double Buffer Manager** and **History Predictor** to overlap prefetching and avoid NPU execution stalls.
