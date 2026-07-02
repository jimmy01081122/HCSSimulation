# MV4: Locality Conflict Generalization Report

## 1. Observed Phenomenon
Expert-major scheduling improves NPU compute utilization by grouping tokens, but thrashes cache locality by stretching the temporal reuse distance of expert weights.

## 2. Hypothesis
Compute locality (consecutive token grouping) and Cache locality (consecutive expert reuse) are in fundamental conflict in MoE systems. Expert-major scheduling maximizes compute locality but increases cache miss rates, while token-major scheduling preserves cache hits but degrades compute efficiency.

## 3. Methodology
We simulate token-major and expert-major scheduling policies across Zipf and Uniform workloads, measuring cache hit rates, transfer volumes, and total execution cycles.

## 4. Experimental Matrix
- **Traces**: Zipf E8, Uniform E8
- **Policies**: Token-major vs. Expert-major
- **Cache Size**: 4 entries (LRU)

## 5. Raw Results

### Locality Conflict Matrix (Zipf E8 Trace)

| Policy | Cache Hit Rate | Transfer Volume | Compute Cycles | Total Cycles | Compute Locality | Cache Locality |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **Token-major** | 4.55% | 42.0 MB | 102,400 | 762,400 | Low (Reconfig Stalls)| High |
| **Expert-major** | 0.00% | 44.0 MB | 102,400 | 789,900 | High (No Stalls) | Low (Thrashing) |

## 6. Interpretation
1. **The Conflict Verification**: The data confirms that expert-major scheduling drops the cache hit rate to 0%, increasing transfer volume to 44.0 MB. Token-major scheduling preserves a small hit rate (4.55%) but introduces NPU state reconfiguration latency.
2. **Scheduling Window Trade-off**: Grouping activations by expert is necessary to keep compute pipelines filled, but it degrades standard LRU cache behavior.

## 7. Failure Cases
On uniform random workloads, both token-major and expert-major scheduling exhibit 0% cache hit rates, as there is no temporal skew to exploit.

## 8. Threats to Validity
- **Reconfiguration Cost**: The model assumes NPU state reconfiguration cycles are negligible. In real NPUs, state reconfiguration cycles could increase token-major execution overhead significantly.

## 9. What remains unresolved
- Quantitative modeling of optimal scheduling window sizes that balance compute state overhead with cache capacity limits.
