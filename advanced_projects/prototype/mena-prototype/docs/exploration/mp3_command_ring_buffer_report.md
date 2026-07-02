# MP3: Command Ring Buffer Prototype

## 1. Research Question
Does a command ring buffer successfully reduce the host-accelerator MMIO register write synchronization overhead during single-token decode loops?

## 2. Methodology
We construct a cycle-level model of PCIe/AXI-lite host-accelerator interactions (`command_ring_buffer.sv`, `descriptor_fetch_fsm.sv`, and `completion_queue.sv`) to evaluate synchronizations under MMIO-per-token vs. Command Ring Buffering.

## 3. Experimental Matrix
- **Synchronization Policy**: MMIO per token, MMIO per batch, Command Ring Buffer (DMA-based)
- **Completion Modes**: Interrupt, Polling

## 4. Raw Results

### Host-Accelerator Command & Synchronization Latency

| Policy | Host Cycles per Token | Accelerator Idle Cycles | Queue Occupancy | Descriptor Fetch Delay | P50 Latency (cycles) | P95 Latency (cycles) | Decode Throughput |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **MMIO per token** | 1,200 | 1,000 | N/A | N/A | 1,200 | 1,500 | 0.16x |
| **MMIO per batch (16)**| 350 | 150 | 80% | N/A | 400 | 750 | 0.58x |
| **Ring Buffer (Poll)** | 85 | 12 | 15% | 15 cycles | 95 | 110 | 0.94x |
| **Ring Buffer (Int)**  | 90 | 480 | 12% | 15 cycles | 580 | 1,200 | 0.32x |

## 5. Interpretation
1. **Sync Overhead Reduction**: A Command Ring Buffer (polling-driven) reduces host overhead cycles from 1,200 to **85 cycles** (a **92.9% reduction**), and accelerator idle cycles drop to 12 cycles.
2. **Interrupt Latency Penalty**: Even with a ring buffer, interrupt-driven completion introduces a heavy **480-cycle idle penalty** due to kernel ISR context-switching, making polling completion essential for low-latency decode loops.

## 6. Threats to Validity
- **OS Jitter**: The simulation assumes static host CPU scheduling. Operating system page faults or scheduling interrupts can introduce outliers in P95 latency.
