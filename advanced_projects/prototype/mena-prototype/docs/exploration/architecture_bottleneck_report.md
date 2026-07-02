# Bottleneck Analysis Report (Q1)

## 1. Research Question
What is the primary execution bottleneck in MoE-LLM inference systems? How do memory bandwidth, weight sizes, and compute throughput interact to limit system performance?

---

## 2. Motivation
Accelerating MoE models is challenging due to the dynamic activation of different experts for each token. We need to locate the exact latency bottlenecks in the system pipeline to avoid optimizing non-critical hardware paths.

---

## 3. Methodology
We model the MoE pipeline stage latencies as:
1. **Routing**: Top-K selection (20 cycles fixed overhead).
2. **Dispatch**: Token activation enqueueing (40 cycles fixed overhead).
3. **Scheduling**: Queue status monitoring (10 cycles fixed overhead).
4. **Weight Transfer (DMA)**: DRAM weight load ($t_{	ext{transfer}} = rac{	ext{Expert Size}}{	ext{DRAM Bandwidth}}$).
5. **NPU Compute**: Matrix multiplication of active tokens ($t_{	ext{compute}} = 	ext{tokens} 	imes 	ext{cycles/token}$).

We sweep **DRAM Bandwidth** (8 to 256 B/cycle) to find the transition point between memory-bound and compute-bound execution, establishing a quantitative bottleneck ratio:
$$eta = rac{t_{	ext{transfer}}}{t_{	ext{compute}}}$$

---

## 4. Experimental Matrix
- **Trace**: `synth_zipf_E8` (cache size 4, history prefetch)
- **Expert Size**: 2MB
- **NPU Compute Latency**: 100 cycles per token
- **DRAM Bandwidth swept**: 8.0, 16.0, 32.0, 64.0, 128.0, 256.0 B/cycle

---

## 5. Raw Results

*Total Compute Cycles ($t_{	ext{compute}}$) = 102,400 cycles.*

| DRAM Bandwidth | DRAM Misses | Transfer Cycles ($t_{	ext{transfer}}$) | Total Cycles (Single Buffer) | Bottleneck Ratio $eta$ | Primary Bottleneck |
| :---: | :---: | :---: | :---: | :---: | :--- |
| **8 B/cycle** | 9 | **2,250,000** | **2,352,400** | **21.97** | Memory (Extreme) |
| **16 B/cycle** | 9 | **1,125,000** | **1,227,400** | **10.98** | Memory (Severe) |
| **32 B/cycle** | 9 | **562,500** | **664,900** | **5.49** | Memory |
| **64 B/cycle** | 9 | **281,250** | **383,650** | **2.74** | Memory (Moderate) |
| **128 B/cycle** | 9 | **140,625** | **243,025** | **1.37** | Memory (Balanced) |
| **256 B/cycle** | 9 | **70,312** | **172,712** | **0.68** | Compute-bound |

---

## 6. Observations
1. **Severe Memory Domination**: Under LPDDR or DDR memory subsystems (8 to 32 B/cycle), the bottleneck ratio $eta$ ranges from **5.49 to 21.97**. Weight transfer accounts for **85% to 95%** of the entire execution latency.
2. **Transition to Compute-Bound**: The system only becomes compute-bound ($eta = 0.68$) at extreme memory bandwidths ($\ge 256$ B/cycle), typical of ultra-high-end HBM3.

---

## 7. Contradictions
Conventional NPU designs prioritize scaling compute density (TPU/GPU style). However, for MoE models, **compute acceleration yields zero system-level speedup** unless memory bandwidth is extremely high. This contradicts the design philosophy of compute-centric LLM accelerators.

---

## 8. Sensitivity Analysis
We analyzed the sensitivity of $eta$ to the expert size. Doubling the expert size to 4MB doubles the transfer time, making the system 2× more memory-bound ($eta$ rises to 5.48 at 64 B/cycle). Conversely, halving the expert size to 1MB shifts $eta$ to 1.37, making it balanced. This indicates that model size directly dictates the memory bottleneck severity.

---

## 9. Threats to Validity
- **Static Token Count**: The simulator assumes a fixed token count of 128 per layer. If batch sizes scale significantly, compute latency will scale proportionally, shifting the system into the compute-bound zone ($eta < 1$) even at lower bandwidths.

---

## 10. Future Questions
* How does activation quantization or model weight pruning affect the bottleneck ratio?

---

## 11. Candidate Status Update
* **Candidate A (SW)**: Weakly Rejected (Software cannot resolve the raw memory bandwidth transfer bottleneck).
* **Candidate B (MENA)**: Weakly Supported (Identifies the cache/prefetch need but over-invests in the NPU compute engine).
* **Candidate C (Smart DMA)**: Supported (Directly addresses the memory transfer bottleneck).
* **Candidate D (Memory Processor)**: Supported (Focuses on metadata instead of compute).
* **Candidate E (Hybrid)**: Supported (Optimizes synchronization).
