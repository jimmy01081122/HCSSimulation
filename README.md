# HCSSimulation

HCSSimulation 是一個以 gem5 為核心的異構系統架構模擬與教學專案。專案內容分成兩條主線：一條是 x86、ARM、RISC-V 的 gem5 入門實驗框架，另一條是針對資料搬運、MoE routing 與 KV cache prefetch 的架構研究雛形。

此倉庫主要保存 gem5 設定檔、workload 原始碼、執行腳本與教學文件；gem5 原始碼與編譯產物通常不建議提交到 Git。

## 專案內容

### gem5 入門實驗

`experiments/` 內提供三種 ISA 的最小 SE mode 範例與統計解析工具：

| 目錄 | ISA | 主要設定檔 | 統計工具 |
|---|---|---|---|
| `experiments/gem5-learning/` | x86 | `configs/se_hello.py` | `scripts/parse_stats.py` |
| `experiments/gem5-learning-arm/` | ARM | `configs/se_hello_arm.py` | `scripts/parse_stats_arm.py` |
| `experiments/gem5-learning-riscv/` | RISC-V | `configs/se_hello_riscv.py` | `scripts/parse_stats_riscv.py` |

相關完整教學文件：

- `prompt_generated_gem5_tutorial.md`
- `prompt_generated_gem5_tutorial_ARM.md`
- `prompt_generated_gem5_tutorial_RISCV.md`
- `TUTORIAL_START_HERE.md`
- `ISA_SELECTION_GUIDE.md`

### 三個研究方向

`dir/TRY/` 內包含三個較接近架構研究題目的實驗：

| 方向 | 目錄 | 主題 | 重點 |
|---|---|---|---|
| 方向一 | `dir/TRY/dir1_memory_dma/` | 記憶體頻寬與 DMA 資料搬運 | 觀察大量資料複製造成的 bus 與 DRAM 壓力 |
| 方向二 | `dir/TRY/dir2_moe_routing/` | MoE routing 延遲分析 | 以 O3 CPU 觀察 softmax/top-k routing 的 ROI 效能 |
| 方向三 | `dir/TRY/dir3_kv_cache/` | KV cache 分散收集與軟體預取 | 以雙核心與共享 L2 評估 prefetch 對 cache miss 的影響 |

各方向的說明文件位於：

- `docs/direction1_dma_tutorial.md`
- `docs/direction2_moe_routing_tutorial.md`
- `docs/direction3_kv_cache_tutorial.md`

## 目錄結構

```text
HCSSimulation/
├── Dockerfile
├── README.md
├── WSL2_GUIDE.md
├── ISA_SELECTION_GUIDE.md
├── TUTORIAL_START_HERE.md
├── prompt_generated_gem5_tutorial*.md
├── docs/
│   ├── direction1_dma_tutorial.md
│   ├── direction2_moe_routing_tutorial.md
│   ├── direction3_kv_cache_tutorial.md
│   └── visualization_tools_tutorial.md
├── experiments/
│   ├── gem5-learning/
│   ├── gem5-learning-arm/
│   └── gem5-learning-riscv/
├── dir/TRY/
│   ├── dir1_memory_dma/
│   ├── dir2_moe_routing/
│   └── dir3_kv_cache/
└── scripts/
    └── setup_wsl2.sh
```

## 環境需求

建議使用 WSL2 或 Linux 環境搭配 Docker。Dockerfile 已包含 gem5 常用建置套件、Graphviz/pydot，以及 ARM cross compiler。

基本需求：

- Docker Desktop 或 Docker Engine
- Git
- 充足記憶體，建議 12 GB 以上
- gem5 原始碼與對應 ISA 的編譯結果

## 快速開始

### 1. 建立 Docker 映像檔

在專案根目錄執行：

```bash
docker build -t gem5-env .
```

### 2. 啟動容器

```bash
docker run --rm -it -v "$(pwd)":/work gem5-env
```

以下命令假設你已在容器內，且專案掛載於 `/work`。

### 3. 取得並編譯 gem5

若新環境尚未準備 gem5：

```bash
cd /work
git clone https://gem5.googlesource.com/public/gem5
cd gem5
```

依照要執行的 ISA 編譯：

```bash
scons build/X86/gem5.opt -j$(nproc)
scons build/ARM/gem5.opt -j$(nproc)
scons build/RISCV/gem5.opt -j$(nproc)
```

若只需要跑 `dir/TRY/` 內目前的方向實驗，優先編譯 ARM：

```bash
scons build/ARM/gem5.opt -j$(nproc)
```

## 執行入門實驗

### x86

```bash
cd /work/gem5
./build/X86/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_hello \
  ../experiments/gem5-learning/configs/se_hello.py
```

### ARM

```bash
cd /work/gem5
./build/ARM/gem5.opt \
  --outdir=../experiments/gem5-learning-arm/results/se_hello \
  ../experiments/gem5-learning-arm/configs/se_hello_arm.py
```

### RISC-V

```bash
cd /work/gem5
./build/RISCV/gem5.opt \
  --outdir=../experiments/gem5-learning-riscv/results/se_hello \
  ../experiments/gem5-learning-riscv/configs/se_hello_riscv.py
```

## 執行研究方向實驗

三個方向目錄都提供 `run.sh`。建議先確認腳本具有執行權限：

```bash
chmod +x dir/TRY/dir1_memory_dma/run.sh
chmod +x dir/TRY/dir2_moe_routing/run.sh
chmod +x dir/TRY/dir3_kv_cache/run.sh
```

執行方向一：

```bash
cd /work/dir/TRY/dir1_memory_dma
./run.sh
```

執行方向二：

```bash
cd /work/dir/TRY/dir2_moe_routing
./run.sh
```

執行方向三：

```bash
cd /work/dir/TRY/dir3_kv_cache
./run.sh
```

注意：目前各方向腳本可能假設 gem5 位於 `/work/gem5` 或 `/work/gem5-project/gem5`。若你的環境路徑不同，請先打開對應的 `run.sh`，把 gem5 executable、include 目錄與 `libm5.a` 路徑改成實際位置。多數方向實驗使用 ARM gem5，請先確認 `build/ARM/gem5.opt` 已完成編譯。

## 模擬輸出與分析

gem5 的輸出通常會放在 `m5out/` 或 `--outdir` 指定的結果目錄。常見檔案包含：

- `stats.txt`：效能統計，例如 sim ticks、IPC、cache miss、bus transaction。
- `config.ini`：模擬系統的文字化設定。
- `config.json`：模擬系統的結構化設定。
- `config.dot` / `config.dot.pdf` / `config.dot.svg`：由 Graphviz 產生的系統拓樸圖。

解析入門實驗統計：

```bash
python3 experiments/gem5-learning/scripts/parse_stats.py \
  experiments/gem5-learning/results/se_hello \
  --out experiments/gem5-learning/results/summary.csv
```

視覺化工具與 O3 pipeline viewer 的使用方式請參考 `docs/visualization_tools_tutorial.md`。

## 建議學習順序

1. 先閱讀 `TUTORIAL_START_HERE.md`，了解 gem5 SE mode、CPU model 與統計輸出。
2. 依需求選擇 x86、ARM 或 RISC-V，執行 `experiments/` 內的 hello 範例。
3. 閱讀 `ISA_SELECTION_GUIDE.md`，決定後續要使用的 ISA。
4. 進入 `dir/TRY/` 的三個方向實驗，搭配 `docs/` 內文件觀察瓶頸與可優化方向。
5. 使用 `stats.txt`、Graphviz 拓樸圖與 pipeline trace 進行分析。

## 跨環境開發注意事項

- 只提交此專案內的設定檔、原始碼、腳本與文件。
- 不建議提交 `gem5/` 原始碼、`build/`、大型 binary 或大量模擬輸出。
- 新機器可依 `WSL2_GUIDE.md` 與 `scripts/setup_wsl2.sh` 重新建立環境。
- 若編譯 gem5 時記憶體不足，請增加 Docker/WSL2 可用記憶體或降低 `-j` 平行度。
### 進階專案

advanced_projects/ 內保存從既有教學與研究方向延伸出的完整進階專案。

| 專案 | 目錄 | 主題 | 重點 |
|---|---|---|---|
| MoE Routing-aware Prefetch | advanced_projects/moe_routing_prefetch/ | MoE expert cache 與 routing-aware prefetch | 使用 synthetic/toy MoE trace、trace-driven hint、Python cache simulator 與 Verilog RTL simulation，評估 expert cache metadata 與 prefetch controller |

## 參考文件


- `WSL2_GUIDE.md`：Windows/WSL2 環境建置流程。
- `docs/HANDOVER.md`：交接與專案狀態補充。
- `docs/visualization_tools_tutorial.md`：Graphviz 與 O3 pipeline viewer 教學。
- `MULTI_ISA_SUMMARY.md`：多 ISA 版本摘要。
- `FINAL_CHECKLIST.txt`：檢查清單。
