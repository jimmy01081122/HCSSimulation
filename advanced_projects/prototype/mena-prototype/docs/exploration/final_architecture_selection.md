# MAEP: Mixture-of-Experts Architecture Exploration Platform
## Final Architecture Selection & Recommendation

This document presents the final architecture selection between Candidate C (Smart DMA Sidecar) and Candidate E (Hybrid Sidecar), establishing the definitive research path for the next 24 months.

---

## 1. The Definitive Choice
We select **Candidate C (Smart DMA Sidecar)** as the primary research architecture.

---

## 2. Answers to the 10 Selection Questions

### 1. 為什麼選它？ (Why did we choose it?)
* **理由**：Candidate C (Smart DMA Sidecar) 達到了最優的風險與回報平衡。它在 28nm 下僅需 **0.15 mm²** 面積，耗費 PYNQ-Z2 僅 **7.5% 的 LUT 資源**，且開發與調試週期僅需 **5 個月**。在效能上，它實現了 **2.79× 的加速**，與複雜的 MENA (2.75×) 相當。這保證了極高的畢業確定性。

### 2. 為什麼放棄另一個？ (Why did we reject the other?)
* **理由**：放棄 Candidate E (Hybrid Sidecar)。雖然 Candidate E 能提供額外 36% 的效能提升（3.80x 加速），但它引入了 **RISC-V 軟核、匯流排仲裁、以及軟硬體編譯器協同** 的複雜度。這將開發與調試週期拉長至 **12 個月（增加 240%）**，且需要消耗 3× 的 FPGA 資源，對於單個碩士生而言，時間風險與 timing 收斂風險過高。

### 3. 最大的風險是什麼？ (What is the biggest risk?)
* **風險**：Reviewer（尤其是電腦架構頂會如 ISCA/MICRO 的審稿人）可能會質疑 Candidate C **過於簡單**，認為其只是「傳統 AXI DMA 控制器的增量（incremental）優化」，缺乏足夠的架構層面創新。

### 4. 最大的研究價值是什麼？ (What is the biggest research value?)
* **價值**：揭示了 MoE 在 expert-major 排程下會遭遇 0% 快取命中率的「階梯式快取崩塌（Thrashing Cliff）」，並在硬體上實作了「自信度預取門檻（Confidence Gating）」與「雙緩衝重疊」，以最簡潔、面積最低的硬體代價解決了 MoE 的記憶體頻寬瓶頸。

### 5. 接下來 6 個月要做什麼？ (Next 6 Months Focus)
* **優化模擬器與數據收集**：
  * 在 MAEP 中新增對 Mixtral、DeepSeekMoE 真實 model trace 的全面測試。
  * 撰寫論文的前半部分（包括 Bottleneck Analysis 和 Locality Exploration 數據）。

### 6. 接下來 12 個月要做什麼？ (Next 12 Months Focus)
* **完成 Candidate C RTL 與 Verilator 測試**：
  * 實作年齡公平排程佇列（Age-based Scheduler RTL）。
  * 實作一階馬可夫自信度預取門檻邏輯（Prefetch Gater RTL）與雙緩衝 handshake。

### 7. 接下來 24 個月要做什麼？ (Next 24 Months Focus)
* **FPGA 部署與論文投稿**：
  * 將 Smart DMA 控制器掛載至 Zynq AXI-HP 匯流排，在 PYNQ-Z2 板端測量真實的 DRAM 頻寬下降率與延遲。
  * 投遞論文至 DATE、DAC 或 FPGA 會議。

### 8. 第一個值得實作的 RTL 是什麼？ (What is the first RTL module worth implementing?)
* **Confidence-based Prefetch Gater (自信度預取門檻控制器)**：這是預防 125% DRAM 頻寬膨脹與快取污染的關鍵電路。

### 9. 第一個值得做的 FPGA Demo 是什麼？ (What is the first FPGA Demo worth doing?)
* **AXI-HP DMA 搬移與雙緩衝乒乓 Handshake**：在 FPGA 上使用 ILA 抓取雙緩衝在 DMA 傳輸與虛擬 NPU 計算之間的 overlapping timeline。

### 10. 若研究失敗，退路是什麼？ (If research fails, what is the backup plan?)
* **退路**：退回到 **純軟體 Runtime 輔助排程與預取（Candidate A）**，在主機端使用多執行緒模擬 overlaps，撰寫系統層面分析論文（投遞至 MLSys 或中階軟體會議如 ICPADS/LCTES），以確保順利畢業。
