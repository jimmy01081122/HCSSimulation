# Architecture Design Notes / 架構設計筆記
# ==========================================

## 1. System Architecture Overview / 系統架構概覽

```
                        ┌───────────────────────────────────────┐
                        │           HEXA-MoE Framework           │
                        └───────────────────────────────────────┘
                                         │
           ┌─────────────────────────────┼─────────────────────────────┐
           │                             │                             │
           ▼                             ▼                             ▼
   ┌───────────────┐           ┌─────────────────┐          ┌──────────────────┐
   │  F-ELAS       │           │  DCMD-Opt        │          │ Expert-Affinity   │
   │  Scheduler    │           │  Decision Engine │          │ Queue (gem5)      │
   └───────┬───────┘           └────────┬────────┘          └─────────┬────────┘
           │                            │                             │
           │  scheduling_order          │  TRANSFER / OFFLOAD         │  lock-free enqueue
           │                            │                             │
           ▼                            ▼                             ▼
   ┌───────────────────────────────────────────────────────────────────────────┐
   │                         Hardware Abstraction Layer                        │
   │  ┌──────────┐  ┌────────────┐  ┌──────────────┐  ┌────────────────────┐  │
   │  │ PCIe Bus │  │ GPU Expert │  │ CPU Core     │  │ GPU Dense          │  │
   │  │ (DMA Ch) │  │ Cache      │  │ Pool         │  │ Compute Unit       │  │
   │  │  N=2..4  │  │ LRU/LFU   │  │ K=2..32      │  │ (fixed latency)    │  │
   │  └──────────┘  └────────────┘  └──────────────┘  └────────────────────┘  │
   └───────────────────────────────────────────────────────────────────────────┘
```

---

## 2. F-ELAS Scheduling Algorithm / F-ELAS 調度演算法

### 2.1 設計理念

普通的 ELAS (Expert-Locality-Aware Scheduling) 會優先調度那些所需 expert
已經在 GPU cache 中的 request，以最大化 cache 命中率。但這會導致需要
"冷" expert 的 request 被無限期推遲 (starvation)。

F-ELAS 引入 **aging 機制**：每個未被服務的 request 累積 `waiting_ticks`，
當超過門檻 `θ_starve` 時，不論 locality score 如何，強制提升該 request
的優先級。

### 2.2 演算法偽代碼

```
function F_ELAS_Schedule(active_requests, cache, θ_starve):
    // Phase 1: 檢查是否有飢餓的 request
    for r in active_requests:
        if r.waiting_ticks >= θ_starve:
            r.waiting_ticks = 0
            return r  // 強制服務

    // Phase 2: 按 locality score 排序
    scores = {}
    for r in active_requests:
        expert_needed = r.get_current_expert()
        if cache.contains(expert_needed):
            scores[r] = 100 + cache.get_recency(expert_needed)  // 命中加分
        else:
            scores[r] = 0  // 未命中無加分

    // Phase 3: 選擇最高分者
    best = argmax(scores)

    // Phase 4: 更新其他 request 的等待計數
    for r in active_requests:
        if r != best:
            r.waiting_ticks += 1

    return best
```

### 2.3 θ_starve 參數分析

| θ_starve | 行為描述                                      | 公平性 | 吞吐量 |
|----------|-----------------------------------------------|--------|--------|
| 1        | 幾乎是 Round-Robin，無 locality 優化          | 最高   | 最低   |
| 3        | 輕度 locality 偏好，快速恢復公平               | 高     | 中等   |
| 5        | 推薦值，平衡 locality 與公平性                 | 中     | 高     |
| 10       | 強 locality 偏好，可能產生長尾                  | 低     | 最高   |
| ∞        | 退化為純 ELAS，無公平保證                      | 無     | 最高   |

---

## 3. DCMD-Opt Cost Model / DCMD-Opt 成本模型

### 3.1 核心洞察

在做 CPU vs. GPU 決策時，不能只看靜態延遲 (4.0ms vs. 0.2ms)，
還必須考慮**動態排隊狀態**：

- **PCIe Bus 排隊**: 如果有 5 個 transfer 在排隊，新的 transfer 需要等 5 × 62.5ms
- **CPU Core 排隊**: 如果 8 個 core 都在忙，新的 offload 需要等最短完成者

### 3.2 成本計算公式

```
Cost_transfer(expert_i) =
    Σ(remaining_transfer_time[ch] for ch in dma_channels) / num_channels  // 排隊等待
  + expert_size / pcie_bandwidth                                           // 傳輸時間
  + gpu_expert_latency                                                      // GPU 計算

Cost_offload(expert_i) =
    min(cpu_queue_remaining[core] for core in cpu_cores)                   // CPU 等待
  + cpu_expert_latency                                                      // CPU 計算
  + result_transfer_overhead                                                // 結果回傳

Decision(expert_i) = argmin(Cost_transfer, Cost_offload)
```

### 3.3 邊界條件

| 場景                              | 預期決策   | 原因                     |
|-----------------------------------|------------|--------------------------|
| 空 PCIe + 空 CPU + cache miss     | TRANSFER   | GPU 比 CPU 快 20 倍      |
| 滿 PCIe + 空 CPU                  | OFFLOAD    | PCIe 排隊太長            |
| 空 PCIe + 滿 CPU                  | TRANSFER   | CPU 排隊太長             |
| 滿 PCIe + 滿 CPU                  | 比較兩者   | 動態比較決定             |
| cache hit                          | N/A        | 直接 GPU 計算，不走 DCMD |

---

## 4. PCIe DMA Multi-Channel Model / PCIe 多通道 DMA 模型

### 4.1 模型描述

現代 GPU (如 NVIDIA A100) 通常有 2-4 個獨立的 Copy Engine (DMA channel)。
每個 channel 可以獨立發起一個 H2D 或 D2H 傳輸。

但是，所有 channel 共享同一條 PCIe x16 物理鏈路的頻寬 (16 GB/s for Gen4)。
因此，當多個 channel 同時傳輸時，每個 channel 的有效頻寬 = 總頻寬 / 活躍通道數。

### 4.2 頻寬分配模型

```
effective_bandwidth(channel_i) = total_bandwidth / num_active_channels

transfer_time(expert, channel_i) = expert_size / effective_bandwidth(channel_i)
```

注意：隨著其他 channel 完成傳輸，剩餘的 channel 會獲得更多頻寬。
這在離散事件模擬中需要動態更新。

### 4.3 模擬實作提示
```python
class PCIeBus:
    def tick(self, dt_ms):
        active = [ch for ch in self.channels if ch.is_busy]
        if not active:
            return
        bw_per_ch = self.total_bandwidth / len(active)
        for ch in active:
            bytes_transferred = bw_per_ch * dt_ms / 1000 * 1e9  # GB/s → bytes/ms
            ch.remaining_bytes -= bytes_transferred
            if ch.remaining_bytes <= 0:
                ch.finish()
```

---

## 5. CPU Core Pool Model / CPU 核心池模型

### 5.1 模型描述

CPU 擁有 K 個核心 (K=2..32)。每個核心一次只能執行一個 expert 計算任務。
當所有核心都在忙時，新的 offload 任務進入等待佇列 (FIFO)。

### 5.2 等待時間估算

```
estimated_wait(cpu_pool) =
    if any_core_idle:
        return 0
    else:
        return min(core.remaining_time for core in cores)
```

### 5.3 與 DCMD 的整合

DCMD 在做決策時，會呼叫 `cpu_pool.get_estimated_wait()` 來獲取
當前的 CPU 排隊延遲，並加入 offload 成本計算中。

---

## 6. Expert Cache Design Space / 專家快取設計空間

### 6.1 支援的淘汰策略

| Policy | Full Name                    | 描述                                    |
|--------|------------------------------|-----------------------------------------|
| LRU    | Least Recently Used          | 淘汰最久未被使用的 expert               |
| LFU    | Least Frequently Used        | 淘汰使用次數最少的 expert               |
| ARC    | Adaptive Replacement Cache   | 動態調整 LRU 和 LFU 比例的自適應策略     |

### 6.2 ARC 策略描述 (新增)

ARC 維護兩個 LRU 列表 T1 和 T2，分別追蹤 recency 和 frequency：
- T1: 只被訪問過一次的 expert
- T2: 被訪問過兩次以上的 expert
- p: 動態參數，控制 T1 和 T2 的容量分配

當 cache miss 時：
- 如果 expert 在 T1 的 ghost list 中 → 增加 p (偏好 recency)
- 如果 expert 在 T2 的 ghost list 中 → 減少 p (偏好 frequency)

---

## 7. Discrete-Event Simulation Design / 離散事件模擬設計

### 7.1 為何不用固定步長

先前的實作使用固定時間步長 (1 tick = 某個時間單位)，這在
事件間隔差異極大時 (0.1ms orchestration vs. 62.5ms PCIe transfer)
會造成大量無效迭代。

### 7.2 事件驅動架構

```python
class Event:
    def __init__(self, time, event_type, request_id, data=None):
        self.time = time
        self.event_type = event_type  # "ORCH_DONE", "DENSE_DONE", "TRANSFER_DONE", ...
        self.request_id = request_id
        self.data = data

class EventQueue:
    def __init__(self):
        self.queue = []  # min-heap by time

    def push(self, event):
        heapq.heappush(self.queue, (event.time, event))

    def pop(self):
        return heapq.heappop(self.queue)[1]
```

### 7.3 主循環

```python
while not event_queue.empty():
    event = event_queue.pop()
    current_time = event.time

    if event.event_type == "REQUEST_ARRIVAL":
        # 加入 active_requests
        # 觸發 scheduling

    elif event.event_type == "ORCH_DONE":
        # CPU orchestration 完成
        # 進入 GPU_DENSE 階段

    elif event.event_type == "DENSE_DONE":
        # Dense layer 完成
        # 檢查 MoE expert cache → 決定 TRANSFER or OFFLOAD

    elif event.event_type == "TRANSFER_DONE":
        # PCIe 傳輸完成
        # 進入 GPU_MOE_EXECUTING

    elif event.event_type == "GPU_MOE_DONE":
        # GPU expert 計算完成
        # advance_pipeline (下一層或下一 token)

    elif event.event_type == "CPU_MOE_DONE":
        # CPU expert 計算完成
        # 釋放 CPU core, advance_pipeline
```

---

## 8. Key Metrics Derivation / 關鍵指標推導

### 8.1 Total Simulation Time
```
total_sim_time = max(r.finish_time for r in all_requests) - min(r.arrival_time)
```

### 8.2 Per-Request Latency
```
latency(r) = r.finish_time - r.arrival_time
```

### 8.3 GPU Utilization
```
gpu_utilization = Σ(gpu_busy_time) / (total_sim_time × 1)  // 1 GPU
```

### 8.4 CPU Utilization
```
cpu_utilization = Σ(core_busy_time) / (total_sim_time × num_cores)
```

### 8.5 P99 Latency
```
latencies = sorted([r.latency for r in all_requests])
p99 = latencies[int(0.99 * len(latencies))]
```
