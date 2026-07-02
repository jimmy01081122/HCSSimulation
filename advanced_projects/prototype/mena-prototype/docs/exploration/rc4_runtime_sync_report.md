# RC4: Runtime Synchronization Overhead Study

## 1. Observed Phenomenon
Host CPU register writes and synchronization handshakes add cycles to accelerator execution, potentially offsetting hardware latency savings.

## 2. Hypothesis
Host-accelerator MMIO synchronizations introduce latency overhead that is non-critical during long prefill compute phases, but becomes a primary bottleneck during sequential single-token decode phases.

## 3. Methodology
We model host-accelerator communication overhead (MMIO write, polling, interrupts, DMA setup) using a gem5 timing model and evaluate their cycle costs under different token batch sizes.

## 4. Experimental Matrix
- Comm Modes: MMIO Polling, Interrupt-driven
- Token Batch sizes: 1 (decode), 128 (prefill)

## 5. Raw Results

### Host-Accelerator Handshake Latency Breakdown

| Phase / Batch | Comm Mode | MMIO Setup (cycles) | Polling / ISR (cycles) | DMA Descriptor Setup | Total Sync Latency | Execution Cycles | Sync Overhead Share |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **Prefill (128)**| Polling | 200 | 500 | 500 | 1,200 | 25,600 | **4.5%** |
| **Prefill (128)**| Interrupt | 200 | 4,500 | 500 | 5,200 | 25,600 | **16.9%** |
| **Decode (1)** | Polling | 200 | 500 | 500 | 1,200 | 200 | **85.7%** |
| **Decode (1)** | Interrupt | 200 | 4,500 | 500 | 5,200 | 200 | **96.3%** |

## 6. Interpretation
1. **Decode Handshake Bottleneck**: During single-token decode, host synchronization (polling) takes 1,200 cycles while execution takes only 200 cycles, consuming **85.7%** of execution cycles. Interrupt-driven communication increases this share to **96.3%**.
2. **Prefill Negligibility**: During prefill, the overhead drops to 4.5% due to longer parallel compute phases, verifying that host overhead is only critical in the sequential decode loop.

## 7. Failure Cases
If the host operating system suffers from driver context-switch delays, interrupt-driven sync latency can exceed 15,000 cycles, completely stalling the decode pipeline.

## 8. Threats to Validity
- **Bus Speed**: Assumes a fast PCIe/AXI-lite connection. Slow interfaces will scale the synchronization overhead even further.

## 9. What remains unresolved
- The development of a hardware command queue (ring buffer) to batch decode requests and minimize host handshakes.
