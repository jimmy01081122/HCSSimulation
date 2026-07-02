# Architecture Boundary Mapping Report (Q12)

## 1. Research Question
What are the exact operational boundaries of the MENA accelerator architecture? Under what workload parameters, memory bandwidth constraints, and model configurations is MENA highly effective, marginal, or completely useless?

---

## 2. Motivation
LLM workloads are highly dynamic. We must map the exact operating boundaries of MENA to define when it adds value and when it should be bypassed or disabled.

---

## 3. Methodology
We synthesize the sweep data from all design space explorations to establish a boundary map based on three parameters:
1. **Bottleneck Ratio ($\beta = \frac{t_{\text{transfer}}}{t_{\text{compute}}}$)**: Indicates memory vs. compute dominance.
2. **Routing Entropy ($H$)**: Indicates workload routing concentration.
3. **Double-Buffering Overlap Efficiency ($\eta = \frac{t_{\text{compute}}}{\max(t_{\text{transfer}}, t_{\text{compute}})}$)**.

---

## 4. Experimental Matrix
We map the system states across three operational regions:
- **Effective Region**: High speedup, high cache hit rates, low overheads.
- **Marginal Region**: Modest gains, high prefetch sensitivity.
- **Failure Region**: Complete cache thrashing, high prefetch bloat, or 0% overlap efficiency.

---

## 5. Raw Results

### The MENA Architectural Boundary Map

| Operational Region | Workload Type | Bandwidth Range | Bottleneck Ratio $\beta$ | Cache Hit Rate | Double-Buffering Speedup |
| :--- | :--- | :--- | :---: | :---: | :---: |
| **Effective Region** | Type A, Type B | Medium/High (64-128 B/cycle) | $0.5 \le \beta \le 2.0$ | $50\% \text{ to } 75\%$ | **1.36× to 1.42×** |
| **Marginal Region** | Type D (Shift) | Low/Medium (16-64 B/cycle) | $2.0 < \beta < 5.0$ | $10\% \text{ to } 40\%$ | **1.08× to 1.18×** |
| **Failure Region** | Type C, Type E | Extreme Low/High (<16 or >256) | $\beta < 0.1 \text{ or } \beta > 5.0$| $0.00\%$ | **1.00× to 1.04×** |

---

## 6. Observations
1. **The Sweet Spot (Effective Region)**: MENA achieves its maximum latency reduction (up to 2.80× overall speedup) when routing entropy is low ($H < 2.0$, allowing high prefetch hit rates) and the system is balanced ($\beta \approx 1.0$, allowing double-buffering to hide compute and transfer latencies concurrently).
2. **The Dead Zone (Failure Region)**:
  - **Severe Memory-Bound**: If DRAM bandwidth is extremely low ($\beta > 5.0$), weight transfer completely dominates, and double buffering speedup drops to <4%.
  - **Random Routing**: If routing is uniform random ($H \ge 2.9$), cache hit rate drops to 0%, and prefetching causes up to 93% bandwidth bloat, making MENA worse than the baseline.

---

## 7. Contradictions
Architectural projects often claim that their accelerators work "across all LLM workloads". However, the data contradicts this: **MENA is highly specialized**. It performs exceptionally well in skewed generation (Type A/B), but is completely useless in high-entropy (Type C) or memory-starved systems, where it only contributes to area and power overhead.

---

## 8. Implications
- **System Design Boundary**: MENA must be integrated with a **bypassing path** in hardware. When a Type C workload is detected (high cache misses and low prefetch confidence), the system should bypass the cache, disable prefetching, and run in serial mode to protect bandwidth.
- **DRAM Scaling**: Double-buffered prefetching is only worth implementing if the SoC provides HBM-class bandwidth (64-128 B/cycle) to keep $\beta$ within the balanced effective region.

---

## 9. Threats to Validity
- **Co-run Interference**: Other hardware blocks on the SoC might generate memory traffic that dynamically shifts $\beta$, causing a system in the Effective Region to fall into the memory-bound Marginal Region.

---

## 10. Future Questions
* Can compile-time schedules predict and dynamically adjust threshold policies for each layer?

---

## 11. Whether Results Support MENA
* **Status**: **SUPPORTED WITH EXPLICIT BOUNDARIES.**
* **Reason**: The mapping quantitatively defines the boundaries of the MENA accelerator, proving it is highly effective in balanced HBM environments with structured workloads, while defining the exact failure zones that must be avoided.
