# Predictor Hardware Cost Analysis Report (Q11)

## 1. Research Question
What is the hardware overhead (SRAM capacity, silicon area, lookup latency, and access energy) of implementing first-order (Markov-1) and second-order (Markov-2) transition predictors in the MoE accelerator? Does the predictor's area/energy cost exceed the system-level benefits it provides?

---

## 2. Motivation
Markov predictors require storing transition counts in on-chip SRAM. As the number of experts scales, these matrices grow quadratically (Markov-1) or cubically (Markov-2). We must evaluate if this cost is reasonable compared to off-chip DRAM load energy.

---

## 3. Methodology
We model the hardware implementation of Markov-1 ($E \times E$ transition matrix) and Markov-2 ($E^2 \times E$ transition matrix) using 8-bit counters. We estimate:
- **SRAM Storage**: Counter matrix capacity.
- **Silicon Area**: Estimated in TSMC 28nm using a single-port SRAM cell size of $0.12 \mu m^2$ with a 30% peripheral overhead.
- **Access Energy**: Calculated using 28nm SRAM read energy of $0.1 \text{ pJ/bit}$ and compared against off-chip LPDDR4 DRAM access energy of $20 \text{ pJ/bit}$ ($160 \text{ pJ/byte}$) for loading a 2MB expert weight.

---

## 4. Experimental Matrix
We evaluate the predictor configurations across four expert counts representing different MoE model scales:
- **$E = 8$** (e.g. Mixtral 8x7B)
- **$E = 16$** (e.g. Qwen-MoE)
- **$E = 64$** (e.g. DeepSeekMoE routed set)
- **$E = 256$** (Next-generation fine-grained MoE models)

---

## 5. Raw Results

### Predictor Storage, Area, and Energy Sweep (TSMC 28nm)

| Experts ($E$) | Predictor Type | Counter Entries | SRAM Size (KB) | Silicon Area ($mm^2$) | Lookup Energy (nJ) |
| :---: | :--- | :---: | :---: | :---: | :---: |
| **8** | Markov-1 | 64 | 0.062 KB | 0.000080 | 0.05 nJ |
| | Markov-2 | 512 | 0.500 KB | 0.000639 | 0.41 nJ |
| **16** | Markov-1 | 256 | 0.250 KB | 0.000319 | 0.20 nJ |
| | Markov-2 | 4,096 | 4.000 KB | 0.005112 | 3.28 nJ |
| **64** | Markov-1 | 4,096 | 4.000 KB | 0.005112 | 3.28 nJ |
| | Markov-2 | 262,144 | 256.000 KB | 0.327156 | 209.72 nJ |
| **256** | Markov-1 | 65,536 | 64.000 KB | 0.081789 | 52.43 nJ |
| | Markov-2 | 16,777,216 | 16,384.000 KB | 20.937966 | 13,421.77 nJ |

* **Off-chip DRAM load energy for ONE 2MB expert weight**: **320,000 nJ (0.32 mJ)**.

---

## 6. Observations
1. **The Markov-2 Area Explosion**: For large expert counts ($E = 256$), Markov-2 requires **16 MB of SRAM**, occupying **20.93 mm²** of silicon area in 28nm. This is an enormous area cost, representing a large portion of a typical edge accelerator chip.
2. **Energy Payback Ratio**: Reading a row of the Markov-1 predictor for $E = 64$ consumes **3.28 nJ** of energy. In comparison, loading a single 2MB expert weight from DRAM consumes **320,000 nJ**. The DRAM weight load is **97,560× more energy-intensive** than the SRAM lookup.

---

## 7. Contradictions
System designers often assume that adding prediction logic to an accelerator is too complex and energy-intensive for simple cache control. However, the data contradicts this: **the energy overhead of SRAM lookups is completely negligible compared to the massive energy savings from reducing even a single DRAM expert load**. Saving a single miss pays back the lookup energy costs of 97,000+ cache checks.

---

## 8. Implications
- **Area-Inefficient Zone**: Markov-2 is completely unfeasible in hardware for expert counts $E \ge 64$. First-order Markov-1 (requiring only 4 KB for $E=64$) is highly feasible and area-efficient.
- **Predictor Design Guideline**: Accelerators should implement a **Markov-1 table** rather than Markov-2 for hardware savings. If higher-order dependencies are needed, they must be implemented using **sparse hashing tables** or handled in software.

---

## 9. Threats to Validity
- **Counter Bitwidth**: If 8-bit counters are insufficient to capture long-term frequency shifts, increasing counter bitwidth will scale SRAM capacity and area proportionally.

---

## 10. Future Questions
* Can sparse transition tables or dynamic pruning reduce Markov-2 storage requirements?

---

## 11. Whether Results Support MENA
* **Status**: **SUPPORTED (Markov-1) / REJECTS (Markov-2).**
* **Reason**: The data supports MENA's Markov-1 prefetcher, proving it is highly area-efficient (4 KB for $E=64$) and energy-justified. However, it rejects a hardware Markov-2 implementation due to the 20 mm² area overhead at larger expert scales.
