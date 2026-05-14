# 視覺化工具教學：系統拓樸與 O3 管線追蹤

在硬體設計中，「看不見」往往是最痛苦的。gem5 提供了強大的視覺化功能，讓我們能一眼看穿虛擬硬體的連接關係與 CPU 指令的執行細節。

## 1. Graphviz 系統拓樸圖

### 自動生成機制
gem5 在執行 `m5.instantiate()`（實例化）時，會掃描 Python 環境中是否存在 `pydot` 套件。
- 若偵測成功，它會將 `config.ini` 中的階層關係轉換為 DOT 語言。
- 最終在輸出目錄 (預設為 `m5out/`) 產生 `config.dot.pdf`。

### 如何解讀拓樸圖
- **節點 (Nodes)**: 代表硬體組件，例如 `system.cpu` 或 `system.membus`。
- **連接線與埠 (Lines & Ports)**: 
    - 線段代表數據流動的路徑。
    - 連接點通常標記為 `cpu_side_ports` (靠近處理器的一端) 或 `mem_side_ports` (靠近記憶體的一端)。
- **階層關係**: 透過框線，你可以清晰看到哪些快取 (L1/L2) 屬於哪個核心，以及總線 (XBar) 是如何串接所有組件。

---

## 2. O3 Pipeline Viewer (管線視覺化)

對於 `DerivO3CPU` 這類複雜的亂序執行核心，純粹的統計數字 (IPC) 無法告訴我們為什麼指令會卡住。這時我們需要「管線追蹤」。

### 步驟一：啟動追蹤標籤
執行 gem5 時，必須加入特定的除錯標籤 (Debug Flags)：
- `--debug-flags=O3PipeView`: 啟動管線視圖追蹤。
- `--debug-file=trace.out`: 指定追蹤資訊輸出的檔名（預設會放在 `m5out/`）。

### 步驟二：格式轉換
gem5 輸出的原始追蹤檔 (`trace.out`) 體積巨大且難以閱讀。我們必須使用 gem5 內建的腳本進行轉換：
```bash
# -c 500 代表僅轉換前 500 條指令 (避免檔案過大)
# -o pipeview.out 為輸出結果
python3 /work/gem5/util/o3-pipeview.py -c 500 -o pipeview.out m5out/trace.out
```

### 步驟三：解讀管線流動 (Pipeline Stages)
開啟 `pipeview.out` 後，你會看到類似下方的文字矩陣：
- **f (Fetch)**: 指令從記憶體中被抓取。
- **d (Decode)**: 指令被解碼為微指令 (uops)。
- **n (Rename)**: 暫存器重命名，用來解決數據相關性。
- **p (Dispatch)**: 指令被分配到發射佇列 (Issue Queue)。
- **i (Issue)**: 指令正式進入運算單元執行。
- **c (Complete)**: 運算完成。
- **r (Retire)**: 指令正式提交並更新系統狀態（這是指令生命週期的終點）。

**觀察重點**：
- 如果你發現某一列的 `p` 到 `i` 之間空了很久，代表指令在等待操作數 (Operand) 到位，這就是典型的數據相依性瓶頸。
- 如果 `f` 到 `d` 斷斷續續，可能是分支預測失敗導致的清除 (Flush)。

---

## 3. 常見問題解答
- **為什麼沒看到 PDF？**: 請確認 Dockerfile 更新後有重新 `docker build`。
- **為什麼 trace.out 是空的？**: 確認你的配置檔中使用的是 `DerivO3CPU` 而非 `SimpleCPU`，因為只有 O3 核心支援管線視覺化。
