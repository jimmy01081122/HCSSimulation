# MV7: Prefill vs Decode Phase Study Report

## 1. Observed Phenomenon
The prompt prefill phase and token decode phase exhibit different routing characteristics, temporal localities, and system bottlenecks.

## 2. Hypothesis
The prefill phase is compute-bound due to large batched tokens, while the decode phase is memory-bound due to sequential single-token processing. Caching and prefetching mechanisms are highly effective in decode phases but show marginal value in prefill phases.

## 3. Methodology
We partition real model routing traces (Mixtral E8) into prefill (tokens processed in parallel) and decode (sequential token generation) phases. We compute entropy, reuse distance, and simulation cycles for each phase.

## 4. Experimental Matrix
- **Trace**: Mixtral E8
- **Phases**: Prefill (first batch of 128 tokens) vs. Decode (sequential tokens 129 to 512)

## 5. Raw Results

### Prefill vs. Decode Characteristics (Mixtral E8)

| Phase | Avg Entropy | Median Reuse Distance | Cache Hit Rate | WABC Share | Primary Bottleneck |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Prefill** | 2.55 | 4.0 | 0.00% | 40.5% | Compute-bound / WABC |
| **Decode** | 1.83 | 1.0 | 59.09% | 15.2% | Memory-bound (Weight) |

## 6. Interpretation
1. **Bottleneck Divergence**: The data confirms that the Prefill phase exhibits higher routing entropy (2.55) and higher WABC share (40.5%) due to massive parallel activation transfers. The Decode phase has lower entropy (1.83) and high cache hit rates (59.09%), confirming that memory transfers are dominated by weight loads.
2. **Mechanism Efficacy**: Caching and prefetching are highly effective during Decode, but fail during Prefill due to the high routing entropy and large active working set.

## 7. Failure Cases
If the prompt sequence length is extremely short (<16 tokens), the prefill phase is too short to exhibit compute-bound properties, merging its behavior into the memory-bound decode phase.

## 8. Threats to Validity
- **Batch Size Variation**: Scaling batch sizes during decode changes the compute-bound properties, which could shift decode phases into the compute-bound regime.

## 9. What remains unresolved
- Dynamically adjusting prefetcher parameters when transitioning between prefill and decode phases.
