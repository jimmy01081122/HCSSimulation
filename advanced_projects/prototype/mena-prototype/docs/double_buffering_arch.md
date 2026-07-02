# MENA Double-Buffering Architecture

> **Scope**: Hardware micro-architecture reference for the Mixture-of-Experts
> Neural Accelerator (MENA) weight-streaming engine.  Covers system-level
> block diagram, timing, register map, RTL roadmap, and key efficiency
> metrics derived from the Python prototype simulation.

---

## 1. System Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────────┐
│                          MENA Accelerator SoC Tile                      │
│                                                                         │
│  ┌──────────────────┐      ┌────────────────────┐                       │
│  │  Routing         │      │  Expert Cache       │  ← tag array (SRAM)  │
│  │  Predictor       │─────▶│  (4–16 entries)     │  ← data SRAM (BRAMs) │
│  │  (Zipf / History)│      └────────┬───────────┘                       │
│  └──────────────────┘               │  cache hit/miss                   │
│                                     ▼                                   │
│  ┌──────────────────────────────────────────────────────────────────┐   │
│  │                   Expert Queue Scheduler                          │   │
│  │   priority queue → decides which expert weights to prefetch next  │   │
│  └──────┬────────────────────────────────────┬────────────────────┘   │
│         │  DMA kick (on miss)                 │  NPU dispatch          │
│         ▼                                     ▼                        │
│  ┌──────────────┐    AXI-stream       ┌──────────────────────────────┐ │
│  │  DMA Engine  │◀══════════════════▶│     NPU (MoE Compute Core)   │ │
│  │  (DRAM r/w)  │    weight chunks    │                              │ │
│  └──────┬───────┘                     │  ┌────────────┐              │ │
│         │  write                      │  │  Weight     │              │ │
│         ▼                             │  │  Buffer A   │◀─ active    │ │
│  ┌──────────────────┐                 │  └────────────┘              │ │
│  │  Weight Buffer A │ (ping)          │  ┌────────────┐              │ │
│  │  Weight Buffer B │ (pong)          │  │  Weight     │              │ │
│  └──────────────────┘                 │  │  Buffer B   │◀─ shadow    │ │
│   ↑ DMA fills shadow                  │  └────────────┘              │ │
│   ↑ NPU reads active                  └──────────────────────────────┘ │
│                                                                         │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │           AXI-Lite Control Interface  (CPU / Host)               │    │
│  │   CTRL · STATUS · NUM_EXPERTS · CACHE_SIZE · LATENCY · RESULT   │    │
│  └─────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
         ▲
         │  AXI-MM (DRAM)
         ▼
┌──────────────────┐
│  External DRAM   │  (expert weight banks, up to N experts × W bytes each)
└──────────────────┘
```

### Signal Legend
| Arrow | Meaning |
|-------|---------|
| `─▶`  | Combinational / registered data path |
| `══▶` | High-bandwidth streaming interface (AXI-stream) |
| `◀══` | Bi-directional (DMA read-back / write-back) |
| `↑`   | Control annotation (not a physical net) |

---

## 2. Double-Buffering Timing Diagram

### 2.1 Concept

Double buffering decouples DRAM load latency from NPU compute latency.
While the NPU executes on the weights in Buffer A (the *active* buffer),
the DMA engine simultaneously pre-fetches the *next* expert's weights
into Buffer B (the *shadow* buffer).  At the swap point both operations
complete and the roles of A and B are exchanged.

### 2.2 ASCII Timeline (one token, six experts, 500-cycle DMA per expert)

```
Cycle:   0    500   1000   1500   2000   2500   3000   3500
         │     │     │      │      │      │      │      │
DMA:   ══E0══╗══E1══╗══E2══╗══E3══╗══E4══╗══E5══╗
             ║      ║      ║      ║      ║      ║
NPU:         ╚══E0══╩══E1══╩══E2══╩══E3══╩══E4══╩══E5══╗
             ↑                                          ↑
         overlap                                  last expert
          begins                                  (no next DMA)

Buffer:  ─A─ ─B─ ─A─ ─B─ ─A─ ─B─ ─A─ ─B─   (active alternates each swap)
```

Key observations:
- **Without DB**: total = DMA + compute for every expert =  
  `N × (t_dma + t_exec)` cycles
- **With DB**: total ≈ `t_dma + N × max(t_dma, t_exec)` cycles  
  (first DMA has no overlap; thereafter overlap saves min(t_dma, t_exec) each)
- Simulation result: **338,500 cycles** (Zipf + History + DB) vs  
  **1,071,150 cycles** (serial + LRU) → **3.17× speedup**

### 2.3 Buffer Swap Handshake

```
         DMA side                      NPU side
            │                              │
            │  (fill shadow complete)       │  (exec active complete)
            │──── SHADOW_READY ───────────▶│
            │◀─── ACTIVE_DONE  ────────────│
            │                              │
            │       SWAP BUFFERS           │
            │  A↔B (one register-bit flip) │
            │                              │
```

---

## 3. AXI-Lite Register Map

All registers are 32-bit, little-endian.  The aperture is 256 bytes
(`0x100`) starting at the base address configured in the system address map.

| Offset | Name            | R/W | Reset | Description |
|--------|-----------------|-----|-------|-------------|
| `0x00` | **CTRL**        | R/W | `0x0` | bit[0]=START: write 1 to begin operation; bit[1]=RESET: synchronous reset of all state |
| `0x04` | **STATUS**      | RO  | `0x0` | `0`=IDLE, `1`=BUSY (DMA+compute in flight), `2`=DONE (result valid) |
| `0x08` | **NUM_EXPERTS** | R/W | `0x8` | Number of active MoE experts for this inference pass (1–256) |
| `0x0C` | **CACHE_SIZE**  | R/W | `0x4` | Expert weight-cache capacity (entries); affects hit-rate model |
| `0x10` | **LATENCY_CYCLES** | RO | —  | Elapsed cycles for the last completed operation (written by HW on DONE) |
| `0x14` | **RESULT**      | RO  | `0x0` | Expert dispatch count output for the last completed token |

### CTRL Bit-Field Detail

```
 31                        2   1     0
 ┌───────────────────────────┬─────┬─────┐
 │         reserved          │RST  │ START│
 └───────────────────────────┴─────┴─────┘
```

### STATUS Encoding

| Value | Name | Description |
|-------|------|-------------|
| `0`   | IDLE | Device ready, no operation pending |
| `1`   | BUSY | DMA load + NPU compute in progress |
| `2`   | DONE | Operation complete; LATENCY_CYCLES and RESULT are valid |

> **Note**: Software must re-write CTRL[0]=1 to start the next token;
> STATUS does not auto-reset to IDLE.

---

## 4. Data Flow: One Full Inference Pass

The following numbered steps describe a single token traversing the full
MENA pipeline from SW dispatch to RESULT read-back.

```
Step  Actor        Action
────  ──────────   ──────────────────────────────────────────────────────────
 1    Host CPU     Write NUM_EXPERTS and CACHE_SIZE to configure the device.
 2    Host CPU     Write CTRL[0]=1 (START).
 3    HW / RTL     Routing Predictor ranks experts by predicted token affinity
                   (Zipf distribution or history-based heuristic).
 4    HW / RTL     Expert Queue Scheduler builds a priority-ordered list of
                   expert IDs to dispatch this token.
 5    HW / RTL     Expert Cache lookup for expert[0].
                     → HIT : weight base-address forwarded directly to NPU.
                     → MISS: DMA Engine issued to fetch weights from DRAM
                              into the current shadow buffer.
 6    DMA Engine   Streams expert[0] weights via AXI-stream into Weight
                   Buffer B (shadow).  Signals SHADOW_READY on completion.
 7    NPU          If prior expert (or cache hit) is available in Buffer A,
                   begins execution concurrently with step 6.
 8    HW / RTL     SHADOW_READY ∧ ACTIVE_DONE → swap A↔B.
                   Buffer B becomes active; Buffer A becomes shadow.
 9    HW / RTL     Repeat steps 5–8 for experts[1 … N-1].
10    HW / RTL     On last expert completion: STATUS ← DONE,
                   LATENCY_CYCLES ← elapsed_ticks / clk_period,
                   RESULT ← total_dispatches.
11    Host CPU     Poll STATUS until 2 (DONE).
12    Host CPU     Read RESULT and LATENCY_CYCLES registers.
13    Host CPU     Optionally write CTRL[1]=1 (RESET) to return to IDLE.
```

### Cache Interaction Detail

```
Expert ID request
       │
       ▼
┌──────────────────┐   hit  ┌─────────────────┐
│  Tag Array (SRAM)│───────▶│ Data SRAM (BRAM) │──▶ NPU Buffer
│  (fully assoc,   │        │ (weight rows)    │
│   LRU/FIFO evict)│   miss └─────────────────┘
└──────────────────┘
       │ miss
       ▼
  DMA Engine → DRAM fetch → shadow buffer → evict LRU → update tag
```

---

## 5. RTL Implementation Roadmap

The following RTL modules must be built to graduate from the gem5 dummy
model to a synthesisable implementation (target: FPGA / ASIC).

### 5.1 Module List

| Priority | File | Function |
|----------|------|----------|
| 1 | `rtl/weight_buffer_ctrl.sv` | Double-buffer manager; owns `buf_sel` register, swap handshake, A/B ownership signals |
| 2 | `rtl/dma_engine.sv` | AXI4 master; reads expert weight rows from DRAM into shadow buffer; generates `shadow_ready` |
| 3 | `rtl/expert_cache_ctrl.sv` | Fully-associative tag array + data SRAM controller; LRU/FIFO replacement; feeds hit data or miss IDs |
| 4 | `rtl/routing_predictor.sv` | Registers expert access history; scores experts by Zipf rank or frequency counter |
| 5 | `rtl/expert_queue_sched.sv` | Priority queue (min-heap, depth=NUM_EXPERTS); arbitrates DMA and NPU dispatch order |
| 6 | `rtl/npu_core_stub.sv` | Placeholder NPU execution engine; parameterised latency `EXEC_CYCLES`; drives `active_done` |
| 7 | `rtl/axilite_regs.sv` | AXI-lite register file for CTRL/STATUS/NUM_EXPERTS/CACHE_SIZE/LATENCY/RESULT |
| 8 | `rtl/mena_top.sv` | Top-level integration; wires all sub-modules; exports AXI-lite + AXI-MM ports |

### 5.2 `weight_buffer_ctrl.sv` – Key Signals

```systemverilog
module weight_buffer_ctrl #(
    parameter int BUF_WORDS  = 4096,  // words per buffer
    parameter int DATA_WIDTH = 32
) (
    input  logic        clk, rst_n,
    // Swap handshake
    input  logic        shadow_ready,   // from DMA Engine
    input  logic        active_done,    // from NPU Core
    output logic        buf_sel,        // 0=A active, 1=B active
    // Buffer A ports
    output logic        buf_a_wr_en,
    output logic [12:0] buf_a_wr_addr,
    output logic [31:0] buf_a_wr_data,
    output logic [12:0] buf_a_rd_addr,
    input  logic [31:0] buf_a_rd_data,
    // Buffer B ports (mirrored)
    output logic        buf_b_wr_en,
    output logic [12:0] buf_b_wr_addr,
    output logic [31:0] buf_b_wr_data,
    output logic [12:0] buf_b_rd_addr,
    input  logic [31:0] buf_b_rd_data
);
```

### 5.3 `dma_engine.sv` – Key Signals

```systemverilog
module dma_engine #(
    parameter int AXI_ADDR_WIDTH = 40,
    parameter int AXI_DATA_WIDTH = 128,
    parameter int EXPERT_BYTES   = 16384   // bytes per expert weight tensor
) (
    input  logic                      clk, rst_n,
    // Control
    input  logic                      start,
    input  logic [AXI_ADDR_WIDTH-1:0] expert_base_addr,
    output logic                      shadow_ready,
    output logic                      busy,
    // AXI4 Master (DRAM)
    output logic [AXI_ADDR_WIDTH-1:0] m_axi_araddr,
    output logic                      m_axi_arvalid,
    input  logic                      m_axi_arready,
    input  logic [AXI_DATA_WIDTH-1:0] m_axi_rdata,
    input  logic                      m_axi_rvalid,
    input  logic                      m_axi_rlast,
    output logic                      m_axi_rready,
    // Shadow buffer write port
    output logic                      buf_wr_en,
    output logic [12:0]               buf_wr_addr,
    output logic [31:0]               buf_wr_data
);
```

### 5.4 Integration Topology (`mena_top.sv`)

```
                    ┌─────────────────────────────────────────────┐
 AXI-Lite Host      │  mena_top                                    │
 ──────────────────▶│  axilite_regs ──▶ routing_predictor          │
                    │                ──▶ expert_queue_sched         │
 AXI-MM (DRAM)      │  dma_engine ◀══════════════════════════════  │
 ══════════════════▶│                                              │
                    │  weight_buffer_ctrl ──▶ npu_core_stub        │
                    │  expert_cache_ctrl ──▶ (cache hit bypass)     │
                    └─────────────────────────────────────────────┘
```

---

## 6. Key Metrics and Formulas

### 6.1 Overlap Efficiency

Measures how well DMA and NPU compute are overlapped across the expert
sequence.  A value of 1.0 means perfectly pipelined; 0.5 means half the
time is idle waiting.

```
                    exec_cycles
overlap_efficiency = ─────────────────────────────────
                    max(dma_cycles, exec_cycles) × N
```

Where:
- `exec_cycles` = NPU compute cycles for one expert tensor
- `dma_cycles`  = DRAM fetch cycles for one expert tensor
- `N`           = number of experts dispatched per token

### 6.2 Total Latency (with double buffering)

```
T_db = t_dma_first + (N - 1) × max(t_dma, t_exec) + t_exec_last
```

For the common case where `t_dma ≈ t_exec`:

```
T_db ≈ t_dma + N × t_exec        (fully overlapped pipeline)
```

### 6.3 Speedup over Serial Baseline

```
          T_serial     N × (t_dma + t_exec)
speedup = ──────── = ────────────────────────
          T_db         t_dma + N × t_exec
```

### 6.4 Simulation-Derived Numbers

| Configuration             | Total Cycles | Notes |
|---------------------------|-------------|-------|
| Serial + LRU cache        | 1,071,150   | Baseline: no overlap, LRU eviction |
| Serial + History predict  |   ~850,000  | Better hit rate, still no overlap |
| **Zipf + History + DB**   | **338,500** | Full pipeline, cache-aware prefetch |
| Theoretical minimum       | ~280,000    | Perfect oracle cache, zero DMA stall |

**Overall speedup (prototype vs baseline): 3.17×**

### 6.5 Breakeven Analysis

Double buffering requires two weight-buffer banks (2× SRAM cost).  The
breakeven point (where the area cost is justified by throughput gain)
depends on the DMA-to-compute ratio:

```
breakeven:  t_dma / t_exec > 0   (always beneficial if any DMA latency exists)
area cost:  2 × (expert_size × n_active_banks × SRAM_cost_per_bit)
```

For `expert_size = 16 KB` and `SRAM_cost ≈ 0.5 mm²/MB` at 7 nm:
- Buffer pair cost ≈ **0.016 mm²** – negligible vs typical NPU tile area.

### 6.6 Cache Hit Rate Impact

```
            cache_hits
hit_rate = ────────────────
            total_accesses

T_effective_dma = T_dma × (1 - hit_rate)   # cache hits bypass DMA entirely
```

With `cache_size=4` experts and Zipf α=1.2 over 8 experts, the
history-based predictor achieves **~60% hit rate** in steady state,
reducing effective DMA traffic by 3/5.

---

## 7. Open Questions / Future Work

| # | Question | Impact |
|---|----------|--------|
| 1 | Optimal expert weight tensor granularity for AXI burst sizing | DMA efficiency |
| 2 | Dynamic cache resize based on token-batch diversity | Hit rate vs area |
| 3 | Interrupt vs polling model for STATUS readback in OS driver | SW latency |
| 4 | Multi-token parallelism (N tokens in flight across the DB pipeline) | Throughput scaling |
| 5 | Power gating of idle weight buffers between inferences | Energy efficiency |

---

*Document generated: 2024 – MENA Prototype Project*  
*Simulation data: `/home/a/prototype/mena-prototype/` Python prototype*
