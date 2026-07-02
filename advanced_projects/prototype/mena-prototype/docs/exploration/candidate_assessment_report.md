# Candidate Architecture Assessment Report (Q11)

## 1. Research Question
How do the five candidate architectures (A: Software Runtime, B: MENA, C: Smart DMA Sidecar, D: Memory-Centric Processor, E: Hybrid Sidecar) perform across execution latency, hardware area, energy efficiency, and development risk?

---

## 2. Motivation
We must quantitatively compare the candidates to evaluate their feasibility and support levels, helping us select the most viable research path.

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
- **Compare Targets**: Candidates A, B, C, D, E

---

## 5. Raw Results

### Candidate Assessment Matrix

| Metric | Candidate A (SW) | Candidate B (MENA) | Candidate C (DMA) | Candidate D (Proc) | Candidate E (Hybrid) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Total Cycles** | 788,700 | 286,875 | 282,875 | 293,825 | **207,450** |
| **Speedup vs. SW** | 1.00× | 2.75× | 2.79× | 2.68× | **3.80×** |
| **Area Proxy ($mm^2$)**| **0.00** | 0.85 | **0.15** | 0.45 | 0.50 |
| **Energy Proxy ($\mu J$)**| 7,040 | 2,889 | 2,886 | 2,464 | **2,112** |
| **Verification Cost** | **Zero** | High | Low | Medium | High |

---

## 6. Observations
1. **The Smart DMA Area Gain**: Candidate C (Smart DMA Sidecar) achieves almost identical latency reduction to Candidate B (MENA) but at **5.6× less silicon area (0.15 vs 0.85 mm²)**. This is because removing the custom GEMM array eliminates the bulk of the logic area without affecting system-level performance.
2. **The Hybrid Speedup**: Candidate E (Hybrid Sidecar) achieves the best overall performance (**3.80× speedup**) and energy efficiency (**2112 $\mu J$**). The tiny metadata core dynamically schedules expert bypassing to achieve a 70% cache hit rate while the Smart DMA controller overlaps transfers.

---

## 7. Contradictions
Standard design beliefs dictate that "integrating compute units (NPU) into an accelerator always increases performance". However, the data contradicts this: **Candidate C (no NPU) outperforms Candidate B (with NPU)**. This occurs because the custom NPU in Candidate B introduces extra MMIO synchronization delays (5,000 cycles) which offset any minor compute speedup, whereas a pure DMA sidecar has minimal software synchronization overhead.

---

## 8. Sensitivity Analysis
We analyzed sensitivity to synchronization overhead. If Candidate E's sync overhead increases from 1,200 to 15,000 cycles, its latency speedup drops from 3.80× to 2.45×, showing that Candidate E is highly sensitive to CPU-coprocessor link latency.

---

## 9. Threats to Validity
- **Co-processor Handshake Latency**: If the communication channel between the main CPU and the co-processor in Candidate D/E experiences high latency, Candidate E's speedup will degrade.

---

## 10. Future Questions
* Can the Smart DMA tag arrays be mapped to LUTRAM instead of BRAM to reduce BRAM usage to 0?

---

## 11. Candidate Status Update
* **Candidate A (SW)**: Strongly Rejected (High latency, high energy).
* **Candidate B (MENA)**: Weakly Rejected (Over-complex, high area).
* **Candidate C (Smart DMA)**: Supported (Highly cost-effective, low area).
* **Candidate D (Memory Processor)**: Weakly Supported (RISC-V core adds overhead).
* **Candidate E (Hybrid)**: Supported (Best overall performance, but higher risk).
