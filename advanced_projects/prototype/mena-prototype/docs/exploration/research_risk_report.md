# Research Risk Analysis Report (Q16)

## 1. Research Question
What are the technical, verification, FPGA prototyping, time, and publication risks associated with each candidate architecture? Which candidate represents the most viable path for completion within a typical master's thesis timeline?

---

## 2. Motivation
Academic research is constrained by tight timelines. Selecting a high-risk architecture (like building a full NPU) can lead to project delays or failure to compile under FPGA limits, preventing graduation. We must evaluate risks quantitatively.

---

## 3. Methodology
We establish a risk score matrix (1 to 5, where 1 is lowest risk and 5 is highest risk) across five risk dimensions:
1. **Technical Risk**: Likelihood of design bugs or logic compilation failure.
2. **Verification Risk**: Complexity of writing testbenches and verifying correctness.
3. **FPGA Prototyping Risk**: Difficulty of fitting the design into budget FPGA boards (e.g. PYNQ-Z2).
4. **Time Risk**: Risk of exceeding a 12-month development and testing cycle.
5. **Publication Risk**: Risk of rejection due to low novelty or low speedup compared to baselines.

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

### Candidate Risk Matrix (1 = Low Risk, 5 = High Risk)

| Risk Dimension | Candidate A (SW) | Candidate B (MENA) | Candidate C (DMA) | Candidate D (Proc) | Candidate E (Hybrid) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Technical Risk** | **1** | 5 | 2 | 4 | 3 |
| **Verification Risk**| **1** | 5 | 2 | 3 | 4 |
| **FPGA Prototyping** | **1** | 5 | 2 | 3 | 4 |
| **Time Risk** | **1** | 5 | 2 | 4 | 3 |
| **Publication Risk** | 5 | 3 | 3 | 2 | **1** |
| **Total Risk Score** | **9** | 23 | **11** | 16 | 15 |

---

## 6. Observations
1. **The MENA High-Risk Trap**: Candidate B (MENA) has the highest total risk score (**23**). Integrating a custom NPU, cache controller, queue dispatcher, and predictor in RTL is extremely complex and will likely exceed a 12-month development window.
2. **Smart DMA Low-Risk Profile**: Candidate C (Smart DMA Sidecar) has the lowest hardware risk score (**11**). It only requires designing standard DMA logic and tag arrays, which is highly feasible for FPGA compilation.

---

## 7. Contradictions
Students often believe that "building a more complex system (like MENA) guarantees a better thesis". However, the data contradicts this: **the time spent debugging complex arithmetic blocks in MENA directly detracts from time available for architectural research**, increasing graduation risk without improving publication potential.

---

## 8. Implications
- **Graduation Optimization**: **Candidate C (Smart DMA)** is the safest hardware path for a master's thesis.
- **Top-Tier Publication Optimization**: **Candidate E (Hybrid Sidecar)** represents the best risk-to-reward ratio. Its publication risk is extremely low due to high novelty, while its technical risk is manageable by reusing a small open-source RISC-V control core.

---

## 9. Threats to Validity
- **Open-source Core Integration**: The risk score for Candidate D/E assumes that integrating an open-source RISC-V core is straightforward. If bus-protocol mismatches occur, the technical risk of D/E could rise.

---

## 10. Future Questions
* Can a software emulator bypass FPGA physical mapping entirely to mitigate prototyping risk?

---

## 11. Whether Results Support MENA
* **Status**: **REJECTED.**
* **Reason**: The risk analysis reveals that MENA (Candidate B) is a high-risk architecture that is highly likely to fail within a master's program timeline due to the high verification and FPGA prototyping overhead of custom NPU logic.
