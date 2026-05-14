# 方向一：記憶體頻寬與數據搬運瓶頸 (DMA 通訊開銷評估)

## 1. 實驗構想
在現代計算機架構中，CPU 的主要任務是進行邏輯與運算，而「數據搬運」通常被視為昂貴的開銷。透過本實驗，我們將觀察 CPU 直接進行大量數據搬運時對總線 (Bus) 與記憶體造成的壓力。

## 2. 實作步驟

### A. 撰寫 Workload (`src/dma_test.c`)
我們撰寫了一個簡單的 C 語言程式，分配 1MB 的空間並進行 `dst[i] = src[i]` 的操作。這模擬了最基礎的數據複製動作。

### B. 配置系統 (`configs/simple_dma.py`)
我們建立了一個含有：
- `TimingSimpleCPU`: 1GHz 頻率。
- `SystemXBar`: 連接所有組件的總線。
- `DDR3_1600_8x8`: 提供 512MB 的記憶體。

### C. 執行模擬
在 Docker 容器內執行：
```bash
# 編譯測試程式 (使用靜態連結以確保在 gem5 內能執行)
gcc -static src/dma_test.c -o src/dma_test

# 執行模擬
/work/gem5/build/ARM/gem5.opt configs/simple_dma.py src/dma_test
```

## 3. 程式碼邏輯解釋
- **為什麼使用 TimingSimpleCPU？** 因為我們需要量測時間 (Cycles)，`TimingSimpleCPU` 會在每次記憶體存取時停下並等待記憶體控制器的回覆，這能讓我們觀察到真實的延遲。
- **Bus 的角色**：所有的數據存取都必須通過 `system.membus`。如果多個組件 (如 CPU 與未來的 DMA) 同時爭搶 Bus，就會產生競爭延遲。

## 4. 數據報告與分析 (範例數據)

| 指標 | 數值 | 說明 |
| :--- | :--- | :--- |
| sim_seconds | 0.004521 | 模擬的實體時間 (秒) |
| sim_insts | 12,450,210 | CPU 執行的總指令數 |
| system.membus.trans_dist::ReadReq | 1,048,576 | 總線讀取請求次數 (對應 1MB 數據) |
| system.membus.trans_dist::WriteReq | 1,048,576 | 總線寫入請求次數 |

### 分析與觀測
1. **Bus Traffic**: 觀察 `stats.txt` 中的 `system.membus.pkt_count`。當數據量增大時，總線的利用率會接近飽和。
2. **瓶頸點**: 目前 CPU 必須等待每一筆寫入完成才能進行下一筆讀取。

## 5. 未來優化方向
- **引入 DMA 設備**：在配置中加入 `DmaDevice`，讓 CPU 只需下達指令 (如給予 src/dst 地址與 size)，之後 CPU 即可進入休眠或進行其他計算。
- **調整 Chunk Size**：DMA 搬運時通常會分塊 (Chunk)。較大的 Chunk 可以提升頻寬利用率，但會佔用 Bus 較久，導致 CPU 存取延遲增加。
