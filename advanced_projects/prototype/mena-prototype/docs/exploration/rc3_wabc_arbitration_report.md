# RC3: WABC Cycle-Level Memory Arbitration Study

## 1. Observed Phenomenon
Shared memory buses suffer from WABC conflicts where activation writebacks block weight load requests, degrading double-buffering latency overlap.

## 2. Hypothesis
A prioritized memory controller arbitration policy (e.g. Weight-Priority) can mitigate double-buffering degradation by prioritizing weight requests during prefetch phases, compared to standard Round-Robin policies.

## 3. Methodology
We simulate cycle-level DRAM arbitration under five scheduling/arbitration policies (Round-Robin, FCFS, Weight-Priority, Activation-Priority, and QoS threshold policy) and measure the resulting NPU stall bubbles and overlap efficiency.

## 4. Experimental Matrix
- Memory model: Shared DRAM bus, DDR5-like (32 B/cycle)
- Arbitration policies swept: Round-Robin, FCFS, Weight-Priority, Activation-Priority, QoS threshold

## 5. Raw Results

### Bus Arbitration Performance (Zipf Trace, 32 B/cycle)

| Arbitration Policy | Effective Weight BW | Weight latency (cycles) | NPU Stall Bubbles | Starvation Risk | Overlap Efficiency |
| :--- | :---: | :---: | :---: | :--- | :---: |
| **Round-Robin** | 25.5 B/cyc | 351,562 | 112,500 | Low | 78.5% |
| **FCFS** | 24.1 B/cyc | 373,400 | 134,300 | Low | 73.1% |
| **Weight-Priority** | 30.2 B/cyc | 298,000 | 48,000 | **High (Activation Queue)** | 92.4% |
| **Activation-Priority**| 20.8 B/cyc | 432,600 | 193,700 | Low | 62.1% |
| **QoS Threshold** | 29.5 B/cyc | 305,000 | 55,000 | Low | 90.8% |

## 6. Interpretation
1. **Arbitration Impact**: Round-Robin degrades effective weight bandwidth to 25.5 B/cycle due to constant interleaving. Implementing Weight-Priority arbitration increases effective bandwidth to 30.2 B/cycle and reduces NPU stalls to 48,000 cycles.
2. **Starvation Risk**: Weight-Priority arbitration introduces a high risk of activation queue starvation, as activation writebacks are continuously delayed during long sequences. A QoS Threshold policy (balancing priority dynamically) achieves high efficiency (90.8%) while preventing starvation.

## 7. Failure Cases
Under long context lengths (>2048 tokens), Weight-Priority arbitration causes the activation FIFO to overflow, triggering a system-level backpressure stall.

## 8. Threats to Validity
- **Trace Length**: Simulated traces are short. Long-term memory controller traffic might show different queuing dynamics.

## 9. What remains unresolved
- The hardware implementation complexity of a QoS-based arbiter in a standard DDR5 controller.
