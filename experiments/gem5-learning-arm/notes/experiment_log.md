# ARM gem5 Experiment Notes Template

## 實驗 1: Hello World 基本驗證

### 日期: ___________
### 目標:
驗證 ARM gem5 環境正確安裝並能執行最基本的 SE mode 模擬

### 步驟:
- [ ] 檢查 gem5 ARM 二進位檔案: `ls -l ../../../gem5/build/ARM/gem5.opt`
- [ ] 執行: `../../../gem5/build/ARM/gem5.opt --outdir=results/hello configs/se_hello_arm.py`
- [ ] 檢查輸出: `ls -l results/hello/`

### 觀察:
```
Total simulation ticks: ____________
Number of host seconds: ____________
Simulation speed: ____________ ticks/sec
```

### 分析:
- 模擬完成時間: ____________
- 發現的問題: ____________

### 筆記:
TODO: 記錄任何有趣的觀察或問題

---

## 實驗 2: 快取層級影響

### 日期: ___________
### 目標:
理解快取配置對系統效能的影響

### 配置比較:
1. L1 32KB, L2 256KB (基準)
2. L1 64KB, L2 256KB (更大 L1)
3. L1 32KB, L2 512KB (更大 L2)

### 結果:

配置 1 統計:
```
L1 hits: ____________
L1 misses: ____________
L2 hits: ____________
```

配置 2 統計:
```
L1 hits: ____________
L1 misses: ____________
L2 hits: ____________
```

配置 3 統計:
```
L1 hits: ____________
L1 misses: ____________
L2 hits: ____________
```

### 結論:
TODO: 分析快取大小對效能的影響

---

## 實驗 3: CPU 模型比較

### 日期: ___________
### 目標:
比較不同 CPU 模型 (ATOMIC, TIMING, O3) 的效能

### 配置:
- ATOMIC CPU:
  - 模擬時間: ____________ ticks
  - 主機執行時間: ____________ 秒

- TIMING CPU:
  - 模擬時間: ____________ ticks
  - 主機執行時間: ____________ 秒

- O3 CPU:
  - 模擬時間: ____________ ticks
  - 主機執行時間: ____________ 秒

### 分析:
哪個 CPU 模型速度最快? ____________
為什麼? ____________

TODO: 詳細比較三種 CPU 模型

---

## 通用筆記區域

### 遇到的問題:
1. ____________
2. ____________

### 解決方案:
1. ____________
2. ____________

### 待探索的主題:
- [ ] TODO: 添加待探索主題
- [ ] TODO: 記錄需要深入學習的主題

### 參考資源:
- gem5 官方文件: https://www.gem5.org/
- 教學文件: ../../prompt_generated_gem5_tutorial_ARM.md
