# Topic 04: Python 組態系統與 SimObject 機制

## 1. 學習目標
* 深入理解 gem5 的 C++ `SimObject` 在 Python 層級的映射模型。
* 學會使用低階的 `m5.objects` API 手動建立系統，不依賴 `gem5` 標準函式庫。
* 掌握 `clk_domain`、`voltage_domain`、`SystemXBar` 與埠 (Ports) 的連線原理。
* 理解 `m5.instantiate()` 的兩階段初始化過程。

## 2. 架構與設計思維
在底層，gem5 模擬器的所有元件 (例如 CPU 核心、快取控制器、匯流排、記憶體介面) 都是以 C++ 實作的 `SimObject`。
Python 在 gem5 中扮演**組態描述語言 (Configuration Language)** 的角色。當你寫下：
```python
system = System()
system.cpu = TimingSimpleCPU()
```
Python 會解析這個層級結構。在執行 `m5.instantiate()` 時，gem5 會遍歷 Python 物件樹，並呼叫對應 C++ SimObject 的 `create()` 函數。接著進行 C++ 物件的動態配置與連接。

### 埠 (Port) 的雙向綁定
在電腦硬體中，資料傳輸需要透過匯流排。在 gem5 中，SimObjects 透過 **Ports** 互聯。
Python 語法使用等號來連接兩個埠：
```python
system.cpu.icache_port = system.membus.cpu_side_ports
```
這代表將 CPU 的指令埠接到記憶體匯流排 (SystemXBar) 的 CPU 側。gem5 在 C++ 底層會自動將其解析為雙向指針。

### 兩階段初始化機制
1. **Instantiation**：呼叫 `m5.instantiate()`，遍歷樹狀結構、建立 C++ 實體、解析埠連結、進行記憶體映射，並呼叫 `regStats()` 初始化統計模組。
2. **Simulation**：呼叫 `m5.simulate()`，啟動 C++ 事件迴圈 (Event Loop)，系統開始執行。

## 3. 程式碼架構解析
本實驗擺脫 `SimpleBoard` 封裝，手動建立一個最原始的單核系統：
* `System()`：系統根節點。
* `SrcClockDomain` / `VoltageDomain`：配置電壓與頻率網域。
* `TimingSimpleCPU`：直接操作 C++ CPU 實體。
* `SystemXBar`：手動接線的非相干匯流排 (Non-coherent crossbar)。
* `DDR3_1600`：以低階對象連接到 `DRAMController`。

## 4. 學生練習 (TODO)
在 `config.py` 中，你需要：
1. 手動建立 `System` 實例，設定時脈與電壓。
2. 設定 `SystemXBar` 作為記憶體匯流排。
3. 建立 CPU 並設定運算元大小與執行模式。
4. 連接 CPU 的 `icache_port` 與 `dcache_port` 到 `membus.cpu_side_ports`。
5. 建立 `DDR3_1600` 與 `DRAMController` 並設定位址區間，將控制器埠接到 `membus.mem_side_ports`。
6. 設定行程 (Process) 並透過 `m5.instantiate()` 進行動態配置，最後呼叫 `m5.simulate()`。
