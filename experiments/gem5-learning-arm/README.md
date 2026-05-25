# gem5 Learning Framework - ARM ISA

ARM 架構特定的 gem5 實驗框架。

## 快速開始

### 編譯 ARM gem5

```bash
cd /home/a/HCSSimulation/gem5
python3 `which scons` build/ARM/gem5.opt -j$(nproc)
```

### 執行 Hello World

```bash
./build/ARM/gem5.opt \
  --outdir=../experiments/gem5-learning-arm/results/se_hello \
  ../experiments/gem5-learning-arm/configs/se_hello_arm.py
```

## 目錄結構

- `configs/` - ARM 配置檔案
- `scripts/` - 統計解析工具
- `results/` - 模擬輸出
- `workloads/` - ARM 測試程式
- `notes/` - 實驗記錄

## 配置檔案

| 檔案 | 說明 |
|---|---|
| se_hello_arm.py | ATOMIC CPU，無 cache |
| se_classic_cache_arm.py | TIMING CPU，經典 cache |
| se_cpu_select_arm.py | CPU 型別選擇 |
| se_dse_config_arm.py | DSE 參數化配置 |

## 執行實驗

```bash
# 基本實驗
./build/ARM/gem5.opt --outdir=../experiments/gem5-learning-arm/results/exp1 \
  ../experiments/gem5-learning-arm/configs/se_hello_arm.py

# 統計解析
python3 ../experiments/gem5-learning-arm/scripts/parse_stats_arm.py \
  results/exp1 --out results/summary.csv
```

## ARM ISA 特性

- **完全支援**：ARMv7/v8
- **多核心**：支援多核心配置
- **一致性**：Ruby MESI 支援
- **FS mode**：Full system 模擬支援

## 文件

查看 `/home/a/HCSSimulation/prompt_generated_gem5_tutorial_ARM.md` 獲得完整教學。

