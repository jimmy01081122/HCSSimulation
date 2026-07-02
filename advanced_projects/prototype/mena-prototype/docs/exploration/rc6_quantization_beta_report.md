# RC6: Quantization and Beta Shift Study

## 1. Observed Phenomenon
Compression (quantization) of expert weights reduces the weight load volume, reducing transfer times and scaling down the bottleneck ratio $\beta$.

## 2. Hypothesis
If lower precision (e.g. INT4) reduces $\beta$ below 1.0, the system shifts to a compute-bound state. In this region, scaling compute capacity (matrix arrays) becomes a key performance factor.

## 3. Methodology
We calculate $\beta$ across FP16, BF16, FP8, INT8, and INT4 weight-only configurations on a 1GHz NPU.

## 4. Experimental Matrix
- Precision: FP16, BF16, FP8, INT8, INT4 weight-only
- Expert size: 2MB (FP16/BF16)

## 5. Raw Results

### Quantization & Beta Sweeps

| Precision Setting | Expert Size (MB) | Transfer Cycles | Compute Cycles | Bottleneck Ratio $\beta$ | Primary Bottleneck |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **FP16 / BF16** | 2.00 | 1,375,000 | 102,400 | **13.43** | Memory-bound (Severe) |
| **FP8 / INT8** | 1.00 | 687,500 | 102,400 | **6.71** | Memory-bound |
| **INT4 weight-only**| 0.50 | 343,750 | 102,400 | **3.36** | Memory-bound (Moderate) |

## 6. Interpretation
1. **Persistent Memory Limit**: Compressing weights to INT4 reduces $\beta$ to 3.36, but the system remains memory-bound ($\beta > 1.0$) under DDR5-like (32 B/cycle) bandwidths. Weight transfer is still the primary limit.
2. **Compute-Bound Transition**: The system only becomes compute-bound ($\beta < 1.0$) if we transition to high-bandwidth memory (e.g. HBM3, >128 B/cycle), where INT4 weight transfer cycles drop to 85,900, shifting $\beta$ to 0.84. In this HBM + INT4 region, compute scaling becomes effective.

## 7. Failure Cases
If VRAM bandwidth is extremely low (<16 B/cycle), even INT4 weight-only precision is memory-bound ($\beta > 6.0$).

## 8. Threats to Validity
- **Decompression Logic**: On-chip decompression delay for INT4 adds compute cycles, which shifts $\beta$ down slightly.

## 9. What remains unresolved
- The exact area and latency cost of SystemVerilog INT4 decompression modules.
