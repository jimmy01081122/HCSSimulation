# RC7: Mechanism Confidence Reclassification

## 1. Observed Phenomenon
Hardware constraints (routing timing, memory bus arbitration, host interface latency) affect the effectiveness of MoE optimization mechanisms.

## 2. Hypothesis
We can reclassify all proposed MoE optimization mechanisms based on their timing, area, and systems-level feasibility under reality checks.

## 3. Methodology
We evaluate mechanisms under RC1-RC6 findings and assign confidence levels: Strongly Supported, Conditionally Supported, Weakly Supported, Inconclusive, Weakened, and Contradicted.

## 4. Experimental Matrix
- All mechanisms evaluated.

## 5. Raw Results

### Mechanism Confidence Classification

| Mechanism | Confidence Level | Validation Scope / Findings | Hardware Area / timing Cost |
| :--- | :--- | :--- | :--- |
| **Cache × Prefetch** | **Strongly Supported** | Critical for decode phases; recovers hit rates. | High timing path cost for large CAM arrays. |
| **Double Buffering** | **Conditionally Supported** | Balanced bandwidths ($0.5 \le \beta \le 2.0$). Fails in prefill. | SRAM area overhead (double buffers). |
| **WPDA Arbiter** | **Inconclusive** | Starvation risk for token activation queues. | Shared memory bus controller complexity. |
| **Adaptive Batching** | **Weakly Supported** | Requires runtime software coordination. | Host sync latency overhead. |
| **Standard LRU Cache** | **Weakened / Thrashing**| Hit rates drop to 0% under expert-major order without prefetching. | Small tag registers. |

## 6. Interpretation
1. **Prefetch-Cache Synergy**: This remains the most robust optimization mechanism. However, CAM timing constraints force a transition to set-associative tag arrays using BRAM instead of fully associative CAM.
2. **WPDA Starvation Risk**: Reclassified from Key Mechanism to Inconclusive due to the starvation risk on shared buses, highlighting the need for QoS-based arbiters.

## 7. Failure Cases
Large-scale caches (>64 entries) using fully associative CAM fail physical timing closure, rendering the CAM cache design invalid.

## 8. Threats to Validity
- ASIC implementations could mitigate CAM delays, altering the timing boundaries.

## 9. What remains unresolved
- Simulating a QoS arbiter to resolve the WPDA starvation risk.
