# advanced_moe_routing_prefetch_tutorial.md

# MoE Routing-aware Expert Prefetch 進階專案教學文件

本文件為 `advanced_projects/moe_routing_prefetch/` 進階專案的整合導引。

---

## 1. 專案定位
本專案屬於 `HCSSimulation` 的**進階研究專案**。專案在軟硬體共同設計的脈絡下，探討 Mixture-of-Experts (MoE) 推理系統中，如何透過預期路由資訊 (routing-aware prefetch hint) 來隱藏 Expert 權重從外部記憶體載入的延遲。本專案並不旨在設計矩陣乘法加速器，而是著重於 Expert 快取 (expert cache metadata) 替換演算法、預取控制器、以及 DMA 行為的硬體控制邏輯建模。

---

## 2. 與既有方向的關係
本專案銜接並整合了 `dir/TRY/` 目錄下的三個基礎研究方向：
1. **dir1_memory_dma**：提供 memory/DMA bottleneck 的背景參考，了解外部 memory 傳輸頻寬與延遲對系統的影響。
2. **dir2_moe_routing**：提供 MoE routing 演算法背景。方向四以此為基礎，使用路由得分 (router score) 來預測未來的 expert 需求。
3. **dir3_kv_cache**：提供 prefetch 與 cache miss 的優化思維。本專案借用 prefetch 的概念，並將其目標從 KV cache 轉移到 MoE expert cache metadata。

---

## 3. 為什麼放 advanced_projects 而不是 dir/TRY
- **整合度更高**：本專案並非單一背景的微實驗，而是包含合成 Trace 產生、真實 PyTorch 路由模式分析、Python 快取模擬器、以及完整 Verilog RTL 控制邏輯與 Testbench 驗證的全棧整合專案。
- **不污染基礎教學區**：`dir/TRY/` 中的三個方向是較為簡單的 gem5 入門研究題目。將本專案獨立放置在 `advanced_projects/` 下，能清晰區分「基礎教學實驗」與「完整系統級進階專案」，便於學習者逐步進階。

---

## 4. 快速開始
在 `advanced_projects/moe_routing_prefetch/` 目錄下，您可以透過自動化腳本執行完整流程。

### 步驟 A：產生 Trace 與進行模擬
```bash
# 進入專案目錄
cd advanced_projects/moe_routing_prefetch

# 一鍵執行 smoke test (包含產生 trace、進行 Python 模擬、轉換 testbench hex 檔與 RTL 模擬)
bash scripts/run_smoke_test.sh
```

### 步驟 B：手動執行個別指令

1. **產生合成 Trace 與 Hint Trace**：
   ```bash
   python3 python/generate_synthetic_moe_trace.py \
     --num-experts 8 \
     --top-k 1 \
     --num-layers 1 \
     --num-tokens 64 \
     --distribution zipf \
     --zipf-s 1.2 \
     --prefetch-distance 4 \
     --output traces/smoke_trace.csv \
     --output-hint traces/smoke_hint.csv \
     --seed 42
   ```

2. **執行 Python 快取模擬**：
   ```bash
   python3 python/expert_cache_sim.py \
     --trace traces/smoke_trace.csv \
     --hint-trace traces/smoke_hint.csv \
     --policy routing_aware_next \
     --cache-size 4 \
     --dma-latency 10 \
     --score-threshold 0.1 \
     --output results/smoke_sim.csv
   ```

3. **轉換為 RTL 模擬輸入**：
   ```bash
   python3 python/trace_to_testbench.py \
     --trace traces/smoke_trace.csv \
     --hint-trace traces/smoke_hint.csv \
     --output-req tb/stimulus_req.hex \
     --output-hint tb/stimulus_hint.hex
   ```

---

## 5. 實驗指標
在模擬與 RTL 模擬中，我們主要觀察以下指標：
- **hit_rate (快取命中率)**：反映快取策略對局部性的利用率。
- **prefetch_issued_count**：發出的預取請求次數。
- **prefetch_useful_count / cnt_prefetch_hit**：預取的內容被實際命中且成功隱藏延遲的次數。
- **prefetch_useless_count / cnt_prefetch_useless**：預取的 Expert 在被替換前從未被使用，造成頻寬浪費。
- **prefetch_dropped_count / cnt_prefetch_dropped**：因為 DMA busy 或 duplicate check 而被捨棄的預取次數。
- **estimated_cycles (預估時脈週期)**：評估延遲隱藏對執行時間的整體改進效益。

---

## 6. 限制與不可宣稱事項
- **Metadata-only 限制**：RTL 快取僅建模 metadata (tag array)，不包含實體權重資料的儲存與搬移。
- **FPGA 限制**：第一版僅在 iverilog/Verilator 中進行時序邏輯模擬，不可宣稱等同於實體 FPGA 上的電路效能。
- **Hint 合法性限制**：若 hint 是由未來 target 倒推生成 (如 `synthetic_future_offset`)，則僅能用於 RTL/debug 驗證，不可作為主要效能宣稱。若要宣稱 routing-aware prefetch 的實際效益，必須使用當前 cycle 可取得的 router score/hint。
