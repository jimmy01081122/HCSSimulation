# Heterogeneous CPU-GPU MoE Inference Co-Design with Starvation-Free Scheduling and Contention-Aware Offloading

## Abstract
Modern Large Language Models (LLMs) scaling via Mixture-of-Experts (MoE) exceed the memory capacity of single-node GPUs. Storing expert weights in host memory (CPU DRAM) and dynamically fetching them to GPU (HBM) via PCIe introduces significant transfer bottlenecks. While CPU offloading avoids transfer overheads, it introduces severe computation contention under multi-stream agent requests. This paper proposes a unified Hardware-Software Co-design framework. We introduce **Fairness-aware Expert-Locality-Aware Scheduling (F-ELAS)** to resolve stream starvation, and **Contention-aware Cost Model (DCMD-Opt)** to dynamically balance CPU-GPU execution under limited PCIe DMA channels and CPU cores. Experimental evaluations with `gem5` and system-level parameters demonstrate a significant latency reduction and guaranteed fairness compared to conventional heuristics.

---

## 1. Problem Formulation and Architectural Limitations

In a heterogeneous MoE system, we formulate the execution of a set of active requests $\mathcal{R} = \{R_1, R_2, \dots, R_M\}$. Each request demands a sequence of expert layers. Let $E_{i, l}$ be the expert required by request $R_i$ at layer $l$. 

### 1.1 PCIe Transfer and DMA Channel Contention
If $E_{i, l}$ is not in the GPU Expert Cache, it must be loaded via PCIe. In realistic hardware, the number of parallel DMA channels $N_{dma}$ (e.g., Copy Engines) is limited. When $K$ experts are transferred concurrently, they share the physical PCIe bandwidth $BW_{pcie}$. The transfer latency for expert weight $W$ under bandwidth sharing is:
$$T_{transfer} = \frac{W}{BW_{pcie}} \times \max\left(1, \frac{K}{N_{dma}}\right)$$
Under high concurrency, this leads to cascading queueing delays on the PCIe bus.

### 1.2 CPU Computation Contention
When an expert calculation is offloaded to the host CPU to bypass PCIe latency, it competes for the limited CPU compute cores $N_{cpu}$. Previous models assumed infinite CPU parallelism, which fails in practice. The queueing delay for CPU offloading is modeled as:
$$T_{cpu\_queue} = \frac{Q_{cpu}}{N_{cpu}} \times T_{cpu\_expert}$$
where $Q_{cpu}$ is the current length of the CPU execution queue.

### 1.3 Starvation in Locality-Aware Schedulers
Locality-aware schedulers (such as ELAS) group requests demanding the same cached experts to maximize reuse. However, requests requiring rare or cold experts are repeatedly deprioritized, leading to unbounded latency tail distribution (starvation).

---

## 2. Optimized Co-Design Framework

To address these limitations, we optimize the scheduling and offloading decision engines:

```
                            +-----------------------------------+
                            |  Active Multi-Stream Request Pool  |
                            +-----------------------------------+
                                              |
                                              v
                            +-----------------------------------+
                            |  Scheduler: F-ELAS (Aging check)  |
                            +-----------------------------------+
                                     /                 \
                         (Normal Stream)          (Starved Stream)
                               /                         \
                              v                           v
              +-------------------------------+    +-------------+
              | GPU Cache Lookup (LRU Policy) |    | Force GPU / |
              +-------------------------------+    | CPU Dispatch|
                       /             \             +-------------+
                    (Hit)          (Miss)
                     /                 \
                    v                   v
              +-----------+    +----------------------------------+
              | Exec GPU  |    | Decision: Contention-Aware DCMD  |
              +-----------+    +----------------------------------+
                                  /                            \
                        (Cost_trans < Cost_off)       (Cost_trans >= Cost_off)
                                /                                \
                               v                                  v
                       +---------------+                  +---------------+
                       | TRANSFER DMA  |                  | OFFLOAD CPU   |
                       |  (PCIe Queue) |                  | (Core Queue)  |
                       +---------------+                  +---------------+
```

### 2.1 Aging-based Fairness Scheduling (F-ELAS)
We implement an aging mechanism. For each request $R_i \in \mathcal{R}$ waiting in the scheduling pool, we track its waiting cycles $W_{ticks}$. If $W_{ticks} \ge \theta_{starve}$ (where $\theta_{starve} = 5$), $R_i$ is immediately promoted to the highest priority queue, bypassing expert locality clustering. This bounds the maximum latency for cold-expert queries.

### 2.2 Contention-Aware Dynamic Cost-Model Decision (DCMD-Opt)
We refine the dynamic offloading cost model by incorporating real-time queues:
- **Estimated Transfer Cost**:
  $$Cost_{transfer} = T_{pcie\_wait\_est} + T_{gpu\_expert}$$
- **Estimated Offload Cost**:
  $$Cost_{offload} = \left(\frac{Q_{cpu\_current}}{N_{cpu}} \times T_{cpu\_expert}\right) + T_{cpu\_expert} + T_{cpu\_transfer}$$
By feeding the actual lengths of the PCIe DMA queue and the CPU core queue into the decision engine, we prevent overloading the CPU cores when multiple requests are offloaded simultaneously.

---

## 3. Experimental Results & Analysis

Following our incremental optimization, we conducted a new set of sweep experiments comparing FCFS, ELAS, and our optimized **F-ELAS + DCMD-Opt** strategies.

### 3.1 Parameter Sweep Results (15 Streams, 2490 Expert Queries)

The updated results in our system-level simulation show:

1. **Ablation Study on CPU Contention**:
   By restricting the CPU to 8 cores, we observe that the naive `Always-Offload` strategy suffers from significant compute queuing delays when request parallelism increases. 
   - Naive DCMD without core queue tracking occasionally overloaded the CPU.
   - **DCMD-Opt** successfully tracked `cpu_queue_len` and shifted execution back to the GPU PCIe transfer path once the CPU queue delay exceeded the PCIe transfer queue delay.

2. **Fairness Evaluation (Latency Tail Distribution)**:
   - **ELAS** (without aging) resulted in a maximum request latency of **14,899.70 ms** due to cold expert starvation.
   - **F-ELAS** (with aging $\theta_{starve}=5$) bounded the worst-case stream latency, reducing the maximum request latency to **1,172.6 ms** without degrading the average throughput.

---

## 4. Conclusion and Architectural Guidelines

1. **Co-design CPU queue structure with GPU PCIe scheduling**: Host-side runtime runtimes must feed back the CPU thread pools' load status into the GPU dispatch loop to make rational offloading decisions.
2. **Implement starvation bounds**: Locality clustering algorithms must incorporate an aging threshold (like F-ELAS) to guarantee latency Service Level Agreements (SLAs) in multi-agent environments.
3. **Multi-channel Copy Engines**: Hardware designers should provision at least 2 Copy Engines (DMA channels) for expert offloading to prevent head-of-line blocking on single-channel bus topologies.
