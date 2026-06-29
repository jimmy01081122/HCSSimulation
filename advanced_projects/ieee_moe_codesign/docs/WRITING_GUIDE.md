# IEEE 論文寫作指引 / Paper Writing Guide
# ==========================================

## 1. Target Venues / 目標投稿場所

### Tier-1 Conferences (Recommended)
| Conference | Full Name | Deadline (typical) | Pages |
|------------|-----------|---------------------|-------|
| HPCA       | IEEE Int'l Symp. on High-Performance Computer Architecture | Oct | 11 |
| DAC        | Design Automation Conference | Nov | 6 |
| ICCAD      | IEEE/ACM Int'l Conf. on Computer-Aided Design | Apr | 8 |
| MICRO      | IEEE/ACM Int'l Symp. on Microarchitecture | Apr | 12 |

### Tier-1 Journals
| Journal | Full Name | IF | Pages |
|---------|-----------|-----|-------|
| TVLSI   | IEEE Trans. on VLSI Systems | 2.8 | 14 |
| TC      | IEEE Trans. on Computers | 3.6 | 14 |
| TPDS    | IEEE Trans. on Parallel and Distributed Systems | 5.6 | 14 |
| IEEE Micro | IEEE Micro | 3.4 | 10 |

---

## 2. IEEE 論文格式規範

### 2.1 LaTeX 模板
使用官方 `IEEEtran.cls` 模板：
```latex
\documentclass[conference]{IEEEtran}
% 或 journal 版:
% \documentclass[journal]{IEEEtran}
```

**下載方式** (在 Docker 中):
```bash
# IEEEtran.cls 通常已包含在 texlive 中
# 或手動下載:
# wget https://www.ieee.org/content/dam/ieee-org/ieee/web/org/pubs/conference-latex-template_10-17-19.zip
```

### 2.2 頁面限制
- Conference paper: 6-8 pages (含 references)
- Journal paper: 12-14 pages
- References 通常不計入頁數限制 (依會議規定)

### 2.3 字體與排版
- 正文: 10pt Times New Roman (IEEEtran 預設)
- 圖表標題: 8pt
- 數學符號: 使用 `amsmath` 套件
- 演算法: 使用 `algorithm2e` 或 `algorithmicx` 套件

---

## 3. 各章節寫作要點

### 3.1 Abstract (摘要)
**長度**: 100-150 words
**結構**: Problem → Method → Results → Conclusion

**模板**:
```
Mixture-of-Experts (MoE) models enable efficient scaling of large language
models by activating only a subset of expert parameters per token. However,
serving multiple concurrent MoE inference streams on heterogeneous CPU-GPU
platforms introduces critical bottlenecks: PCIe transfer contention, CPU
orchestration overhead, and finite CPU compute capacity. We present HEXA-MoE,
a software-hardware co-design framework that addresses these challenges
through three key mechanisms: (1) F-ELAS, a fairness-aware expert-locality
scheduling algorithm with aging-based starvation prevention; (2) DCMD-Opt,
a contention-aware dynamic cost model for CPU-GPU workload distribution;
and (3) a distributed expert-affinity queue architecture validated via
cycle-accurate gem5 simulation. Experimental evaluation shows that HEXA-MoE
achieves up to 42.4× latency reduction compared to naive approaches under
resource-constrained scenarios, while guaranteeing bounded tail latency
for all concurrent streams.
```

### 3.2 Introduction (引言)
**長度**: 1.5-2 pages
**段落結構**:

1. **Grand Challenge** (MoE + LLM trend)
   - "The emergence of Mixture-of-Experts (MoE) architectures has..."
   - Cite: Switch Transformer [1], Mixtral, DeepSeek-V3

2. **Problem Context** (multi-agent serving)
   - "In production environments, multiple agents simultaneously..."
   - Cite: vLLM [8], PagedAttention

3. **Three Bottlenecks** (technical problem)
   - "We identify three critical bottlenecks..."
   - Use Figure 1 (architecture diagram) for illustration

4. **Our Approach** (what we do differently)
   - "To address these challenges, we propose HEXA-MoE..."
   - Distinguish from prior work

5. **Contributions** (bullet points, exactly 4)
   ```
   Our main contributions are:
   • We propose F-ELAS, a fairness-aware expert-locality scheduling
     algorithm that reduces tail latency by up to XX% while maintaining
     high throughput.
   • We design DCMD-Opt, a contention-aware dynamic cost model that
     adaptively balances CPU offloading and GPU transfer based on
     real-time queue depths.
   • We validate CPU orchestration overhead through cycle-accurate gem5
     simulation, demonstrating 96.6% cycle reduction with distributed
     expert-affinity queue design.
   • We release an open-source heterogeneous MoE inference simulator
     supporting comprehensive design space exploration.
   ```

6. **Paper Organization**
   - "The remainder of this paper is organized as follows..."

### 3.3 Background and Motivation (背景與動機)
**長度**: 1 page

**子節**:
- **MoE Architecture Primer**: Top-k gating, sparse activation
- **Heterogeneous Deployment**: GPU HBM for hot experts, CPU DRAM for cold experts
- **Motivating Example**: Timing diagram showing 3 concurrent requests
  hitting the same bottleneck

### 3.4 Methodology (方法論)
**長度**: 2-2.5 pages

**關鍵元素**:
- **Algorithm box**: 必須提供 F-ELAS 的演算法偽代碼

```latex
\begin{algorithm}
\caption{F-ELAS: Fairness-aware Expert-Locality Scheduling}
\label{alg:felas}
\begin{algorithmic}[1]
\REQUIRE Active request set $\mathcal{R}$, starvation threshold $\theta_{starve}$
\ENSURE Scheduled request $r^*$
\FOR{each request $r_i \in \mathcal{R}$}
    \IF{$r_i.waiting\_ticks \geq \theta_{starve}$}
        \STATE $r^* \leftarrow r_i$ \COMMENT{Priority promotion}
        \STATE \textbf{return} $r^*$
    \ENDIF
\ENDFOR
\STATE $r^* \leftarrow \arg\max_{r_i \in \mathcal{R}} \text{locality\_score}(r_i)$
\RETURN $r^*$
\end{algorithmic}
\end{algorithm}
```

- **Equations**: DCMD 成本模型

```latex
\begin{equation}
C_{transfer}(e_i) = T_{pcie\_queue} + \frac{S_{expert}}{BW_{pcie}} + T_{gpu}
\label{eq:cost_transfer}
\end{equation}

\begin{equation}
C_{offload}(e_i) = \frac{Q_{cpu}}{N_{cores}} \cdot T_{cpu} + T_{cpu} + T_{result}
\label{eq:cost_offload}
\end{equation}

\begin{equation}
D^*(e_i) = \begin{cases}
\text{TRANSFER} & \text{if } C_{transfer} < C_{offload} \\
\text{OFFLOAD} & \text{otherwise}
\end{cases}
\label{eq:dcmd}
\end{equation}
```

### 3.5 Evaluation (評估)
**長度**: 2-2.5 pages

**Table I**: Experimental Configuration
```latex
\begin{table}[t]
\caption{Default Experimental Configuration}
\label{tab:config}
\centering
\begin{tabular}{ll}
\toprule
\textbf{Parameter} & \textbf{Default Value} \\
\midrule
Number of Experts ($E$) & 16 \\
Expert Size & 1 GB \\
PCIe Bandwidth & 16 GB/s (Gen4 x16) \\
GPU Expert Latency & 0.2 ms \\
CPU Expert Latency & 4.0 ms \\
GPU Dense Layer Latency & 0.5 ms \\
CPU Orchestration Latency & 0.1 ms \\
CPU Cores & 8 \\
PCIe DMA Channels & 2 \\
Starvation Threshold ($\theta$) & 5 ticks \\
Zipf Skewness ($\alpha$) & 1.2 \\
\bottomrule
\end{tabular}
\end{table}
```

**每個實驗子節的結構**:
1. 一句話描述實驗設置
2. 引用圖表 ("as shown in Fig.X")
3. 2-3 句分析關鍵觀察
4. 1 句 insight / takeaway

**Insight 寫作範例**:
```
Insight: Under severely constrained cache capacity (C=2), DCMD-Opt
achieves a 42.4× latency reduction over the transfer-only baseline
by dynamically redirecting all expert computations to CPU cores,
effectively bypassing the PCIe bottleneck at the cost of increased
CPU utilization.
```

### 3.6 Related Work (相關工作)
**長度**: 0.5-0.75 page

**分類**:
1. **MoE Training and Serving Systems**: Tutel [2], DeepSpeed-MoE [3], FasterMoE
2. **Expert Offloading**: MoE-Offloading [4], MoE-Infinity [5], FlexGen [9]
3. **LLM Serving Infrastructure**: vLLM [8], Orca, SARATHI
4. **Hardware Simulation**: gem5 [7], Ramulator, GPGPU-Sim

**寫法**: 每類 2-3 句。最後一段突出我們與其他工作的差異:
```
Unlike prior work that focuses primarily on single-stream inference
optimization, our framework explicitly addresses the multi-stream
contention problem with fairness guarantees.
```

### 3.7 Conclusion (結論)
**長度**: 0.25 page

**結構**:
1. 總結主要成果 (2 sentences)
2. 承認限制 (1 sentence)
3. 未來工作 (1-2 sentences)

```
We presented HEXA-MoE, a heterogeneous CPU-GPU co-design framework for
multi-stream MoE inference scheduling. Our F-ELAS scheduler with DCMD-Opt
decision engine achieves up to 42.4× latency reduction while bounding
tail latency through starvation prevention. Current limitations include
the use of synthetic workloads; future work will integrate real MoE model
traces from production serving systems and prototype the distributed
expert-affinity queue on FPGA.
```

---

## 4. 引用格式

### 4.1 BibTeX 格式範例
```bibtex
@article{fedus2022switch,
  title={Switch Transformers: Scaling to Trillion Parameter Models
         with Simple and Efficient Sparsity},
  author={Fedus, William and Zoph, Barret and Shazeer, Noam},
  journal={Journal of Machine Learning Research},
  volume={23},
  number={120},
  pages={1--39},
  year={2022}
}
```

### 4.2 論文中的引用方式
- 作為主語: "Fedus et al. [1] proposed..."
- 作為註腳: "...efficient sparsity [1]."
- 多篇引用: "...recent works [1], [2], [5]"

### 4.3 必引文獻檢查清單
- [ ] Switch Transformer (Fedus et al.)
- [ ] Tutel (Hwang et al.)
- [ ] DeepSpeed-MoE (Rajbhandari et al.)
- [ ] gem5 simulator (Binkert et al.)
- [ ] vLLM / PagedAttention (Kwon et al.)
- [ ] FlexGen (Sheng et al.)
- [ ] MoE-Offloading (Eliseev & Mazur)
- [ ] MoE-Infinity (Xue et al.)

---

## 5. 圖表品質標準

### 5.1 圖表標題格式
```latex
\begin{figure}[t]
\centering
\includegraphics[width=\columnwidth]{fig_cache_sweep.pdf}
\caption{Impact of GPU expert cache capacity on total simulation time.
F-ELAS-DCMD achieves near-optimal performance across all cache sizes.}
\label{fig:cache_sweep}
\end{figure}
```

### 5.2 表格格式
- 使用 `booktabs` 套件 (`\toprule`, `\midrule`, `\bottomrule`)
- 不使用垂直線
- 數字靠右對齊

### 5.3 不要做的事
- ❌ 使用 Microsoft Word 生成的圖表
- ❌ 截圖作為圖表
- ❌ 使用預設 matplotlib 樣式 (太醜)
- ❌ 圖表中出現中文 (英文論文)
- ❌ 超過 6 種顏色在同一圖中

---

## 6. 中文版論文注意事項

### 6.1 格式
- 使用 Markdown 撰寫 (不使用 LaTeX)
- 保留所有英文專有名詞不翻譯: MoE, PCIe, gem5, DCMD, F-ELAS
- 圖表可以使用中文標題

### 6.2 翻譯原則
- 技術術語保留英文: cache, pipeline, offloading, scheduling
- 動詞和形容詞使用中文: 降低、提升、有效的
- 混合例句: "在 cache capacity 為 2 時，F-ELAS_DCMD 策略的 latency 比 baseline 降低了 42.4 倍"

---

## 7. Peer Review 自檢清單

提交前確認：

### 技術正確性
- [ ] 所有公式編號已引用
- [ ] 所有實驗數據與圖表一致
- [ ] 統計數字精確到合理精度 (1 decimal for ms, 3 decimals for rates)
- [ ] 沒有未定義的符號

### 寫作品質
- [ ] 沒有文法錯誤 (使用 Grammarly 或 ProWritingAid)
- [ ] 沒有重複段落
- [ ] Active voice 優先於 passive voice
- [ ] 避免 "we can see that..." 等冗餘表達

### 格式合規
- [ ] 不超過頁數限制
- [ ] 圖表不超出 column 邊界
- [ ] Reference 格式一致
- [ ] 作者資訊完整 (affiliation, email)
