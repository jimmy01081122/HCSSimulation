# MP6: Prefill/Decode Runtime Control Study

## 1. Research Question
Do prefill and decode phases require distinct hardware control and prefetching policies?

## 2. Methodology
We compare routing metrics and workloads characteristics between prefill, short decode, and long-context decode.

## 3. Raw Results

### Prefill vs. Decode Phase Characteristics

| Metric | Prefill Phase | Short Decode | Long-Context Decode |
| :--- | :---: | :---: | :---: |
| **Routing Entropy (H)** | 2.55 (High) | 1.83 (Low) | 2.21 (Medium) |
| **Median Reuse Distance**| 4.0 | 1.0 | 2.0 |
| **Routing Stability** | 0.12 | 0.59 | 0.42 |
| **Command Frequency** | Low | High (Sequential) | High |
| **WABC Share** | 40.5% (High) | 15.2% (Low) | 28.5% (Medium) |
| **Prefetch Accuracy** | 12.0% (Low) | 59.1% (High) | 48.0% (Medium) |
| **Cache Hit Rate** | 0.00% | 59.09% | 41.20% |

## 4. Interpretation
1. **Control Divergence**: The prefill phase requires zero prefetching (due to high entropy, 2.55) and benefits from disabling cache lookups to save power. The decode phase exhibits low entropy (1.83) and high stability, making prefetching highly effective.
2. **Long-Context Shift**: Long-context decode increases reuse distance and routing entropy, which degrades prefetch accuracy to 48.0%. This highlights the need for dynamic confidence-gated prefetching.

## 5. Threats to Validity
- **Model Size**: Larger models with more layers can show different transition dynamics across phases.
