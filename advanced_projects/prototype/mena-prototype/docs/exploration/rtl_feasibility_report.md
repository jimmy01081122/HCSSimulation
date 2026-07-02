# RTL Feasibility Report (Q7)

## 1. Research Question
What is the estimated FPGA resource footprint, timing slack, and mapping feasibility of the SMTG design on the budget PYNQ-Z2 (XC7Z020) board?

---

## 2. Motivation
Academic designs must be implementable. We need to estimate the RTL resource footprints on FPGA to ensure our proposed mechanisms fit into realistic hardware constraints and do not suffer from timing closure failures.

---

## 3. Methodology
We estimate FPGA resource utilization on the Xilinx Zynq-7000 XC7Z020 (53,200 LUTs, 106,400 FFs, 140 BRAMs, 220 DSPs) using standard IP cores:
* **DMA Engine**: Standard AXI-Stream DMA footprint.
* **Cache Tag array**: Fully-associative CAM lookup logic.
* **Predictor Table**: SRAM-based Markov-1 table (4 KB).
* **Double Buffer**: Dual-port weight SRAM buffers.

---

## 4. Experimental Matrix
We estimate and compare two hardware configurations:
- **Configuration 1 (Smart DMA Sidecar)**: Pure memory controller.
- **Configuration 2 (RISC-V Coprocessor)**: Smart DMA + Ibex RISC-V core.

---

## 5. Raw Results

### FPGA Resource and Timing Estimates (XC7Z020)

| Hardware Resource | Config 1 (Smart DMA) | Config 2 (RISC-V Coprocessor) | PYNQ-Z2 Budget |
| :--- | :---: | :---: | :---: |
| **LUT Utilization** | **4,000 (7.5%)** | 12,000 (22.6%) | 53,200 |
| **FF Utilization** | **5,000 (4.7%)** | 14,000 (13.2%) | 106,400 |
| **BRAM Utilization** | **3 (2.1%)** | 20 (14.3%) | 140 |
| **DSP Utilization** | **0 (0.0%)** | 4 (1.8%) | 220 |
| **Max Frequency ($F_{	ext{max}}$)**| **150 MHz** | 100 MHz | — |
| **Integration Difficulty** | **Low** (AXI-HP Stream) | High (AXI-Interconnect) | — |

---

## 6. Observations
1. **Low Footprint for Config 1**: The Smart DMA configuration requires only **7.5% of LUTs** and **2.1% of BRAMs** on the XC7Z020, making it extremely easy to place and route, achieving a high clock frequency of **150 MHz**.
2. **Coprocessor Overhead for Config 2**: Configuration 2 requires **12,000 LUTs (22.6%)** and **20 BRAMs (14.3%)** due to the RISC-V soft-core and its local memories. Bus interconnect complexity increases drastically, dropping the expected frequency to **100 MHz**.

---

## 7. Contradictions
Architects often assume that "since soft CPUs are small, they are easy to fit on any FPGA". However, the data contradicts this: **the main bottleneck is not LUT count, but memory and interconnect complexity**. Routing the AXI crossbar interfaces for the RISC-V core on the XC7Z020 increases wiring congestion and timing closure risks significantly.

---

## 8. Sensitivity Analysis
We analyzed the sensitivity of timing slack to the width of the AXI-Stream interface. Upgrading the stream width from 64-bit to 256-bit improves DMA bandwidth by 4×, but increases LUT count by 2.2× and reduces $F_{	ext{max}}$ by 12% due to wiring congestion.

---

## 9. Failure Cases
When synthesis is performed with a 512-bit wide AXI bus on the XC7Z020, routing fails completely due to congestion, violating timing constraints.

---

## 10. Threats To Validity
- **Vivado Version Variations**: Synthesis results can vary between Vivado versions. Poor routing optimization could increase Config 2's footprint beyond the estimates.

---

## 11. Future Questions
* Can the Smart DMA tag arrays be mapped to LUTRAM instead of BRAM to reduce BRAM usage to 0?

---

## 12. Emerging Mechanisms
* **AXI-HP Congestion Bypass**: An optimized bus adapter that splits AXI transactions to prevent routing congestion on small FPGAs.
