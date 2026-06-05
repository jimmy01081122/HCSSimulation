# HCSSimulation - gem5 Learning Project

HCSSimulation 是一個以 gem5 為核心的計算機架構模擬教學專案。本專案專注於 ARM 與 RISC-V 兩種指令集架構，涵蓋從基礎 SE mode 模擬到進階 Design Space Exploration 的完整學習路徑。

## 學習主題

本專案共分為 10 個主題，按照循序漸進的方式安排，每個主題同時提供 ARM 與 RISC-V 兩種版本：

| 編號 | 主題 | 說明 |
|------|------|------|
| 01 | SE Mode | Syscall Emulation 模式基礎 |
| 02 | FS Mode | Full System 模式概念與實作 |
| 03 | Standard Library | gem5 標準函式庫元件介紹 |
| 04 | Python Configuration | Python 組態系統深入解析 |
| 05 | CPU Model | CPU 模型比較 (Atomic/Timing/Minor/O3) |
| 06 | Cache Hierarchy | 快取階層設計與配置 |
| 07 | Memory System | 記憶體子系統探索 |
| 08 | Ruby Coherence | Ruby 快取一致性協定 |
| 09 | Statistics Parsing | 統計資料解析與分析 |
| 10 | Design Space Exploration | 自動化設計空間探索 |

## 目錄結構

```text
HCSSimulation/
├── Dockerfile                 Docker 環境定義
├── README.md                  本文件
├── INDEX.md                   閱讀順序索引
├── prompt.md                  原始需求文件
├── topics/                    主題學習區 (核心內容)
│   ├── 01_se_mode/
│   │   ├── README.md          教學文件
│   │   ├── arm/
│   │   │   ├── config.py      ARM 組態腳本
│   │   │   └── run.sh         ARM 執行腳本
│   │   └── riscv/
│   │       ├── config.py      RISC-V 組態腳本
│   │       └── run.sh         RISC-V 執行腳本
│   ├── 02_fs_mode/
│   │   └── ...
│   └── ... (共 10 個主題)
├── docs/                      參考文件
│   ├── references/            原始教學文件備份
│   └── ...                    研究方向等其他文件
├── scripts/                   輔助腳本
└── gem5/                      gem5 原始碼 (需另行 clone)
```

## 環境需求

- Docker Desktop 或 Docker Engine
- Git
- 記憶體建議 12 GB 以上
- 磁碟空間建議 20 GB 以上 (gem5 編譯產物)

## 快速開始

### 1. 建立 Docker 映像檔

```bash
docker build -t gem5-env .
```

### 2. 啟動容器

```bash
docker run --rm -it -v "$(pwd)":/work gem5-env
```

以下操作均假設已在容器內，專案掛載於 `/work`。

### 3. 取得並編譯 gem5

```bash
cd /work
git clone https://github.com/gem5/gem5
cd gem5
```

依照需要的 ISA 編譯：

```bash
# 編譯 ARM 版本
scons build/ARM/gem5.opt -j$(nproc)

# 編譯 RISC-V 版本
scons build/RISCV/gem5.opt -j$(nproc)
```

### 4. 開始學習

進入 `topics/` 目錄，從 `01_se_mode` 開始按順序學習：

```bash
cd /work/topics/01_se_mode
cat README.md                 # 閱讀教學文件
cd riscv                      # 或 cd arm
./run.sh                      # 執行模擬
```

## 模擬輸出

gem5 的模擬輸出位於各主題的 `m5out/` 目錄，常見檔案包含：

| 檔案 | 說明 |
|------|------|
| `stats.txt` | 效能統計 (simTicks, IPC, cache miss rate 等) |
| `config.ini` | 模擬系統的文字化設定 |
| `config.json` | 模擬系統的結構化設定 (JSON) |

## 建議學習順序

詳細的閱讀順序請參考 [INDEX.md](INDEX.md)。建議按照主題編號 01 至 10 循序學習，每個主題內先閱讀 `README.md`，再依據選擇的 ISA 進入 `arm/` 或 `riscv/` 目錄執行實驗。

### 進階專案

`advanced_projects/` 內保存從既有教學與研究方向延伸出的完整進階專案。

| 專案 | 目錄 | 主題 | 重點 |
|---|---|---|---|
| MoE Routing-aware Prefetch | [advanced_projects/moe_routing_prefetch/](file:///home/a/HCSSimulation/advanced_projects/moe_routing_prefetch/) | MoE expert cache 與 routing-aware prefetch | 使用 synthetic/toy MoE trace、trace-driven hint、Python cache simulator 與 Verilog RTL simulation，評估 expert cache metadata 與 prefetch controller |

### 進階專案驗收與測試

針對 `advanced_projects/moe_routing_prefetch/` 專案：
- **計數器自洽性要求**：Python 行為模擬器與 RTL 模擬器皆須滿足 $\text{cache\_hits} + \text{cache\_misses} == \text{total\_requests}$，且已發送預取數必須等於有效預取與無效預取之和（即 $\text{prefetch\_issued} == \text{prefetch\_useful} + \text{prefetch\_useless}$，在無 in-flight 殘留下）。
- **Python 與 RTL 週期精確對齊**：在相同輸入 Trace 刺激下，Python 模擬器與 RTL 模擬器的所有計數器值必須 100% 吻合（週期精確度為 0 誤差）。
- **自動化比對**：每次修改後應運行 [run_smoke_test.sh](file:///home/a/HCSSimulation/advanced_projects/moe_routing_prefetch/scripts/run_smoke_test.sh)，該腳本會對 Python 和 Verilog 的計數器做自動比對，若不一致將回傳非零代碼並中止整合。

## 參考文件

所有參考文件已歸檔至 `docs/` 目錄：

- `docs/HANDOVER.md` - 專案交接與狀態補充
- `docs/WSL2_GUIDE.md` - Windows/WSL2 環境建置
- `docs/ISA_SELECTION_GUIDE.md` - ARM 與 RISC-V 選擇指南
- `docs/references/` - 原始教學文件備份

## 注意事項

- 此專案使用 Docker 做容器化環境管理
- 啟動指令：`docker run --rm -it -v $(pwd):/work gem5-env`
- 不建議將 `gem5/` 原始碼與編譯產物提交到 Git
- 若編譯時記憶體不足，降低 `-j` 平行度

