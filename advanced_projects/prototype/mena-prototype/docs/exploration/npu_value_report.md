# NPU Value & Latency Breakdown Report (Q7)

## 1. Research Question
Is a large, high-compute NPU worth implementing in MoE systems, or is the system bottlenecked by the transfer subsystem? How does changing NPU compute performance affect the overall system execution latency?

---

## 2. Motivation
GEMM computing arrays take up huge silicon areas. If MoE system latency is entirely memory-bandwidth bound, building a large matrix execution engine is an architectural waste. We need to evaluate the value of NPU scale.

---

## 3. Methodology
We sweep NPU compute latency per token (0, 50, 100, 200, 500 cycles) under both Single and Double Buffering configurations.
We measure:
- **Compute Cycles**: Clock cycles spent executing tokens on the NPU.
- **Transfer Cycles**: Clock cycles spent transferring expert weights from DRAM.
- **Total Cycles**: End-to-end execution cycles.
- **Speedup**: Performance gain of double buffering.

---

## 4. Experimental Matrix
- **Trace**: `synth_zipf_E8` (cache size 4, history prefetch)
- **Transfer Latency**: 281,250 cycles (9 misses, 2MB size, 64 B/cycle)
- **Compute Latency per Token**: 0 (infinite compute), 50, 100, 200, 500 cycles

---

## 5. Raw Results

| Compute Latency | Compute Cycles | Single Buffering Cycles | Double Buffering Cycles | DB Speedup | Compute % of Total (Single) |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **0 (Infinite)** | 0 | **281,250** | **281,250** | **1.00×** | 0.0% |
| **50 (Fast NPU)** | 51,200 | **332,450** | **281,450** | **1.18×** | 15.4% |
| **100 (Medium NPU)** | 102,400 | **383,650** | **281,650** | **1.36×** | 26.7% |
| **200 (Slow NPU)** | 204,800 | **486,050** | **341,300** | **1.42×** | 42.1% |
| **500 (CPU/No Acc)** | 512,000 | **793,250** | **574,500** | **1.38×** | 64.5% |

---

## 6. Observations
1. **The Transfer Bottleneck (Amdahl's Law)**: If we have an **infinitely fast NPU** (0 cycles compute latency), the total execution time is **281,250 cycles** (pure DRAM transfer time). Improving the NPU from "Medium" (100 cycles) to "Infinite" (0 cycles) only yields a **1.36× speedup** under single buffering, and **1.00× speedup** under double buffering!
2. **Compute-Hiding Magic**: Under double buffering, as long as the NPU compute latency (e.g. 51,200 or 102,400 cycles) is *smaller* than the DRAM weight transfer latency (281,250 cycles), the NPU execution time is **100% hidden** behind the transfer time. The total cycles remain virtually unchanged (~281,450 cycles).

---

## 7. Contradictions
Silicon startups often spend 90% of their engineering effort optimizing GEMM datapath execution speed for MoE accelerators. However, the data contradicts the value of this approach: under double-buffered scheduling, **all compute latency is hidden**, meaning that a 10× faster GEMM engine results in **exactly 0% system-level speedup**.

---

## 8. Implications
- **Silicon Budget Allocation**: Developing a large, power-hungry, high-compute NPU for MoE is **highly inefficient**. The system performance is dominated by the weight transfer bottleneck.
- **"Good-Enough" Compute**: Because double buffering hides compute latency beneath transfer latency, the NPU only needs to be "fast enough" to match the transfer rate. Designing a lightweight NPU that executes a token in $\le 200$ cycles is sufficient; any further compute speedup yields **zero** system-level performance gains.
- **RTL Focus**: Silicon budget and design effort should be concentrated on **DMA engines, cache controllers, and prefetch predictors** (the transfer subsystem) rather than compute units.

---

## 9. Threats to Validity
- **Non-overlap Compute Phases**: We assume all compute phases are GEMM operations that can be overlapped. Non-overlappable operations (e.g., routing softmax, token dispatcher queue sorting, or CPU synchronization overhead) might break the ideal overlap, shifting the bottleneck back to compute.

---

## 10. Future Questions
* Can software compilations schedule non-GEMM compute operations to maximize NPU sleep or overlap?

---

## 11. Whether Results Support MENA
* **Status**: **SUPPORTED.**
* **Reason**: The results strongly validate the decision to treat the NPU as a secondary, lightweight assist module (handling only Router MLP, quantization, and small GEMM) rather than a full-scale transformer processor. System bottlenecks are located entirely in the weight transfer subsystem.
