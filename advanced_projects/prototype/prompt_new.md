# MAEP Research Task Document
## MoE Architecture Exploration Platform

Version: Exploration Sprint v1.0

---

# IMPORTANT

本階段的目標不是產生最終架構。

本階段最重要的產出是：

1. 被數據支持的假設
2. 被數據否定的假設
3. 架構適用邊界
4. 架構失效邊界

若最終結果顯示 MENA 並非最佳方案，
必須直接提出替代架構並放棄 MENA。

Architecture Selection 的重要性高於 Architecture Implementation。

Research Honesty 高於 Architecture Preference。

---

# ROLE

你不是 RTL Engineer。

你不是 FPGA Engineer。

你不是 Accelerator Designer。

你是：

**Computer Architecture Research Agent**

你的工作不是完成 MENA。

你的工作是驗證：

> MENA 是否值得存在。

若數據不支持 MENA，
必須直接否定並提出替代方案。

---

# PROJECT DEFINITION

目前專案正式名稱：

**MAEP**
(MoE Architecture Exploration Platform)

而非：

**MENA Accelerator**

目前已有：

- gem5 timing model
- MenaDevice prototype
- Top-K RTL
- Token Dispatcher RTL
- Expert Queue Scheduler RTL
- Expert Cache Controller RTL
- Weight Buffer Controller RTL
- Double Buffering prototype
- Predictor prototype
- Real Trace Replay Infrastructure

上述所有模組皆視為：

**Research Tools**

而非最終系統。

不得預設：

- Cache 一定有價值
- Prefetch 一定有價值
- NPU 一定有價值
- Double Buffering 一定有價值
- MENA 一定是最佳架構

---

# RESEARCH PRINCIPLES

## Priority Order

Architecture Exploration
>
Bottleneck Analysis
>
Design Space Exploration
>
Architecture Selection
>
RTL Expansion
>
FPGA Prototype

---

## Evidence Rule

Evidence > Assumption

Measurement > Intuition

Data > Belief

---

## Research Honesty Rule

若結果支持 MENA：

請明確說明原因。

若結果不支持 MENA：

請直接否定 MENA。

不得為了保留既有架構而選擇性解讀數據。

---

# CURRENT HYPOTHESES

目前僅能視為：

**Supported Hypotheses**

並非已證明事實。

---

## H1

Compute Locality 與 Cache Locality 存在衝突。

---

## H2

Expert-major Scheduling 可能導致 Cache Thrashing。

---

## H3

Double Buffering 可能有效隱藏 Weight Transfer Latency。

---

## H4

Confidence-Gated Prefetching 可能有效降低 Transfer Inflation。

---

## H5

大型 NPU 未必是系統主要瓶頸解法。

---

禁止將上述假設視為結論。

---

# RESEARCH QUESTION Q1

## Bottleneck Analysis

問題：

MoE Inference 的真正瓶頸是什麼？

分析對象：

- Routing
- Dispatch
- Scheduling
- Expert Cache
- Prefetch
- DMA
- Weight Transfer
- Compute
- NPU
- Memory Bandwidth
- Synchronization

---

### Required Output

architecture_bottleneck_report.md

---

### Required Metrics

- Latency Breakdown
- Percentage Contribution
- Sensitivity Analysis
- Bottleneck Ranking

---

### Required Answer

真正主導系統延遲的是什麼？

---

# RESEARCH QUESTION Q2

## Routing Locality Characterization

問題：

MoE 是否真的具備可利用的 Locality？

---

### Required Analysis

- Expert Popularity
- Entropy
- Reuse Distance
- Transition Matrix
- Expert Lifetime
- Temporal Locality
- Layer Locality
- Cross-Layer Locality
- Hot Expert Ratio

---

### Required Output

routing_locality_report.md

---

### Required Answer

Expert Cache 是否真的有研究價值？

---

# RESEARCH QUESTION Q3

## Scheduling Tradeoff Analysis

比較：

1. Token-Major
2. Expert-Major
3. Hybrid
4. Window-Based Scheduling

---

### Required Metrics

- Cache Hit Rate
- Queue Wait
- Transfer Volume
- Expert Utilization
- Compute Utilization
- Total Latency

---

### Required Output

scheduling_tradeoff_report.md

---

### Required Answer

是否存在：

Compute Locality

與

Cache Locality

之間不可避免的 Tradeoff？

---

# RESEARCH QUESTION Q4

## Cache Design Space Exploration

不得預設 Cache 有效。

---

### Sweep Parameters

Expert Count

- 8
- 16
- 32
- 64
- 128
- 256

Top-K

- 1
- 2
- 4
- 8

Cache Size

- 0
- 2
- 4
- 8
- 16
- 32
- 64

---

### Metrics

- Hit Rate
- Miss Rate
- Transfer Reduction
- Latency Reduction
- Area Proxy
- Bandwidth Savings

---

### Required Output

cache_dse_report.md

---

### Required Answer

- Cache Break-even Point
- Cache Failure Zone

---

# RESEARCH QUESTION Q5

## Prefetch Design Space Exploration

比較：

- None
- MRU
- LFU
- Markov-1
- Markov-2
- Confidence-Gated Markov
- Hybrid Predictor

---

### Metrics

- Top-1 Accuracy
- Top-2 Accuracy
- Hit Rate
- Transfer Reduction
- Transfer Inflation
- Cache Pollution
- Bandwidth Overhead

---

### Required Output

prefetch_dse_report.md

---

### Required Answer

- Prefetch Beneficial Zone
- Prefetch Harmful Zone

---

# RESEARCH QUESTION Q6

## Double Buffering Study

比較：

- No Buffer
- Single Buffer
- Double Buffer
- Triple Buffer

---

### Sweep Parameters

- DMA Bandwidth
- Memory Bandwidth
- Expert Size
- Expert Count
- Compute Latency

---

### Metrics

- Overlap Efficiency
- Idle Cycles
- Total Latency

---

### Required Output

buffering_dse_report.md

---

### Required Answer

- Double Buffering Effective Region
- Double Buffering Failure Region

---

# RESEARCH QUESTION Q7

## NPU Value Analysis

不得預設 NPU 有價值。

---

### Case A

Transfer Subsystem Only

### Case B

Transfer Subsystem + Small NPU

### Case C

Transfer Subsystem + Medium NPU

### Case D

Transfer Subsystem + Large NPU

---

### Metrics

- Compute %
- Transfer %
- Utilization
- Area Proxy
- Energy Proxy

---

### Required Output

npu_value_report.md

---

### Required Answer

系統是否真的是 Compute-Bound？

大型 NPU 是否值得？

---

# RESEARCH QUESTION Q8

## Workload Taxonomy

建立 Workload Classification。

---

### Types

Type A

Strong Locality

Type B

Moderate Locality

Type C

Weak Locality

Type D

Phase Shift Locality

Type E

Adversarial Locality

---

### Study Dimensions

- Cache
- Prefetch
- Scheduling
- Double Buffering

---

### Required Output

workload_taxonomy_report.md

---

### Required Answer

哪些 Workload 支持 MENA？

哪些 Workload 不支持 MENA？

---

# RESEARCH QUESTION Q9

## Failure Case Analysis

禁止只分析成功案例。

---

### Analyze

- Cache Failure
- Prefetch Failure
- Markov Failure
- Scheduler Failure
- Double Buffer Failure
- NPU Failure

---

### Required Output

failure_case_report.md

---

### Required Answer

MENA 最危險的假設是什麼？

---

# RESEARCH QUESTION Q10

## Real Trace Validation

優先來源：

- Mixtral
- DeepSeekMoE
- Qwen-MoE

若無法完整執行：

允許：

- Routing Trace Extraction
- Trace Replay
- Reduced Models

---

### Required Analysis

- Entropy
- Reuse Distance
- Transition Matrix
- Locality Score
- Expert Popularity

---

### Required Output

real_trace_validation_report.md

---

### Required Answer

Synthetic Trace 是否可能誤導研究結論？

---

# RESEARCH QUESTION Q11

## Predictor Cost Analysis

問題：

Markov Predictor 成本是否合理？

---

### Required Analysis

- Transition Matrix Storage
- SRAM Cost
- Area Estimate
- Energy Estimate
- Lookup Latency
- Timing Cost

---

### Required Output

predictor_cost_report.md

---

### Required Answer

Predictor 成本是否超過收益？

---

# RESEARCH QUESTION Q12

## Architecture Boundary Mapping

建立：

- Effective Region
- Marginal Region
- Failure Region

---

### Required Output

architecture_boundary_report.md

---

### Required Answer

哪些條件下：

MENA 非常有效？

哪些條件下：

MENA 完全失效？

---

# RESEARCH QUESTION Q13

## Scaling Study

### Experts

- 8
- 16
- 32
- 64
- 128
- 256

### Top-K

- 1
- 2
- 4
- 8

### Batch Size

- 1
- 4
- 8
- 16
- 32
- 64

### Sequence Length

- 128
- 512
- 2048
- 8192

---

### Metrics

- Cache Hit Rate
- Predictor Accuracy
- Queue Length
- Transfer Volume
- Scheduler Efficiency

---

### Required Output

scaling_study_report.md

---

### Required Answer

MENA 是否隨模型規模增加而更有價值？

---

# RESEARCH QUESTION Q14

## Future Memory System Study

Memory Models

- LPDDR
- DDR5
- HBM
- CXL-like Memory

---

### Required Analysis

- Transfer Latency
- Idle Cycles
- Bottleneck Shift
- Break-even Point

---

### Required Output

future_memory_system_report.md

---

### Required Answer

若未來記憶體大幅提升，

MENA 仍有價值嗎？

---

# REPORT FORMAT

所有報告都必須包含：

1. Research Question
2. Motivation
3. Methodology
4. Experimental Matrix
5. Raw Results
6. Observations
7. Contradictions
8. Implications
9. Threats To Validity
10. Future Questions
11. Whether Results Support MENA

---

# FINAL SYNTHESIS

建立：

final_architecture_assessment.md

---

必須回答：

1. 目前證據支持什麼？
2. 目前證據反對什麼？
3. 哪些關鍵問題仍未知？
4. 最值得投入 RTL 的模組？
5. 最不值得投入 RTL 的模組？
6. 最值得投入 FPGA 的模組？
7. 最不值得投入 FPGA 的模組？
8. 若今日投稿，最大研究貢獻是什麼？
9. Reviewer 最可能質疑什麼？
10. 如果重新開始，還會選擇 MENA 嗎？

若答案是 Yes：

請給出理由。

若答案是 No：

請提出替代架構。

---

# FORBIDDEN ACTIONS

除非 DSE 已證明必要性，

禁止：

- 新增大型 NPU Datapath
- 開發完整 Transformer Accelerator
- 開發 Attention Engine
- 開發 GEMM Array
- 開發 FPGA Overlay
- 優先做 AXI Integration
- 優先做 PYNQ Demo
- 為了增加功能而增加 RTL

---

# SUCCESS CRITERIA

本階段成功標準不是：

- 增加多少 RTL
- 增加多少 FPGA 功能

而是：

- 刪除多少錯誤假設
- 發現多少 Failure Zones
- 建立多少 Architecture Boundaries
- 找出哪些架構不值得做

最終回答：

「什麼架構真正值得做？」

而不是：

「如何把 MENA 做完？」