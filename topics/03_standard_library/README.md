# Topic 03: gem5 Standard Library 元件化系統建模

## 1. 學習目標
* 理解 gem5 Standard Library 的模組化、元件化建模思想。
* 熟悉各核心元件之繼承關係及其對應的 C++ 實體類別。
* 學習如何在 `SimpleBoard` 整合 L1/L2 快取階層與多通道記憶體系統。
* 掌握物件導向系統組態設計。

## 2. 架構與設計思維
在早期的 gem5 版本中，建置一個包含快取、匯流排、記憶體控制器的系統需要手動實例化數十個 `SimObject`，並手動透過埠 (Ports) 進行繁瑣的連接。
gem5 Standard Library (以下簡稱 stdlib) 引入了**元件化設計 (Component-based Design)**。它將系統高度抽象化為：
1. **Board** (板卡元件)：定義整個晶片或系統的骨架 (如 SE 或 FS 模式板)。
2. **Processor** (處理器元件)：包含多個處理器核心與核心微架構。
3. **Memory System** (記憶體系統)：封裝實體通道、大小、與時序控制器。
4. **Cache Hierarchy** (快取階層)：決定系統的快取一致性模型、快取深度、容量與總線路由。

### 元件類別繼承結構 (UML 思想)
* `AbstractBoard` -> `SimpleBoard`
* `AbstractProcessor` -> `SimpleProcessor`
* `AbstractMemorySystem` -> `SingleChannelDDR3_1600` / `DualChannelDDR4_2400`
* `AbstractCacheHierarchy` -> `PrivateL1PrivateL2CacheHierarchy`

stdlib 使用 Python 的 `@overrides` 與抽象方法強制各元件實作一致的界面。例如，任何快取階層都必須實作 `incorporate_cache(board)`，在此方法內自動將快取埠接到處理器的 CPU 埠以及板子的記憶體匯流排，這避免了人為接線的錯誤。

## 3. 程式碼架構解析
本主題將組態升級為：
* **Processor**：1 核心 `TIMING` 處理器。
* **Cache Hierarchy**：`PrivateL1PrivateL2CacheHierarchy`，配置有獨立的 `32KiB` L1 指令快取、`32KiB` L1 資料快取，以及一個私有 `256KiB` L2 快取。
* **Memory**：`SingleChannelDDR3_1600`，容量增大至 `512MiB` 以模擬更複雜的測試負載。

## 4. 學生練習 (TODO)
在 `config.py` 中，你需要：
1. 配置好包含 L1 與 L2 快取階層物件的實例。
2. 使用 `SimpleProcessor` 與對應的 CPU 模型。
3. 配置時脈為 `3GHz`。
4. 設定本地 Hello World 工作負載。
