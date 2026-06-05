# gem5 Learning Project - 閱讀順序索引

## 學習路線圖

請按照以下順序進行學習。每個主題都提供 ARM 和 RISC-V 兩種版本，選擇其中一種 ISA 即可開始。

### 基礎篇 (Topics 01-04)

建立 gem5 模擬的基本觀念與操作能力。

| 順序 | 主題目錄 | 內容 | 預計時間 |
|------|----------|------|----------|
| 1 | [topics/01_se_mode/](topics/01_se_mode/) | SE 模式基礎 - 學習最簡單的模擬方式 | 1-2 小時 |
| 2 | [topics/02_fs_mode/](topics/02_fs_mode/) | FS 模式概念 - 理解全系統模擬 | 1-2 小時 |
| 3 | [topics/03_standard_library/](topics/03_standard_library/) | 標準函式庫 - 掌握元件化設計 | 2-3 小時 |
| 4 | [topics/04_python_config/](topics/04_python_config/) | Python 組態 - 深入 SimObject 系統 | 2-3 小時 |

### 進階篇 (Topics 05-08)

深入理解計算機架構的核心子系統。

| 順序 | 主題目錄 | 內容 | 預計時間 |
|------|----------|------|----------|
| 5 | [topics/05_cpu_model/](topics/05_cpu_model/) | CPU 模型 - 比較不同微架構模型 | 2-3 小時 |
| 6 | [topics/06_cache_hierarchy/](topics/06_cache_hierarchy/) | 快取階層 - 設計與效能分析 | 2-3 小時 |
| 7 | [topics/07_memory_system/](topics/07_memory_system/) | 記憶體系統 - DRAM 控制器與頻寬 | 2-3 小時 |
| 8 | [topics/08_ruby_coherence/](topics/08_ruby_coherence/) | 快取一致性 - Ruby 協定模擬 | 3-4 小時 |

### 應用篇 (Topics 09-10)

將所學知識整合，進行系統分析與設計探索。

| 順序 | 主題目錄 | 內容 | 預計時間 |
|------|----------|------|----------|
| 9 | [topics/09_statistics_parsing/](topics/09_statistics_parsing/) | 統計分析 - 解析與視覺化模擬結果 | 2-3 小時 |
| 10 | [topics/10_design_space_exploration/](topics/10_design_space_exploration/) | 設計空間探索 - 自動化參數掃描 | 3-4 小時 |

## 各主題使用方式

每個主題目錄的結構如下：

```
topics/XX_topic_name/
├── README.md         閱讀此檔案了解概念與教學內容
├── arm/
│   ├── config.py     ARM 版本的 gem5 組態腳本
│   └── run.sh        ARM 版本的執行腳本
└── riscv/
    ├── config.py     RISC-V 版本的 gem5 組態腳本
    └── run.sh        RISC-V 版本的執行腳本
```

操作步驟：
1. 閱讀主題的 `README.md`
2. 選擇 `arm/` 或 `riscv/` 進入對應 ISA 目錄
3. 在 Docker 容器內執行 `./run.sh`
4. 觀察輸出結果並對照 README 中的說明

## 快速命令參考

```bash
# 啟動 Docker 容器
docker run --rm -it -v "$(pwd)":/work gem5-env

# 編譯 gem5 (容器內)
cd /work/gem5
scons build/ARM/gem5.opt -j$(nproc)
scons build/RISCV/gem5.opt -j$(nproc)

# 執行主題實驗
cd /work/topics/01_se_mode/riscv
./run.sh
```

## 參考文件

歸檔於 `docs/` 的參考資料：

| 檔案 | 說明 |
|------|------|
| `docs/ISA_SELECTION_GUIDE.md` | ARM 與 RISC-V 選擇指南 |
| `docs/WSL2_GUIDE.md` | Windows/WSL2 環境建置 |
| `docs/HANDOVER.md` | 專案交接紀錄 |
| `docs/TUTORIAL_START_HERE.md` | 原始入門教學 |
| `docs/references/` | 原始完整教學文件備份 |
