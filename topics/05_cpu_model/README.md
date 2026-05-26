# Topic 05: CPU 模型與微處理器架構比較

## 1. 學習目標
* 理解 gem5 中四種核心 CPU 模型：ATOMIC、TIMING、MINOR、O3。
* 掌握各模型的硬體微架構抽象層級，特別是 Pipeline 的設計特點。
* 學會使用命令行參數 (`argparse`) 動態切換模擬的處理器微架構。
* 比較並分析不同 CPU 微架構執行相同程式時的效能 (IPC、模擬時間、執行 Cycles)。

## 2. 架構與設計思維
微處理器設計的核心在於運算單元與流水線 (Pipeline) 的設計。gem5 提供了四個主要微架構模型，用以滿足不同的效能/精準度需求：

### CPU 模型對比

```
1. ATOMIC CPU: Single-Cycle / No-Pipeline (速測功能)
   [Fetch/Decode/Execute/Commit in 1 Tick] ---> [DRAM (Immediate)]

2. TIMING CPU: Sequential / No-Pipeline (加入記憶體延遲)
   [CPU request] ---> [Wait for Bus/DRAM Latency] ---> [Resume execution]

3. MINOR CPU: In-Order Pipeline (有序流水線)
   [Fetch] ---> [Decode] ---> [Rename] ---> [Execute] ---> [Commit]

4. O3 CPU: Out-of-Order Execution (亂序發射/超純量)
   [Fetch] ---> [Decode] ---> [Rename] ---> [Dispatch] ---> [Issue Queue] ---> [Execute] ---> [ROB Commit]
```

### 微架構特點
* **AtomicSimpleCPU (`ATOMIC`)**：最簡單的模型。所有指令在單個時脈週期內完成狀態更新，不計算詳細時序延遲，記憶體存取也是立即完成。常用於大段程式碼的快速跳過 (Fast-forwarding)。
* **TimingSimpleCPU (`TIMING`)**：與 ATOMIC 類似，但是會暫停 CPU 以等待快取或記憶體系統回傳資料。依然是單週期指令集，但不忽略資源競爭。
* **MinorCPU (`MINOR`)**：精確模擬有序執行流水線。具有可配置的流水線級數、分支預測器、以及功能單元 (Functional Units) 延遲。適合模擬微控制器或嵌入式處理器。
* **O3CPU (`O3`)**：高度精確的超純量亂序執行模型。模擬重新命名暫存器、保留站 (Reservation Stations)、重排序緩衝區 (Reorder Buffer, ROB)、寫回與提交。適合模擬高效能應用處理器 (如 Intel Core 或 ARM Cortex-A)。

## 3. 程式碼架構解析
本組態導入 Python 的 `argparse` 模組：
```python
import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--cpu', choices=['atomic', 'timing', 'minor', 'o3'], default='timing')
args = parser.parse_args()
```
根據傳入的參數，動態對應到 `CPUTypes.ATOMIC`、`CPUTypes.TIMING`、`CPUTypes.MINOR` 或 `CPUTypes.O3`。如果是非 ATOMIC 模型，本組態配置一組 L1/L2 快取階層；如果是 ATOMIC 則不配置快取 (NoCache)。

## 4. 學生練習 (TODO)
在 `config.py` 中，你需要：
1. 設計命令行參數解析器，支援 `--cpu` 參數。
2. 根據 CPU 模型類型，動態選擇是否啟用快取 (ATOMIC 不適用時序快取)。
3. 初始化 `SimpleProcessor`。
4. 設定執行時間上限或正常結束條件，執行並對比模擬的 ticks 數。
