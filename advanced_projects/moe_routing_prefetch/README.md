# README.md

# Routing-aware Prefetch for Mixture-of-Experts Inference

本目錄為 HCSSimulation 的獨立進階研究專案：**MoE routing-aware expert prefetch 軟硬體共同設計**。

---

## 1. 專案目的
探討在 MoE 推理架構下，如何利用 router 的 softmax 分布與 prefetch hint，在 expert 被實際需要之前提前將其 metadata 載入快取，以隱藏從外部記憶體搬運 Expert 的延遲。

---

## 2. 為何不做矩陣乘法加速
矩陣乘法加速 (如 systolic array) 在硬體加速器領域中已有成熟且被廣泛探討的方案。本專案聚焦於 MoE 的 memory-bound 瓶頸，實作一個跨層級 (軟體路由得分至硬體預取控制器) 的控制邏輯共同設計，其設計空間更有探索與教學價值。

---

## 3. metadata-only RTL 限制
本 RTL 僅建模 expert cache metadata (tag array/valid/prefetched)，不儲存真實的 expert weight data。Cache fill 與 DMA 傳輸均為控制流與 metadata 層次的模擬，不涉及 BRAM/DRAM 中的實際 byte 資料搬移。

---

## 4. 快速開始與 Smoke Test
您可以透過一鍵執行腳本完成所有流程的煙霧測試：
```bash
# 執行煙霧測試
bash scripts/run_smoke_test.sh
```
成功後會輸出 `SMOKE TEST PASSED`。

---

## 5. Python Simulator 指令
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

---

## 6. RTL Simulation 指令
```bash
# 編譯並執行所有 Verilog testbench
bash scripts/run_iverilog_tests.sh
```
成功後會輸出 `ALL RTL TESTS PASSED`。

---

## 7. 可宣稱與不可宣稱事項
- **可以宣稱**：在給定偏置局部性的 synthetic trace 下，預取策略對命中率及延遲隱藏的改善趨勢；RTL 統計暫存器與 Python 模擬器的結果完全一致。
- **不可宣稱**：Toy model 產生的 trace 與真實大型 LLM 等同；RTL 模擬週期與實體 FPGA/ASIC 執行時間等同。

---

## 8. Phase B TODO
- [ ] 補齊真實 PyTorch MoE trace 產生器 (`generate_toy_moe_trace.py`)
- [ ] 補齊 trace 統計分析與熱力圖繪製工具 (`analyze_trace.py`)
- [ ] 實作 LRU 替換演算法 RTL 模組 (`lru_replacement.v`)
- [ ] 實作批量敏感度實驗與 sweeps 腳本 (`run_experiments.py`)
- [ ] 撰寫完整專案研究報告 (`docs/final_report.md`)
