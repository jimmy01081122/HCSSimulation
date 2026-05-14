# 方向二：MoE 路由邏輯的運算延遲對比 (CPU vs. 加速器構想)

## 1. 實驗構想
在大型語言模型 (LLM) 中，MoE (Mixture of Experts) 架構需要動態地將 Token 分配給不同的專家。這個「路由 (Routing)」過程包含 Softmax 與 Top-K 運算。雖然邏輯不複雜，但當 Token 數量龐大時，會成為 CPU 的負擔。本實驗觀察 CPU 執行這段邏輯的效能。

## 2. 實作步驟

### A. 撰寫 Workload (`src/moe_routing.cpp`)
- 使用 C++ STL 的 `std::exp` 與 `std::max_element` 模擬 Softmax 與路由。
- 呼叫 `m5_work_begin()` 與 `m5_work_end()`。這兩個函數是 gem5 提供的「後門」，當程式執行到這裡時，模擬器會記錄下當前的所有統計數據。

### B. 配置系統 (`configs/routing_test.py`)
- 使用 `DerivO3CPU`: 這是一個亂序執行 (Out-of-Order) CPU，能模擬現代處理器的高性能行為。
- 頻率設定為 2GHz。
- 引入 L1 Cache 以減少記憶體存取對計算效能的干擾。

### C. 執行模擬
```bash
# 編譯時需要包含 gem5 的標頭檔路徑並連結 m5 庫
g++ -static -I/work/gem5/include src/moe_routing.cpp /work/gem5/util/m5/build/arm64/out/libm5.a -o src/moe_routing

# 執行模擬
/work/gem5/build/ARM/gem5.opt configs/routing_test.py src/moe_routing
```

## 3. 關鍵概念：m5 ops
`m5_work_begin/end` 允許我們在 `stats.txt` 中看到多個「ROI (Region of Interest)」。
- `work_begin` 之前的數據是程式初始化（如分配記憶體、初始化向量）。
- `work_begin` 與 `work_end` 之間的數據才是我們核心算法的效能。

## 4. 數據報告與分析 (範例數據)

| 指標 | 數值 | 說明 |
| :--- | :--- | :--- |
| system.cpu.ipc | 1.85 | 每週期執行指令數 (越高代表計算越密集) |
| system.cpu.dcache.overall_miss_rate | 0.02 | 數據快取缺失率 |
| work_item_time | 450,000 ticks | 路由邏輯執行的總時間 |

### 分析與觀測
1. **IPC 分析**: 若 IPC 較低，可能是因為 `exp` 運算導致流水線停頓，或者分支預測失敗。
2. **加速器構想**: 如果我們開發一個硬體加速器，將 Softmax 改為查表法或專用硬體電路，預期 `work_item_time` 將大幅縮減。

## 5. 未來優化方向
- **探討 Amdahl's Law**: 即使路由邏輯加速 100 倍，如果整體的 Token 運算中路由只佔 5%，總效能提升也有限。
- **SIMD 指令**: 嘗試使用 ARM NEON 指令集優化 Softmax，觀察 IPC 的變化。
