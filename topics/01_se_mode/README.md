# Topic 01: Syscall Emulation (SE) Mode 基礎與系統建置

## 1. 學習目標
* 理解 gem5 中 Syscall Emulation (SE) 模式的工作原理。
* 掌握 gem5 標準函式庫 (gem5 Standard Library) 的基本元件結構：Board、Processor、Memory、CacheHierarchy。
* 學習如何建置一個最基礎的單核心無快取模擬系統，並執行 Hello World 程式。
* 理解模擬產生的 `stats.txt` 檔案基礎指標。

## 2. 架構與設計思維
Syscall Emulation (SE) 模式是計算機架構模擬中，用於快速驗證 CPU 微架構與記憶體階層的一種高效方法。
在 SE 模式下，gem5 不需要模擬真實硬體的周邊設備 (如硬碟、網路卡、中斷控制器)，也不需要啟動作業系統核心。當被模擬的應用程式執行到系統呼叫 (Syscall，例如 `read()`, `write()`, `exit()`) 時，gem5 會攔截這些指令，並在宿主機 (Host, 如你的 Docker 容器) 的作業系統上直接執行，再將結果回傳給模擬器內部的暫存器。

### 系統拓撲結構 (ASCII Art)
```
+---------------------------------------------------+
|                  SimpleBoard                      |
|  +-------------+  +-------------+  +-----------+  |
|  |  Processor  |  |    Cache    |  |  Memory   |  |
|  | (1 Core,    |  |  Hierarchy  |  |  System   |  |
|  |  TIMING CPU)|  | (No Cache)  |  | (DDR3)    |  |
|  +------+------+  +------+------+  +-----+-----+  |
|         |                |               |        |
|  =======+================+===============+======= |
|                    System Bus                     |
+---------------------------------------------------+
```

### 設計考量與 trade-offs
* **優點**：模擬速度快，記憶體佔用極小，不需要複雜的磁碟映像檔與核心配置。最適合用於使用者層級 (User-space) 的應用程式效能特徵分析或編譯器優化評估。
* **缺點**：無法精確模擬作業系統核心行為、分頁分配 (Page Allocation)、中斷處理、多執行緒同步 (Pthreads 部分支援但有限制) 以及任何硬體驅動程式。

## 3. 程式碼架構解析
我們使用 gem5 Standard Library 的 API 來進行系統設計。主要元件如下：
* `SimpleBoard`：作為系統的骨架，將處理器、記憶體、快取階層黏合在一起。
* `SimpleProcessor`：設定 CPU 微架構模型 (本章使用 `TIMING` 模型，代表暫存器傳輸級別的時序模擬)。
* `NoCache`：不使用快取，CPU 記憶體請求直接送往記憶體控制器，便於觀察記憶體存取延遲。
* `SingleChannelDDR3_1600`：單通道 DDR3 記憶體控制器。
* `BinaryResource`：直接載入編譯好的本機靜態連結二進位檔。

## 4. 學生練習 (TODO)
在 `config.py` 中，你需要：
1. 宣告對應的 ISA 需求宣告。
2. 建立 `NoCache` 快取階層物件。
3. 建立 `SingleChannelDDR3_1600` 記憶體子系統，大小設為 `32MiB`。
4. 建立 `SimpleProcessor`，指定核心數為 `1`，並指定微架構模型與 ISA。
5. 完成 `SimpleBoard` 物件的初始化。
6. 設定 SE 模式的工作負載為本機的 `hello` 執行檔。
7. 啟動 `Simulator` 進行模擬。
