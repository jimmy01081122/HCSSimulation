# IEEE Research Project: Heterogeneous CPU-GPU Co-Design for Multi-Stream MoE Inference Scheduling
# 異質 CPU-GPU 多流 MoE 推理排程軟硬體協同設計研究專案

---

## 1. Project Overview / 專案概述

This folder (`ieee_moe_codesign/`) is an independent research workspace targeting
IEEE-level publication in venues such as:

- **IEEE Transactions on VLSI Systems (TVLSI)**
- **IEEE/ACM International Conference on Computer-Aided Design (ICCAD)**
- **IEEE International Symposium on High-Performance Computer Architecture (HPCA)**
- **Design Automation Conference (DAC)**
- **IEEE Micro Top Picks**

**Research Title (Proposed):**

> *HEXA-MoE: A Heterogeneous Expert-Aware Scheduling Framework for
> Multi-Stream Mixture-of-Experts Inference on CPU-GPU Platforms*

**中文研究標題 (暫定):**

> *HEXA-MoE：面向 CPU-GPU 異質平台之多流混合專家推理排程框架*

---

## 2. Research Problem Statement / 研究問題陳述

### 2.1 Background
Large Language Models (LLMs) increasingly adopt Mixture-of-Experts (MoE)
architectures (e.g., Mixtral-8x7B, DeepSeek-V3, Snowflake Arctic) where
each transformer layer activates only a sparse subset of experts.
In multi-agent serving scenarios, multiple concurrent inference requests
trigger parallel MoE streams, creating a complex resource contention problem.

### 2.2 Bottleneck Analysis (研究瓶頸)

```
┌──────────────────────────────────────────────────────────────┐
│                  Multi-Agent MoE Serving                      │
│                                                                │
│  Agent 1 ──┐                                                   │
│  Agent 2 ──┼──→ CPU Orchestrator ──→ GPU Expert Cache ──→ Out  │
│  Agent N ──┘    (queue/schedule)     (dense + cached experts)  │
│                       │                       │                │
│                       ▼                       ▼                │
│              CPU Expert Exec          PCIe Transfer Queue      │
│              (missing experts)         (bandwidth contention)  │
└──────────────────────────────────────────────────────────────┘
```

**Three critical bottlenecks:**

1. **PCIe Transfer Contention**: When GPU expert cache misses occur,
   fetching expert weights (1-2 GB each) via PCIe Gen4 x16 (16 GB/s)
   causes 62.5-125 ms per expert. Under concurrent streams, DMA queue
   head-of-line blocking cascades latency.

2. **CPU Orchestration Overhead**: The host CPU handles request queueing,
   tool-call dispatching, and expert scheduling. Centralized lock-based
   task queues cause cache coherence storms on multi-core CPUs.

3. **CPU Compute Contention**: When experts are offloaded to CPU execution
   (to avoid PCIe transfer), finite CPU cores (typically 8-32) become
   saturated, creating a secondary queueing bottleneck.

### 2.3 Research Questions (RQs)

- **RQ1**: How does expert-locality-aware scheduling reduce GPU cache
  thrashing under concurrent multi-stream MoE workloads?
- **RQ2**: Can a runtime cost-model dynamically balance CPU offloading
  versus GPU transfer to minimize end-to-end latency?
- **RQ3**: What microarchitectural queue design minimizes CPU orchestration
  overhead in terms of cache misses and lock contention?
- **RQ4**: How does an aging-based fairness mechanism (F-ELAS) prevent
  request starvation while preserving throughput?

---

## 3. Directory Structure / 目錄結構

```
ieee_moe_codesign/
├── README.md                       ← This file (計畫書)
├── HANDOFF.md                      ← Gemini 3.5 Flash 接手指南
├── docker/
│   └── Dockerfile                  ← 實驗容器環境定義
├── simulator/
│   ├── src/                        ← 改進後的系統級模擬器原始碼
│   │   ├── hardware_model.py       ← 硬體模型 (PCIe, GPU, CPU)
│   │   ├── cache.py                ← GPU Expert Cache (LRU/LFU/ARC)
│   │   ├── decision_engine.py      ← DCMD 動態決策引擎
│   │   ├── scheduler.py            ← F-ELAS 公平性調度器
│   │   ├── simulator.py            ← 核心模擬引擎 (整合)
│   │   └── workload_gen.py         ← 工作負載產生器
│   └── tests/                      ← 單元測試
│       └── test_simulator.py
├── gem5_workloads/
│   ├── src/                        ← gem5 SE 模式 C++ 基準測試
│   │   ├── centralized_queue.cpp
│   │   ├── distributed_queue.cpp
│   │   └── lock_free_queue.cpp     ← 新增: 無鎖佇列比較
│   ├── configs/                    ← gem5 Python 配置腳本
│   │   └── se_moe_bench.py
│   └── scripts/                    ← gem5 執行與統計提取
│       ├── run_gem5_all.sh
│       └── parse_stats.py
├── experiments/
│   ├── configs/                    ← 實驗參數 YAML 配置
│   │   ├── exp1_cache_sweep.yaml
│   │   ├── exp2_pcie_sweep.yaml
│   │   ├── exp3_concurrency.yaml
│   │   ├── exp4_zipf_alpha.yaml
│   │   └── exp5_cpu_cores.yaml
│   ├── scripts/                    ← 自動化實驗腳本
│   │   ├── run_all_experiments.sh
│   │   └── run_single_exp.py
│   ├── results/                    ← 實驗數據輸出 (CSV/JSON)
│   └── figures/                    ← 實驗圖表 (PNG/PDF)
│       └── generate_all_figures.py
├── traces/                         ← 合成 trace 資料
│   └── generate_traces.py
├── paper/
│   ├── ieee_en/                    ← IEEE 格式英文論文
│   │   ├── main.tex
│   │   ├── abstract.tex
│   │   ├── introduction.tex
│   │   ├── background.tex
│   │   ├── methodology.tex
│   │   ├── evaluation.tex
│   │   ├── related_work.tex
│   │   ├── conclusion.tex
│   │   └── IEEEtran.cls
│   ├── ieee_zh/                    ← 中文翻譯版本
│   │   └── main_zh.md
│   └── bib/
│       └── references.bib          ← BibTeX 參考文獻
├── docs/
│   ├── EXPERIMENT_GUIDE.md         ← 實驗操作詳細教學
│   ├── WRITING_GUIDE.md            ← 論文寫作指引
│   └── ARCHITECTURE_NOTES.md       ← 架構設計筆記
└── Makefile                        ← 一鍵自動化管理
```

---

## 4. Proposed Contributions / 預期貢獻

### Contribution 1: F-ELAS Scheduler (Fairness-aware Expert-Locality-Aware Scheduling)
- Groups concurrent token requests by expert affinity to maximize GPU cache reuse
- Incorporates an aging mechanism ($\theta_{starve}$) to bound worst-case latency
- Novelty over prior work: explicit fairness guarantees for cold-expert requests

### Contribution 2: DCMD-Opt (Contention-aware Dynamic Cost-Model Decision)
- Runtime cost model that estimates:
  - $Cost_{transfer} = T_{pcie\_queue} + N_{tokens} \times T_{gpu\_expert}$
  - $Cost_{offload} = \frac{Q_{cpu}}{N_{cores}} \times T_{cpu\_expert} + N_{tokens} \times T_{cpu\_expert} + T_{result\_transfer}$
- Dynamically balances CPU vs. GPU execution based on real-time queue depths

### Contribution 3: Distributed Expert-Affinity Queue Architecture
- gem5-validated microarchitectural comparison showing 96.6% cycle reduction
  vs. centralized lock-based queue
- Work-stealing with expert-affinity partitioning

### Contribution 4: Comprehensive System-Level DSE Framework
- Open-source simulator for heterogeneous MoE inference evaluation
- Parameterized sweeps over cache capacity, PCIe bandwidth, CPU cores,
  workload concurrency, and Zipf distribution skewness

---

## 5. Experimental Plan / 實驗計劃

### Experiment 1: GPU Expert Cache Capacity Sweep
| Parameter          | Values                    |
|--------------------|---------------------------|
| Cache Capacity     | 2, 4, 6, 8, 12, 16       |
| Strategies         | FCFS_T, ELAS_T, ELAS_O, ELAS_DCMD, F-ELAS_DCMD |
| Fixed Parameters   | BW=16 GB/s, E=16, α=1.2  |
| Metrics            | SimTime, HitRate, AvgLatency, MaxLatency, P99Latency |

### Experiment 2: PCIe Bandwidth Sweep
| Parameter          | Values                    |
|--------------------|---------------------------|
| PCIe Bandwidth     | 8, 16, 32, 64 GB/s       |
| Cache Capacity     | 4 (intentionally small)   |
| Strategies         | All 5                     |

### Experiment 3: Concurrency Scaling
| Parameter          | Values                    |
|--------------------|---------------------------|
| Num Requests       | 5, 10, 20, 40, 80        |
| Arrival Rate λ     | 0.05, 0.1, 0.2, 0.5      |
| Fixed              | Cap=4, BW=16              |

### Experiment 4: Workload Skewness (Zipf α)
| Parameter          | Values                    |
|--------------------|---------------------------|
| Zipf α             | 0.8, 1.0, 1.2, 1.5, 2.0  |
| Fixed              | Cap=4, BW=16, N=15        |

### Experiment 5: CPU Core Scaling
| Parameter          | Values                    |
|--------------------|---------------------------|
| CPU Cores          | 2, 4, 8, 16, 32          |
| Fixed              | Cap=4, BW=16, N=15        |

### Experiment 6: gem5 Microarchitecture Validation
| Configuration      | Values                    |
|--------------------|---------------------------|
| CPU Model          | O3CPU                     |
| L1D Cache          | 32kB, 64kB, 128kB        |
| L2 Cache           | 256kB, 512kB, 1MB         |
| Queue Policies     | Centralized, Distributed, Lock-free |
| Metrics            | Cycles, IPC, D-Cache Misses, L2 Misses |

---

## 6. Methodology / 研究方法論

### 6.1 Two-Level Evaluation Strategy

```
Level 1: System-Level Simulation (Python HMCS Simulator)
├── Models: PCIe DMA bus, GPU Expert Cache, CPU Core Pool
├── Policies: F-ELAS scheduler, DCMD-Opt decision engine
├── Output: Latency distributions, hit rates, utilization
└── Purpose: Design space exploration & sensitivity analysis

Level 2: Microarchitectural Validation (gem5 SE Mode)
├── Models: O3CPU pipeline, cache hierarchy, memory controller
├── Workloads: C++ MoE scheduling queue benchmarks
├── Output: Cycle counts, IPC, cache miss rates
└── Purpose: Validate CPU orchestration overhead claims
```

### 6.2 Baseline Comparisons

| Label              | Scheduler | Offloader        | Description                           |
|--------------------|-----------|------------------|---------------------------------------|
| FCFS-T (Baseline)  | FCFS      | Always Transfer  | Naive FIFO with full GPU execution    |
| ELAS-T             | ELAS      | Always Transfer  | Locality-aware but transfer-only      |
| ELAS-O             | ELAS      | Always Offload   | Locality-aware but CPU-only fallback  |
| ELAS-DCMD          | ELAS      | DCMD (dynamic)   | Co-design without fairness            |
| **F-ELAS-DCMD**    | **F-ELAS**| **DCMD-Opt**     | **Proposed full framework**           |

---

## 7. Paper Outline (IEEE Format) / 論文大綱

### English Paper (IEEE Conference Format, 6-8 pages)

1. **Abstract** (150 words)
2. **I. Introduction** (1.5 pages)
   - MoE scaling trend, multi-agent serving motivation
   - Problem statement: PCIe contention + CPU bottleneck
   - Contributions summary (4 bullet points)
3. **II. Background and Motivation** (1 page)
   - MoE architecture primer
   - Heterogeneous inference deployment
   - Motivating example with timing diagram
4. **III. HEXA-MoE Framework** (2 pages)
   - A. System Architecture Overview
   - B. F-ELAS: Fairness-aware Expert-Locality Scheduling
   - C. DCMD-Opt: Contention-aware Dynamic Decision
   - D. Distributed Expert-Affinity Queue
5. **IV. Evaluation** (2 pages)
   - A. Experimental Setup (simulator, gem5, parameters)
   - B. Cache Capacity Sensitivity
   - C. PCIe Bandwidth Impact
   - D. Concurrency Scaling
   - E. Microarchitectural Validation (gem5)
   - F. Fairness Analysis (CDF of per-request latency)
6. **V. Related Work** (0.5 page)
7. **VI. Conclusion** (0.25 page)
8. **References** (30+ citations)

---

## 8. Key References / 關鍵參考文獻

The following references MUST be cited:

1. Fedus et al., "Switch Transformers: Scaling to Trillion Parameter Models with Simple and Efficient Sparsity," JMLR 2022.
2. Hwang et al., "Tutel: Adaptive Mixture-of-Experts at Scale," MLSys 2023.
3. Rajbhandari et al., "DeepSpeed-MoE: Advancing Mixture-of-Experts Inference and Training to Power Next-Generation AI Scale," ICML 2022.
4. Eliseev & Mazur, "Fast Inference of Mixture-of-Experts Language Models with Offloading," arXiv 2024.
5. Xue et al., "MoE-Infinity: Offloading-Efficient MoE Model Serving," arXiv 2024.
6. Aminabadi et al., "DeepSpeed Inference: Enabling Efficient Inference of Transformer Models at Unprecedented Scale," SC 2022.
7. Binkert et al., "The gem5 simulator," ACM SIGARCH Computer Architecture News, 2011.
8. Kwon et al., "Efficient Memory Management for Large Language Model Serving with PagedAttention," SOSP 2023.
9. Sheng et al., "FlexGen: High-Throughput Generative Inference of Large Language Models with a Single GPU," ICML 2023.
10. Tang et al., "FuseMoE: Mixture-of-Experts Transformers for Fleximodal Fusion," arXiv 2024.

---

## 9. Timeline / 時程規劃

| Phase | Task                                  | Duration   | Status     |
|-------|---------------------------------------|------------|------------|
| P0    | Project plan & workspace setup        | Day 1      | ✅ Done     |
| P1    | Simulator refactoring & unit tests    | Day 2-3    | 🔄 Next    |
| P2    | gem5 workload expansion (lock-free)   | Day 3-4    | ⬜ Pending  |
| P3    | Trace generation & validation         | Day 4      | ⬜ Pending  |
| P4    | Full DSE sweep execution (Exp 1-5)    | Day 5-6    | ⬜ Pending  |
| P5    | gem5 microarch validation (Exp 6)     | Day 6-7    | ⬜ Pending  |
| P6    | Figure generation & data analysis     | Day 7-8    | ⬜ Pending  |
| P7    | Paper drafting (English + Chinese)    | Day 8-12   | ⬜ Pending  |
| P8    | Review, revision & finalization       | Day 12-14  | ⬜ Pending  |

---

## 10. Environment Requirements / 環境需求

- **Host OS**: WSL2 Ubuntu (user: a, pass: a)
- **Docker Images**: `hetero-moe-env:latest` (已建立)
- **gem5**: `/home/a/gem5/build/ARM/gem5.opt` (已編譯)
- **Python**: 3.10+ with numpy, pandas, matplotlib, scipy, pyyaml
- **Cross-compiler**: `aarch64-linux-gnu-g++` (Docker 內)

---

## 11. Critical Constraints / 關鍵限制

1. **不得修改** `/home/a/HCSSimulation` 下原本存在的任何檔案
2. 所有新增程式碼與文件僅存放於 `advanced_projects/ieee_moe_codesign/`
3. 只使用開源軟體 (gem5, Python, Docker)
4. Docker 容器化管理所有工具依賴
5. 下載超過 2GB 資源前須徵求許可
