# gem5 Learning Framework - RISCV ISA

RISCV 架構特定的 gem5 實驗框架。

## 快速開始

### 編譯 RISCV gem5

```bash
cd /home/a/HCSSimulation/gem5
python3 `which scons` build/RISCV/gem5.opt -j$(nproc)
```

### 執行 Hello World

```bash
./build/RISCV/gem5.opt \
  --outdir=../experiments/gem5-learning-riscv/results/se_hello \
  ../experiments/gem5-learning-riscv/configs/se_hello_riscv.py
```

## 目錄結構

- `configs/` - RISCV 配置檔案
- `scripts/` - 統計解析工具
- `results/` - 模擬輸出
- `workloads/` - RISCV 測試程式
- `notes/` - 實驗記錄

## 配置檔案

| 檔案 | 說明 |
|---|---|
| se_hello_riscv.py | ATOMIC CPU，無 cache |
| se_classic_cache_riscv.py | TIMING CPU，經典 cache |
| se_cpu_select_riscv.py | CPU 型別選擇 |
| se_dse_config_riscv.py | DSE 參數化配置 |

## 執行實驗

```bash
# 基本實驗
./build/RISCV/gem5.opt --outdir=../experiments/gem5-learning-riscv/results/exp1 \
  ../experiments/gem5-learning-riscv/configs/se_hello_riscv.py

# 統計解析
python3 ../experiments/gem5-learning-riscv/scripts/parse_stats_riscv.py \
  results/exp1 --out results/summary.csv
```

## RISCV ISA 特性

- **開放標準**：完全開源
- **簡潔設計**：最小化指令集
- **模組化**：基礎 ISA + 擴展
- **多變體**：RV32I/RV64I + 擴展

## 文件

查看 `/home/a/HCSSimulation/prompt_generated_gem5_tutorial_RISCV.md` 獲得完整教學。

