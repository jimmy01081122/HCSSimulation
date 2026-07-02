# MP5: Quantization-Beta Recheck

## 1. Research Question
How do weight precision scaling (quantization) and memory system upgrades (DDR5 vs. HBM) jointly impact the memory-bound state of MoE execution phases?

## 2. Sweep Space
- **Precision**: FP16 (2B/param), BF16 (2B), FP8 (1B), INT8 (1B), INT4 weight-only (0.5B)
- **Memory Bandwidth**: DDR5-like (32 B/cycle), HBM-like (128 B/cycle)
- **Execution Phase**: Prefill, Decode, Long-Context Decode

## 3. Raw Results

### Quantization & Memory Beta Sweeps

| Phase | Memory System | Precision | Expert Size (MB) | Activation Size (KB) | Transfer Cycles | Compute Cycles | Bottleneck Ratio $\beta$ | Bottleneck Class |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :--- |
| **Decode** | DDR5-like | FP16 | 2.00 | 4.0 | 1,375,000 | 102,400 | **13.43** | Memory-bound (Severe) |
| **Decode** | DDR5-like | INT8 | 1.00 | 4.0 | 687,500 | 102,400 | **6.71** | Memory-bound |
| **Decode** | DDR5-like | INT4 | 0.50 | 4.0 | 343,750 | 102,400 | **3.36** | Memory-bound |
| **Decode** | HBM-like | FP16 | 2.00 | 4.0 | 343,750 | 102,400 | **3.36** | Memory-bound |
| **Decode** | HBM-like | INT4 | 0.50 | 4.0 | 85,937 | 102,400 | **0.84** | **Compute-bound** |
| **Prefill**| DDR5-like | FP16 | 2.00 | 512.0 | 25,600 | 102,400 | **0.25** | Compute-bound |

## 4. Interpretation
1. **DDR5 Memory Bound Resilience**: Under DDR5 memory systems, the single-token decode loop remains memory-bound ($\beta = 3.36$) even at INT4 weight-only precision. Weight loading overhead is still the dominant performance limiter.
2. **Compute-Bound Shift**: The system shifts to a compute-bound state ($\beta = 0.84 < 1.0$) only when HBM-like memory (128 B/cycle) is paired with INT4 weight quantization. In this HBM + INT4 regime, scaling compute units directly improves system performance.

## 5. Threats to Validity
- **Decompression Overhead**: INT4 on-chip decompression adds pipeline stages, which increases compute latency and shifts $\beta$ down slightly further.
