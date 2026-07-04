# 碩士學位論文計劃書 (Thesis Proposal)

## 題目：面向資源受限平台之混合專家大語言模型推理優化：協同記憶體調度與控制通路硬體協同設計
## English Title: Co-design of Collaborative Memory Scheduling and Control-Path Hardware for MoE-LLM Inference on Resource-Constrained Platforms

---

## 1. 緒論 (Introduction)

隨著深度學習技術的飛速發展，大語言模型（Large Language Models, LLMs）在自然語言處理與生成任務中展現出革命性的能力。然而，其龐大的參數規模（通常達數十億至數千億）也帶來了極高的運算與記憶體需求。混合專家模型（Mixture of Experts, MoE）通過門控機制（Gate Mechanism）動態激活少數專家（Active Experts），在保持參數容量的同時降低了單次推理的激活計算量，成為當前大模型架構的核心技術（例如 Qwen-MoE 與 Mixtral）。

儘管 MoE 降低了運算複雜度，但其海量的權重數據仍需駐留在記憶體中。在資源受限平台上（例如 WSL2 虛擬化環境、Intel i5-14400 處理器、僅配備 6GB VRAM 的 RTX 3050 顯示卡），GPU 顯示記憶體（VRAM）容量不足以存放完整的 MoE 權重。因此，頻繁地通過低頻寬的 PCIe 匯流排（PCIe Bus）從系統主記憶體（RAM）將未命中的專家權重動態加載到顯存，成為系統瓶頸所在。同時，自注意力機制（Self-Attention）所產生的 KV Cache 記憶體開銷，也與專家權重在共享匯流排上產生嚴重的頻寬競爭。

本研究提出一種**軟硬體協同設計（Software-Hardware Co-design）**的推理加速方案，結合高層軟體協同調度算法與輕量級 RTL 硬體控制通路前端，動態解決專家權重與 KV Cache 塊的記憶體頻寬衝突。我們在消費級硬體上基於真實的 `Qwen/Qwen1.5-MoE-A2.7B` 門控軌跡（Routing Trace）進行實驗，驗證本提案的可行性。

---

## 2. 背景技術 (Background)

### 2.1 Mixture of Experts (MoE) 門控機制
在解碼階段（Decoding Phase）的每個 Token 運算中，輸入向量經過門控網路計算得到前 $K$ 個最合適的專家索引。這些門控決策是動態且不可預知的。如果被選中的專家權重當前不在顯存中，則觸加載缺失（Demand Miss），運算單元必須停頓（Stall），直至權重從 PCIe 匯流排加載完畢。

### 2.2 KV Cache Page 虛擬化
為避免頻繁的動態記憶體碎片化，現代 LLM 推理框架（如 vLLM）將 KV Cache 分割為固定大小的虛擬塊（Block），並在解碼時動態加載。當解碼序列增長時，KV 塊的讀寫需求將佔用大量的 PCIe 匯流排頻寬。

### 2.3 記憶體匯流排瓶頸
在消費級 CPU-PCIe-GPU 架構中，PCIe Gen4 x8 或 Gen3 x8 的理論頻寬僅為 8-16 GB/s。以 Qwen1.5-MoE-A2.7B 為例，其每個非激活專家大小約為 64MB。加載單個專家需耗時約 4 ms。自注意力運算的計算 Slack（約 2 ms）不足以完全掩蓋此載入延遲，因此專家加載引發了嚴重的停頓。

---

## 3. 動機與 Trace 特徵分析 (Motivation and Trace Characterization)

為評估 MoE 專家的訪問特徵，本研究在 GSM8K 數據集上運行了 `Qwen1.5-MoE-A2.7B`（14.3B 總參數，2.7B 激活參數，Top-4 路由），提取了 cycle-level 的專家門控軌跡（Routing Trace）。數據分析如下：

1. **全局熱度分佈 (Gini Coefficient)：** 全局 Gini 係數僅為 **0.0692**。這表明現代 MoE 在全局範圍內其專家被訪問的熱度極為均勻，並不存在個別明星專家承包多數運算（**無 substantial skew**）的情形。因此，傳統靜態熱度緩存（Static Hot Expert Cache）的命中率上限受限（僅 27.84%）。
2. **時間局部性 (Reuse Distance)：** 重用距離的平均值為 **5.48 Token**，中位數僅為 **3.0 Token**。這表明一旦專家被激活，在極短的時間內將被再次激活，具備顯著的短期時間局部性（Temporal Locality）。
3. **相鄰步驟專家重疊度 (Consecutive Overlap)：** 相鄰 Token 的專家重疊率為 **16.42%**。
4. **熱專家比例 (Hot Expert Ratio)：** 76.67% 的專家處理了 80% 的工作載荷。

**結論：** MoE 推理緩存優化的關鍵在於捕捉短期的**時間局部性**與**相鄰 Token 序列轉移規律**，而非依賴全局靜態熱度。

---

## 4. 獨立 Expert 預取失效分析 (Standalone Expert Prefetch Failure Analysis)

為了捕捉時間局部性，一項直觀的設計是引入獨立的跨層親和預取緩存（Layer-Temporal Affinity Cache）。該緩存基於歷史轉移概率（Transition Probability）預測下一層可能激活的專家並自動發起 PCIe 預取。

我們採用時間精確（Time-accurate）的 PCIe 匯流排衝突模擬器，將此策略在 Qwen 軌跡上運行，結果如下：

* 基準 LRU 緩存命中率為 21.97%，總 PCIe 傳輸量為 316,416.0 MB。
* 跨層親和預取緩存（預取門檻值設為 0.03）成功將命中率提升至 **54.40%**。
* 然而，預取引發了海量的預測流量，總 PCIe 傳輸量暴增至 **4,606,464.0 MB (4.6 TB)**，相較於 LRU 產生了高達 **14.56 倍的流量膨脹（Traffic Expansion）**。
* 由於無節制的預取佔滿了唯一的 PCIe 共享匯流排，後續急需進行計算的 Demand 加載請求被迫排隊，造成高達 287.8 秒的匯流排停頓（Exposed Stall），導致推理速度相較於無緩存架構僅有 **0.09x**（即變慢 10 倍）。

**本研究動機：** 獨立預取器因缺乏對匯流排擁堵的感知，其激進的預取會引發嚴重的**匯流排爭用**。因此，必須引入一個能感知硬體擁堵、協同調度 Demand 加載與 Prefetch 加載的**協同記憶體調度器（Joint Memory Scheduler）**。

---

## 5. 所提之協同記憶體調度器 (Proposed Collaborative Memory Scheduler)

針對共享匯流排衝突，本研究提出協同記憶體調度算法（`full_proposed`），核心機制包括：

1. **關鍵度敏感仲裁 (Criticality-Aware Arbitration)：** 將 KV Cache 的讀取請求與當前解碼步驟急需的 Demand 專家加載標記為 High Criticality；將預測的 Prefetch 加載標記為 Low Criticality。當兩者衝突時，優先保障 Critical 請求。
2. **擁堵感知預取丟棄 (Congestion-Aware Prefetch Dropping)：** 當調度器檢測到 Demand 隊列長度堆積，或當前發生雙重未命中（KV cache 與 Expert 同時 Demand miss），自動開啟丟棄機制，直接丟棄低置信度（Confidence < 0.70）的預取請求，將頻寬全力留給 Demand 載入。
3. **計算與預取重疊 (Prefetch-Compute Overlap)：** 利用注意力機制的計算時段（Attention Slack），在背景並行發送專家預取。

### Latency / Deadline Pareto 權衡分析
在高層模擬中，我們執行了多策略消融實驗：

* 協同調度器 `full_proposed` 並非絕對延遲最低的策略；完全不丟棄預取的 `no_prefetch_drop` 其延遲最低（23,326.5 ms，1.15x 加速），但會引發 3.00% 的請求超時（Deadline Miss）。
* `full_proposed` 通過在擁堵時主動丟棄部分低置信度預取，提供了最優的帕累托前沿（Pareto Frontier）：在維持 **1.13x 延遲加速** 的同時，將請求超時率（Deadline Miss Ratio）壓縮至最優的 **1.00%**。

---

## 6. RTL 控制通路前端 (RTL Control-Path Frontend)

為驗證上述調度算法在晶片上的實現可行性，我們使用 SystemVerilog 設計了硬體控制前端（`moe_accelerator_frontend_proposed`），包含以下硬體模組：
- **Demand Queue & Prefetch Queue:** 分別維護容量為 8 的 FIFO 緩衝區。
- **EDF & Priority Arbiter:** 兩級優先級加截止時間最先（EDF）硬件仲裁器。
- **Prefetch Filter & Congestion Controller:** 實現置信度閾值比較與 Demand 隊列水位監測。

### 硬體綜合結果 (Yosys 0.9)
設計在 Yosys 0.9 下完成電路綜合，統計數據如下：
* **邏輯閘單元 (Logic Cells):** **68,600** 個邏輯單元（主要由多路選擇器構成，用於優先級排序與隊列管理）。
* **暫存器 (DFF Flip-Flops):** **3,405** 個觸發器。
* **記憶體位元 (Memory Bits):** **0**（全部數據以暫存器堆形式實現在片上以換取極低讀寫延遲）。
* **接口兼容性:** 提供 AXI4-Lite 寄存器映射，允許主機 CPU 動態配置預取閾值與隊列閾值。

---

## 7. 實驗方法論 (Experimental Methodology)

本研究的驗證框架由以下三層級構成：
1. **門控軌跡生成 (Trace Profiling):** 基於 PyTorch + HuggingFace Transformers，在 GSM8K 基準測試上運行 `Qwen1.5-MoE-A2.7B` 收集動態路由決策。
2. **高層行為模擬 (High-Level Simulation):** 使用自研 Python 模擬器，基於真實 PCIe 頻寬（16 GB/s）、計算 Slack（Attention 2 ms, MoE 4 ms）與 KV Page 失效率進行策略消融。
3. **硬體級 RTL 驗證 (RTL Testbench Simulation):** 使用 Icarus Verilog 運行含有 14,160 個硬件請求的測試軌跡，並將週期精確（Cycle-accurate）日誌與 Python 黃金模型（Golden Model）進行對比，開展雙模冗餘校對。

---

## 8. 預期成果與可行性證據 (Expected Results and Current Feasibility Evidence)

基於前期的可行性實驗，本研究所提架構的可行性證據如下：

1. **時序對齊驗證 (Timestamp Alignment):**
   RTL 硬件模擬與 Python 黃金模型在請求完成時間（Ready Timestamp）上實現了 **0 週期誤差** 的完全對齊。雖然在內部的周期級仲裁狀態上因硬體流水線暫存器延遲產生了 26,159 週期的狀態偏移，但這證實了硬件邏輯在宏觀功能與完成時間上是精確無誤的。
2. **綜合可行性:** RTL 佔用邏輯資源小（僅 3.4K FFs），能輕易整合至片上 SoC。
3. **優化潛力:** 相比於 Baseline，高層協同調度可降低推理超時至 1.00%，並提供 1.13x 的 decode 推理加速。

---

## 9. 研究限制與挑戰 (Limitations)

1. **RTL 截止時間逾時率高:**
   在 Cycle-accurate 的硬件仿真中，當前 UUT 模型的 request-level deadline miss ratio 高達 **85.52%**（對應 Python 黃金模型為 **35.24%**）。這主要是由於 trace 中給定的截止時間極為嚴苛（Expert 100 週期，KV 25 週期），且硬件 Demand 隊列容量僅為 8，極易造成隊列阻塞。後續研究需對該硬體截止時間進行微調，或擴大 Demand Queue 深度以減緩擁堵。
2. **簡化的計算延遲模型:**
   當前模擬器將自注意力與 MoE 的計算延遲視為常數，而真實硬件上的計算時間受矩陣大小及算子優化程度影響會發生動態波動。

---

## 10. 研究時程規劃 (Timeline)

本研究預計時程為期 12 個月，規劃如下：

* **第 1-2 個月：文獻探討與基礎架構設計**
  * 調研 MoE 緩存與 offloading 推理的最新 ISCA/MICRO 論文。
  * 確定 SoC 片上寄存器映射與匯流排協議。
* **第 3-5 個月：RTL 模組重構與硬體截止時間優化**
  * 改進 `moe_accelerator_frontend_proposed`，加入流水線旁路（Pipeline Bypass）以降低超時率。
  * 增加硬體隊列容量，重新進行綜合與時序分析。
* **第 6-8 個月：Gem5 週期的模擬器整合**
  * 在 `mena-prototype-gem5-dev` 虛擬環境中搭建 CPU-PCIe-GPU 拓撲。
  * 將 RTL 軌跡回放與系統級記憶體流量相結合，實現真實 PCIe 爭用仿真。
* **第 9-10 個月：系統級消融與論文撰寫**
  * 收集完整的 Gem5 週期的加速比、頻寬佔用率與功耗數據。
  * 撰寫學位論文初稿，準備發表研討會論文。
  * 進行高壓縮 KV cache 策略（4-bit）的精確度損失評估。
* **第 11-12 個月：學位論文口試與修改**
  * 學位論文最終修改與投遞，口試答辯。
