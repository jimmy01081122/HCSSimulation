# Experiment Operation Guide / 實驗操作詳細教學
# ============================================

## 前言

本文件提供一份從零開始，逐步復現本研究所有實驗結果的完整操作手冊。
面向入門工程師，確保即使沒有該領域經驗也能按照指令完成所有步驟。

---

## 1. 環境建置 (Environment Setup)

### 1.1 前置需求
- WSL2 Ubuntu (已安裝)
- Docker Engine (已安裝)
- Python 3.10+ (已安裝)
- gem5 已編譯 (位於 `/home/a/gem5/build/ARM/gem5.opt`)

### 1.2 安裝 Python 依賴
```bash
pip3 install numpy pandas matplotlib scipy pyyaml pytest
```

### 1.3 建置 Docker 環境
```bash
cd /home/a/HCSSimulation/advanced_projects/ieee_moe_codesign
docker build -t ieee-moe-env -f docker/Dockerfile .
```

### 1.4 驗證 Docker 環境
```bash
docker run --rm ieee-moe-env python3 -c "import numpy; print('OK', numpy.__version__)"
```

### 1.5 驗證 gem5
```bash
docker run --rm \
  -v /home/a/gem5:/gem5 \
  ieee-moe-env \
  /gem5/build/ARM/gem5.opt -h 2>&1 | head -3
```
預期輸出：`Usage` 和 `=====`。

---

## 2. 模擬器使用教學

### 2.1 模擬器架構說明

```
workload_gen.py  ──→  simulator.py  ──→  結果 CSV
                        ├── hardware_model.py  (PCIe bus, GPU, CPU pool)
                        ├── cache.py           (LRU/LFU GPU expert cache)
                        ├── scheduler.py       (FCFS / ELAS / F-ELAS)
                        └── decision_engine.py (TRANSFER / OFFLOAD / DCMD)
```

**執行流程**:
1. `workload_gen.py` 使用 Zipf 分布和 Poisson 到達生成 request 列表
2. `simulator.py` 對每個 request 進行 token-by-token, layer-by-layer 模擬
3. 每個 MoE layer 需要一個 expert：
   - 如果 GPU cache 中有此 expert → GPU 計算 (0.2ms)
   - 如果沒有，由 `decision_engine` 決定：
     - TRANSFER: 透過 PCIe 搬運到 GPU → GPU 計算
     - OFFLOAD: 在 CPU 上直接計算 (4.0ms)
4. 模擬結束後輸出各項效能指標

### 2.2 執行單一配置

```bash
cd /home/a/HCSSimulation/advanced_projects/ieee_moe_codesign

python3 -c "
from simulator.src.workload_gen import WorkloadGenerator
from simulator.src.simulator import HMCSimulator

# 生成 workload
wg = WorkloadGenerator(
    num_requests=15,
    num_experts=16,
    tokens_range=(16, 48),
    layers=6,
    zipf_alpha=1.2,
    arrival_rate=0.15,
    random_seed=42
)
requests = wg.generate()

# 建立模擬器
sim = HMCSimulator(
    gpu_cache_capacity=4,
    pcie_bandwidth_gbs=16.0,
    expert_size_gb=1.0,
    cpu_expert_latency_ms=4.0,
    gpu_expert_latency_ms=0.2,
    gpu_dense_latency_ms=0.5,
    cpu_orch_latency_ms=0.1,
    cpu_cores=8,
    pcie_dma_channels=2,
    scheduling_policy='f-elas',
    offloading_policy='dcmd',
    cache_policy='lru',
    starvation_threshold=5
)

# 執行
results = sim.run(requests)
print(results)
"
```

### 2.3 理解輸出指標

| 指標名稱           | 意義                                      | 理想值   |
|--------------------|-------------------------------------------|----------|
| total_sim_time_ms  | 所有 request 從提交到完成的最長時間        | 越小越好 |
| avg_latency_ms     | 每個 request 的平均端到端延遲              | 越小越好 |
| p99_latency_ms     | 99th percentile 延遲 (tail latency)       | 越小越好 |
| gpu_hit_rate       | GPU cache 命中率 (0.0 ~ 1.0)             | 越大越好 |
| cpu_utilization    | CPU 核心平均使用率                         | 0.5~0.8  |
| gpu_utilization    | GPU 使用率                                 | 越大越好 |

---

## 3. DSE 實驗操作

### 3.1 實驗 1: Cache Capacity Sweep

**目的**: 評估 GPU expert cache 容量對各策略性能的影響

**操作**:
```bash
python3 experiments/scripts/run_single_exp.py \
  --config experiments/configs/exp1_cache_sweep.yaml
```

**預期結果**:
- `experiments/results/exp1_cache_sweep.csv` 包含 30 行數據 (6 cache sizes × 5 strategies)
- 當 cache 小 (C=2) 時，F_ELAS_DCMD 遠優於 FCFS_TRANSFER
- 當 cache 大 (C=16) 時，所有策略趨近相同

**分析要點**:
- Always-Transfer 在 C=2 時的延遲放大倍率
- DCMD 策略在什麼 cache size 下開始選擇 TRANSFER 而非 OFFLOAD
- F-ELAS 與 ELAS 在 max latency 上的差距

### 3.2 實驗 2: PCIe Bandwidth Sweep

**目的**: 模擬不同 PCIe 世代的影響

**操作**:
```bash
python3 experiments/scripts/run_single_exp.py \
  --config experiments/configs/exp2_pcie_sweep.yaml
```

**預期結果**:
- 低頻寬 (8 GB/s ≈ PCIe Gen3) 下 Transfer 策略災難性退化
- 高頻寬 (64 GB/s ≈ PCIe Gen5 x16) 下仍受排隊延遲影響

### 3.3 實驗 3: Concurrency Scaling

**目的**: 評估隨著並發 request 數增加的可擴展性

**操作**:
```bash
python3 experiments/scripts/run_single_exp.py \
  --config experiments/configs/exp3_concurrency.yaml
```

**預期結果**:
- FCFS_TRANSFER 隨並發數增加延遲爆炸
- F_ELAS_DCMD 展現次線性增長

### 3.4 實驗 4: Zipf α Sensitivity

**目的**: 評估 expert 選擇分布的偏斜度對 cache 效果的影響

**操作**:
```bash
python3 experiments/scripts/run_single_exp.py \
  --config experiments/configs/exp4_zipf_alpha.yaml
```

**預期結果**:
- 高 α (=2.0): 少數 expert 被高頻使用，cache 命中率高
- 低 α (=0.8): expert 接近均勻分布，cache 幾乎無效

### 3.5 實驗 5: CPU Core Scaling

**目的**: 評估 CPU 核心數對 offloading 策略的影響

**操作**:
```bash
python3 experiments/scripts/run_single_exp.py \
  --config experiments/configs/exp5_cpu_cores.yaml
```

**預期結果**:
- 2 cores: CPU offloading 嚴重排隊
- 32 cores: CPU offloading 幾乎無等待

---

## 4. gem5 微架構實驗

### 4.1 編譯 C++ 基準測試

```bash
docker run --rm \
  -v /home/a/HCSSimulation:/work \
  -v /home/a/gem5:/gem5 \
  hetero-moe-env \
  bash -c "
    cd /work/advanced_projects/ieee_moe_codesign/gem5_workloads/src
    aarch64-linux-gnu-g++ -static -O3 -o centralized_arm centralized_queue.cpp
    aarch64-linux-gnu-g++ -static -O3 -o distributed_arm distributed_queue.cpp
    aarch64-linux-gnu-g++ -static -O3 -o lock_free_arm lock_free_queue.cpp
  "
```

### 4.2 執行 gem5 SE 模擬

```bash
docker run --rm \
  -v /home/a/HCSSimulation:/work \
  -v /home/a/gem5:/gem5 \
  hetero-moe-env \
  /gem5/build/ARM/gem5.opt \
    /gem5/configs/deprecated/example/se.py \
    --cpu-type=O3CPU \
    --caches --l2cache \
    --l1d_size=64kB --l2_size=512kB \
    -c /work/advanced_projects/ieee_moe_codesign/gem5_workloads/src/centralized_arm \
    --output-dir=/work/advanced_projects/ieee_moe_codesign/experiments/results/gem5/centralized_64k_512k
```

### 4.3 解析結果

```bash
python3 gem5_workloads/scripts/parse_stats.py \
  --input-dir experiments/results/gem5/ \
  --output experiments/results/gem5_summary.csv
```

---

## 5. 圖表生成

### 5.1 生成所有實驗圖表

```bash
python3 experiments/figures/generate_all_figures.py
```

### 5.2 圖表品質要求

- **解析度**: 300 DPI (用 `plt.savefig(fname, dpi=300, bbox_inches='tight')`)
- **字體大小**: 標籤 10pt, 刻度 8pt
- **圖例**: 置於圖內右上或上方
- **色板**: 使用 `tab10` 或自定義色盲友善色板
- **格式**: PDF 用於 LaTeX, PNG 用於預覽
- **標註**: X/Y 軸必須有單位和描述

### 5.3 關鍵繪圖程式碼模板

```python
import matplotlib.pyplot as plt
import matplotlib
matplotlib.rcParams.update({
    'font.size': 10,
    'font.family': 'sans-serif',
    'figure.dpi': 300,
    'axes.grid': True,
    'grid.alpha': 0.3,
})

fig, ax = plt.subplots(figsize=(3.5, 2.5))  # IEEE single column width

# ... plot data ...

ax.set_xlabel('GPU Expert Cache Capacity', fontsize=10)
ax.set_ylabel('Total Simulation Time (ms)', fontsize=10)
ax.legend(fontsize=7, loc='upper right')
plt.tight_layout()
plt.savefig('fig_cache_sweep.pdf', dpi=300, bbox_inches='tight')
plt.savefig('fig_cache_sweep.png', dpi=300, bbox_inches='tight')
```

---

## 6. 疑難排解 (Troubleshooting)

### 問題 1: gem5 報 libpython3.12 錯誤
```
error while loading shared libraries: libpython3.12.so.1.0
```
**解法**: 必須在 Docker 容器中執行 gem5，不能在 host 上直接跑。

### 問題 2: se.py 找不到
```
FileNotFoundError: .../configs/example/se.py
```
**解法**: 使用 `configs/deprecated/example/se.py` 路徑。

### 問題 3: aarch64 binary 執行失敗
```
Exec format error
```
**解法**: 確認使用 `-static` 編譯，且使用 gem5 的 ARM ISA。

### 問題 4: Docker 找不到 gem5
**解法**: 掛載 gem5 目錄: `-v /home/a/gem5:/gem5`

---

## 附錄 A: 完整 Makefile 目標

| Target    | 描述                           |
|-----------|--------------------------------|
| image     | 建立 Docker image              |
| test      | 執行模擬器單元測試             |
| compile   | 編譯 gem5 C++ workloads        |
| exp1-5    | 執行對應實驗                   |
| gem5      | 執行 gem5 微架構驗證           |
| figures   | 生成所有圖表                   |
| all       | 執行以上所有步驟               |
