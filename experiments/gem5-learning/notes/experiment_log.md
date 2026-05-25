# gem5 實驗記錄

## 使用說明

本檔案用於記錄 gem5 實驗的配置、結果與觀察。
每次進行新的實驗時，請複製相應的模板並填入實驗數據。

---

## 實驗模板

### 基本資訊
- **日期**：YYYY-MM-DD
- **實驗名稱**：
- **目的**：
- **gem5 版本**：（執行 `cd gem5 && git log -1 --oneline` 確認）
- **WSL2 版本**：Ubuntu 22.04 或 24.04

### 系統配置

| 參數 | 值 |
|---|---|
| gem5 binary | ./build/ALL/gem5.opt 或 ./build/X86/gem5.opt |
| Configuration file | configs/... |
| Workload | （指定程式或 resource） |
| CPU 型別 | ATOMIC / TIMING / O3 |
| 核心數 | |
| ISA | x86 / ARM 等 |
| L1I Cache | 大小、關聯度 |
| L1D Cache | 大小、關聯度 |
| L2 Cache | 大小、關聯度 |
| Memory | 大小、型別（DDR3/DDR4） |
| 其他參數 | |

### 執行指令

```bash
./build/ALL/gem5.opt \
  --outdir=PATH_TO_OUTDIR \
  PATH_TO_CONFIG \
  [ADDITIONAL_ARGS]
```

### 重要統計

| 指標 | 值 |
|---|---|
| simInsts | |
| simTicks | |
| hostSeconds | |
| hostTickRate | |
| ipc | |
| L1D misses | |
| L1I misses | |
| L2 misses | |

### 觀察與分析

- 

### 與其他實驗的比較

- 

### 發現與結論

- 

### 問題與排查

如遇到問題，記錄現象、可能原因與解決方案：

**問題 1**：
- 現象：
- 原因：
- 解決：

---

## 已完成的實驗

（在此記錄已執行的實驗）

