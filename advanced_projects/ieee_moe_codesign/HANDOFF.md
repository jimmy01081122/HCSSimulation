# Gemini 3.5 Flash 接手指南 (Handoff Guide)

> **重要**：本文件為 Claude Opus 撰寫，用於指導 Gemini 3.5 Flash 接續完成
> IEEE 級研究論文的實施、實驗、撰寫工作。請嚴格遵循以下流程。

---

## 0. 上下文摘要

你正在 `/home/a/HCSSimulation/advanced_projects/ieee_moe_codesign/` 工作空間中，
執行一個面向 IEEE 頂級會議/期刊的異質 CPU-GPU MoE 推理排程軟硬體協同設計研究。

**關鍵約束：**
- ❌ 不得修改 `/home/a/HCSSimulation/` 下任何原本存在的檔案
- ✅ 所有新增程式碼僅存放在 `advanced_projects/ieee_moe_codesign/` 中
- ✅ 使用 Docker 管理工具環境
- ✅ gem5 二進位位於 `/home/a/gem5/build/ARM/gem5.opt`
- ✅ Docker image `hetero-moe-env:latest` 已可用

---

## 1. Phase P1: 模擬器重構與單元測試

### 目標
將先前 `hetero_moe_scheduling/simulator/` 中的程式碼重構為更嚴謹的版本，
支援 IEEE 論文所需的所有實驗維度。

### 具體步驟

```bash
# Step 1: 建立模擬器原始碼
# 將以下檔案寫入 simulator/src/ 目錄：
```

#### 1.1 `simulator/src/hardware_model.py`
- 從 `hetero_moe_scheduling/simulator/hardware_model.py` 獲取靈感
- **必須改進的地方**：
  - 增加 `pcie_dma_channels` 參數 (多通道 DMA)
  - 增加 `cpu_cores` 參數 (CPU 核心數限制)
  - PCIe tick() 中實現 bandwidth sharing (多通道均分頻寬)
  - `get_estimated_delay()` 需考慮通道並行

#### 1.2 `simulator/src/cache.py`
- 支援 LRU, LFU 淘汰策略
- **新增 ARC (Adaptive Replacement Cache)** 策略做 comparison
- 追蹤所有 eviction 與 insertion 事件以便統計

#### 1.3 `simulator/src/scheduler.py`
- 實現三種調度策略：
  - `fcfs`: First-Come-First-Served baseline
  - `elas`: Expert-Locality-Aware Scheduling
  - `f-elas`: F-ELAS with aging-based starvation prevention
- F-ELAS 的 aging 門檻 θ_starve 應為可配置參數

#### 1.4 `simulator/src/decision_engine.py`
- 實現三種決策策略：
  - `transfer`: 永遠搬移到 GPU
  - `offload`: 永遠在 CPU 執行
  - `dcmd`: 動態成本模型決策
- **DCMD 公式**:
```python
cost_transfer = pcie_queue_delay + num_tokens * gpu_expert_latency
cost_offload  = (cpu_queue_len / cpu_cores) * cpu_expert_latency \
              + num_tokens * cpu_expert_latency + cpu_transfer_latency
```

#### 1.5 `simulator/src/simulator.py`
- 整合所有元件的核心模擬引擎
- 使用離散事件模擬 (discrete-event simulation) 而非固定步長
- **輸出指標必須包含**:
  - `total_sim_time_ms`: 總模擬時間
  - `avg_latency_ms`: 平均請求延遲
  - `p50_latency_ms`: P50 延遲
  - `p99_latency_ms`: P99 延遲
  - `max_latency_ms`: 最大延遲
  - `gpu_hit_rate`: GPU 快取命中率
  - `cpu_offloads`: CPU 卸載次數
  - `pcie_transfers`: PCIe 傳輸次數
  - `gpu_utilization`: GPU 利用率
  - `cpu_utilization`: CPU 利用率

#### 1.6 `simulator/src/workload_gen.py`
- 使用 Zipf 分布生成專家需求
- 支援 Poisson 到達過程
- 參數化：num_requests, arrival_rate, num_experts, tokens_range, layers, zipf_alpha
- **必須設定隨機種子**以確保可復現性

#### 1.7 `simulator/tests/test_simulator.py`
- 使用 Python unittest 或 pytest
- **必須通過的測試案例**：
  1. 單一 request 在空快取下的正確狀態轉換
  2. Cache hit 時不觸發 PCIe 傳輸
  3. CPU offload 時不觸發 PCIe 傳輸
  4. F-ELAS aging 機制在正確的 tick 數後觸發
  5. DCMD 在 PCIe 空閒時選擇 TRANSFER
  6. DCMD 在 CPU 佇列為空時選擇 OFFLOAD (如果 CPU 更快)
  7. 統計計數器自洽性: `hits + misses == total_requests`

### 驗證方式
```bash
cd /home/a/HCSSimulation/advanced_projects/ieee_moe_codesign
python3 -m pytest simulator/tests/ -v
```

---

## 2. Phase P2: gem5 工作負載擴展

### 目標
在先前的 centralized/distributed queue 基礎上，新增 **lock-free queue** 實作，
以提供更完整的微架構比較。

### 具體步驟

#### 2.1 `gem5_workloads/src/lock_free_queue.cpp`
- 實作基於 CAS (Compare-And-Swap) 的無鎖佇列
- 使用 `__atomic_compare_exchange_n` 或 `__sync_bool_compare_and_swap`
- 靜態連結編譯為 ARM64：
```bash
aarch64-linux-gnu-g++ -static -O3 -o lock_free_queue_arm lock_free_queue.cpp
```

#### 2.2 `gem5_workloads/configs/se_moe_bench.py`
- 支援參數化的 L1/L2 cache 大小
- 支援命令列傳入 binary 路徑與參數
- 配置矩陣:
  - L1D: [32kB, 64kB, 128kB]
  - L2: [256kB, 512kB, 1MB]
  - Queue: [centralized, distributed, lock_free]

#### 2.3 `gem5_workloads/scripts/run_gem5_all.sh`
- 在 Docker 容器內執行所有 gem5 模擬組合
- 輸出至 `experiments/results/gem5/` 目錄
- 每個配置生成獨立的 `stats.txt`

#### 2.4 `gem5_workloads/scripts/parse_stats.py`
- 提取: simSeconds, simTicks, numCycles, ipc, dcache misses, l2 misses
- 輸出為 CSV 格式方便後續繪圖

### 驗證方式
```bash
# 在 Docker 中執行
docker run --rm \
  -v /home/a/HCSSimulation:/work \
  -v /home/a/gem5:/gem5 \
  hetero-moe-env \
  bash /work/advanced_projects/ieee_moe_codesign/gem5_workloads/scripts/run_gem5_all.sh
```

---

## 3. Phase P3: Trace 生成與驗證

### 具體步驟

#### 3.1 `traces/generate_traces.py`
- 生成多組 synthetic workload traces
- 控制變量: num_experts ∈ {8, 16, 32, 64}, zipf_alpha ∈ {0.8, 1.0, 1.2, 1.5, 2.0}
- 輸出 CSV 格式: `request_id, token_id, layer, expert_id, arrival_time_ms`
- 使用固定隨機種子 42

---

## 4. Phase P4: 完整 DSE 參數掃描

### 具體步驟

#### 4.1 實驗配置檔案
建立 YAML 配置檔案於 `experiments/configs/`:

```yaml
# experiments/configs/exp1_cache_sweep.yaml
experiment_name: "cache_capacity_sweep"
base_config:
  num_experts: 16
  expert_size_gb: 1.0
  pcie_bandwidth_gbs: 16.0
  cpu_expert_latency_ms: 4.0
  gpu_expert_latency_ms: 0.2
  gpu_dense_latency_ms: 0.5
  cpu_orch_latency_ms: 0.1
  cpu_cores: 8
  pcie_dma_channels: 2
  num_requests: 15
  arrival_rate: 0.15
  tokens_range: [16, 48]
  layers: 6
  zipf_alpha: 1.2
  random_seed: 42

sweep_variable: "gpu_cache_capacity"
sweep_values: [2, 4, 6, 8, 12, 16]

strategies:
  - {sched: "fcfs", offload: "transfer", name: "FCFS_TRANSFER"}
  - {sched: "elas", offload: "transfer", name: "ELAS_TRANSFER"}
  - {sched: "elas", offload: "offload",  name: "ELAS_OFFLOAD"}
  - {sched: "elas", offload: "dcmd",     name: "ELAS_DCMD"}
  - {sched: "f-elas", offload: "dcmd",   name: "F_ELAS_DCMD"}
```

#### 4.2 `experiments/scripts/run_single_exp.py`
- 讀取 YAML 配置檔案
- 執行模擬並輸出 CSV 結果至 `experiments/results/`

#### 4.3 `experiments/scripts/run_all_experiments.sh`
- 循序執行所有 5 組實驗
- 最後呼叫 `generate_all_figures.py` 產出圖表

### 驗證方式
```bash
cd /home/a/HCSSimulation/advanced_projects/ieee_moe_codesign
python3 experiments/scripts/run_single_exp.py --config experiments/configs/exp1_cache_sweep.yaml
```

---

## 5. Phase P5: gem5 微架構驗證

與 Phase P2 的腳本一起執行。重點是收集以下比較數據：

| Queue Design    | Cycles       | IPC   | D$ Misses | L2 Misses |
|-----------------|-------------|-------|-----------|-----------|
| Centralized     | (from gem5) |       |           |           |
| Distributed     | (from gem5) |       |           |           |
| Lock-free       | (from gem5) |       |           |           |

---

## 6. Phase P6: 圖表生成

### 必須生成的圖表 (共 8 張)

#### 6.1 `experiments/figures/generate_all_figures.py`

| Figure # | 內容                              | 圖表類型            |
|----------|-----------------------------------|---------------------|
| Fig.1    | System Architecture Diagram       | 手繪 or Tikz        |
| Fig.2    | Cache Capacity vs. SimTime        | Line plot (5 lines) |
| Fig.3    | PCIe Bandwidth vs. SimTime        | Line plot (5 lines) |
| Fig.4    | Concurrency Scaling               | Line plot           |
| Fig.5    | Zipf α Sensitivity                | Line plot           |
| Fig.6    | CPU Core Scaling                  | Line plot           |
| Fig.7    | CDF of Per-Request Latency        | CDF plot            |
| Fig.8    | gem5 Queue Microarch Comparison   | Grouped bar chart   |

### 繪圖風格要求 (IEEE 標準)
- 字體: 8-10pt, sans-serif
- 圖片寬度: single column (3.5 in) 或 double column (7 in)
- DPI: ≥ 300
- 顏色: 使用色盲友善色板 (colorblind-safe)
- 圖例位置: 圖內或圖上方
- 存檔格式: PDF 用於 LaTeX, PNG 用於預覽

---

## 7. Phase P7: 論文撰寫

### 7.1 英文論文 (`paper/ieee_en/main.tex`)

使用 IEEEtran.cls 模板。每一節的寫作要點：

#### Abstract (≤150 words)
- 問題: MoE inference + multi-agent serving bottleneck
- 方法: HEXA-MoE framework (F-ELAS + DCMD-Opt)
- 結果: XX% latency reduction, YY% fairness improvement
- 結論: first comprehensive co-design framework

#### I. Introduction
- Para 1: LLM scaling with MoE (cite [1-3])
- Para 2: Multi-agent serving creates concurrency (cite [8])
- Para 3: Three bottlenecks (PCIe, CPU orchestration, CPU compute)
- Para 4: Our contributions (4 bullet points)
- Para 5: Paper organization

#### II. Background and Motivation
- MoE architecture (top-k gating, sparse activation)
- Heterogeneous deployment (GPU HBM + CPU DRAM)
- Motivating example: timing diagram showing 3 concurrent requests

#### III. HEXA-MoE Framework
- III-A: System architecture overview (Fig.1)
- III-B: F-ELAS algorithm (Algorithm 1 pseudocode)
- III-C: DCMD-Opt cost model (Equations)
- III-D: Distributed Expert-Affinity Queue

#### IV. Evaluation
- IV-A: Experimental setup (Table I: default parameters)
- IV-B–F: One subsection per experiment
- Each subsection: setup → result → analysis → insight

#### V. Related Work
- Expert parallelism: Tutel [2], DeepSpeed-MoE [3]
- Offloading: MoE-Offloading [4], MoE-Infinity [5], FlexGen [9]
- Scheduling: vLLM [8], Orca
- gem5 simulation: [7]

#### VI. Conclusion
- Summary of findings
- Limitations (synthetic workload, no real GPU)
- Future work (FPGA prototype, real trace from HuggingFace)

### 7.2 中文翻譯 (`paper/ieee_zh/main_zh.md`)
- 全文中文翻譯，使用 Markdown 格式
- 包含所有圖表引用
- 保留原始英文專有名詞 (如 MoE, PCIe, gem5)

### 7.3 參考文獻 (`paper/bib/references.bib`)
- 至少 30 筆參考文獻
- 使用標準 BibTeX 格式
- 確保所有 citation key 與論文中一致

---

## 8. Phase P8: 品質審查清單

在提交前，確認以下所有項目：

- [ ] 所有實驗可透過 `make all` 一鍵復現
- [ ] 所有圖表為 300 DPI 以上
- [ ] 論文不超過 8 頁 (含參考文獻)
- [ ] 每個圖表都有 caption 和 cross-reference
- [ ] 所有數字精確到小數點後一位
- [ ] README.md 中的數據與論文中的數據一致
- [ ] 參考文獻格式符合 IEEE 標準
- [ ] 程式碼有足夠的中英文註釋
- [ ] Docker 容器可在全新環境中成功建置
- [ ] gem5 模擬結果可在 Docker 中復現

---

## 9. 執行指令速查表

```bash
# 建立 Docker 環境
cd /home/a/HCSSimulation/advanced_projects/ieee_moe_codesign
docker build -t ieee-moe-env -f docker/Dockerfile .

# 執行模擬器單元測試
python3 -m pytest simulator/tests/ -v

# 執行單一實驗
python3 experiments/scripts/run_single_exp.py \
  --config experiments/configs/exp1_cache_sweep.yaml

# 執行所有實驗 + 繪圖
bash experiments/scripts/run_all_experiments.sh

# 執行 gem5 微架構驗證
docker run --rm \
  -v /home/a/HCSSimulation:/work \
  -v /home/a/gem5:/gem5 \
  hetero-moe-env \
  bash /work/advanced_projects/ieee_moe_codesign/gem5_workloads/scripts/run_gem5_all.sh

# 生成圖表
python3 experiments/figures/generate_all_figures.py
```

---

## 10. 常見問題 (FAQ)

**Q: gem5 在 host 上無法直接執行？**
A: gem5 需要 Python 3.12 的動態庫，必須在 Docker 容器 (`hetero-moe-env`) 中執行。
使用 `-v /home/a/gem5:/gem5` 掛載。

**Q: 如何確保不修改原始專案檔案？**
A: 所有檔案只在 `advanced_projects/ieee_moe_codesign/` 目錄下建立。
在每次操作前確認 `pwd` 在正確目錄下。

**Q: 先前的 `hetero_moe_scheduling` 結果可以引用嗎？**
A: 可以作為 preliminary results 引用，但論文中的所有圖表必須由
`ieee_moe_codesign/` 中的程式碼重新生成，以確保可復現性。

**Q: Makefile 中的 `make all` 應該做什麼？**
A: 依序執行: 建立 Docker → 編譯 gem5 workloads → 跑 5 組 DSE 實驗 →
跑 gem5 模擬 → 產出所有圖表 → 輸出報告。
