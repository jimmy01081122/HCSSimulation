# Novelty Assessment Report (Q17)

## 1. Research Question
What is the novelty, publication potential, lit-overlap, and reviewer attack surface of each candidate architecture? Which direction represents the most defensible contribution for top-tier computer architecture and hardware design conferences?

---

## 2. Motivation
Academic publication requires clear novelty. If our architecture overlaps too heavily with existing MoE runtimes or accelerators (like Google's TPU MoE support or Microsoft's DeepSpeed), reviewers will reject the paper as incremental. We must evaluate novelty.

---

## 3. Methodology
We characterize the publication potential of each candidate across four dimensions:
1. **Academic Novelty**: Uniqueness of the architectural concept.
2. **Lit-Overlap**: Overlap with existing public research (lower is better).
3. **Reviewer Attack Surface**: Common weaknesses or gaps that reviewers can exploit.
4. **Target Conferences**: Best-fit venues for publication (e.g. ISCA, MICRO, HPCA, ASPLOS, DAC, DATE, FPGA).

---

## 4. Experimental Matrix
We assess the five candidates:
- **A**: Software Baseline
- **B**: MENA (NPU + Cache)
- **C**: Smart DMA Sidecar
- **D**: Memory-Centric Processor
- **E**: Hybrid Sidecar

---

## 5. Raw Results

### Academic Novelty & Publication Defensibility Matrix

| Metric | Candidate A (SW) | Candidate B (MENA) | Candidate C (DMA) | Candidate D (Proc) | Candidate E (Hybrid) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Novelty** | None | Low-Medium | Medium | High | **Highest** |
| **Lit-Overlap** | High (DeepSpeed) | High (Standard Accel) | Medium | Low | **Lowest** |
| **Attack Surface**| "Incremental SW only" | "Predictor SRAM cost", "Custom GEMM area"| "No compiler support", "Too simple" | "High control overhead" | **"Minimal"** (Balanced SW/HW) |
| **Target Venues**| MLSys, ATC | DAC, DATE | DATE, FPGA, DAC | MICRO, HPCA | **ISCA, MICRO, HPCA** |

---

## 6. Observations
1. **The MENA Attack Surface**: Candidate B (MENA) is highly vulnerable to reviewer attack. Reviewers will ask: *"Why build a custom NPU when commodity NPUs are 10× faster? How does the area of your Markov predictor compare to simply increasing weight cache capacity?"*
2. **The Hybrid Sidecar Strength**: Candidate E (Hybrid Sidecar) has the highest novelty and lowest attack surface. It combines hardware-efficient DMA with a runtime metadata processor, representing a realistic and highly defensible co-design contribution.

---

## 7. Contradictions
Architectural researchers often assume that "adding more custom hardware blocks (like custom GEMM/NPU in MENA) increases paper acceptance probability". However, the data contradicts this: **adding custom compute blocks actually increases the attack surface**, as reviewers will compare your custom NPU against industry-standard Google TPUs or NVIDIA Tensor Cores, leading to rejection.

---

## 8. Implications
- **ISCA/MICRO/HPCA Target**: We must choose **Candidate E (Hybrid Sidecar)**. It defines a new category of "intelligent memory controllers for MoE" rather than a generic accelerator, which is highly valued by top-tier reviewers.
- **DAC/DATE Target**: **Candidate C (Smart DMA)** is highly suitable for hardware-centric venues, as its implementation is clean and area-efficient.

---

## 9. Threats to Validity
- **Reviewer Bias**: Top-tier architecture reviewers might still prefer large-scale full-system simulations (like gem5 full-system with Linux) over custom trace replays, regardless of the architecture candidate selected.

---

## 10. Future Questions
* How does the publication potential of Candidate E change if we integrate real trace evaluations from DeepSeek-V3?

---

## 11. Whether Results Support MENA
* **Status**: **REJECTED.**
* **Reason**: The novelty assessment rejects MENA (Candidate B). Its heavy overlap with generic transformer accelerators and high attack surface (due to custom GEMM logic) makes it very difficult to publish in top-tier computer architecture venues.
