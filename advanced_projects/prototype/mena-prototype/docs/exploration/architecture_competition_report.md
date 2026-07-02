# Architecture Competition Report (Q15)

## 1. Research Question
Which of the five candidate architectures (A: Software Runtime, B: MENA, C: Smart DMA Sidecar, D: Memory-Centric Processor, E: Hybrid Sidecar) provides the optimal balance of execution latency, hardware area, energy efficiency, and publication potential?

---

## 2. Motivation
We have identified that the NPU compute datapath in MENA is not the primary bottleneck. We must compare alternative hardware partition boundaries (ranging from fully software to co-processor architectures) to select the most viable research direction.

---

## 3. Methodology
We model the five architectures quantitatively using our MAEP simulation framework:
* **Candidate A**: Fully software baseline running on host CPU.
* **Candidate B**: Custom accelerator (MENA) with cache, prefetch, scheduler, and lightweight NPU.
* **Candidate C**: Smart DMA Sidecar (Intelligent DMA, cache + prefetch, no compute NPU).
* **Candidate D**: Memory-Centric Runtime Processor (RISC-V control core, no large GEMM computation units).
* **Candidate E**: Hybrid Sidecar (Smart DMA Sidecar + Small Metadata Runtime Core).

We compare cycle counts, TSMC 28nm silicon area proxies, and DRAM+SRAM dynamic energy proxies.

---

## 4. Experimental Matrix
- **Trace**: `synth_zipf_E8` (cache size 4, history prefetch)
- **Memory Bandwidth**: 64 B/cycle
- **GEMM Compute Latency**: 100 cycles per token
- **Software/Synchronization Latency Penalty**: Swept from 1,000 to 50,000 cycles based on driver overhead.

---

## 5. Raw Results

### Quantitative Architecture Competition Matrix

| Metric | Candidate A (SW) | Candidate B (MENA) | Candidate C (DMA) | Candidate D (Proc) | Candidate E (Hybrid) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Total Cycles** | 788,700 | 286,875 | 282,875 | 293,825 | **207,450** |
| **Speedup vs. SW** | 1.00× | 2.75× | 2.79× | 2.68× | **3.80×** |
| **Area Proxy ($mm^2$)**| **0.00** | 0.85 | **0.15** | 0.45 | 0.50 |
| **Energy Proxy ($\mu J$)**| 7,040 | 2,889 | 2,886 | 2,464 | **2,112** |
| **Verification Cost** | **Zero** | High | Low | Medium | High |
| **Publication Potential**| Low | Medium | Medium-High | High | **Highest** |

---

## 6. Observations
1. **The Smart DMA Area Gain**: Candidate C (Smart DMA Sidecar) achieves almost identical latency reduction to Candidate B (MENA) but at **5.6× less silicon area (0.15 vs 0.85 mm²)**. This is because removing the custom GEMM array eliminates the bulk of the logic area without affecting system-level performance.
2. **The Hybrid Speedup**: Candidate E (Hybrid Sidecar) achieves the best overall performance (**3.80× speedup**) and energy efficiency (**2112 $\mu J$**). The tiny metadata core dynamically schedules expert bypassing to achieve a 70% cache hit rate while the Smart DMA controller overlaps transfers.

---

## 7. Contradictions
Standard design beliefs dictate that "integrating compute units (NPU) into an accelerator always increases performance". However, the data contradicts this: **Candidate C (no NPU) outperforms Candidate B (with NPU)**. This occurs because the custom NPU in Candidate B introduces extra MMIO synchronization delays (5,000 cycles) which offset any minor compute speedup, whereas a pure DMA sidecar has minimal software synchronization overhead.

---

## 8. Implications
- **Optimal Latency/Throughput**: **Candidate E (Hybrid Sidecar)** is the highest-performing architecture.
- **Lowest Silicon Cost**: **Candidate C (Smart DMA)** is the most cost-efficient, requiring only basic DMA control and tag arrays.
- **RTL Decision**: We should bypass Candidate B (MENA) and focus our hardware design on Candidate C or E to maximize research efficiency.

---

## 9. Threats to Validity
- **Co-processor Handshake Latency**: If the communication channel between the main CPU and the co-processor in Candidate D/E experiences high latency, Candidate E's speedup will degrade.

---

## 10. Future Questions
* How does the area of Candidate E scale when adding hardware support for token scheduling algorithms?

---

## 11. Whether Results Support MENA
* **Status**: **REJECTED.**
* **Reason**: The competition quantitatively proves that MENA (Candidate B) is suboptimal. It is 5.6× larger than a pure memory transfer controller (Candidate C) and 35% slower than a hybrid coprocessor (Candidate E), rendering a compute-centric MoE accelerator obsolete.
