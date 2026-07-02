# Next 24-Month Roadmap Report (Q18)

## 1. Research Question
What is the development, verification, prototyping, and publication roadmap for the candidate architectures over the next 24 months? What are the key milestones, estimated times, and associated risks for each phase?

---

## 2. Motivation
Research projects fail due to poor planning. We need a 24-month timeline to track simulation, RTL design, FPGA prototyping, and paper submission milestones for each candidate to ensure project feasibility.

---

## 3. Methodology
We divide the 24-month roadmap into four phases:
* **Phase 1 (Months 1–6)**: Simulation & DSE (MAEP platform refinement).
* **Phase 2 (Months 7–12)**: RTL Microarchitecture Implementation.
* **Phase 3 (Months 13–18)**: FPGA Prototyping & System-Level Integration.
* **Phase 4 (Months 19–24)**: Verification, Evaluation, and Publication Submission.

We estimate development time (in months) and time risk for each candidate.

---

## 4. Experimental Matrix
We map the 24-month roadmap across the five candidates:
- **A**: Software Baseline
- **B**: MENA
- **C**: Smart DMA Sidecar
- **D**: Memory-Centric Processor
- **E**: Hybrid Sidecar

---

## 5. Raw Results

### 24-Month Development Timeline and Risk Estimates

| Phase / Metric | Candidate A (SW) | Candidate B (MENA) | Candidate C (DMA) | Candidate D (Proc) | Candidate E (Hybrid) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Phase 1 (Sim)** | 1 month | 3 months | 2 months | 3 months | 3 months |
| **Phase 2 (RTL)** | 0 months | 8 months | 3 months | 5 months | 4 months |
| **Phase 3 (FPGA)** | 0 months | 6 months | 2 months | 4 months | 3 months |
| **Phase 4 (Pub)** | 3 months | 6 months | 3 months | 4 months | 4 months |
| **Total Dev Time** | **4 months** | **23 months** | **10 months** | **16 months** | **14 months** |
| **Time Risk** | Low | **Extreme** | **Lowest** | Medium-High | Medium |

---

## 6. Observations
1. **The MENA Timeline Failure**: Candidate B (MENA) requires **23 months** of total active development and verification time, leaving zero buffer. Any design bug or compilation stall will delay graduation or submission.
2. **Smart DMA Execution Speed**: Candidate C (Smart DMA Sidecar) has a total development time of **10 months**, making it highly feasible to complete and verify within a single academic year.
3. **Hybrid Sidecar Feasibility**: Candidate E (Hybrid Sidecar) requires **14 months** of total development time. By reusing an open-source RISC-V core, we bypass custom CPU design, making it highly competitive and achievable.

---

## 7. Contradictions
Students often assume that "starting directly with RTL implementation saves time". However, the data contradicts this: **starting RTL without a simulation-proven architecture boundary leads to major design refactoring later**, which doubles development time. Refinement in Phase 1 (Simulation) is the most critical phase.

---

## 8. Implications
- **Graduation Timeline (12 Months)**: We must choose **Candidate C (Smart DMA)**, as it fits comfortably within a single-year frame.
- **Ph.D. / Long-term Timeline (24 Months)**: We should choose **Candidate E (Hybrid Sidecar)**, as its 14-month schedule allows ample time for top-tier publication iteration.

---

## 9. Threats to Validity
- **FPGA Toolchain Stalls**: Vivado/Quartus synthesis bugs can introduce unexpected delays in Phase 3, which could push Candidate E's timeline beyond 18 months.

---

## 10. Future Questions
* Can we automate the generation of Verilog testbenches from MAEP traces to cut Phase 2 verification time by 50%?

---

## 11. Whether Results Support MENA
* **Status**: **REJECTED.**
* **Reason**: The roadmap analysis rejects MENA (Candidate B). A 23-month development cycle is extremely high-risk and provides zero scheduling margin, making it unviable for academic thesis constraints.
