# MAEP: Agent Architecture Guide & Onboarding Manual
*Designed for AI coding agents to quickly comprehend the MoE Architecture Exploration Platform (MAEP).*

---

## 1. Project Overview & Paradigm Shift
This project is designated as **MAEP (MoE Architecture Exploration Platform)**, a research platform designed to explore and optimize the design space of Mixture-of-Experts (MoE) LLM inference accelerators.

### The Paradigm Shift:
* **Old Direction (MENA)**: A compute-centric accelerator that attempted to implement custom GEMM arrays, routing accelerators, and hardware controllers.
* **New Direction (SMTG / Sidecar)**: Based on rigorous sensitivity analysis, **custom NPU compute arrays are completely redundant** because compute latency is 100% hidden beneath memory weight transfer under double-buffered scheduling.
* **Current Focus**: The design space is restricted to a **Smart Memory Transfer Gateway (SMTG)** — a pure memory transfer gateway that intercepts DRAM transfers and schedules them to commodity NPUs.

---

## 2. The 8 Design Space Factors
Future agents must evaluate architectural mechanisms across these 8 independent factors:

1. **Compute**: Range from CPU to custom NPUs. (Compute is a secondary concern since it is easily hidden).
2. **Transfer**: Swept from serial reactive loading to predictive double/triple buffering.
3. **Cache**: Expert weight caches suffer from a step-function thrashing cliff under expert-major scheduling.
4. **Prediction**: Markov-1 table is highly cost-effective (4 KB SRAM for $E=64$), whereas Markov-2 table experiences an area explosion (16 MB SRAM for $E=256$).
5. **Scheduling**: Expert-major scheduling maximizes NPU compute locality but thrashes caches unless paired with active prefetching.
6. **Metadata Handling**: Hardwired tag/queue logic has 1500× lower synchronization overhead than soft-core (RISC-V) runtime processors.
7. **Expert Placement**: cached, streamed, or streamed on-demand.
8. **WABC (Weight-Activation Bandwidth Contention)**: *Newly discovered factor.* Shares memory bus bandwidth between weight transfers and token activations. If activation share exceeds 30%, double buffering speedups collapse.

---

## 3. Existing Simulation & Toolchain Infrastructure
When executing modifications or sweeps, locate these key instruments:

### Simulation:
* [**`sim/mena_sim.py`**](file:///home/a/prototype/mena-prototype/sim/mena_sim.py): Core event-driven simulator calculating queue depths, double buffering timeline, and cycles.
* [**`sim/expert_cache.py`**](file:///home/a/prototype/mena-prototype/sim/expert_cache.py): Implements LRU, LFU, and History (Markov-1) tag lookups and evictions.

### Execution Sweeps & Scripts:
* [**`scripts/dse_explorations_v3.py`**](file:///home/a/prototype/mena-prototype/scripts/dse_explorations_v3.py): Script conducting parametric scaling, real model trace replays, and failure mode sweeps.
* [**`scripts/calculate_predictor_cost.py`**](file:///home/a/prototype/mena-prototype/scripts/calculate_predictor_cost.py): Estimates TSMC 28nm SRAM area and access energy vs DRAM load energy.
* [**`scripts/build_discovery_reports.py`**](file:///home/a/prototype/mena-prototype/scripts/build_discovery_reports.py): Compiles the 13 discovery reports.

### Environment & Toolchain:
* [**`docker/Dockerfile.gem5`**](file:///home/a/prototype/mena-prototype/docker/Dockerfile.gem5): Ubuntu 22.04 base container containing scons and gem5 build dependencies.
* [**`docker-compose.yml`**](file:///home/a/prototype/mena-prototype/docker-compose.yml): Launches the isolated gem5-dev container mounting `/home/a/gem5-new` and the local prototype path.

---

## 4. Current Candidate Evaluation
Status of candidate architectures based on DSE sweeps:
* **Candidate A (SW)**: Weakly Rejected (sync latency of 15,000 cycles dominates).
* **Candidate B (MENA)**: Weakly Rejected (over-complex compute engine is hidden).
* **Candidate C (Smart DMA Sidecar)**: Supported (simplest and most cost-effective candidate).
* **Candidate D (Memory Processor)**: Weakly Supported (RISC-V core area cost is high).
* **Candidate E (Hybrid Sidecar)**: Supported (highest speedup of 3.80×, but high verification risk).
* **Candidate F (SMTG)**: Supported (discovered optimal hardwired gateway).

---

## 5. Next Steps for Future Agents
If continuing the development sprint, follow these directions:
1. **Implement Confidence Gating in RTL**: Complete `rtl/prefetch_gate.sv` ensuring prediction confidence check logic ($P \ge 0.7$) compiles warning-free under Verilator.
2. **Implement Weight-Priority DRAM Arbiter (WPDA)**: Design memory arbiter logic to prioritize weight transfers over activation writebacks to protect double-buffering efficiency from WABC.
3. **Zynq XC7Z020 FPGA timing check**: Place and route the SMTG design in Vivado, verifying BRAM utilization is under 15% and clock frequency reaches 150 MHz.
