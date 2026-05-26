# Topic 08: Ruby Cache Coherence 快取一致性模擬

## 1. 學習目標
* 理解多核心系統中「快取一致性 (Cache Coherence)」的設計挑戰。
* 熟悉 MESI (Modified, Exclusive, Shared, Invalid) 快取狀態轉換協議。
* 學習如何在 gem5 啟用 Ruby 記憶體系統與自訂快取協議。
* 掌握 Ruby 系統的網路拓撲結構配置及其對模擬的約束。

## 2. 架構與設計思維
在對稱多處理器 (SMP) 系統中，多個 CPU 核心各自擁有私有的快取 (L1 Cache)，卻共享同一個實體記憶體空間。若多個核心同時讀寫同一個記憶體位址，若無硬體機制控制，會造成快取內容不一致的錯誤 (Coherence Problem)。

### MESI 協議狀態說明
* **Modified (M)**：資料已修改且為本快取獨有，與主記憶體內容不一致。本快取有義務在該快取行被替換時將其回寫記憶體。
* **Exclusive (E)**：資料與主記憶體內容一致，且為本快取獨有。可以不發送匯流排事件直接修改為 Modified。
* **Shared (S)**：資料與主記憶體內容一致，並可能存在於其他核心的快取中。若要寫入，必須先發送 Invalid 廣播。
* **Invalid (I)**：本快取行資料無效。

### gem5 Ruby 記憶體系統
gem5 具備兩種快取系統模型：
1. **Classic Caches**：架構簡單、速度快，不支援複雜的目錄協議，只支援基本的窺探 (Snooping) 快取一致性。
2. **Ruby Memory System**：高度靈活的記憶體模擬框架。它使用 SLICC (Specification Language for Implementing Cache Coherence) 描述複雜的一致性狀態機，能模擬各種互連拓撲 (如 Mesh, Crossbar) 與目錄 (Directory-based) 協議。

> **編譯限制**：Ruby 的協議是在編譯時期硬性綁定。若要執行本實驗，gem5 必須使用對應的協議進行編譯。
> 例如：`scons build/RISCV/gem5.opt -j$(nproc) PROTOCOL=MESI_Two_Level`。
> 若預設編譯無此協定，請在 `run.sh` 閱讀重編譯說明。

## 3. 程式碼架構解析
本實驗的 `config.py` 建置一個雙核心系統，並啟用 Ruby 支援：
* **Processor**：2 核心 `TIMING` 處理器。
* **Cache Hierarchy**：`MESITwoLevelCacheHierarchy`，底層對應 `MESI_Two_Level` 協議。

## 4. 學生練習 (TODO)
在 `config.py` 中，你需要：
1. 確認執行此腳本前 gem5 二進位檔的協定宣告。
2. 建立 `MESITwoLevelCacheHierarchy` 快取物件，設定 L1i, L1d 與 L2 快取大小。
3. 建立 2 核心處理器。
4. 在 `run.sh` 中加入驗證與錯誤提醒邏輯。
