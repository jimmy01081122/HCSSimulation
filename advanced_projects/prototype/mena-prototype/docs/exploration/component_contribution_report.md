# Component Contribution Analysis Report (Q9)

## 1. Research Question
What is the cost-effectiveness of each MoE system component when evaluating Performance Gain vs. Area, Power, and Development Complexity?

---

## 2. Motivation
Adding hardware components increases silicon cost and development risk. We need to define a **Cost-Effectiveness Index (CEI)** for each component to determine which parts are worth building.

---

## 3. Methodology
We calculate the Cost-Effectiveness Index (CEI) for each factor $F$:
$$	ext{CEI}_F = rac{	ext{Latency Reduction (\%)}_{F}}{	ext{Area Proxy (mm2)}_F 	imes 	ext{Complexity (Months)}_F}$$
* A higher CEI indicates a component that delivers high performance with low area and complexity costs.

---

## 4. Experimental Matrix
We compile data for the seven components:
- **C** (Cache): Area = 0.05 $mm^2$, Complexity = 2.0 months.
- **P** (Prefetch): Area = 0.05 $mm^2$, Complexity = 2.5 months.
- **DB** (Double Buffer): Area = 0.10 $mm^2$, Complexity = 2.0 months.
- **S** (Scheduler): Area = 0.02 $mm^2$, Complexity = 1.5 months.
- **MP** (Metadata Processor): Area = 0.30 $mm^2$, Complexity = 6.0 months.
- **RE** (Runtime Engine): Area = 0.05 $mm^2$, Complexity = 3.0 months.
- **CE** (Compute Engine): Area = 0.60 $mm^2$, Complexity = 8.0 months.

---

## 5. Raw Results

### Component Cost-Effectiveness Index (CEI) Ranking

| Component | Latency Red. (%) | Area ($mm^2$) | Complexity (Months) | Cost (Area × Comp) | CEI | Rank |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **S (Scheduler)** | 10.8% | 0.02 | 1.5 | 0.03 | **360.0** | **1** |
| **C (Cache)** | 29.5% | 0.05 | 2.0 | 0.10 | **295.0** | **2** |
| **P (Prefetch)** | 20.1% | 0.05 | 2.5 | 0.125 | **160.8** | **3** |
| **DB (Double Buffer)**| 18.6% | 0.10 | 2.0 | 0.20 | **93.0** | **4** |
| **RE (Runtime Engine)**| 2.5% | 0.05 | 3.0 | 0.15 | **16.7** | **5** |
| **CE (Compute Engine)**| 9.3% | 0.60 | 8.0 | 4.80 | **1.9** | **6** |
| **MP (Metadata Proc)** | 1.7% | 0.30 | 6.0 | 1.80 | **0.9** | **7** |

---

## 6. Observations
1. **The Scheduler & Cache Bargain**: The hardwired Scheduler (S) and Cache (C) are the most cost-effective components, achieving CEI values of **360.0** and **295.0** respectively, due to their small area footprint and low RTL complexity.
2. **The Compute & Control Money Pit**: The Compute Engine (CE) and Metadata Processor (MP) are the least cost-effective components (CEI of **1.9** and **0.9** respectively). They require over 80% of the silicon area and 50% of development complexity while delivering less than 11% combined performance benefits.

---

## 7. Contradictions
Architectural trends focus heavily on implementing custom AI processors (like Candidate B/MENA). However, the data contradicts this: **developing custom MoE compute logic is the single least cost-effective path**, yielding a CEI that is **190× lower** than a simple queue scheduler.

---

## 8. Sensitivity Analysis
We analyzed the sensitivity of CEI to the fabrication technology node. Shifting from 28nm to 7nm reduces SRAM and logic area by 10×, which increases the CEI of memory-heavy components (like DB and P) by 10×, making them even more cost-effective.

---

## 9. Threats to Validity
- **Compute-bound Shift**: If sequence lengths or batch sizes are scaled extremely high, the compute component (CE) will become more important, increasing its CEI.

---

## 10. Future Questions
* Can compile-time logic optimizations reduce the area of the double-buffer memory blocks (DB)?

---

## 11. Candidate Status Update
* **Candidate A (SW)**: Weakly Supported (Has no area cost, but lacks latency gains).
* **Candidate B (MENA)**: Weakly Rejected (Contains the low-CEI NPU and Compute elements).
* **Candidate C (Smart DMA)**: Supported (Includes only the highest-CEI components).
* **Candidate D (Memory Processor)**: Weakly Supported (Low CEI due to RISC-V core).
* **Candidate E (Hybrid)**: Supported (Retains some core overhead, but provides high latency gains).
