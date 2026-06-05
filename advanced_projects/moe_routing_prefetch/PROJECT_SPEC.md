# PROJECT_SPEC.md

# Routing-aware Prefetch for Mixture-of-Experts Inference

## 針對 MoE 推理之 Routing-aware Expert Prefetch 軟硬體共同設計

### 教學版專案設計規格書 (v1.2 - 進階專案版)

---

**版本**: 1.2  
**狀態**: 待審查 (Pre-implementation Review)  
**上層專案**: HCSSimulation  
**專案層級**: 進階研究專案  
**建議目錄**: advanced_projects/moe_routing_prefetch/  
**硬體限制**: RTX 3050 6GB / 免費 Google Colab / Icarus Verilog or Verilator

---

## 目錄

1. [Project Overview](#1-project-overview)
2. [Hardware Feasibility](#2-hardware-feasibility)
3. [Relationship to Existing HCSSimulation Components](#3-relationship-to-existing-hcssimulation-components)
4. [HCSSimulation Integration Requirements](#4-hcssimulation-integration-requirements)
5. [Learning Roadmap](#5-learning-roadmap)
6. [Repository Structure](#6-repository-structure)
7. [File-by-file Specification](#7-file-by-file-specification)
8. [Trace Format Specification](#8-trace-format-specification)
9. [Toy PyTorch MoE Model Specification](#9-toy-pytorch-moe-model-specification)
10. [Python Simulator Specification](#10-python-simulator-specification)
11. [RTL Architecture Specification](#11-rtl-architecture-specification)
12. [Experiments and Assignments](#12-experiments-and-assignments)
13. [Acceptance Criteria](#13-acceptance-criteria)
14. [Implementation Readiness Checklist](#14-implementation-readiness-checklist)
15. [Revision Notes v1.2](#15-revision-notes-v12)

---

## 1. Project Overview

### 1.1 研究問題

Mixture-of-Experts (MoE) 架構透過稀疏啟動機制，將推理計算分散至多個 expert sub-network。每個 token 只會被 router 選中少數幾個 expert (top-k)，其餘 expert 不參與運算。這帶來計算效率的提升，但也引入了新的系統瓶頸：

- **Expert weight 的搬運延遲**：每個 expert 的權重參數可能數十到數百 MB。若 expert 未在本地記憶體中，必須從遠端記憶體 (HBM、DRAM、甚至 SSD) 載入，造成數百至數千 cycle 的延遲。
- **動態性與不可預測性**：不同 token 可能選中不同 expert，routing 決策依賴 input-dependent softmax 分布，使得傳統靜態 cache 策略效果有限。
- **Expert activation locality**：部分 MoE routing workload 可能呈現 temporal locality 或 popularity skew。本專案透過 synthetic trace 與 toy MoE trace 分析這些條件下 routing-aware prefetch 是否有效。若 trace 不具 locality，prefetch 可能無明顯效益，甚至可能因 useless prefetch 增加 bandwidth pressure。

本專案的核心問題為：

> 能否利用 router 已知的 routing score 分布，在 expert 真正被需要之前，提前將其權重預取 (prefetch) 至本地 cache，以隱藏記憶體搬運延遲？

### 1.2 本專案範圍

本專案為教學版 full-stack HW/SW co-design 專案，最終目標為 RTL simulation，涵蓋以下四層：

| 層次 | 內容 | 實作深度 |
|------|------|----------|
| Layer 1: Trace Generation | Synthetic trace + Toy PyTorch MoE trace | 完整實作 |
| Layer 2: Python Cache/Prefetch Simulator | 多種 policy 比較與 latency 模型 | 完整實作 |
| Layer 3: gem5 Modeling Plan | CPU-memory-accelerator 系統建模 | 規劃文件，不完整實作 |
| Layer 4: Verilog RTL Simulation | Prefetch controller 與 expert cache RTL | 完整實作至 simulation (不要求 FPGA 上板) |

### 1.3 本專案不處理的內容

- 矩陣乘法加速器設計 (matrix multiplication accelerator)
- 完整的 MoE inference engine 實作
- 大型 MoE LLM (Mixtral 8x7B、DeepSeek-MoE、Switch Transformer) 的完整執行
- FPGA 上板驗證 (本專案第一版不要求 FPGA 上板)
- 商用 EDA tool (Synopsys、Cadence) 的使用
- 完整 gem5 full-system simulation
- Linux kernel driver 開發
- CUDA kernel 實作
- ASIC timing signoff

### 1.4 為何不做矩陣乘法加速

矩陣乘法加速是 DNN accelerator 領域中研究最充分的題目，已有大量成熟方案 (systolic array、NVDLA、TPU)。本專案選擇聚焦 MoE routing-aware prefetch 的原因：

1. **差異化**：矩陣乘法加速的設計空間已被大量探索，routing-aware prefetch 則相對新穎，特別是在 expert-level granularity 的 cache/prefetch 策略。
2. **跨層設計思維**：prefetch 策略需要同時理解 (a) 軟體層 of router 行為、(b) 系統層 of 記憶體階層、(c) 硬體層 of cache/DMA 控制邏輯，是真正的軟硬體共同設計問題。
3. **教學價值**：cache replacement + prefetch 是計算機結構教學的經典主題，將其放在 MoE inference 的場景中，為學生建立從傳統 cache 理論到前沿 LLM 系統的橋梁。
4. **資源友善**：prefetch controller 的 RTL 複雜度遠低於矩陣乘法加速器，適合在教學時間內完成。

### 1.5 為何 routing-aware prefetch 有研究價值

1. **大型 MoE 模型的部署瓶頸**：GPT-4 (推測)、Mixtral、DeepSeek-V2 等模型使用 MoE 架構。Expert 數量可達 64-256 個，每個 expert 數百 MB，無法全部常駐 GPU memory。
2. **Expert offloading 是實際部署策略**：將冷 expert 放在 CPU memory 或 SSD，需要時再載入 GPU memory。載入延遲是效能瓶頸。
3. **Router score 包含預測訊號**：Router 的 softmax 輸出不僅給出 top-k 選擇，其 score 分布也暗示了哪些 expert 即將被選中，可作為 prefetch hint。
4. **傳統 cache 策略不足**：LRU/FIFO 等被動策略只依賴歷史存取模式，無法利用 router 提供的前瞻資訊。Routing-aware prefetch 結合了 domain-specific knowledge 與硬體 prefetch 機制。

---

## 2. Hardware Feasibility

### 2.1 硬體環境

```text
本機 GPU: NVIDIA RTX 3050 6GB (Ampere, CUDA Compute 8.6)
雲端:     免費版 Google Colab：GPU 型號與可用時間不保證，可能無 GPU。本專案不可依賴特定 Colab GPU。
CPU:      一般 x86-64 Linux 主機
RTL 工具: Icarus Verilog (iverilog) 或 Verilator (open-source)
Python:   3.8+ with PyTorch, NumPy, Pandas, Matplotlib
FPGA 板:  使用者有 FPGA 開發板，但本專案第一版不要求 FPGA 上板，本階段仍只做到 RTL simulation。FPGA 上板為後續延伸，不列入必要驗收。
```

### 2.2 可行性分析

| 項目 | 可行性 | 說明 |
|------|--------|------|
| Synthetic trace 產生 | 完全可行 | 純 Python，無 GPU 需求 |
| Toy PyTorch MoE trace 產生 | 完全可行 | 小型模型 (< 100MB)，RTX 3050 或 CPU 即可 |
| Python cache/prefetch simulator | 完全可行 | 純 Python，處理 CSV trace |
| Verilog RTL simulation (Icarus Verilog) | 完全可行 | 純 CPU，無商用 EDA 需求 |
| Verilog RTL simulation (Verilator) | 完全可行 | 純 CPU，C++ 編譯 |
| gem5 modeling plan (文件) | 完全可行 | 僅需撰寫規劃文件 |
| 大型 MoE LLM 推理 | 不可行 | 在 FP16/BF16 或常見推理配置下通常遠超 RTX 3050 6GB 可直接容納的範圍。即使使用量化與 offloading，也會引入額外環境與效能變數。因此大型 MoE LLM 不列入本專案必要流程。 |
| 多 GPU 分散式推理 | 不可行 | 本機僅單 GPU |
| FPGA 上板驗證 | 選配/延伸 | 第一版不列入必要驗收，主要完成 RTL 模擬。 |
| 商用 EDA synthesis | 不可行 | 無 Synopsys/Cadence license |
| 完整 gem5 full-system simulation | 不建議 | 編譯耗時且 debug 困難，不在教學必要範圍 |

### 2.3 需要簡化的部分

1. **MoE 模型規模**：使用 hidden_dim=128 或 256、num_experts=4 到 16 的 toy model，不使用大型 pretrained model。
2. **Trace 規模**：Synthetic trace 長度以 1K~100K 筆 request 為主，足以觀察 cache/prefetch 行為。
3. **RTL 模擬規模**：Testbench 以數百 cycle 為單位，不模擬本量級的完整推理流程。
4. **記憶體與 Cache 實作**：本 RTL 第一版只建模 expert cache metadata，不儲存真實 expert weight data。Cache fill 代表 tag/metadata fill，不代表實際權重搬入 BRAM/DRAM。使用固定延遲 (fixed latency) DMA 模型，不模擬 DRAM timing 細節。

### 2.4 為何 synthetic trace 與 toy MoE trace 是合理起點

1. **Synthetic trace** 允許完全控制 expert activation 的分布 (均勻分布、Zipf 分布、突發切換)，是分離各 policy 效能差異的最佳方式。學術論文中，synthetic workload 普遍用於 cache/prefetch 研究的初步驗證。
2. **Toy PyTorch MoE trace** 使用真正的 softmax router，展示 router score 如何隨 input 分布變化。相較 synthetic trace，它增加了真實感，但模型規模仍在 RTX 3050 可執行的範圍內。
3. **兩者互補**：synthetic trace 用於可控實驗與 RTL testbench 驅動，toy trace 用於展示 real router 行為。

### 2.5 明確聲明

- 大型 MoE LLM (Mixtral 8x7B、DeepSeek-MoE、Switch Transformer 等) 不可作為本專案必要流程。
- Toy MoE trace 不可過度宣稱代表真實大型 LLM 之 routing 行為。Toy model 的 router 尚未經過大規模訓練，其 routing 分布可能與實際 LLM 有顯著差異。本專案的價值在於展示 routing-aware prefetch 的機制與分析方法，而非宣稱實驗結果可直接推廣至生產級 MoE 模型。

---

## 3. Relationship to Existing HCSSimulation Components

本專案與 HCSSimulation 既有組件的關係如下：

- **experiments/**：  
  提供 gem5 入門實驗（x86, ARM, RISC-V 等 SE mode 模擬）。與本進階專案無直接依賴，且方向四不會修改其下的任何配置與代碼。
- **dir/TRY/dir1_memory_dma**：  
  提供 memory/DMA bottleneck 的背景概念。本進階專案引用其教學概念，但第一版程式碼與測試負載與其完全獨立，不直接依賴。
- **dir/TRY/dir2_moe_routing**：  
  提供 MoE routing 運算延遲分析背景。本進階專案可引用其概念，但方向四使用獨立的 synthetic/toy trace 產生器，不直接依賴 dir2 的程式碼與輸出。
- **dir/TRY/dir3_kv_cache**：  
  提供 cache/prefetch 與 memory miss 分析背景。本進階專案借用其 prefetch 思路，但研究對象與行為模型從 KV cache 轉向為 MoE expert cache metadata。
- **advanced_projects/moe_routing_prefetch**：  
  作為獨立的進階專案，它被安置在 HCSSimulation 根目錄下的 `advanced_projects/` 中，以保證與入門級實驗區隔，且不修改任何 `dir/` 或 `dir/TRY/` 下的既有檔案。

---

## 4. HCSSimulation Integration Requirements

為確保與既有 HCSSimulation 框架的和諧整合，本專案必須遵守以下規範：

1. **不得破壞舊有流程**：第一版實作不得修改或刪除既有 `dir/TRY/dir1_memory_dma/`、`dir/TRY/dir2_moe_routing/`、`dir/TRY/dir3_kv_cache/` 以及 `experiments/` 的任何原始碼、設定檔或測試負載。
2. **獨立進階專案定位**：本專案不作為 `dir/TRY/` 下的第四方向。本專案是位於根目錄 `advanced_projects/` 下的獨立進階研究專案。
3. **引用而非挪用**：若方向四需要共用或參考其他研究方向的文檔或邏輯，僅能以 Markdown 連結與引用說明，不得直接搬移舊有檔案。
4. **根目錄 README.md 更新**：根目錄的 [README.md](file:///home/a/HCSSimulation/README.md) 需要新增「進階專案」章節，說明其下各進階整合專案之配置與重點。更新範例如下：
   
   ```markdown
   ## 進階專案
   
   `advanced_projects/` 內保存從既有教學與研究方向延伸出的完整進階專案。
   
   | 專案 | 目錄 | 主題 | 重點 |
   |---|---|---|---|
   | MoE Routing-aware Prefetch | advanced_projects/moe_routing_prefetch/ | MoE expert cache 與 routing-aware prefetch | 使用 synthetic/toy MoE trace、trace-driven hint、Python cache simulator 與 Verilog RTL simulation，評估 expert cache metadata 與 prefetch controller |
   ```

5. **文檔增設**：於 `docs/` 下新增 [advanced_moe_routing_prefetch_tutorial.md](file:///home/a/HCSSimulation/docs/advanced_moe_routing_prefetch_tutorial.md) 作為本進階專案的教學指引，不修改既有方向一至三的教程。

---

## 5. Learning Roadmap

### Stage 0: 背景與問題定義

**學習目標**：理解 MoE 架構的基本運作、expert routing 的概念、以及為何 expert weight 搬運是效能瓶頸。

**必要背景**：
- 基本深度學習概念 (FFN、softmax)
- 稀疏啟動 (sparse activation) 的意義
- 記憶體階層 (cache、DRAM、bandwidth)

**實作任務**：
- 閱讀 MoE 論文 (Shazeer et al., 2017; Fedus et al., 2022) 的 introduction 與 architecture 章節
- 手動計算一個 8-expert、top-2 的 MoE layer 在不同 expert 大小下的 memory footprint
- 計算若每個 expert 100MB、cache 僅能容納 3 個 expert 時，最壞情況下的 miss rate

**預期輸出**：一份 1-2 頁的問題定義文件，包含 MoE routing 示意圖與 memory footprint 計算表

**觀察指標**：能否正確計算 memory footprint、能否解釋 expert cache miss 的成因

**常見錯誤**：
- 將 MoE 與 model parallelism 混淆
- 忽略 expert weight 大小，只考慮 activation 大小
- 以為所有 expert 都會被用到

---

### Stage 1: Synthetic Trace

**學習目標**：設計並產生結構化的 MoE routing trace，理解不同分布對 cache 行為的影響。

**必要背景**：
- CSV 格式與 Python I/O
- 機率分布 (均勻分布、Zipf 分布)
- Random seed 的作用

**實作任務**：
- 實作 `generate_synthetic_moe_trace.py`
- 產生至少三種分布模式：uniform、zipf、phase-shift
- 產生不同 top_k 與 num_experts 組合的 trace

**預期輸出**：
- `traces/synthetic_uniform_E8_K2.csv`
- `traces/synthetic_zipf_E16_K2.csv`
- `traces/synthetic_phase_E8_K2.csv`

**觀察指標**：
- 各 expert 的 activation frequency 分布是否符合預期
- Zipf 分布中 top expert 的出現頻率是否顯著高於 bottom expert

**常見錯誤**：
- 未正確實作 Zipf 分布，導致所有 expert 頻率相近
- 忘記設定 random seed，導致結果不可重現
- Trace CSV 欄位缺漏或格式不一致

---

### Stage 2: Toy PyTorch MoE Trace

**學習目標**：建構一個可執行且可控制 routing bias 的小型 MoE 模型，產生來自真實 softmax router 的 routing trace。

**必要背景**：
- PyTorch 基礎 (nn.Module、forward)
- Softmax 與 top-k 操作
- GPU memory 估算

**實作任務**：
- 實作 `generate_toy_moe_trace.py`，支援可控的 routing bias 參數：
  - `--router-bias-mode {uniform,hot_expert,temporal,layer_bias}`
  - `--router-temperature`
  - `--input-locality-strength`
- 建構 1-4 layer 的 toy MoE model，每 layer 有 4-16 個 expert
- 使用隨機 input tensor 進行 forward pass，收集 routing decisions 與 router scores

**預期輸出**：
- `traces/toy_pytorch_E8_K2_L2.csv`
- 包含 router score 及其對應之 hint trace 格式

**觀察指標**：
- Router score 的分布形狀 (是否有明顯的 winner-take-all 現象，受 bias-mode 影響)
- 不同 layer 的 routing pattern 是否不同
- 是否有 expert 從未被選中

**常見錯誤**：
- Model 太大導致 OOM
- 忘記設定 `torch.no_grad()` 導致記憶體浪費
- 將這些教學參數產生的 trace 宣稱為真實大型 LLM 的行為

---

### Stage 3: Trace Analysis

**學習目標**：分析 trace 的統計特性，建立對 expert activation locality 的量化理解。

**必要背景**：
- Pandas DataFrame 操作
- 基礎統計 (頻率、直方圖、熱力圖)
- Matplotlib 繪圖

**實作任務**：
- 實作 `analyze_trace.py`
- 計算每個 expert 的 activation frequency
- 計算 inter-arrival distance (同一 expert 兩次被選中之間的間隔)
- 繪製 expert activation heatmap (x 軸 = cycle/request、y 軸 = expert_id)
- 計算 reuse distance distribution

**預期輸出**：
- `results/expert_frequency.png`
- `results/activation_heatmap.png`
- `results/reuse_distance_histogram.png`
- `results/trace_statistics.json`

**觀察指標**：
- Expert frequency skew ratio (最熱 expert 頻率 / 最冷 expert 頻率)
- 平均 reuse distance
- Phase shift 是否能在 heatmap 上觀察到

**常見錯誤**：
- 以為 activation frequency 等於 access pattern，忽略時間順序
- 統計方法錯誤 (例如將 top-k rank 誤算為 frequency)
- Heatmap 色階設定不當，無法觀察分布差異

---

### Stage 4: Python Cache/Prefetch Simulator

**學習目標**：實作多種 cache/prefetch 策略，比較其在不同 trace 與 prefetch hint 條件下的效能差異。

**必要背景**：
- Cache 基礎概念 (hit、miss、replacement)
- FIFO 與 LRU replacement policy
- Prefetch 的概念 (提前載入) 與 trace-driven prefetch hint

**實作任務**：
- 實作 `prefetch_policies.py` (各 policy 的 class)
- 實作 `expert_cache_sim.py` (simulator 主體，需正確讀取 hint trace 並根據 hint 觸發 prefetch)
- 實作 `latency_model.py` (延遲估算模型)
- 實作 `run_experiments.py` (批量實驗腳本)

**預期輸出**：
- `results/policy_comparison.csv`
- `results/hit_rate_comparison.png`
- `results/prefetch_accuracy_comparison.png`
- `results/latency_comparison.png`

**觀察指標**：
- hit_rate、miss_count、prefetch_count、useful_prefetch、useless_prefetch
- prefetch_accuracy = useful_prefetch / prefetch_count
- estimated_latency (考慮 hit latency 與 miss penalty)
- bandwidth_usage

**常見錯誤**：
- LRU 實作未正確處理 hit時的 recency 更新
- Prefetch 未限制 prefetch queue depth，導致不切實際的頻寬假設
- Oracle policy 未正確使用 future window，而是使用了整條 trace 的全局資訊
- 未考慮 DMA bandwidth contention (同時 prefetch 多個 expert 的延遲)

---

### Stage 5: gem5 Modeling Plan

**學習目標**：理解如何在 gem5 中建模 CPU-memory-accelerator 系統，規劃 prefetch controller 的系統級整合。

**必要背景**：
- gem5 SimObject 概念
- MMIO (Memory-Mapped I/O) 通訊
- DMA 模型基礎
- HCSSimulation 專案中已有的 gem5 實驗基礎

**實作任務**：
- 撰寫 gem5 modeling plan 文件 (`docs/gem5_modeling_plan.md`)
- 定義 PrefetchController SimObject 的 interface
- 定義 ExpertCache memory region 的地址空間
- 定義 performance counter 的 gem5 stat 項目

**預期輸出**：
- `docs/gem5_modeling_plan.md`
- SimObject interface 定義 (pseudo-code)
- 地址空間規劃表

**觀察指標**：
- 文件是否清楚定義 CPU-accelerator 通訊介面
- 是否能回答「若未來要接 gem5，應該從哪裡開始」

**常見錯誤**：
- 試圖在此階段完整實作 gem5 SimObject
- 混淆 gem5 的 timing model 與 functional model
- 以為 gem5 可以取代 RTL simulation 或 CUDA profiling

**gem5 適合的建模範圍**：
- CPU-memory-DMA-accelerator 系統行為
- DRAM bandwidth contention
- Accelerator command latency
- Prefetch policy 對系統層延遲的影響

**gem5 不適合的範圍**：
- 取代 RTL simulation (gem5 不模擬 gate-level timing)
- 取代 CUDA profiling (gem5 不模擬 GPU microarchitecture)
- 直接跑大型 PyTorch LLM
- 提供 ASIC/FPGA timing signoff

---

### Stage 6: RTL Module Design

**學習目標**：使用 Verilog 設計 expert cache metadata 與 prefetch controller 的 RTL 模組。

**必要背景**：
- Verilog 語法 (module、always、wire/reg)
- Finite State Machine (FSM) 設計
- Valid/ready handshake protocol
- Cache tag array 概念與 metadata tracking

**實作任務**：
- 實作 7 個 Verilog 模組 (詳見 Section 7 與 Section 11)
- 確保 `expert_cache_tag_array.v` 包含與 prefetch metadata 追蹤相關的 I/O 介面
- 確保所有模組遵循 valid/ready handshake
- 實作 performance counter
- 確保 reset 行為正確，並限定 `replacement_policy_sel` 僅在 reset 時或非 active processing 期間設定

**預期輸出**：
- `rtl/expert_request_queue.v`
- `rtl/expert_cache_tag_array.v`
- `rtl/fifo_replacement.v`
- `rtl/lru_replacement.v`
- `rtl/expert_prefetch_controller.v`
- `rtl/simple_dma_model.v`
- `rtl/top_moe_prefetch_system.v`

**觀察指標**：
- 是否通過 iverilog/Verilator 編譯
- FSM state transition 是否正確
- Valid/ready protocol 是否正確處理 backpressure

**常見錯誤**：
- 使用 blocking assignment 在 sequential logic 中
- 忘記 reset 初始化所有 register
- Valid/ready handshake 中未正確處理 ready deassert 的情況
- Counter 溢位未處理

---

### Stage 7: RTL Testbench

**學習目標**：撰寫 Verilog testbench，驗證 RTL 模組的功能正確性。

**必要背景**：
- Verilog testbench 結構 (`initial`、`$dumpfile`、`$dumpvars`)
- Trace-driven testing 概念與 prefetch hint 模擬

**實作任務**：
- 實作 3 個 testbench (詳見 Section 7)
- 將 synthetic trace 與 hint trace 轉換為 testbench stimulus
- 驗證 10 個測試場景 (詳見 Section 11)

**預期輸出**：
- `tb/tb_expert_cache_tag_array.v`
- `tb/tb_expert_prefetch_controller.v`
- `tb/tb_top_moe_prefetch_system.v`
- `results/rtl_waveforms/*.vcd`

**觀察指標**：
- 所有 testbench 是否 PASS (無 assertion failure)
- Counter 值是否與 Python simulator 的結果一致
- Waveform 中是否可觀察到 prefetch hit 的 latency 隱藏效果

**常見錯誤**：
- Testbench clock generation 不正確
- 未等待足夠的 cycle 讓 DMA 完成
- 未檢查 edge case (例如 cache full 時的 replacement)
- 忘記 `$finish` 導致模擬無限運行

---

### Stage 8: Experiments and Assignments

**學習目標**：透過系統化的實驗，量化逆向參數與 locality 強度對 prefetch 效能的影響。

**必要背景**：
- 實驗設計與控制變因
- 圖表繪製與結果分析

**實作任務**：
- 執行 10 個教學實驗 (詳見 Section 12)
- 整理實驗結果，繪製比較圖表
- 撰寫 analysis 段落

**預期輸出**：
- `results/experiment_*.csv`
- `results/experiment_*.png`
- `results/experiment_summary.md`

**觀察指標**：
- 各實驗的結果趨勢是否符合理論預期
- 是否能識別出 routing-aware prefetch 的最佳參數區間與 locality 敏感度

**常見錯誤**：
- 只跑一個參數組合就下結論
- 未固定其他變因導致結果混淆
- 圖表缺少軸標籤或圖例

---

### Stage 9: Report Writing

**學習目標**：將專案成果整理為結構化報告，練習學術寫作。

**必要背景**：
- 學術報告格式 (Introduction、Method、Results、Discussion)
- 圖表引用與說明

**實作任務**：
- 撰寫完整專案報告
- 包含所有實驗結果與分析
- 明確說明本專案的限制與不可宣稱之事項

**預期輸出**：
- `docs/final_report.md` 或 PDF

**觀察指標**：
- 報告是否清楚區分「可宣稱的結論」與「不可宣稱的結論」
- 是否有明確的 limitations section

**常見錯誤**：
- 將 toy model 的結果過度推廣至真實大型 LLM
- 將 RTL simulation 的 cycle count 宣稱為實體晶片或 FPGA 實測結果
- 省略 limitations

---

## 6. Repository Structure

本進階專案位於 HCSSimulation 根目錄下的 `advanced_projects/moe_routing_prefetch/`。所有內部檔案路徑均相對於該目錄：

```text
HCSSimulation/
├── README.md                          # HCSSimulation 根 README (需更新進階專案說明)
├── docs/
│   ├── direction1_dma_tutorial.md
│   ├── direction2_moe_routing_tutorial.md
│   ├── direction3_kv_cache_tutorial.md
│   └── advanced_moe_routing_prefetch_tutorial.md # 新增進階專案教學文檔
├── experiments/
│   ├── gem5-learning/
│   ├── gem5-learning-arm/
│   └── gem5-learning-riscv/
├── dir/TRY/
│   ├── dir1_memory_dma/
│   ├── dir2_moe_routing/
│   └── dir3_kv_cache/
├── advanced_projects/                 # 獨立的進階專案目錄
│   └── moe_routing_prefetch/          # 本專案目錄
│       ├── README.md                  # 專案獨立 README
│       ├── PROJECT_SPEC.md            # 本規格書
│       ├── Makefile                   # 專案編譯與執行入口
│       ├── docs/                      # 專案特定文檔
│       ├── python/                    # Python 程式碼
│       ├── traces/                    # 產生的 Trace 數據
│       ├── results/                   # 實驗結果與波形
│       ├── rtl/                       # RTL Verilog 程式碼
│       ├── tb/                        # Testbench Verilog 程式碼
│       └── scripts/                   # 自動化執行與編譯腳本
└── scripts/
```

---

## 7. File-by-file Specification

以下檔案路徑均相對於專案目錄 `advanced_projects/moe_routing_prefetch/`：

### 7.1 Python 檔案

---

#### 7.1.1 `python/generate_synthetic_moe_trace.py`

**檔案目的**: 產生可控的 synthetic MoE routing trace，用於 Python simulator 與 RTL testbench。

**輸入**:
- 命令列參數：
  - `--num_experts` (int, default=8): Expert 數量
  - `--top_k` (int, default=2): 每個 token 選中的 expert 數量
  - `--num_layers` (int, default=2): MoE layer 數量
  - `--num_tokens` (int, default=1000): Token 數量
  - `--distribution` (str, default="zipf"): 分布類型，可選 "uniform", "zipf", "phase_shift"
  - `--zipf_s` (float, default=1.2): Zipf 分布參數
  - `--phase_length` (int, default=200): Phase shift 中每個 phase 的長度
  - `--seed` (int, default=42): Random seed
  - `--output` (str): 輸出 CSV 路徑
  - `--output_hint` (str): 輸出 Hint CSV 路徑

**輸出**:
- CSV 檔案，格式遵循 Section 8 Trace Format Specification
- Hint CSV 檔案，格式遵循 Section 8.2 Hint Trace 格式
- stdout 輸出 trace 統計摘要

**主要函式**:
- `generate_uniform_trace(num_experts, top_k, num_tokens) -> List[Dict]`
- `generate_zipf_trace(num_experts, top_k, num_tokens, zipf_s) -> List[Dict]`
- `generate_phase_shift_trace(num_experts, top_k, num_tokens, phase_length) -> List[Dict]`
- `generate_prefetch_hints(trace_records, prefetch_distance) -> List[Dict]`
- `write_trace_csv(trace_records, output_path)`
- `write_hint_csv(hint_records, output_path)`

**參數**:
- `token_size_bytes`: 固定為 hidden_dim * 2 (FP16)，default=256 bytes
- `expert_weight_size_bytes`: 固定為 ffn_dim * hidden_dim * 2 * 2 (兩個 linear layer, FP16)，default=262144 bytes (256KB)

**預期行為**:
- Uniform 分布：每個 expert 被選中的機率相同
- Zipf 分布：少數 hot expert 被頻繁選中，多數 cold expert 很少被選中
- Phase shift：每隔 `phase_length` 個 token，hot expert set 發生切換
- 自動根據 `prefetch_distance` 產生預測性的 trace-driven prefetch hint

**測試方式**:
- 驗證輸出 CSV 的欄位完整性
- 驗證 uniform 分布下各 expert 頻率差異 < 10%
- 驗證 Hint CSV 中的 `target_cycle` 與 `hint_cycle` 間隔符合 `prefetch_distance`

---

#### 7.1.2 `python/generate_toy_moe_trace.py`

**檔案目的**: 使用 PyTorch 建構小型 MoE model，透過可控 bias 參數進行 forward pass 並收集 routing trace。

**輸入**:
- 命令列參數：
  - `--hidden_dim` (int, default=128): Hidden dimension
  - `--ffn_dim` (int, default=512): FFN intermediate dimension
  - `--num_experts` (int, default=8): Expert 數量
  - `--top_k` (int, default=2): top-k routing
  - `--num_layers` (int, default=2): MoE layer 數量
  - `--batch_size` (int, default=4): Batch size
  - `--seq_len` (int, default=128): Sequence length
  - `--num_batches` (int, default=10): 執行的 batch 數量
  - `--device` (str, default="cpu"): "cpu" or "cuda"
  - `--seed` (int, default=42): Random seed
  - `--router-bias-mode` (str, default="hot_expert"): 可選 `--router-bias-mode {uniform,hot_expert,temporal,layer_bias}`
  - `--router-temperature` (float, default=1.0): 調整 softmax 偏斜度 (越低越偏斜)
  - `--input-locality-strength` (float, default=0.5): 控制 input 隨時間重複的強度
  - `--output` (str): 輸出 CSV 路徑
  - `--output_hint` (str): 輸出 Hint CSV 路徑

**輸出**:
- CSV 檔案，格式遵循 Section 8 Trace Format Specification
- Hint CSV 檔案，格式遵循 Section 8.2 Hint Trace 格式
- stdout 輸出模型大小、routing 分布摘要

**主要函式/類別**:
- `class ToyMoERouter(nn.Module)`: Router 網路 (隨機初始化，但 forward 時套用 bias-mode)
- `class ToyExpert(nn.Module)`: 單一 expert FFN
- `class ToyMoELayer(nn.Module)`: 單 layer MoE (router + experts)
- `class ToyMoEModel(nn.Module)`: 多 layer MoE model
- `def collect_routing_trace(model, input_data, bias_config) -> Tuple[List[Dict], List[Dict]]`

**參數**:
- Router 使用 `nn.Linear(hidden_dim, num_experts)`
- 這些 bias 參數是教學用，用於產生可觀察的 routing locality，不得宣稱其代表真實大型 LLM routing 行為。

**預期行為**:
- 模型在不需訓練下執行 forward pass
- 根據 `router-bias-mode` 套用對應的偏置 (例如 `hot_expert` 將固定對某些 expert 增加偏置 logits；`temporal` 隨時間輪動偏置)
- 產生的 routing trace 呈現可控的 locality skew，以便評估 prefetch 效益

**測試方式**:
- 驗證模型記憶體使用量 < 1GB
- 驗證 hot_expert 模式下被選中頻率最高的 expert 是否符合預設的偏置 expert
- 驗證輸出 CSV 的欄位完整性

---

#### 7.1.3 `python/analyze_trace.py`

**檔案目的**: 讀取 trace CSV，計算統計指標並產生視覺化圖表。

**輸入**:
- 命令列參數：
  - `--trace` (str): 輸入 trace CSV 路徑
  - `--output_dir` (str, default="results/"): 輸出目錄

**輸出**:
- `{output_dir}/expert_frequency.png`: Expert activation frequency bar chart
- `{output_dir}/activation_heatmap.png`: Expert activation heatmap
- `{output_dir}/reuse_distance_histogram.png`: Reuse distance distribution
- `{output_dir}/trace_statistics.json`: 統計指標 JSON

**主要函式**:
- `load_trace(csv_path) -> pd.DataFrame`
- `compute_expert_frequency(df) -> pd.Series`
- `compute_reuse_distance(df) -> Dict[int, List[int]]`
- `plot_activation_heatmap(df, output_path)`
- `export_statistics(stats_dict, output_path)`

**預期行為**:
- 正確解析 Section 8 定義的 CSV 格式
- 量化評估該 trace 的 temporal locality 與 popularity skew 指標

**測試方式**:
- 驗證 JSON 輸出包含所有必要欄位且圖表成功產生

---

#### 7.1.4 `python/prefetch_policies.py`

**檔案目的**: 定義各種 cache/prefetch policy 的實作。

**主要類別**:
- `NoCachePolicy`: 無 cache，每次 access 均為 miss。
- `StaticHotExpertPolicy`: 固定常駐在 profiling 階段統計出的 hot experts。
- `FIFOPolicy`: 傳統 FIFO 替換策略。
- `LRUPolicy`: 傳統 LRU 替換策略。
- `RoutingAwareNextPolicy`: **本專案核心實作策略**。使用 trace-driven prefetch hint。當 simulator 處理到 `cycle` 時，若 hint trace 中有 `hint_cycle <= cycle` 且 `hint_score >= threshold`，則提前發出對該 expert 的 prefetch。
- `OracleFutureWindowPolicy`: **理想上限，不可硬體實作**。使用未來 W 個 requests 的 expert_id，做最優替換與 prefetch 決策。

**測試方式**:
- 使用已知 sequence 驗證 LRU, FIFO 以及 prefetch 觸發機制正確。

---

#### 7.1.5 `python/expert_cache_sim.py`

**檔案目的**: Cache simulator 主體，讀取 trace 與 hint trace，進行模擬並收集 metrics。

**輸入**:
- 命令列參數：
  - `--trace` (str): 輸入 trace CSV 路徑
  - `--hint_trace` (str): 輸入 hint trace CSV 路徑
  - `--policy` (str): Policy 名稱
  - `--cache_size` (int): Cache 可容納的 expert 數量
  - `--dma_latency` (int, default=100): DMA 搬運延遲
  - `--score_threshold` (float, default=0.1): prefetch 的 score 門檻
  - `--output` (str): 輸出結果 CSV 路徑

**預期行為**:
- 逐 cycle 模擬。對每筆 request 判斷是否在 cache (metadata hit/miss)
- 處理對應的 prefetch hint：若 policy 為 `routing_aware_next`，依據 hint trace 內相應 cycle 的 hint 資訊在可用頻寬下提前發起 DMA 模擬。
- 同步實作與 RTL 一致的 DMA 仲裁與 duplicate prefetch 排除機制。

**測試方式**:
- 驗證 no_cache 下 hit_rate = 0，miss_count = trace 長度。

---

#### 7.1.6 `python/latency_model.py`

**主要函式**:
- `estimate_total_latency(events, config) -> int`
- 模擬 DMA bandwidth contention，計算 prefetch hit (完全隱藏)、partial hit (部分隱藏) 與 cold miss (完全未隱藏) 的延遲。

---

#### 7.1.7 `python/run_experiments.py`

**主要函式**:
- 批量執行所有 policy 與參數組合，產生比較圖表。

---

#### 7.1.8 `python/trace_to_testbench.py`

**檔案目的**: 將 trace CSV 與 hint trace CSV 轉換為 Verilog testbench 的 stimulus 檔案。

**輸出**:
- `stimulus_req.hex`: 包含 `cycle` 與 `expert_id`，用於模擬 demand request。
- `stimulus_hint.hex`: 包含 `cycle`、`hint_expert_id` 與 `hint_score`，用於驅動 prefetch controller。

---

### 7.2 RTL 檔案

> **重要設計限制**: 本 RTL 第一版只建模 expert cache metadata，不儲存真實 expert weight data。Cache fill 代表 tag/metadata fill，不代表實際權重搬入 BRAM/DRAM。

---

#### 7.2.1 `rtl/expert_request_queue.v`

**功能**: FIFO queue，暫存來自上游的 expert demand request。

---

#### 7.2.2 `rtl/expert_cache_tag_array.v`

**功能**: 維護 fully-associative cache 的 tag 陣列，並追蹤哪些 entry 是由 prefetch 載入且尚未被實際 access 的。

**輸入輸出**:
| 信號 | 方向 | 寬度 | 說明 |
|------|------|------|------|
| clk | input | 1 | System clock |
| rst_n | input | 1 | Active-low reset |
| lookup_valid | input | 1 | Lookup request valid |
| lookup_expert_id | input | EXPERT_ID_W | Expert ID to lookup |
| lookup_ready | output | 1 | Can accept lookup |
| lookup_result_valid | output | 1 | Result valid |
| lookup_hit | output | 1 | Cache hit |
| lookup_miss | output | 1 | Cache miss |
| lookup_hit_way | output | WAY_W | Hit way index |
| lookup_hit_prefetched | output | 1 | Hit on a prefetch-loaded entry (useful prefetch) |
| fill_valid | input | 1 | Cache fill request |
| fill_ready | output | 1 | Tag array ready to fill (1st version always 1) |
| fill_expert_id | input | EXPERT_ID_W | Expert ID to fill |
| fill_way | input | WAY_W | Way to fill |
| fill_is_prefetch | input | 1 | 1=This fill is triggered by prefetch |
| evict_valid | output | 1 | Asserted when an entry is evicted |
| evict_prefetched | output | 1 | 1=The evicted entry was prefetched but never accessed (useless prefetch) |

**時序行為**:
- 內部為每個 way 維護 `valid`、`tag` (expert_id) 與 `prefetched` (1 bit) 暫存器。
- 本教學版 tag array 的 fill 介面採用 valid/ready。`fill_fire = fill_valid && fill_ready`。第一版中 `fill_ready` 恆為 1，代表 tag metadata fill 可在單 cycle 接受。
- `fill_fire` 發生時，同 cycle 更新指定 way 的 tag、valid 與 prefetched bit。
- 若被覆寫的 way 原本 `valid == 1` 且 `prefetched == 1`，則 `evict_valid` 與 `evict_prefetched` 在 `fill_fire` 同 cycle assert。
- 當 `lookup_valid && lookup_hit` 且該 way 的 `prefetched == 1`：
  - 輸出 `lookup_hit_prefetched = 1`。
  - **重要**: 同時在下一個 clock cycle 將該 way 的 `prefetched` 清為 0 (因為該 prefetch 已經被成功 demand access，不再是 "unused")。

---

#### 7.2.3 `rtl/fifo_replacement.v`

**功能**: 提供 FIFO 替換指標。

---

#### 7.2.4 `rtl/lru_replacement.v`

**功能**: 提供 LRU 替換指標。

---

#### 7.2.5 `rtl/expert_prefetch_controller.v`

**功能**: 接收 trace-driven prefetch hint，在滿足 score threshold 且 cache miss 的情況下向 DMA model 發出預取請求。

**輸入輸出**:
| 信號 | 方向 | 寬度 | 說明 |
|------|------|------|------|
| clk | input | 1 | System clock |
| rst_n | input | 1 | Active-low reset |
| prefetch_enable | input | 1 | Enable prefetch |
| prefetch_expert_id | input | EXPERT_ID_W | Expert ID from hint |
| prefetch_score | input | SCORE_W | Router score from hint |
| prefetch_valid | input | 1 | Prefetch hint valid |
| score_threshold | input | SCORE_W | Score threshold |
| cache_hit | input | 1 | Already in cache |
| dma_busy | input | 1 | DMA channel busy |
| prefetch_issue | output | 1 | Issue prefetch to DMA |
| prefetch_target | output | EXPERT_ID_W | Target expert ID |
| prefetch_issued_count | output | CNT_W | Counter: total prefetch issued |
| prefetch_filtered_count | output | CNT_W | Counter: prefetch filtered |

---

#### 7.2.6 `rtl/simple_dma_model.v`

**功能**: 模擬單通道固定延遲之 DMA 載入。

---

#### 7.2.7 `rtl/top_moe_prefetch_system.v`

**功能**: 頂層整合模組。

**特別設計限制**:
- `replacement_policy_sel` 僅允許在 `rst_n` 期間或無 active request 處理時設定，不允許在運作中動態切換，以避免 metadata 狀態不一致。

---

## 8. Trace Format Specification

### 8.1 Demand Request Trace (CSV)

| 欄位 | 資料型態 | 說明 |
|------|----------|------|
| `cycle` | int | 該 request 發生的虛擬 cycle 序號 |
| `request_id` | int | 唯一的 request 識別碼，從 0 開始遞增 |
| `layer_id` | int | MoE layer 編號 (0-indexed) |
| `token_id` | int | Token 在 batch 中的編號 |
| `topk_rank` | int | 該 expert 在此 token 的 top-k ranking (0 = 最高分) |
| `expert_id` | int | 被選中的 expert ID (0-indexed) |
| `router_score` | float | Softmax 後的 router score (0.0 ~ 1.0) |
| `token_size_bytes` | int | Token activation 大小 (bytes) |
| `expert_weight_size_bytes` | int | Expert weight 大小 (bytes) |
| `source` | str | Trace 來源標記 (`synthetic`, `toy_pytorch`) |

### 8.2 Prefetch Hint Trace (CSV)

為了支持 `routing_aware_next` policy 在 simulator 與 RTL testbench 中的前瞻觸發，本專案定義獨立的 Hint Trace 格式：

| 欄位 | 資料型態 | 說明 |
|------|----------|------|
| `hint_cycle` | int | Prefetch hint 發出的時間戳。比 `target_cycle` 提前一個 `prefetch_distance` |
| `target_cycle` | int | 預期該 expert 會被實際 demand request access 的 cycle |
| `layer_id` | int | MoE layer 編號 |
| `token_id` | int | Token 編號 |
| `hint_expert_id` | int | 建議預取的 Expert ID |
| `hint_score` | float | 該預期 Expert 的 router score |
| `hint_source` | str | Hint 產生來源 (`toy_router_score`, `synthetic_scripted_hint`, `synthetic_future_offset`, `oracle_debug_only`) |

- **教學用抽象與合法性說明**: 
  1. `trace-driven hint` 僅用於模擬「某個前級 router/runtime 能提前送出 hint」的介面行為，此設定是教學用抽象，不宣稱等同真實 MoE 系統的實體暫存器通訊。
  2. 它與 `oracle_future_window` 不同。
  3. **重要限制**：若 hint 是由 future target 反推生成 (如 `synthetic_future_offset`)，則**只可用於教學、RTL 驗證與 debug，不可作為主要效能宣稱**。若要宣稱 routing-aware prefetch 的真實可部署性與實際效益，必須使用當前 cycle 或前級 pipeline 中可取得的 router score/hint (如 `toy_router_score`)。

---

## 9. Toy PyTorch MoE Model Specification

### 9.1 規格與可控 Bias 參數

為了讓隨機初始化的 toy MoE 模型能產生有意義的、具備局部性 (locality) 的 routing trace，必須支援以下參數設定：

1. `--router-bias-mode {uniform,hot_expert,temporal,layer_bias}`
   - `uniform`: 不額外加入 router bias，維持隨機均勻分布。
   - `hot_expert`: 固定偏置少數 expert，為其加上較大的 logits bias，模擬 popularity skew。
   - `temporal`: 偏置 expert 隨時間或 phase 改變，模擬 phase-shift。
   - `layer_bias`: 不同 layer 使用不同偏置 expert set。
2. `--router-temperature`
   - 控制 softmax 分布。低溫度會讓 router score 非常集中 (高 skew)，高溫度則讓 score 分布趨向均勻。
3. `--input-locality-strength`
   - 控制 input sequences 的時間重複性，強度越高則前後 token 的 activation features 越相似，進而產生具備高度 temporal locality 的 routing decisions。

*備註: 這些參數皆為教學用途，目的在於產生可觀察的 routing locality 以供驗證。不得宣稱其代表真實大型 LLM routing 行為。*

---

## 10. Python Simulator Specification

### 10.1 Policy 定義與硬體可行性

| Policy | 核心概念 | 需要的資訊 | 未來資訊 | 硬體可行性 | 優缺點 |
|--------|----------|------------|----------|------------|--------|
| `no_cache` | 無 cache | 無 | 否 | 是 (Baseline) | 無 metadata 複雜度；延遲極高 |
| `static_hot_expert` | 靜態常駐 hot experts | 離線 profile 結果 | 否 | 是 | 實作簡單；無法適應 dynamic phase shifts |
| `FIFO` | 先進先出 | 載入順序 | 否 | 是 | 硬體成本極低；易發生 thrashing |
| `LRU` | 最近最少使用 | 存取歷史 recency | 否 | 是 | 善用 temporal locality；硬體比較器成本較高 |
| `routing_aware_next` | **Hint-driven prefetch** | 當下可得 trace hint | 否 (以當下 hint 為主) | 是 (RTL核心) | 提前隱藏延遲；若預測不準會浪費頻寬 |
| `oracle_future_window` | **理想上限 (OPT)** | 未來 W 個 requests | 是 | **否 (僅模擬器評估)** | 提供效能上限基準；不可於真實硬體實現 |

- **Hint 來源合法性區分**：
  - `routing_aware_hint`：可實作策略，但必須依賴當下或前級 pipeline 中可取得的 hint (例如 `toy_router_score`, `synthetic_scripted_hint`)。
  - `synthetic_future_offset`：教學/debug hint，由未來 target 倒推生成，僅能用於 RTL/debug 驗證，不可作為主要效能宣稱。
  - `oracle_future_window`：理想上限，不可硬體實作。

### 10.2 DMA 仲裁與 Duplicate Prefetch 規則

為確保 Python simulator 與 RTL 模擬行為完全一致，必須實作以下核心規則：

1. **DMA Arbitration Policy (DMA 仲裁規則)**:
   - **優先權**：demand miss DMA 的優先權高於 prefetch DMA。
   - **不可搶占**：prefetch DMA 一旦開始傳輸，不可被後續發生的 demand miss 搶占 (non-preemptive)。
   - **同 cycle 衝突**：若 demand miss 與 prefetch 在同 cycle 同時發生，優先處理 demand miss。
   - **Busy Drop**：若 DMA 處於 busy 狀態，新來的 prefetch hint 將直接被 **drop (丟棄)**，不進行佇列 stall。
   - 被 drop 的 prefetch 計入 `prefetch_dropped_count` / `cnt_prefetch_dropped`。
2. **Duplicate Prefetch Handling (重複預取處理規則)**:
   - 若目標 expert 已經在 cache 中，不發出 prefetch。
   - 若目標 expert 目前正由 demand miss DMA 載入中，不發出 prefetch。
   - 若目標 expert 目前正由 prefetch DMA 載入中，不重複發出 prefetch。
   - 在第一版 single DMA channel 下，硬體可使用一個 `in_flight_expert_id` 與 `in_flight_valid` 來追蹤目前傳輸中的目標 expert。

---

## 11. RTL Architecture Specification

### 11.1 metadata-only expert cache 說明

本 RTL 第一版只建模 expert cache metadata。不儲存真實 expert weight data。
- **Cache Hit / Miss**: 僅基於 tag array 的 valid 與 tag 比較。
- **Cache Fill**: 僅為 metadata/tag 的覆寫。
- **DMA Model**: 僅以 busy/ready 及 latency cycle 模擬傳輸時間，不進行實際權重 byte 的數據搬移。
- **未來延伸 (FPGA上板)**: 若未來要進行實體 FPGA 驗證，需於此架構外增設 BRAM/URAM/DDR 作為實際 weight storage、AXI DMA 控制器以及 AXI-Stream weight 傳輸通道。

### 11.2 `expert_cache_tag_array` 與 Useful/Useless Prefetch 追蹤

為了在硬體中準確記錄預取的效益，`expert_cache_tag_array` 需特別維護一組 `prefetched` 暫存器陣列 (與 way 數相同，每個 way 1 bit)。

1. **Useful Prefetch 判定**:
   當下游發送 `lookup_valid` 且匹配成功 (`lookup_hit = 1`)，若此時該 way 的 `prefetched` 為 1，則 assertion 輸出 `lookup_hit_prefetched = 1`。隨後在下一 clock cycle 將該 way 的 `prefetched` 清為 0。這代表該 prefetch 成功被 demand access 命中。
2. **Useless Prefetch 判定**:
   當 `fill_valid` 觸發，且替換演算法選中了 `fill_way` 進行覆寫。若該被覆寫的 entry 的 `valid` 為 1 且其 `prefetched` 暫存器仍為 1 (代表被預取進來後從未被實際 lookup 命中過)，則 assertion 輸出 `evict_valid = 1` 且 `evict_prefetched = 1`。

這些訊號將直接被 `top_moe_prefetch_system` 的 counter 模組捕獲，以精確更新 `cnt_prefetch_hit` 與 `cnt_prefetch_useless`。

---

## 12. Experiments and Assignments

以下為教學實驗規劃：

1. **Cache Size Sweep**: 探討在 Zipf 分布下，增加 way 數對 hit rate 的敏感度。
2. **FIFO vs LRU**: 比較兩者在 Phase-shift 下的 cache thrashing 行為。
3. **Static Hot Expert vs Dynamic Routing-aware Prefetch**: 驗證當 `router-bias-mode = temporal` (時變分布) 時，靜態配置與動態 prefetch 的效能差距。
4. **Prefetch Distance Sweep**: 調整 hint 提前距離，觀察 usefulness 與 latency 隱藏的折衷。
5. **Memory Latency Sweep**: 模擬不同 DRAM 載入延遲下 prefetch 的改進比例。
6. **Router Score Threshold Sweep**: 探討 score threshold 對 prefetch accuracy 與 bandwidth limit 的影響。
7. **Prefetch Queue Depth (DMA Busy) Limit & Dropped rate**: 觀察頻寬受限時，prefetch 被 drop 的機率 (`prefetch_dropped_count`) 與系統效能之關係。
8. **Hot Expert Ratio Sensitivity**: 調整 Zipf `zipf_s` 參數，觀察分布越傾斜時 prefetch 是否有更高的準確度。
9. **Locality Strength Sensitivity**: 改變 `input-locality-strength` 觀察其對 cache hit rate 的直接影響。
10. **RTL Counter Correctness**: 載入 `stimulus_req.hex` 與 `stimulus_hint.hex` 運行 RTL 模擬，驗證 RTL 統計暫存器 (含 `cnt_prefetch_dropped`, `cnt_prefetch_useless`) 與 Python 模擬結果完全一致。

---

## 13. Acceptance Criteria

### 驗收問與答 (v1.2 - 進階專案版)

1. **為什麼這題不是矩陣乘法加速器？**  
   矩陣乘法已有成熟硬體方案。本專案聚焦於 MoE 的 memory-bound 瓶頸，探討 expert weight loading 的延遲隱藏，屬於跨層 (軟體 router score 至硬體 prefetch controller) 的共同設計。
2. **為什麼 RTX 3050 6GB 足夠完成教學版？**  
   本專案使用 toy model 且只在 RTL 層面建模 cache metadata，記憶體使用量低 (<100MB)，無大型 LLM 載入需求。
3. **哪些 trace 是必要，哪些是選配？**  
   必要：Synthetic trace 與 Toy PyTorch MoE trace，且皆須搭配對應的 Prefetch Hint Trace。選配：大型實體模型之 trace。
4. **Python simulator 要比較哪些 policy？**  
   no_cache, static_hot_expert, FIFO, LRU, routing_aware_next (基於 hint), 以及 oracle_future_window (上限基準)。
5. **RTL 需要有哪些模組？**  
   Request queue、Cache tag array (包含 prefetched flag 追蹤與統一的 lookup/fill/evict 命名)、FIFO/LRU replacement 選擇器、Prefetch controller、Simple DMA model 以及頂層系統。
6. **每個模組如何測試？**  
   編寫三個 testbench (tag array, controller, top system)，以 trace-driven hex stimulus 驅動，比對 RTL counter 與 Python 輸出。
7. **哪些結果可以宣稱，哪些不可宣稱？**  
   *可以宣稱*: 在給定 locality trace 下各 prefetch 演算法與參數之相對優劣、使用當下可得 hint (`toy_router_score`) 的實用效益。  
   *不可宣稱*: Toy model 的行為等同真實巨型 MoE LLM、使用 future offset 反推生成的 debug hint 效能代表實際可部署效能、RTL 模擬週期等同 FPGA/ASIC 實體效能。
8. **若未來要接 gem5，應該從哪裡開始？**  
   自 `docs/gem5_modeling_plan.md` 規劃出發，實作 MMIO SimObject 與 memory region。
9. **若未來要接 FPGA，應該從哪裡開始？**  
   將 metadata-only tag array 與 BRAM weight storage 連結，並把 `simple_dma_model` 替換為真正的 AXI DMA IP。

---

## 14. Implementation Readiness Checklist

```text
[x] 研究問題已明確定義，且 non-gemm 聚焦說明清晰
[x] 專案範圍限制在 metadata-only 模擬，FPGA 上板列為選配
[x] 硬體限制已確認，Google Colab 不保證特定 GPU
[x] Toy PyTorch MoE 參數包含 bias-mode, temperature 等以控制 routing bias
[x] 統一 trace 與 hint trace CSV 格式已定義
[x] 預取策略 (routing_aware_next) 使用 trace-driven prefetch hint
[x] Oracle 策略明確標記為不可硬體實作之 Upper Bound
[x] expert_cache_tag_array.v 使用統一的 lookup/fill/evict 命名與 tracking 介面
[x] replacement_policy_sel 限定在 reset 或非 active 時段設定
[x] 訂定一致的 DMA 仲裁與 duplicate prefetch 處理規則
[x] 納入 cnt_prefetch_dropped 指標與實驗對照
[x] 十個教學實驗已規劃，包含 locality 與 counter 對照實驗
```

---

## 15. Revision Notes v1.2

本版本 (v1.2) 相較於 v1.1，進行了以下重大修正以確保專案與 HCSSimulation 整合的完整性與結構清晰度：

1. **HCSSimulation 進階專案定位**: 重新將本專案設定為 HCSSimulation 根目錄 `advanced_projects/` 下的獨立進階整合專案，與既有的入門級實驗及 `dir/TRY/` 舊方向做明確區隔，禁止放入 `dir/` 或 `dir/TRY/` 下。
2. **目錄結構與路徑修正**: 修改 Repository Structure 與本專案中所有檔案路徑為相對於主目錄 `advanced_projects/moe_routing_prefetch/`。
3. **HCSSimulation README 更新要求**: 新增「HCSSimulation Integration Requirements」章節，要求在根目錄 `README.md` 中新增「進階專案」章節，且不得修改既有的方向一至三（dir1_memory_dma, dir2_moe_routing, dir3_kv_cache）與 gem5 入門實驗。於 `docs/` 下新增 `advanced_moe_routing_prefetch_tutorial.md`。
4. **v1.1 殘留文字修正**: 修正了 `hidden_dim=128 或 256`、`num_experts=4 到 16`、將 `co-design movement` 改為 `軟硬體共同設計問題`，以及修正 `synthetic workload 普遍用於` 之文字錯誤。
5. **Trace-driven Hint 合法性說明**: 明確區分可部署策略 (`toy_router_score`) 與僅用於教學/debug 的反推 hint (`synthetic_future_offset`)，限制後者不得作為主要效能宣稱。
6. **Hint Source 分類重整**: 重新定義 `hint_source` 類別為：`toy_router_score`, `synthetic_scripted_hint`, `synthetic_future_offset`, `oracle_debug_only`。
7. **Toy Model 參數格式修正**: 修正 `--router-bias-mode {uniform,hot_expert,temporal,layer_bias}` 參數的表示格式，並對各 mode 提供清晰定義。
8. **統一 Tag Array I/O 命名**: 將 RTL 中的所有信號變數統一規範為 `lookup_`、`fill_`、`evict_` 等前綴，以杜絕名稱混用。
9. **定義 Tag Array Fill 時序**: 明確定義 `fill_fire` 發生時的 metadata 更新與 eviction assertion 發生的單 cycle 時序行為。
10. **定義 DMA 仲裁機制**: 增設 demand miss 優先、非搶占、忙碌時 prefetch 直接 drop 等 DMA arbitration 策略，確保 simulator 與 RTL 行為完全一致。
11. **定義 Duplicate Prefetch Handling**: 設計單通道 DMA 機制下使用 `in_flight_expert_id` 暫存器等排除重複預取的規則。
12. **新增 Prefetch Dropped Counter**: 在指標、Counter 與實驗觀察中引入 `cnt_prefetch_dropped`，以真實呈現高頻寬壓力下預取 hint 被丟棄的狀況。
