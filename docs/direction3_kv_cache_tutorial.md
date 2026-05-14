# 方向三：KV Cache 分散收集與軟體預取 (雙核協作評估)

## 1. 實驗構想
在處理大型模型時，KV Cache 的大小往往超過 L1/L2 Cache。當 Core 0 需要數據時，如果數據還在主記憶體中，會造成嚴重的記憶體停頓 (Memory Stall)。本實驗模擬「軟體預取」：利用一個閒置的核心 (Core 1) 提前將數據從主記憶體搬運到共享的 L2 Cache 中，減少 Core 0 的等待時間。

## 2. 實作步驟

### A. 撰寫 Workload (`src/kv_prefetch.cpp`)
- 使用 `pthread` 建立多執行緒。
- `prefetch_thread` 在 Core 1 上執行，它會掃描 `kv_data` 陣列。
- `main` 執行緒在 Core 0 上執行計算。

### B. 配置系統 (`configs/dual_core_prefetch.py`)
- 雙核 `TimingSimpleCPU`。
- 共享的 `L2Cache` (256kB)。
- 注意：L2 Cache 的大小故意設定得比數據量 (512kB) 小，用以觀察快取替換 (Cache Replacement) 的行為。

### C. 執行模擬
```bash
# 編譯時需連結 pthread 與 m5 庫
g++ -static -I/work/gem5/include src/kv_prefetch.cpp /work/gem5/util/m5/build/arm64/out/libm5.a -lpthread -o src/kv_prefetch

# 執行模擬
/work/gem5/build/ARM/gem5.opt configs/dual_core_prefetch.py src/kv_prefetch
```

## 3. 數據報告與分析 (範例數據)

| 指標 | 數值 (無預取) | 數值 (有預取) | 說明 |
| :--- | :--- | :--- | :--- |
| system.cpu0.numCycles | 2,100,000 | 1,450,000 | 核心 0 執行的總週期數 |
| system.l2cache.overall_misses | 8,192 | 1,024 | L2 快取缺失次數 |
| memory_stall_cycles | 650,000 | 120,000 | 因等待記憶體導致的停頓週期 |

### 分析與觀測
1. **快取預取效果**: 觀察 `system.l2cache.overall_hits`。若預取成功，Core 0 存取數據時會命中 L2 Cache 而非去存取 DRAM。
2. **負面效應 (Pollution)**: 如果 Core 1 預取過頭，可能會把 Core 0 真正需要的指令或舊數據踢出 L2，反而導致效能下降。

## 4. 未來優化方向 (硬體 Scatter-Gather 引擎)
- **硬體化**: 軟體預取會消耗 Core 1 的指令與電力。未來可設計專用的「數據收集單元 (Data Gathering Unit)」，它不具備計算能力，僅負責根據特定的地址模式 (Stride/Indirect) 自動搬運數據。
- **壓縮**: 在 L2 到 DRAM 之間加入壓縮模組，減少 KV Cache 在總線上佔用的頻寬。
