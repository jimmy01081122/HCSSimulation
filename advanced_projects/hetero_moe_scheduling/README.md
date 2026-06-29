# 異質 CPU-GPU MoE 推理協同設計研究 (Heterogeneous CPU-GPU MoE Inference Co-Design Study)

本目錄包含了一個獨立的研究專案：**異質 CPU-GPU MoE 推理任務調度與執行決策軟硬體協同設計**。
專案旨在探討在大模型混合專家架構 (Mixture-of-Experts, MoE) 推理中，如何透過軟硬體協同設計（Software/Hardware Co-design），解決多 Request Stream 下的 PCIe 傳輸延遲與 CPU-GPU 資源利用率瓶頸。

---

## 1. 研究背景與瓶頸分析

隨著混合專家模型 (MoE，如 Mixtral-8x7B) 的普及，模型參數量的急劇增加使得單一 GPU 的顯示記憶體 (HBM) 無法容納所有專家權重 (Expert Weights)。因此，業界常採用**異質儲存架構**：
- **GPU HBM**：快取熱門專家 (Cached Experts) 與儲存 Dense Transformer 層。
- **CPU 主記憶體 (Host Memory)**：儲存其餘專家 (Missing Experts) 的權重。

在多個 Agent/Request 同時觸發多條 MoE Inference Stream 的高並發場景下，會引發嚴重的系統級瓶頸：
1. **PCIe 頻寬瓶頸 (PCIe Contention)**：當 GPU 發生 Expert Cache Miss 時，若選擇將專家權重從 CPU 主記憶體載入 GPU (Transfer)，需要花費數十至數百毫秒 (PCIe Gen4 x16 約 16 GB/s，傳輸 2 GB 專家權重需 125 ms)。當多條 stream 同時 miss，PCIe 匯流排會發生嚴重的排隊阻塞。
2. **CPU 計算瓶頸**：若選擇不搬運權重，而是將 Token 傳回 CPU 執行 (CPU Offload)，雖然避免了 PCIe 搬運延遲，但 CPU 運算力 (FLOPS) 比 GPU 弱 10~50 倍。如果 CPU 分配了太多 token 計算，CPU 本身將成為新的系統瓶頸。
3. **CPU Orchestration 開銷**：CPU 負責 Request Queueing、Scheduling 和 Tool Call。在多 Stream 並行時，多核心共享任務佇列會引發嚴重的快取一致性缺失 (Cache Coherence Invalidation) 與鎖爭用 (Lock Contention)，造成 CPU 本身的調度延遲 (Scheduling Overhead)。

---

## 2. 軟硬體協同設計解法 (Co-Design Methodology)

本研究提出了跨層級的協同優化方案：

### A. 微架構層級 (CPU Orchestration Co-Design)
為了減輕 CPU 排程佇列的鎖爭用與快取失效，我們在 `gem5` 模擬中比較了兩種佇列微架構設計：
1. **Centralized Global Queue (集中式鎖佇列)**：所有 CPU 核心共享一個全局鎖，對任務進行排序以尋找 locality。這會引發嚴重的 L1 D-Cache Invalidations。
2. **Distributed Expert-Affinity Queue (分散式專家親和性佇列)**：將專家需求分散到不同的核心佇列中，只在需要時進行 Work stealing。這大幅減少了鎖爭用，提升了 CPU L1 Cache 命中率。

### B. 系統級調度層級 (Expert-Locality-Aware Scheduling, ELAS)
- 傳統的 **FCFS (先進先出)** 調度會交錯執行不同 Request，導致 GPU Expert Cache 頻繁替換 (Cache Thrashing)。
- **ELAS** 策略會掃描佇列中所有 Ready 的 Token，將要求相同專家的 Token 組合成 Batch 優先發送，極大化 GPU 快取重用，顯著提高命中率。

### C. 執行決策層級 (Dynamic Cost-Model Decision, DCMD)
當發生 GPU Expert Cache Miss 時，動態決策引擎會即時計算並比較兩者成本：
- $Cost_{transfer} = T_{transfer\_queue\_and\_delay} + N_{tokens} \times T_{gpu\_expert}$
- $Cost_{offload} = N_{tokens} \times T_{cpu\_expert} + T_{cpu\_transfer}$
- 如果此時 PCIe 匯流排阻塞或 token 數量極少，決策引擎會將任務 **Offload** 給 CPU；若 token 數量極多（Batch 效應大）且 PCIe 閒置，則選擇 **Transfer**。

---

## 3. 專案目錄結構

```text
hetero_moe_scheduling/
├── Dockerfile                  # 實驗與交叉編譯環境
├── Makefile                    # 一鍵自動化管理腳本
├── README.md                   # 本教學與研究文件
├── config/                     # 系統參數配置
├── gem5_sim/                   # gem5 微架構模擬區
│   ├── cpu_sched_bench.cpp     # CPU 佇列模擬 C++ 程式
│   ├── run_gem5_se.sh          # gem5 執行腳本
│   └── parse_gem5_stats.py     # gem5 數據解析
├── simulator/                  # 異質 MoE 系統級模擬器 (HMCS)
│   ├── hardware_model.py       # PCIe、DRAM、CPU/GPU 計算建模
│   ├── cache.py                # GPU Expert Cache 模擬 (LRU/LFU)
│   ├── decision_engine.py      # CPU Offload DCMD 決策模組
│   ├── scheduler.py            # ELAS 調度器模組
│   └── run_hmcs.py             # 模擬器運行測試
└── experiments/                # 參數掃描與繪圖
    ├── run_sweeps.py           # 執行 DSE 參數掃描
    └── plot_results.py         # 數據分析與視覺化
```

---

## 4. 快速開始與環境建置

本專案使用 `Docker` 與 `Makefile` 進行一鍵部署，所有依賴（編譯器、模擬器、Python 庫）均已容器化。

### 步驟 1：建立專案映像檔
在專案根目錄下，執行以下命令編譯 Docker 鏡像：
```bash
make image
```
這會基於既有的 `gem5-env` 映像檔，加裝 ARM64 交叉編譯器 (`g++-aarch64-linux-gnu`) 以及 `pandas`、`matplotlib` 等數據分析庫。

### 步驟 2：一鍵運行完整實驗 (All-in-One)
```bash
make all
```
這個指令將會在容器中自動執行：
1. **編譯 C++ 基準測試**：將 `cpu_sched_bench.cpp` 編譯成 ARM64 靜態連結檔。
2. **gem5 微架構模擬**：在 gem5 中分別跑 Centralized 與 Distributed 佇列模擬，輸出 `stats.txt` 並提取週期數與快取失效率。
3. **系統級 DSE 掃描**：執行 `run_sweeps.py`，掃描不同的 Cache 空間大小及 PCIe 頻寬，評估 4 種不同調度策略組合。
4. **數據統計與繪圖**：分析掃描結果，繪製實驗圖表至 `reports/` 目錄中。

---

## 5. 指令細部分步執行教學

如果您想手動分步調試各個模組，可以使用以下指令：

### 1. 單獨執行 gem5 微架構模擬
```bash
make gem5
```
這會編譯 C++ 檔案並執行 `run_gem5_se.sh`。它將在 `reports/gem5_centralized/` 和 `reports/gem5_distributed/` 下生成 gem5 的模擬報告，並自動執行 `parse_gem5_stats.py` 來對齊和打印對比結果。

### 2. 單獨執行系統級 MoE 模擬與 Design Space Exploration
```bash
make hmcs
```
這會執行系統級模擬器，對多條並行 MoE stream 進行仿真，並將參數掃描結果寫入 `reports/sweep_results.csv`。

### 3. 生成圖表
```bash
make plot
```
這會讀取 `sweep_results.csv` 並在 `reports/` 下生成 `cache_capacity_sweep.png` 和 `pcie_bandwidth_sweep.png`。

---

## 6. 微架構模擬結果與協同設計洞察

在 `gem5` (O3CPU 模型) 的模擬中，處理 1000 個 MoE 調度任務的數據如下：

- **Centralized Queue (集中式鎖佇列)**：
  - 模擬總時延：`0.018430` 秒
  - CPU 總週期：`36,859,066` cycles
  - L1 D-Cache 缺失：`27,689` misses
  - 指令執行率 (IPC)：`1.971`
- **Distributed Affinity Queue (分散式專家親和性佇列)**：
  - 模擬總時延：`0.000628` 秒
  - CPU 總週期：`1,256,043` cycles
  - L1 D-Cache 缺失：`26,393` misses
  - 指令執行率 (IPC)：`0.881`
- **對比分析與改善**：
  - **CPU 執行週期降低了 96.6% (提速 29.3 倍)**。
  - L1 D-Cache misses 減少了 **4.7%**。
  - **協同設計洞察**：集中式佇列在並行爭搶時，自旋鎖 (Spinlock) 產生了大量無效的 Busy-spinning，雖然在統計上呈現出高 IPC，但其實絕大多數都是無效週期。分散式佇列避免了全局鎖競爭與無謂忙等，大幅提升了 CPU Orchestrator 本身的調度效率。這證明了高效率的 MoE 軟硬體協同設計中，CPU Orchestration 隊列結構的設計至關重要。

---

## 7. 系統級 DSE 實驗結果分析

系統級參數掃描（處理 15 個 Requests，共 2490 個專家 token 計算需求，並限制 CPU 核心數為 8）顯示了以下結果：

1. **Cache Capacity 的影響**：
   - 當 GPU Expert Cache 容量極小 ($C=2$) 時，傳統的 `Always Transfer` 策略因為極高的 Miss Rate，在多個並發 stream 爭搶下，總執行時間高達 **63,312.8 ms**。
   - 此時，我們的 **ELAS + DCMD 協同方案** 能動態識別 PCIe 瓶頸，100% 選擇 CPU Offload。但在引進 CPU 有限核心 (8 cores) 限制後，產生了計算排隊，執行時間變為 **1,490.9 ms**（比 naive-infinite cores 的 1,172.6 ms 稍慢，但**依然比 Always-Transfer 快了 42.4 倍**！）。
   - 優化方案 **F-ELAS + DCMD (Paper Opt)** 在保障每個 stream 最大等待 ticks（防餓死）的同時，維持了與最優解一樣的高性能 (**1,490.9 ms**)，證明其具備極高的實用性與公平性。
   - 當 Cache 容量擴大到 $C=16$（接近 100% 命中）時，`Always Transfer` 降至 **1,173.0 ms**，而 DCMD 動態收斂至 **1,490.9 ms**。
2. **PCIe Bandwidth 的影響**：
   - 在低 PCIe 頻寬 (8.0 GB/s) 下，`Always Transfer` 的傳輸開銷巨大，耗時高達 **93,922.4 ms**；而 DCMD 與 F-ELAS 方案能自動退化為 CPU 運算，維持在 **1,490.9 ms**。
   - 即使頻寬提升至高階的 64.0 GB/s，由於並行 streams 頻繁發起 1GB 專家傳輸，排隊延遲（約 12.0s）依然存在，DCMD 依然做出了正確的 OFFLOAD 決策。

這份研究報告和模擬器提供了一個極佳的框架，方便入門工程師與系統架構師快速上手，並在給定的硬體規格下找出最優的 MoE 推理配置。


