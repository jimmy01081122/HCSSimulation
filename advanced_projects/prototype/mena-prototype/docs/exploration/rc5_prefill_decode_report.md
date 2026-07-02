# RC5: Prefill vs Decode Robustness Study

## 1. Observed Phenomenon
The prompt prefill phase and token decode phase exhibit different routing statistics, temporal reuse distances, and memory bottlenecks.

## 2. Hypothesis
The prefill phase is compute-bound, meaning caching and prefetching are non-critical. The decode phase is memory-bound, meaning caching and prefetching are critical.

## 3. Methodology
We characterize and compare prefill vs. decode parameters across Mixtral E8 routing sequences.

## 4. Experimental Matrix
- Phases: Prefill vs. Decode
- Model: Mixtral E8

## 5. Raw Results

### Phase Parameters Comparison

| Phase | Avg Entropy | RD p50 | Cache Hit Rate | WABC Share | Transfer/Compute $\beta$ | Primary Bottleneck |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **Prefill** | 2.55 | 4.0 | 0.00% | 40.5% | **0.25** | Compute-bound / WABC |
| **Decode** | 1.83 | 1.0 | 59.09% | 15.2% | **3.36** | Memory-bound (Weight) |

## 6. Interpretation
1. **Bottleneck Shift**: The bottleneck ratio $\beta$ shifts from **0.25 (compute-bound)** in prefill to **3.36 (memory-bound)** in decode. This confirms that memory transfers limit decode, while compute efficiency limits prefill.
2. **Mechanism Scope**: Cache and prefetcher validation are highly robust for decode phases, but show negligible benefit in prefill.

## 7. Failure Cases
Under very short context lengths, the prefill phase is too short to show compute-bound characteristics.

## 8. Threats to Validity
- **Workload variation**: Different prompt lengths change the prefill compute duration.

## 9. What remains unresolved
- Dynamic hardware bypass of cache tag checks during prefill phases to save power.
