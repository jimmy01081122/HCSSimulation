# gem5 完整學習指南 - ARM 和 RISCV ISA 版本

歡迎！本項目為您提供了三個完整的 gem5 學習框架，涵蓋不同的指令集架構（ISA）。

---

## 📚 可用版本

### 1. **原始版本（x86 ISA）**
   - 檔案：`prompt_generated_gem5_tutorial.md`
   - 框架：`experiments/gem5-learning/`
   - 用途：通用 gem5 學習
   - 編譯：`build/X86/gem5.opt` 或 `build/ALL/gem5.opt`

### 2. **ARM ISA 版本**
   - 檔案：`prompt_generated_gem5_tutorial_ARM.md` ⭐
   - 框架：`experiments/gem5-learning-arm/`
   - 用途：ARM 架構特定實驗
   - 編譯：`build/ARM/gem5.opt`
   - **特性**：
     - ARMv7/v8 支援
     - 移動設備模擬
     - 功耗效率研究

### 3. **RISCV ISA 版本**
   - 檔案：`prompt_generated_gem5_tutorial_RISCV.md` ⭐
   - 框架：`experiments/gem5-learning-riscv/`
   - 用途：RISCV 架構特定實驗
   - 編譯：`build/RISCV/gem5.opt`
   - **特性**：
     - 開放標準
     - 簡潔指令集
     - 教育優先

---

## 🚀 快速開始指南

### 選擇您的 ISA

您應該選擇哪個版本？

| 選擇 | 理由 |
|---|---|
| **x86 (原始)** | 通用學習，相容性最好 |
| **ARM** | 研究移動/嵌入式系統 |
| **RISCV** | 學習開放標準 ISA |

### 步驟 1：選擇 ISA 版本

**Option A: ARM ISA**
```bash
# 1. 讀取教學文件
cat prompt_generated_gem5_tutorial_ARM.md

# 2. 進入 ARM 框架目錄
cd experiments/gem5-learning-arm

# 3. 查看快速開始
cat README.md
```

**Option B: RISCV ISA**
```bash
# 1. 讀取教學文件
cat prompt_generated_gem5_tutorial_RISCV.md

# 2. 進入 RISCV 框架目錄
cd experiments/gem5-learning-riscv

# 3. 查看快速開始
cat README.md
```

### 步驟 2：編譯對應的 gem5 版本

**對於 ARM：**
```bash
cd /home/a/HCSSimulation/gem5
python3 `which scons` build/ARM/gem5.opt -j$(nproc)
```

**對於 RISCV：**
```bash
cd /home/a/HCSSimulation/gem5
python3 `which scons` build/RISCV/gem5.opt -j$(nproc)
```

### 步驟 3：執行第一個實驗

**ARM Hello World：**
```bash
cd /home/a/HCSSimulation/gem5

./build/ARM/gem5.opt \
  --outdir=../experiments/gem5-learning-arm/results/hello \
  ../experiments/gem5-learning-arm/configs/se_hello_arm.py
```

**RISCV Hello World：**
```bash
cd /home/a/HCSSimulation/gem5

./build/RISCV/gem5.opt \
  --outdir=../experiments/gem5-learning-riscv/results/hello \
  ../experiments/gem5-learning-riscv/configs/se_hello_riscv.py
```

---

## 📖 教學文件位置

### ARM ISA 教學

```
/home/a/HCSSimulation/prompt_generated_gem5_tutorial_ARM.md
```

**包含內容：**
- ARM 架構簡介
- ARM SE mode 實驗
- ARM CPU models (ATOMIC, TIMING, O3)
- ARM cache 層級
- ARM 統計解析
- ARM DSE (設計空間探索)
- 常見 ARM 錯誤排查

### RISCV ISA 教學

```
/home/a/HCSSimulation/prompt_generated_gem5_tutorial_RISCV.md
```

**包含內容：**
- RISCV 架構簡介
- RISCV SE mode 實驗
- RISCV CPU models (ATOMIC, TIMING, O3)
- RISCV cache 層級
- RISCV 統計解析
- RISCV DSE (設計空間探索)
- 常見 RISCV 錯誤排查
- RISCV 優勢說明

---

## 🗂️ 目錄結構

```
/home/a/HCSSimulation/
│
├── 📄 prompt_generated_gem5_tutorial_ARM.md      (ARM 完整教學)
├── 📄 prompt_generated_gem5_tutorial_RISCV.md    (RISCV 完整教學)
├── 📄 prompt_generated_gem5_tutorial.md          (x86 原始教學)
├── 📄 ISA_SELECTION_GUIDE.md                     (本檔案)
│
├── experiments/
│   ├── gem5-learning-arm/
│   │   ├── configs/           (ARM 配置檔案)
│   │   ├── scripts/           (ARM 統計工具)
│   │   ├── results/           (ARM 輸出結果)
│   │   ├── workloads/         (ARM 測試程式)
│   │   ├── notes/             (ARM 實驗記錄)
│   │   └── README.md          (ARM 快速指南)
│   │
│   ├── gem5-learning-riscv/
│   │   ├── configs/           (RISCV 配置檔案)
│   │   ├── scripts/           (RISCV 統計工具)
│   │   ├── results/           (RISCV 輸出結果)
│   │   ├── workloads/         (RISCV 測試程式)
│   │   ├── notes/             (RISCV 實驗記錄)
│   │   └── README.md          (RISCV 快速指南)
│   │
│   └── gem5-learning/         (原始 x86 框架)
│       └── ...
```

---

## 🔄 ISA 對比

| 特性 | ARM | RISCV | x86 |
|---|---|---|---|
| **開放性** | ❌ 閉源 | ✅ 完全開源 | ❌ 閉源 |
| **簡潔性** | 中等 | ✅ 最簡潔 | ❌ 複雜 |
| **業界應用** | ✅ 廣泛（手機） | 新興 | ✅ 廣泛（PC） |
| **學習難度** | 中等 | ✅ 最簡單 | 高 |
| **gem5 支援** | ✅ 完整 | ✅ 完整 | ✅ 完整 |
| **適合研究** | 移動/嵌入式 | 開放標準 | 通用 |

---

## 💡 學習建議

### 初學者
1. **開始**：閱讀 ARM 或 RISCV 教學（選您感興趣的）
2. **執行**：運行 Hello World 實驗
3. **練習**：完成基礎 SE mode 實驗
4. **進度**：學習 CPU models 與 cache

### 有經驗的使用者
1. **跳過**：環境設置和基礎概念
2. **專注**：ISA 特定的優化與特性
3. **實驗**：設計空間探索（DSE）

### 研究人員
1. **深入**：特定架構的微架構細節
2. **比較**：跨 ISA 性能分析
3. **定制**：修改配置進行研究

---

## ⚡ 常見工作流程

### 工作流程 1：快速驗證

```bash
# 選擇 ISA（本例為 ARM）
cd experiments/gem5-learning-arm

# 執行基本實驗
../../gem5/build/ARM/gem5.opt \
  --outdir=results/test1 \
  configs/se_hello_arm.py

# 查看結果
cat results/test1/stats.txt | head -50
```

### 工作流程 2：參數掃描（DSE）

```bash
# 執行不同配置
for cpu in ATOMIC TIMING O3; do
  ../../gem5/build/ARM/gem5.opt \
    --outdir=results/dse_$cpu \
    configs/se_cpu_select_arm.py --cpu $cpu
done

# 解析結果
python3 scripts/parse_stats_arm.py \
  results/dse_* --out results/dse_summary.csv

# 分析
cat results/dse_summary.csv
```

### 工作流程 3：多 ISA 比較

```bash
# ARM 實驗
cd experiments/gem5-learning-arm
../../gem5/build/ARM/gem5.opt \
  --outdir=results/comparison \
  configs/se_hello_arm.py

# RISCV 實驗
cd ../gem5-learning-riscv
../../gem5/build/RISCV/gem5.opt \
  --outdir=results/comparison \
  configs/se_hello_riscv.py

# 比較 stats
diff experiments/gem5-learning-arm/results/comparison/stats.txt \
    experiments/gem5-learning-riscv/results/comparison/stats.txt
```

---

## 🔧 配置檔案速查

### ARM 配置

| 檔案 | CPU | Cache | 用途 |
|---|---|---|---|
| se_hello_arm.py | ATOMIC | NoCache | 快速驗證 |
| se_classic_cache_arm.py | TIMING | L1+L2 | Cache 行為 |
| se_cpu_select_arm.py | 可選 | 可選 | 比較 CPU |
| se_dse_config_arm.py | 參數化 | 參數化 | DSE |

### RISCV 配置

| 檔案 | CPU | Cache | 用途 |
|---|---|---|---|
| se_hello_riscv.py | ATOMIC | NoCache | 快速驗證 |
| se_classic_cache_riscv.py | TIMING | L1+L2 | Cache 行為 |
| se_cpu_select_riscv.py | 可選 | 可選 | 比較 CPU |
| se_dse_config_riscv.py | 參數化 | 參數化 | DSE |

---

## 📊 統計解析

### ARM 統計

```bash
python3 experiments/gem5-learning-arm/scripts/parse_stats_arm.py \
  experiments/gem5-learning-arm/results/exp1 \
  experiments/gem5-learning-arm/results/exp2 \
  --out comparison.csv
```

### RISCV 統計

```bash
python3 experiments/gem5-learning-riscv/scripts/parse_stats_riscv.py \
  experiments/gem5-learning-riscv/results/exp1 \
  experiments/gem5-learning-riscv/results/exp2 \
  --out comparison.csv
```

---

## 🐛 故障排除

### ARM 問題

查看：`prompt_generated_gem5_tutorial_ARM.md` → 第六部分

常見問題：
- `build/ARM/gem5.opt` 不存在 → 編譯 ARM 版本
- ARM 資源找不到 → 檢查 gem5 resources

### RISCV 問題

查看：`prompt_generated_gem5_tutorial_RISCV.md` → 第六部分

常見問題：
- `build/RISCV/gem5.opt` 不存在 → 編譯 RISCV 版本
- RISCV 資源找不到 → 檢查 gem5 resources

---

## 📚 學習路徑建議

### 路徑 A：ARM 架構（推薦從事移動/嵌入式研究）

1. **基礎** (2-3 小時)
   - 讀 ARM 教學第 1-3 部分
   - 執行 se_hello_arm.py
   - 執行 se_classic_cache_arm.py

2. **進階** (4-5 小時)
   - CPU model 比較
   - 統計分析
   - ARM DSE

### 路徑 B：RISCV 架構（推薦學習開放標準）

1. **基礎** (2-3 小時)
   - 讀 RISCV 教學第 1-3 部分
   - 執行 se_hello_riscv.py
   - 執行 se_classic_cache_riscv.py

2. **進階** (4-5 小時)
   - CPU model 比較
   - 統計分析
   - RISCV DSE

### 路徑 C：多 ISA 比較（推薦架構研究者）

1. **並行學習** (5-6 小時)
   - 同時執行 ARM 和 RISCV 實驗
   - 比較性能指標
   - 分析架構差異

---

## ✅ 檢查清單

完成以下項目：

### ARM ISA
- [ ] 讀 ARM 教學文件
- [ ] 編譯 ARM gem5
- [ ] 執行 ARM hello world
- [ ] 執行 ARM cache 實驗
- [ ] 執行 ARM CPU 比較
- [ ] 執行 ARM DSE

### RISCV ISA
- [ ] 讀 RISCV 教學文件
- [ ] 編譯 RISCV gem5
- [ ] 執行 RISCV hello world
- [ ] 執行 RISCV cache 實驗
- [ ] 執行 RISCV CPU 比較
- [ ] 執行 RISCV DSE

---

## 📞 支援資源

- **gem5 官方**：https://www.gem5.org/
- **ARM 架構**：https://developer.arm.com/
- **RISCV 標準**：https://riscv.org/
- **教學文件**：各 ISA 對應的 markdown 檔案

---

## 🎯 下一步

選擇您想學習的 ISA，然後：

1. **讀**：對應的教學文件（ARM 或 RISCV）
2. **設置**：編譯對應的 gem5 版本
3. **運行**：開始第一個實驗
4. **探索**：深入學習該 ISA 的特性

祝您學習愉快！🚀

---

**最後更新**：2024-05-25  
**版本**：1.0 (多 ISA 版本)  
**狀態**：✅ 完成
