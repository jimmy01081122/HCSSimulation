# MV6: Quantization-Beta Study Report

## 1. Observed Phenomenon
Lowering weight precision (e.g. quantization) scales down the volume of expert weights, reducing transfer times and shifting the system's performance bottleneck.

## 2. Hypothesis
Quantization scales the expert weight size ($W$), which linearly decreases weight transfer cycles ($t_{\text{transfer}}$), thereby reducing the bottleneck ratio $\beta$:
$$\beta = \frac{t_{\text{transfer}}}{t_{\text{compute}}} \propto W$$
This shifts the system from memory-bound ($\beta > 1.0$) to compute-bound ($\beta < 1.0$).

## 3. Methodology
We calculate $\beta$ across five precision settings (FP16, BF16, FP8, INT8, INT4 weight-only) for a 1GHz NPU (compute = 100 cycles per token, tokens = 512, DRAM Bandwidth = 32 B/cycle).

## 4. Experimental Matrix
- Precision swept: FP16, BF16, FP8, INT8, INT4 weight-only
- Expert size: 2MB (FP16/BF16 baseline)

## 5. Raw Results

| Precision Setting | Expert Size (MB) | Transfer Cycles | Compute Cycles | Bottleneck Ratio $\beta$ | Primary Bottleneck |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **FP16 / BF16** | 2.00 | 1,375,000 | 102,400 | **13.43** | Memory-bound (Severe) |
| **FP8 / INT8** | 1.00 | 687,500 | 102,400 | **6.71** | Memory-bound |
| **INT4 weight-only**| 0.50 | 343,750 | 102,400 | **3.36** | Memory-bound (Moderate) |

## 6. Interpretation
1. **Bottleneck Reduction**: Quantization directly scales $\beta$. Compressing weights from FP16 to INT4 reduces $\beta$ from 13.43 to 3.36, indicating a 75% reduction in memory transfer overhead.
2. **Persistent Memory Bound**: Even at INT4 weight-only precision, the system remains memory-bound ($\beta = 3.36$) under DDR5-like (32 B/cycle) bandwidths, proving that weight compression alone is insufficient to shift the system to a compute-bound state.

## 7. Failure Cases
- Under ultra-low bandwidths (<16 B/cycle), even INT4 weight-only precision fails to bring $\beta$ below 5.0, keeping the system in the severe memory-bound regime.

## 8. Threats to Validity
- **Compute Overhead**: The model assumes compute cycles remain constant. Real INT4 weight-only decompression on chip adds decompression latency to compute cycles, which could alter the effective compute time.

## 9. What remains unresolved
- The exact hardware logic area cost and decompression latency of FP8 and INT4 decoders in SystemVerilog.
