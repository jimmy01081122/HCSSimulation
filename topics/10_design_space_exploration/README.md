# Topic 10: Design Space Exploration 自動化設計空間探索 (DSE)

## 1. 學習目標
* 理解電腦系統架構設計中的設計空間探索 (Design Space Exploration, DSE)。
* 學習如何利用外殼腳本 (Shell Script) 編寫多變量自動化參數掃描。
* 掌握多組模擬結果的聚合分析方法。
* 學習折衷方案 (Trade-off) 與柏拉圖最佳解 (Pareto Frontier) 概念在晶片架構設計中的應用。

## 2. 架構與設計思維
在晶片設計的前期，架構師面臨著廣闊的設計空間 (Design Space)。我們需要決定：
* 核心類型：Timing 核心還是高複雜度的 O3 核心？
* 快取大小：L1 快取和 L2 快取的容量多大最划算？
* 記憶體匯流排：應該使用高頻寬的 DDR4 還是一般 DDR3？

### 柏拉圖最佳解 (Pareto Frontier)
增加快取容量可以提高處理器的效能，但會導致晶片面積 (Area) 與漏電功耗 (Leakage Power) 呈指數級上升。
我們的目標是找出所有**柏拉圖最佳解 (Pareto optimal solutions)** —— 在給定功耗或面積限制下能提供最高效能，或在給定效能要求下擁有最低功耗與面積的硬體參數組合。

### DSE 自動化流程
```
+--------------------+      +--------------------+      +--------------------+
|   DSE Sweep Script | ---> | gem5 Simulation    | ---> | Result Aggregation |
| (Iterates params)  |      | (Generates output) |      | (Generates CSV)    |
+--------------------+      +--------------------+      +--------------------+
```

## 3. 程式碼架構解析
本主題實作一個自動化掃描系統：
1. `config.py`：接受 `--cpu`, `--l1-size`, `--l2-size`。
2. `run.sh`：一個 Bash 腳本，透過多重 nested-loop (雙重迴圈) 自動啟動多組 gem5 模擬，每次動態生成獨立的 `--outdir` (例如 `m5out_cpu_l1_l2`)。
3. `collect_results.py`：掃描所有 `m5out_*` 目錄，讀取各別的 `stats.txt`，並將資料合併輸出為單一 CSV 檔案 `dse_results.csv`，便於繪圖與分析。

## 4. 學生練習 (TODO)
在 `collect_results.py` 中，你需要：
1. 實作遍歷當前目錄下所有 `m5out_` 開頭的模擬輸出目錄的邏輯。
2. 提取出該配置下的硬體配置與模擬效能 (例如 `simSeconds`)。
3. 將資料彙整寫入 CSV 檔案。
