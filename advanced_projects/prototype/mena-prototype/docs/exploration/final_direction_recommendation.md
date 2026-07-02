# MAEP: Mixture-of-Experts Architecture Exploration Platform
## Final Research Direction Recommendation

This document presents the final research direction recommendation for the MoE Architecture Exploration Platform (MAEP), answering the 11 critical strategy questions for the next 24-month research cycle.

---

## 1. Core Architecture Selection & Roadmap

### 1. 若只能選一個方向，應該選哪個？ (If only one direction, which should we choose?)
* **選擇：Candidate E (Hybrid Sidecar - 智慧 DMA 控制器 + 輕量級 Metadata 核心)**。
* **理由**：Candidate E 在效能上表現最佳（**3.80× 的加速比**，優於 MENA 的 2.75×），且在 28nm 下晶粒面積僅為 **0.50 mm²**。它將控制與傳輸分離，既具備極高的學術新穎性（適合頂級架構會議），又能透過複用開源的 RISC-V 核心控制佇列，避開了自行設計運算單元（NPU）的巨大風險，是最具性價比的研究方向。

---

### 2. 放棄哪些方向？ (Which directions should we reject?)
* **徹底放棄 Candidate B (MENA Accelerator)**：DSE 數據證明，自研 NPU 計算陣列完全被傳輸時間隱藏，算力提升的邊際效益為 0%。自研 NPU 既浪費開發時間，又極易在投稿時被 Reviewer 質疑算力不足，是極度不合理的架構。
* **放棄 Candidate A (Software Runtime)**：純軟體實作缺乏硬體創新，無法投遞電腦架構頂會。

---

### 3. 保留哪些方向作為 Backup？ (Which directions should we keep as backup?)
* **保留 Candidate C (Smart DMA Sidecar) 作為 Backup**：
  * **原因**：若 Candidate E 的 RISC-V 核心在總線掛載或時序收斂上遇到技術瓶頸，可立刻退回 Candidate C（純硬體 DMA 預取與雙緩衝控制器）。Candidate C 開發時間僅需 10 個月，是確保順利畢業的黃金備援方案。

---

### 4. 接下來 6 個月應投入什麼？ (Next 6 Months Focus)
* **Refining MAEP 模擬平台與 Trace 分析**：
  * 實裝對 **DeepSeek-V3/R1** 真實路由 trace 的 offline replay 支援。
  * 在模擬器中精準建模 **CXL 匯流排延遲** 與 **多通道 DRAM 爭搶**，並在 Python 模擬中完成 Candidate E 的編譯器排程調度演算法，為論文準備所有數據圖表。

---

### 5. 接下來 12 個月應投入什麼？ (Next 12 Months Focus)
* **Candidate E/C 的 RTL 開發與驗證**：
  * 實現一階馬可夫預測器（Markov-1 Table）、動態預取門檻暫存器控制邏輯。
  * 實作年齡公平排程佇列（Age-based Scheduler）與雙緩衝 handshake 狀態機，並完成 Verilator 的 Warning-Free 編譯與單元測試。

---

### 6. 接下來 24 個月應投入什麼？ (Next 24 Months Focus)
* **FPGA 系統級整合與論文投稿**：
  * 將 Smart DMA 控制器掛載至 Zynq AXI-HP 總線，與 ARM 主 CPU 進行實體 DMA 權重搬移測試。
  * 撰寫論文並投遞至 ISCA / MICRO / HPCA 等頂級會議，或進行 DATE / DAC 投稿。

---

### 7. 哪些 RTL 完全不值得做？ (Which RTL modules are completely not worth doing?)
* **NPU GEMM 計算陣列、量化（Quantization）執行單元、Softmax 激活硬體**。這些計算電路在雙緩衝覆蓋下對系統加速無任何貢獻。
* **二階馬可夫預測矩陣（Markov-2）**：$E=256$ 時高達 16 MB 的 SRAM 開銷完全不值得用 RTL 實現。

---

### 8. 哪些 FPGA 工作完全不值得做？ (Which FPGA tasks are completely not worth doing?)
* **FPGA NPU Overlay 的開發**。在 FPGA 上部署複雜的卷積或矩陣乘法核會消耗大量邏輯資源並陷入時序收斂地獄，且對驗證 MoE 記憶體傳輸毫無幫助。

---

### 9. 如果目標是畢業，最佳策略是什麼？ (Best strategy for graduation?)
* **採用 Candidate C (Smart DMA Sidecar)**：
  * 專注於將 double buffering 與 history prefetching 實作在 AXI DMA 上，並在 PYNQ-Z2 板上跑通 dummy expert weight 傳輸。
  * 論文主打「面向 MoE 邊緣推論的智慧預取 DMA 控制器」，開發時間短，硬體驗證風險極低，確保 100% 準時畢業。

---

### 10. 如果目標是衝 FPGA/DATE/FCCM，最佳策略是什麼？ (Best strategy for FPGA/DATE/FCCM?)
* **採用 Candidate C (Smart DMA Sidecar) + 實體板端量測**：
  * 重點放在 FPGA 上的實體測量數據。在論文中展示實體 AXI 總線頻寬節省率、DRAM Miss 降低率以及 Zynq 板端的真實功耗下降數據，以實體硬體的高防禦力擊敗對手。

---

### 11. 如果目標是衝 ISCA/HPCA/MICRO，最佳策略是什麼？ (Best strategy for ISCA/HPCA/MICRO?)
* **採用 Candidate E (Hybrid Sidecar) + 全系統模擬**：
  * 論文主打「軟硬體協同的 MoE 執行期控制網關（Software-Hardware Co-design Runtime Gateway）」。
  * 在 gem5 中模擬 RISC-V metadata processor 與 host CPU 的互動，使用 DeepSeek-V3 真實 trace 進行大模型規模化評估，強調架構概念的突破與對下一代稀疏模型的通用性。
