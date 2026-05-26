# Topic 06: Cache Hierarchy 快取階層設計與配置

## 1. 學習目標
* 理解快取設計中的核心指標：容量 (Capacity)、相聯度 (Associativity)、區塊大小 (Block size)。
* 深入理解 Harvard 架構下的 L1 快取設計 (I-Cache / D-Cache 分離)。
* 學習如何在 gem5 實做並修改雙階快取 (Two-level Cache) 組態。
* 掌握如何從模擬報告中提取 Cache Hit/Miss 比率。

## 2. 架構與設計思維
快取是現代處理器克服「記憶體牆 (Memory Wall)」的核心機制。設計快取時，架構師必須平衡以下參數：
* **獨立的 L1 指令與資料快取**：處理器在指令擷取階段需要讀取指令快取 (I-Cache)，在執行階段讀寫資料快取 (D-Cache)。如果採用統一快取，會發生結構衝突 (Structural Hazard)。
* **多級快取結構**：L1 快取追求低延遲 (例如 1~4 週期)，容量較小；L2 快取追求高命中率，容量較大，延遲相對較高 (例如 10~20 週期)。

### 快取失誤 (Cache Miss) 的三 C 定律
1. **Compulsory (強制性失誤)**：首次存取該記憶體塊引起的失誤 (可藉由 Prefetcher 降低)。
2. **Capacity (容量失誤)**：快取空間不足，無法容納所有活躍資料。
3. **Conflict (衝突失誤)**：因為映射規則限制，不同記憶體塊映射到相同的快取組 (Set)，造成提前替換 (可增加相聯度改善)。

## 3. 程式碼架構解析
本實驗的 `config.py` 能夠接收 `--l1d-size`, `--l1i-size`, `--l2-size` 命令行參數。
我們使用時序模型 `CPUTypes.TIMING` 來精確觀察當快取容量變化時，對記憶體存取總延遲 ticks 的影響。

## 4. 學生練習 (TODO)
在 `config.py` 中，你需要：
1. 使用 `argparse` 加入 L1i, L1d, L2 大小的配置參數。
2. 呼叫 `PrivateL1PrivateL2CacheHierarchy` 並傳入上述大小。
3. 執行 sweep 腳本比較在不同的快取大小下，相同的程式執行的 ticks 差異。
