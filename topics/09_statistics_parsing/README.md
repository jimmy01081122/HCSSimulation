# Topic 09: Statistics Parsing 模擬結果分析與自動化提取

## 1. 學習目標
* 理解 gem5 輸出目錄中核心檔案的作用。
* 熟悉 `stats.txt` 檔案格式與關鍵欄位含義。
* 學習利用 Python 撰寫自動化解析腳本，提取模擬時間、指令數、CPI 與快取失誤率等指標。
* 掌握電腦工程實驗結果自動化生成的流程。

## 2. 架構與設計思維
模擬器的核心價值在於產出可供分析的量化指標。在完成模擬後，gem5 會預設在 `m5out/` 目錄生成以下檔案：
* `config.ini`：系統所有 SimObject 實例化後的實體參數配置 (包含埠連線狀態)。可用於確認組態設定是否正確。
* `config.json`：`config.ini` 的 JSON 格式。
* `stats.txt`：最主要的效能統計報告。包含每一項暫存統計計數器的值。

### stats.txt 常用欄位與公式
* `simSeconds`：模擬的處理器虛擬時間 (單位：秒)。
* `simTicks`：模擬的虛擬時鐘 ticks 數 (gem5 預設 1 tick = 1 ps)。
* `simInsts`：模擬提交執行的指令總數。
* `system.cpu.numCycles`：處理器運行的總時脈週期數。
* **CPI (Cycles Per Instruction)**：每個指令執行的時脈週期。公式：$CPI = \frac{\text{numCycles}}{\text{simInsts}}$。
* **IPC (Instructions Per Cycle)**：每個週期執行的指令數。公式：$IPC = \frac{\text{simInsts}}{\text{numCycles}}$。
* **L1 Cache Miss Rate**：$Miss Rate = \frac{\text{overallMisses}}{\text{overallAccesses}}$。

## 3. 程式碼架構解析
本主題包含兩個主要程式：
1. `config.py`：使用 `TimingSimpleCPU` 加上 L1/L2 快取階層運行 Hello World，建立基礎 `stats.txt`。
2. `parse_stats.py`：一個獨立的 Python 腳本。它使用規則運算式 (Regex) 解析 `stats.txt`，尋找關鍵統計項，並在主端將其排版列印成 Markdown Table 或匯出成 CSV。

## 4. 學生練習 (TODO)
在 `parse_stats.py` 中，你需要：
1. 實作以 Python 讀取 `stats.txt` 的邏輯。
2. 使用正則表達式或字串分割技術，提取 `simSeconds`、`simInsts`、`system.cpu.ipc` 等關鍵變數。
3. 格式化列印出分析報告。
