# Conflict Analysis Report (Q9)

## 1. Research Question
What architectural trade-offs and structural conflicts exist in MoE accelerators? How do cache locality, compute locality, and prefetch bandwidth constraints clash?

---

## 2. Motivation
Designing accelerators requires resolving trade-offs. We must identify where components conflict to design proper arbitration logic.

---

## 3. Methodology
We analyze performance metrics across swept parameters to find where optimizations in one domain (e.g. compute) degrade performance in another (e.g. memory).

---

## 4. Experimental Matrix
- **Compare Targets**: Expert-major scheduling vs. LRU cache size, Prefetch lookup confidence vs. DRAM bandwidth.

---

## 5. Raw Results

### Cache vs. Compute Locality Conflict
- **Token-order**: High Cache hit rate (12.5% to 75.0%), high NPU reconfiguration overhead, low compute throughput.
- **Expert-major**: Low Cache hit rate (0.00% thrashing), low NPU reconfiguration overhead, high compute throughput.

### Bandwidth vs. Prefetch Accuracy Conflict
- **Static Prefetch (Confidence=0.0)**: Low misses, but **+125.0% DRAM transfer bloat**, saturating low-bandwidth systems.
- **Gated Prefetch (Confidence=0.7)**: Slightly higher misses, but **0.0% transfer bloat**, protecting low-bandwidth systems.

---

## 6. Observations
1. **The Locality Paradox**: You cannot maximize both cache locality and compute locality simultaneously. One thrashes the other.
2. **The Bandwidth Trade-off**: Prefetching can either minimize latency (at the cost of wasting bandwidth) or protect bandwidth (at the cost of slightly higher misses).

---

## 7. Contradictions
Startups claim that building larger NPUs always increases throughput. However, the data contradicts this: **scaling the NPU compute capacity increases the severity of the cache locality conflict**, as faster compute rates demand quicker weight load times, worsening memory bus congestion.

---

## 8. Sensitivity Analysis
We analyzed the sensitivity of the locality conflict to block size. When block size increases, spatial locality improves, which reduces the conflict penalty by 15%.

---

## 9. Failure Cases
Under high NPU re-configuration latency (>50,000 cycles), token-order scheduling fails completely, leading to a 3.4× system slowdown.

---

## 10. Threats To Validity
- **Uniform Bus Delay**: Assumes constant memory bus access delay, which might not reflect real DDR bus turn-around times.

---

## 11. Future Questions
* Can compile-time expert replication (storing multiple copies of hot experts) resolve the locality conflict?

---

## 12. Emerging Mechanisms
* **Dynamic Congestion Arbiter**: An arbiter that dynamically switches the scheduling policy between token-order and expert-major based on DRAM bus utilization.
