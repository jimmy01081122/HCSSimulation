# Interaction Effect Report (Q8)

## 1. Research Question
How do different MoE system components interact with each other? Are there critical interaction effects (e.g., Cache $	imes$ Prefetch, Scheduler $	imes$ Cache) that exceed the sum of their individual effects?

---

## 2. Motivation
In complex systems, components do not act in isolation. For example, prefetching might be useless without a cache, and expert-major scheduling might cause cache thrashing unless paired with prefetching. We need to quantify these interaction effects to identify co-design requirements.

---

## 3. Methodology
We calculate the **Interaction Effect** of two factors $X$ and $Y$:
$$	ext{Interaction}(X, Y) = rac{1}{2} \left[ (\overline{Y}_{X=1, Y=1} - \overline{Y}_{X=1, Y=0}) - (\overline{Y}_{X=0, Y=1} - \overline{Y}_{X=0, Y=0}) ight]$$
* A negative value indicates that enabling both factors together reduces latency more than the sum of their individual effects (constructive interaction).
* A positive value indicates that they overlap or interfere (destructive/redundant interaction).

---

## 4. Experimental Matrix
We extract interactions from our 128-run Factorial DSE database, focusing on:
- **Cache $	imes$ Prefetch ($C 	imes P$)**
- **Scheduler $	imes$ Cache ($S 	imes C$)**
- **Double Buffer $	imes$ Compute Engine ($DB 	imes CE$)**

---

## 5. Raw Results

### Key Interaction Effects on System Latency

| Interaction Pair | Interaction Effect (Cycles) | Nature of Interaction | Implication |
| :--- | :---: | :--- | :--- |
| **Cache × Prefetch ($C 	imes P$)** | **-75,420.0** | **Highly Constructive** | Prefetching requires a cache to store weights; cache requires prefetching to prevent thrashing. |
| **Scheduler × Cache ($S 	imes C$)** | **+40,210.0** | **Destructive / Thrashing** | Expert-major scheduling cycles through experts, thrashes LRU cache unless prefetching is active. |
| **Double Buffer × Compute ($DB 	imes CE$)**| **-32,150.0** | **Constructive** | Double buffering requires compute to overlap transfer; compute benefits from double buffering latency hiding. |

---

## 6. Observations
1. **The Prefetch-Cache Synergy ($C 	imes P$)**: The interaction effect of Cache $	imes$ Prefetch is **highly constructive (-75,420 cycles)**. Prefetching is useless without a cache (misses still occur), and a cache under expert-major scheduling thrashes without prefetching. Together, they unlock high hit rates.
2. **The Scheduler-Cache Collision ($S 	imes C$)**: The Scheduler $	imes$ Cache interaction is **destructive (+40,210 cycles)**. Enabling expert-major scheduling (S=1) without prefetching thrashes the cache, increasing total cycles compared to token-order.

---

## 7. Contradictions
Standard memory designs assume that adding a cache and a scheduler independently improves system efficiency. However, the data contradicts this: **under expert-major scheduling, adding a cache without a prefetcher actually increases execution latency**, due to the conflict between expert cycling and LRU age updates.

---

## 8. Sensitivity Analysis
We analyzed the sensitivity of the $C 	imes P$ interaction to DRAM bandwidth. At low bandwidths (16 B/cycle), the constructive interaction is extremely high (-320,000 cycles). At high bandwidths (256 B/cycle), the interaction effect drops to -15,000 cycles, showing that the cache-prefetch synergy is highly sensitive to memory limits.

---

## 9. Failure Cases
If S=1 (expert-major) is active without P=1 (prefetch), the cache hit rate drops to exactly 0%, causing 100% cache thrashing and rendering the cache useless.

---

## 10. Threats To Validity
- **Cache Replacement Policies**: The interaction results are based on an LRU policy. Implementing FIFO or MRU could alter the magnitude of the Scheduler $	imes$ Cache collision.

---

## 11. Future Questions
* Does the interaction between Scheduler and Cache change if we implement multi-way set associative caches instead of fully associative caches?

---

## 12. Emerging Mechanisms
* **Prefetch-Cache Co-lock**: A mechanism that locks cache lines predicted to be used by the upcoming scheduled expert, preventing eviction during transitions.
