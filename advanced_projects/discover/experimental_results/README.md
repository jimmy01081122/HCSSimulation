# MoE-LLM Co-Design 實驗備份數據與運行指引 (README)

本目錄備份了混合專家（MoE）大語言模型推理優化及軟硬體協同設計的所有實驗紀錄、軌跡特徵、高層模擬代碼與 RTL 硬體前端驗證數據。

---

## 目錄結構說明

```text
experimental_results/
├── run_pipeline.sh          # 一鍵運行全部模擬與硬體驗證的 Shell 腳本 (附中文參數註解)
├── README.md                # 使用指引手冊 (本文件)
│
├── qwen_moe/                # Qwen1.5-MoE-A2.7B 模型相關實驗結果
│   ├── profiling/           # Qwen-MoE 真實模型在 GSM8K 上的推理分析及 Gate 路由軌跡 CSV
│   │   ├── qwen_moe_gsm8k.py       # GSM8K 推理門控軌跡抓取程式
│   │   ├── qwen_moe_routing_trace.csv
│   │   └── qwen_moe_metrics.json
│   ├── routing/             # 路由特徵分析代碼與圖表 (Gini 係數、重用距離)
│   ├── expert_cache/        # 獨立專家緩存模擬代碼與結果 (包含 LRU, Affinity 等)
│   ├── joint_scheduler/     # 協同記憶體調度模擬代碼與消融實驗數據
│   └── rtl/                 # SystemVerilog 前端設計、測試平台、編譯仿真日誌與綜合指令
│
└── phi2_kv/                 # microsoft/phi-2 模型 KV Cache 壓縮模擬結果
    ├── profiling/           # Phi-2 KV 軌跡與指標數據
    └── kv_cache/            # KV Cache 塊回收模擬、4-bit 壓縮消融模擬代碼與圖表
```

---

## 運行指引 (Quick Start)

您可以使用以下指令一鍵運行所有的分析、模擬與硬體驗證步驟。腳本內部已整合對應的參數定義與詳細註解：

```bash
# 確保位於 experimental_results 備份目錄下
cd /home/a/discover/experimental_results

# 賦予腳本執行權限並運行
chmod +x run_pipeline.sh
./run_pipeline.sh
```

---

## 關鍵程式碼說明與重要參數

### 1. `qwen_moe/profiling/qwen_moe_gsm8k.py`
* **用途**：加載 `Qwen/Qwen1.5-MoE-A2.7B` 模型，使用 `device_map="auto"` 及磁碟 offload 技術以容納 14.3B 參數，運行 GSM8K 數據集並利用 Pytorch Forward Hook 擷取 gate 層的路由專家索引。
* **重要參數**：
  * `device_map="auto"`: 自動切換 CPU/GPU 分流。
  * `offload_folder`: 設定外存 offload 目錄。

### 2. `qwen_moe/joint_scheduler/joint_scheduler_sim.py`
* **用途**：模擬並行 PCIe 共享通道（16 GB/s）下的專家權重加載與 KV 頁傳輸，驗證優先級仲裁與擁堵丟棄策略。
* **重要參數**：
  * `--pcie_bandwidth`: 記憶體通道頻寬（預設 16.0 GB/s）。
  * `--capacity`: 緩存專家個數上限（預設 8 個/層）。

### 3. `phi2_kv/kv_cache/kv_cache_sim.py`
* **用途**：模擬在有限記憶體預算（256MB）下，不同 KV 壓縮與回收算法的重計次數與速度提升。
* **重要參數**：
  * `--num_layers 32`, `--num_heads 32`, `--head_dim 80`: 精確對齊 `microsoft/phi-2` 結構維度。

### 4. `qwen_moe/rtl/moe_accelerator_frontend_proposed.sv`
* **用途**：SystemVerilog 硬體控制通路代碼，實現雙隊列（Demand Queue, Prefetch Queue）仲裁、截止時間追蹤與 prefetch drop 擁堵感知。
