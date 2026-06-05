# MoE Routing-aware Prefetcher 交接與後續研究方向指南

本文件旨在總結本專案目前之研究狀態，並為後續研究人員提供清晰的硬體改進與學術探索方向。

---

## 1. 專案現狀與驗收限制

### 1.1 目前成果總結
- **軟硬體語意對齊**：Python 行為模擬器與 RTL 暫存器級硬體在當前 smoke trace、FIFO/LRU 政策與 testbench 刺激下，其統計暫存器 (PMU Counters) 達到了 100% 事件一致性，順利通過自動化煙霧測試對齊。
- **依賴與工具健壯性**：建立了自動化依賴安裝腳本與建置手冊，確保專案具備跨平台移植能力。
- **實驗數據與報告**：完成了敏感度 sweep 實驗探討，產出了折線圖，並提供完整的學生實驗撰寫模板。

### 1.2 專案邊界與限制聲明
> [!WARNING]
> 本專案目前之 PASS 代表在給定 smoke trace、FIFO/LRU 替換政策、單通道 DMA 傳輸、現有 Python simulator 與 RTL testbench 條件下，Python counters 與 RTL counters 完全一致。這並不代表所有輸入 trace 序列、任意硬體配置與邊界條件均已在實體電路上完成了完整時序驗收。

---

## 2. 後續研究與硬體探索方向 (Future Directions)

本專案之第一版已建立了高可信度的軟硬體共設計基礎，未來研究人員可從以下四個學術方向進行延伸探索：

### 2.1 多通道 DMA 與優先級仲裁 (Multi-channel DMA & Priority Queues)
- **當前限制**：目前的 RTL 僅建模單通道 DMA (`in_flight_expert_id`），當 DMA 忙碌時，新來的預取請求會被無條件捨棄 (dropped)。
- **改進方案**：
  - 設計多通道 DMA 控制器（如 2 條或 4 條獨立搬移通道），支援多個 Expert 並行載入。
  - 在硬體仲裁器中，實作優先級緩衝區（Priority Queue），當發生 Busy Contention 時，可將 prefetch 暫存在緩衝區中，而非直接丟棄；且當 demand miss 發生時，可對緩衝區中的 prefetch 進行搶占或旁路 (bypass)。

### 2.2 動態預取距離控制器 (Dynamic Prefetch Distance Controller)
- **當前限制**：預取距離 $d$ 必須在 trace 生成階段由軟體寫死，無法適應運行時 (Runtime) 動態變化的記憶體頻寬。
- **改進方案**：
  - 於 RTL 控制器中，新增一個動態監控暫存器，實時統計 `cnt_prefetch_dropped` 的增長率與 DMA 忙碌週期比例。
  - 當發現系統頻寬吃緊時，動態收縮預取距離以減少無效預取；當頻寬空閒時，放寬預取距離以隱藏更長之延遲，實現 **Closed-loop dynamic prefetching**。

### 2.3 真實 FPGA 平台整合與 timing closure
- **當前限制**：本 RTL 第一版僅為 **metadata-only** 模擬，快取中沒有真實存放權重數值。
- **改進方案**：
  - 將 Fully Associative tag array 與 FPGA 內置的 BRAM/URAM 結合，當 Cache Fill 時，真實將 Expert 的權重字節填入 BRAM。
  - 將 `simple_dma_model` 替換為 Xilinx AXI DMA 控制器，配合 AXI-Lite 暫存器配置，實現在 Xilinx Zynq 或 UltraScale+ 板卡上進行上板物理測試與 Timing Signoff。

### 2.4 gem5 Timing-driven 全棧效能整合
- **當前限制**：目前僅有規劃書，未在 gem5 模擬器中實例化。
- **改進方案**：
  - 依據 [gem5_modeling_plan.md](file:///home/a/HCSSimulation/advanced_projects/moe_routing_prefetch/docs/gem5_modeling_plan.md) 實作 `MoEPrefetchAccelerator` SimObject，映射 MMIO 地址。
  - 藉由修改 gem5 既有之 CPU timings (如 O3CPU)，分析由於預取命中率改善對整體計算流水線 IPC (Instructions Per Cycle) 帶來的實際提升幅度，評估軟硬體共同設計在系統級架構下的實際回報率。
