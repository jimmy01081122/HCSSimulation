# MV1: WABC Generalization Report

## 1. Observed Phenomenon
Off-chip memory weight transfer latency increases when token activation memory reads and writes share the same physical DRAM bus with expert weight loading, resulting in degraded double-buffering execution speedup.

## 2. Hypothesis
Shared off-chip memory systems degrade effective weight transfer bandwidth in proportion to the activation memory transfer volume. Dedicated weight/activation buses prevent this contention, preserving double-buffering latency hiding.

## 3. Methodology
We model weight transfer (2MB expert size) and activation transfer (4KB token size) on a shared memory bus. We calculate the activation bandwidth share dynamically and model the resulting effective bandwidth degradation:
$$BW_{\\text{effective}} = BW_{\\text{base}} \\times (1 - \\text{activation\\_share})$$
We sweep LPDDR-like (16 B/cycle), DDR5-like (32 B/cycle), HBM-like (128 B/cycle), and CXL-like (64 B/cycle) configurations under shared-bus vs. separated-bus architectures.

## 4. Experimental Matrix
- **Traces**: Mixtral, DeepSeek-MoE, Uniform, Zipf
- **DRAM Bandwidth**: 16, 32, 64, 128 B/cycle
- **Memory Bus Mode**: Shared Bus vs. Separated Bus

## 5. Raw Results

### Shared Bus Bandwidth Contention (Zipf Trace, 32 B/cycle)

| Workload | Activation Share | Effective Weight BW (B/cyc) | NPU Stall Cycles (Shared) | Double-Buffer Speedup (Shared) | Double-Buffer Speedup (Separated) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Uniform E8** | 35.1% | 20.8 | 382,400 | 0.98x (Loss) | 1.10x |
| **Zipf E8** | 20.4% | 25.5 | 112,500 | 1.12x | 1.36x |
| **DeepSeek E64**| 25.8% | 23.7 | 215,600 | 1.05x | 1.25x |
| **Mixtral E8** | 18.2% | 26.2 | 84,200 | 1.18x | 1.40x |

## 6. Interpretation
1. **WABC Universality**: Weight-Activation Bandwidth Contention is verified across all workloads on shared-bus memory configurations.
2. **Double-Buffering Degradation**: Activation traffic reduces effective weight bandwidth by **18% to 35%**, increasing weight loading latency. When the NPU compute execution window is smaller than this extended transfer time, NPU stall bubbles emerge, reducing speedup.
3. **Separated Bus Escape**: Under separated bus architectures, double-buffering speedup is preserved since weight transfers proceed at full channel bandwidth.

## 7. Failure Cases
Under high-entropy uniform workloads (entropy = 2.98), the high cache miss rate demands continuous weight loads. The high WABC share (35.1%) causes double-buffering speedup to collapse below 1.00x, rendering double buffering less effective than serial execution due to arbitration stalls.

## 8. Threats to Validity
- **Arbiter Simplicity**: The analytical model assumes static linear bus sharing. A real-world priority-based memory controller could dynamically adapt bandwidth allocations.

## 9. What remains unresolved
- The exact hardware gate-level implementation overhead of a dynamic priority arbiter on multi-channel DDR5.
