# MAEP: Mixture-of-Experts Architecture Exploration Platform
## Discovered Architecture: Smart Memory Transfer Gateway (SMTG)

This document presents the final architectural synthesis and selection of the MoE Architecture Exploration Platform (MAEP) based on the Factorial Design Space Exploration.

---

## 1. Discovered Architecture: SMTG
We officially define and select a new architecture: **Smart Memory Transfer Gateway (SMTG)**.
* SMTG is a hardware-efficient memory interface gateway consisting *only* of:
  1. **Expert Cache Controller** (lightweight tag array)
  2. **Confidence-gated Prefetcher** (Markov-1 predictor + threshold gate)
  3. **Double Buffer Manager** (ping-pong weight buffer control)
  4. **Expert Queue Scheduler** (hardwired expert-major scheduler)
* SMTG contains **no computation units (NPU)** and **no programmable CPU core (RISC-V)**. It interfaces directly with commodity NPUs.

---

## 2. Answers to the Core Redesign Questions

### 1. 哪些元件必須存在？ (Which components must exist?)
* **Cache, Prefetch, Double Buffer, and Scheduler**.
  * These four components are highly synergistic. The scheduler groups tokens for compute locality; the cache stores active weights; the prefetcher breaks cache thrashing; the double buffer overlaps transfer and execution. Together, they achieve **2.79× system speedup** while consuming only **0.22 mm² of silicon area** (7.5% of PYNQ-Z2 LUTs).

### 2. 哪些元件不應存在？ (Which components should not exist?)
* **Compute Engine (NPU GEMM Arrays) and Metadata Processor (RISC-V core)**.
  * The NPU compute latency is 100% hidden by double buffering, making custom GEMM accelerators redundant.
  * The hardwired scheduler performs all coordination, making a programmable CPU core an unnecessary complexity and area overhead (saving only 1.7% latency for a 2.4× increase in verification risk).

### 3. 哪些元件僅在特定 workload 下有價值？ (Which components are only valuable under specific workloads?)
* **Prefetcher (Markov-1 Predictor)**.
  * The prefetcher is highly valuable for skewed (Type A/B) workloads, restoring cache hit rates to 59.09%. However, under random (Type C) workloads, it must be gated by a **Confidence Threshold ($\ge 0.7$)** to prevent a 93% DRAM bandwidth bloat.

---

## 3. Detailed Factorial DSE Reports (11-Section Format)
1. [**因子分析報告** (`factor_analysis_report.md`)](file:///home/a/prototype/mena-prototype/docs/exploration/factor_analysis_report.md)
2. [**元件成本與性價比報告** (`component_contribution_report.md`)](file:///home/a/prototype/mena-prototype/docs/exploration/component_contribution_report.md)
3. [**跨元件交互作用報告** (`interaction_effect_report.md`)](file:///home/a/prototype/mena-prototype/docs/exploration/interaction_effect_report.md)
4. [**架構發現與 SMTG 規格報告** (`architecture_discovery_report.md`)](file:///home/a/prototype/mena-prototype/docs/exploration/architecture_discovery_report.md)

---

## 4. Next 6-Month RTL Implementation Plan
Based on the SMTG selection, we focus 100% of our future RTL and FPGA resources on:
* **Confidence-based Prefetch Gater RTL**: Implementing the Markov-1 lookup and dynamic confidence gating state machine.
* **AXI DMA Double Buffer Handshake RTL**: Implementing the ping-pong control signals between DRAM weight transfers and the virtual NPU weight buffer.
