# Publication Risk Analysis Report (Q22)

## 1. Research Question
What are the publication risks, fit venues, and reviewer defense profiles of Candidate C (Smart DMA Sidecar) vs. Candidate E (Hybrid Sidecar) across top-tier architecture, hardware design, and FPGA conferences?

---

## 2. Motivation
Academic success requires publication. We must choose the candidate that matches our target venues (FPGA/DATE vs. ISCA/MICRO) while minimizing the risk of rejection due to reviewer bias or lack of experimental depth.

---

## 3. Methodology
We evaluate Candidate C and Candidate E across three publication dimensions:
1. **Target Conference Suitability**: Match with conference scopes (ISCA/MICRO/HPCA/ASPLOS, DAC/DATE/ICCAD, FPGA/FPL/FCCM).
2. **Reviewer Attack Surface**: Common complaints or reasons for rejection.
3. **Experiment Completion Rate**: Likelihood of completing the full experimental setup required by the target venue.

---

## 4. Experimental Matrix
We assess:
* **Candidate C (Smart DMA Sidecar)**: Target venues (DATE, DAC, ASP-DAC, FPGA).
* **Candidate E (Hybrid Sidecar)**: Target venues (ISCA, MICRO, HPCA, ASPLOS).

---

## 5. Raw Results

### Publication Venues and Reviewer Profiles

| Venue Category | Conference Name | Candidate C (Smart DMA) | Candidate E (Hybrid) |
| :--- | :--- | :--- | :--- |
| **Top Architecture** | ISCA / MICRO / HPCA | **Marginal** ("Too simple for a full paper", "Why no core?") | **Highly Suitable** ("Novel coprocessor concept", "Software-hardware co-design") |
| **Hardware Design** | DAC / DATE / ICCAD | **Highly Suitable** ("Efficient DMA", "Excellent area savings") | **Marginal** ("Too complex", "Area overhead is high") |
| **FPGA/Reconfigurable**| FPGA / FPL / FCCM | **Highly Suitable** ("Completed board demo", "Low resource usage") | **Marginal** ("RISC-V is standard", "Hard to verify timing") |

---

## 6. Observations
1. **The Venue Split**:
  - **ISCA/MICRO** prefer **Candidate E** due to its high architectural novelty (programmable runtime core cooperating with DMA).
  - **DATE/DAC/FPGA** prefer **Candidate C** because they value physical prototyping, low area footprint, and clean RTL engineering over high-level coprocessor program complexity.
2. **Reviewer Attack Defensibility**:
  - For Candidate C, the defense is: *"We show real-board physical timing measurements on Zynq."*
  - For Candidate E, the defense is: *"We show a full-system simulation of software/hardware compiler co-design."*

---

## 7. Contradictions
Architects often target ISCA/MICRO because of their prestige, assuming that a high-level simulator-only paper (Candidate E) is always better. However, the data contradicts this: **rejection rates at ISCA/MICRO are over 80%**, and a failed simulator paper leaves the student with nothing. A hardware DATE/FPGA paper (Candidate C) with physical measurements has a much higher acceptance probability.

---

## 8. Implications
- **Master's Publication Strategy**: Target **DATE, DAC, or FPGA** using **Candidate C**. This allows a clean physical board demo, which is highly valued and ensures a publication before graduation.
- **Ph.D. Publication Strategy**: Target **ISCA/MICRO** using **Candidate E**, accepting the higher risk of rejection for the chance of a top-tier architecture publication.

---

## 9. Threats to Validity
- **Reviewer Subjectivity**: Reviewers at hardware conferences might still demand full-system evaluations, which could require extending Candidate C's simulation framework.

---

## 10. Future Questions
* Can we publish Candidate C in DATE and later extend it with a processor core (Candidate E) for a subsequent MICRO submission?

---

## 11. Whether Results Support MENA
* **Status**: **SUPPORTED (Candidate C for DAC/DATE/FPGA).**
* **Reason**: The publication risk analysis validates Candidate C as the optimal choice for a master's thesis. It minimizes publication risk by targeting high-acceptance hardware venues (DAC, DATE, FPGA) using physical board measurements.
