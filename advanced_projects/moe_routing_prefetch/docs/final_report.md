# MoE Routing-aware Prefetcher 完整研究實驗報告與模板

本報告整理了本專案的完整實驗分析與結果，並為後續學習者提供了一份正式的作業報告模板與結果說明。

---

## 1. 專案背景與研究動機

Mixture-of-Experts (MoE) 推理的核心挑戰之一在於 **Memory-bound 瓶頸**。不同於傳統稠密模型 (Dense Models) 中所有權重在每個 token 都要參與運算，MoE 在每次 Forward 推理時僅激活少數 Expert (如 Top-1 或 Top-2)。

然而，由於單一 Expert 的權重體積非常大（通常為數百 MB 至數 GB），若快取無法容納所有 Expert，每當發生路由切換且未命中快取時，系統就必須發起外部記憶體 (DDR/HBM) DMA 搬移。在非矩陣運算 (Non-GEMM) 限制下，這類頻繁的 Expert weight loading 延遲成為了主要的效能瓶頸。

本專案藉由軟硬體共設計的思維，探討在 Router 生成專家預選評分後，如何提早發送預取提示 (Prefetch Hint)，並設計專屬的 **RTL Metadata Cache 控制器** 來自動隱藏載入延遲。

---

## 2. 實驗結果與深度分析 (Section C-3)

我們透過批量 sweeps 敏感度實驗，評估了不同的快取配置與政策。以下為 500 個 Token (Zipf 分布, $s=1.2$) 特徵數據下的實驗成果說明：

### 2.1 快取大小敏感度 (Cache Size Sweep)
- **現象說明**：當快取 Way 數從 1 增加至 8 時，所有政策的 Hit Rate 均呈上升趨勢。
- **政策對比**：
  - **Oracle OPT (Belady's OPT)** 提供了最高的快取命中率上限，反映了在無預取情況下的快取理論空間利用率。
  - **Routing-Aware Prefetch (LRU)** 由於提早預取，在多數 Ways 配置下都極為接近或甚至超越了純 LRU 的性能，成功透過預載入隱藏了冷啟動與替換 Miss 的延遲。
  - **FIFO** 表現最差，在大容量下極易發生 Thrashing。

### 2.2 預取距離敏感度 (Prefetch Distance Sweep)
- **最佳距離折衷 (Trade-off)**：
  - 當預取距離 (Prefetch Distance) 太短（如 1 或 2 週期）時，因為預取指令發出太晚，DMA 未能完成便已收到了 Demand 請求，造成許多 Partial Hits。
  - 當預取距離接近 DMA 延遲時間（如 10 到 12 週期）時，Useful Prefetch 達到峰值，快取命中率也達到最優。
  - 當預取距離過長（如超過 16 週期）時，系統會提前發出過多 Hint，這會佔滿 DMA 頻道導致 Busy Drop，同時被預取進來的 Expert 會在被實際使用前，提早被後續的其他預取給擠出快取，造成嚴重的 **Useless Eviction (無效預取驅逐)**。

### 2.3 評分門檻敏感度 (Score Threshold Sweep)
- **頻寬與精確度折衷**：
  - 當 Score Threshold 設為 `0.0` 時，所有 router scores 的 hint 都會被無條件發出，這導致了大量的預取請求。此時，DMA 經常處於 Busy 狀態，造成了極高的 `prefetch_dropped_count`。
  - 當 Score Threshold 提高到 `0.1` ~ `0.2` 時，Filtered Count 增加，只有高信賴度的預期專家才會觸發 DMA。這時候 Useful Prefetch 比例最高，頻寬資源獲得最優化配置。
  - 當 Score Threshold 高於 `0.5` 時，多數 Hint 都被過濾 (Filtered)，快取表現退化回無預取的基礎 LRU 性能。

### 2.4 記憶體延遲敏感度 (Memory Latency Sweep)
- **時序效益分析**：
  - 當 DMA Latency 從 2 週期上升至 50 週期時，無快取 (no_cache) 與傳統 FIFO/LRU 政策的系統總運行時鐘週期 (Estimated Cycles) 呈線性暴增。
  - **Routing-Aware Prefetch (LRU)** 能在 DMA 延遲拉長時，透過 overlap 的方式將大部分載入延遲隱藏。因此其 Cycles 成長斜率顯著低於傳統快取，證明了其在大延遲 (如 DDR 存取) 架構下的獨特價值。

---

## 3. 學生實驗報告撰寫模板 (Section C-2)

本節為後續學習者完成實驗作業時的 Markdown 報告模板。請拷貝並完成以下內容的填寫。

```markdown
# 姓名：[您的名字] | 學號：[您的學號] | 實驗報告

## 1. 實驗環境與硬體參數
- 操作系統與版本：[例如 Ubuntu 22.04 LTS]
- Icarus Verilog 版本：[例如 v11.0]
- Python 版本：[例如 v3.10.12]
- PyTorch 平台：[例如 CPU-only 2.12]

## 2. 煙霧測試結果對齊記錄

請在專案目錄下執行 `bash scripts/run_smoke_test.sh`，並填入您的比對結果表：

### 2.1 FIFO 替換政策比對表
| 指標項目 (Metrics) | Python 統計值 | RTL 統計值 | 是否一致 (Pass/Fail) |
| :--- | :---: | :---: | :---: |
| total_requests | | | |
| cache_hits | | | |
| cache_misses | | | |
| prefetch_issued_count | | | |
| prefetch_useful_count | | | |
| prefetch_useless_count | | | |
| prefetch_dropped_count | | | |
| prefetch_filtered_count| | | |

### 2.2 LRU 替換政策比對表
| 指標項目 (Metrics) | Python 統計值 | RTL 統計值 | 是否一致 (Pass/Fail) |
| :--- | :---: | :---: | :---: |
| total_requests | | | |
| cache_hits | | | |
| cache_misses | | | |
| prefetch_issued_count | | | |
| prefetch_useful_count | | | |
| prefetch_useless_count | | | |
| prefetch_dropped_count | | | |
| prefetch_filtered_count| | | |

## 3. 敏感度 sweeps 實驗觀察

請檢視您執行 `python3 python/run_experiments.py` 後產出的 4 張 PNG 圖表，並回答以下問題：

### 3.1 問題一：Cache Size Sweep 觀察
- 在您的圖表中，當 Cache Size 達到多少 Ways 時，LRU 相比 FIFO 表現出更為顯著的優勢？請嘗試從 Expert 時間局部性 (Temporal Locality) 的角度分析原因。
- [在此寫下您的回答]

### 3.2 問題二：Prefetch Distance 與 Useless Prefetch 的關係
- 當預取距離設為 20 時，Useless Prefetch 的次數有何變化？這對實際硬體設計中的 DMA 頻寬會帶來什麼負面影響？
- [在此寫下您的回答]

### 3.3 問題三：Score Threshold 對於頻寬節省的改善
- 為了在命中率不受嚴重影響的前提下最大限度減少 Dropped Prefetch (節省 DMA 頻寬爭搶)，您認為您的 Sweeps 圖表中所反映的「最佳 score threshold 區間」是多少？
- [在此寫下您的回答]

## 4. 教學反思與建議
- 在本次軟硬體共同設計實驗中，您獲得最大的收穫是什麼？您認為有哪些硬體機制在未來可以進一步以 RTL 實現？
- [在此寫下您的回答]
```
