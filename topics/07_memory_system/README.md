# Topic 07: Memory System DRAM 控制器與記憶體子系統

## 1. 學習目標
* 理解動態隨機存取記憶體 (DRAM) 的物理結構 (Rank, Bank Group, Bank, Row, Column)。
* 熟悉常見 DDR 時序參數 (tRC, tRCD, tRP, tCAS) 與通道寬度計算。
* 學習如何在 gem5 模擬不同的記憶體控制器 (DDR3 vs DDR4 vs DDR5)。
* 掌握記憶體頻寬 (Bandwidth) 對系統吞吐量的影響。

## 2. 架構與設計思維
實體記憶體子系統是由動態電容矩陣組成。為了獲得高頻寬與低延遲，DRAM 被劃分出多個獨立平行結構：
* **Channels (通道)**：獨立的實體介面，擁有獨立的控制線與資料線。多通道可以成倍增加記憶體介面頻寬。
* **Ranks (晶片排)**：共用資料匯流排但擁有獨立片選信號的 DRAM 晶片集合。
* **Banks (儲存陣列)**：每個 Rank 內部細分的儲存網格。Bank 之間可以平行操作。DRAM 內部使用列緩衝區 (Row Buffer) 快取整列資料。

### 行緩衝區原則 (Row Buffer Management)
* **Open-Page (開頁原則)**：當完成讀取後，將 Row 保持在 Row Buffer 中。若下一次存取在同一 Row (Row Buffer Hit)，延遲極低。最適合高度局部性存取的應用。
* **Close-Page (閉頁原則)**：每次存取完成後立即關閉該 Row，並回寫儲存單元。最適合高度隨機存取 (Row Buffer Conflict 高) 的多核應用。

## 3. 程式碼架構解析
本實驗的 `config.py` 接受 `--mem-type` 命令行參數。
我們可以用命令行傳入以下實體記憶體對象：
* `SingleChannelDDR3_1600`
* `SingleChannelDDR4_2400`
* `DIMM_DDR5_4400`

## 4. 學生練習 (TODO)
在 `config.py` 中，你需要：
1. 實作解析 `--mem-type` 命令列參數。
2. 根據輸入的類型動態實例化記憶體子系統。
3. 比較不同 DDR 世代記憶體在模擬中產生的讀寫延遲指標。
