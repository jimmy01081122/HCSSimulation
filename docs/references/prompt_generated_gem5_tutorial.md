# gem5 完整學習與實驗教學指南

本教學目標是幫助已在 WSL2 環境中安裝 gem5 的學習者，提供一份完整、可操作的 gem5 建構引導、實作教學、詳細實驗流程與排查指南。

---

## 前置檢查：確認 gem5 環境

在開始實驗前，請執行以下檢查以確保環境準備完整。

### 1. 確認當前目錄與 gem5 根目錄

執行以下指令確認是否在 gem5 根目錄：

```bash
pwd
```

**預期輸出**：應該看到類似 `/home/a/HCSSimulation/gem5` 或類似路徑。

### 2. 檢查 gem5 根目錄結構

```bash
ls -la | head -20
```

**預期看到以下目錄**：
- `build/` - 編譯後的二進位檔
- `configs/` - 官方配置範例
- `src/` - gem5 源程式碼
- `util/` - 工具程式
- `ext/` - 外部相依

### 3. 確認 gem5 二進位是否存在

```bash
```

**目的**：gem5.opt 是優化版本的 gem5 模擬器，用於實際實驗。

- 若找到 `build/ALL/gem5.opt`，請使用 `./build/ALL/gem5.opt`
- 若兩者都不存在，需要編譯 gem5

### 4. 如果需要編譯 gem5

```bash
cd /home/a/HCSSimulation/gem5
```

**說明**：
- 此命令使用 scons 編譯系統編譯 gem5
- `-j$(nproc)` 使用所有可用 CPU 核心加快編譯
- 編譯通常需要 15-30 分鐘

### 5. 確認 Python 版本

```bash
python3 --version
```

**要求**：Python 3.6 以上（建議 3.8+）

### 6. 確認 WSL2 可用 CPU 核心數

```bash
nproc
cat /proc/cpuinfo | grep processor | wc -l
```

**目的**：了解平行編譯與實驗能力。

### 7. 確認 gem5 standard library 配置範例

```bash
ls configs/example/gem5_library/ | head -20
```


### 8. 確認 Python 環境中 gem5 模組可用

```bash
python3 -c "import sys; sys.path.insert(0, '/home/a/HCSSimulation/gem5'); from gem5.resources import obtain_resource; print('gem5 resources available')"
```

**預期輸出**：`gem5 resources available`

**若出錯**：需要確認 PYTHONPATH 設定或 gem5 編譯完成。

---

## 第一部分：gem5 執行模式總覽

### SE mode 與 FS mode 概念

gem5 提供兩種主要模擬模式，適合不同的實驗需求：

| 項目 | SE mode | FS mode |
|---|---|---|
| **模擬範圍** | 只模擬應用程式執行環境 | 模擬完整系統，包括 OS kernel |
| **是否需要 OS image** | 不需要 | 需要 kernel 與 disk image |
| **啟動速度** | 快速（秒級） | 緩慢（分鐘級） |
| **可觀察現象** | 應用級別性能指標、cache 行為 | 系統級別行為、OS 互動 |
| **適合實驗** | CPU model、cache、memory 參數掃描 | 系統優化、驅動程式、OS 行為 |
| **缺點** | 無法觀察 OS 層面現象 | 啟動慢、資源消耗大 |

### 為什麼初學者應先從 SE mode 開始

1. **快速迭代**：SE mode 模擬快速，適合快速驗證假設
2. **專注架構**：無需 OS 複雜性，可集中學習微架構設計
3. **資源需求低**：SE mode 需要的計算資源較少
4. **便於 DSE**：設計空間探索（Design Space Exploration）在 SE mode 中更實用

### WSL2 與 KVM 注意事項

- FS mode 若使用 KVM CPU 模型，需要 nested virtualization 支援
- WSL2 上 KVM 支援受限於 Windows Hyper-V 設定
- 建議初期使用 TIMING 或 ATOMIC CPU 進行 FS mode 功能驗證，再考慮 KVM

---

## 第二部分：gem5 standard library 核心概念

### standard library 的角色

gem5 standard library 是一套預先設計的 Python 模組，提供可重用的系統設計 component，大幅降低初期配置負擔。

### 核心 Component 介紹

#### 1. Board

`Board` 是系統的頂級容器，負責連接所有硬體 component 並管理它們的互動。

**作用**：
- 統一管理 Processor、Memory、CacheHierarchy
- 自動處理 port 連接
- 提供統一的 workload 設定介面

**可選 Board 類型**：
- `SimpleBoard`：簡單系統，適合基礎實驗

#### 2. Processor

`Processor` 代表 CPU 或 CPU 集群。

**作用**：
- 容納 CPU core
- 支援多核心配置
- 定義 ISA（Instruction Set Architecture）

**主要參數**：
- `cpu_type`：`CPUTypes.ATOMIC`、`CPUTypes.TIMING`、`CPUTypes.O3` 等
- `isa`：`ISA.X86`、`ISA.ARM64` 等
- `num_cores`：核心數量

#### 3. Memory

`Memory` 代表系統記憶體系統。

**作用**：
- 定義記憶體大小
- 配置記憶體控制器
- 支援單通道或多通道配置

**常見配置**：
- `SingleChannelDDR3_1600`：單通道 DDR3-1600 記憶體
- 支援自訂記憶體大小（預設通常 2 GiB）

#### 4. CacheHierarchy

`CacheHierarchy` 定義 cache 層級與協議。

**作用**：
- 建立 L1I、L1D、L2 等 cache 層級
- 選擇 cache 一致性協議（如 MESI、MOESI）
- 定義 cache 參數（大小、關聯度）

**常見配置**：
- `NoCache`：無 cache（用於 ATOMIC CPU）
- `PrivateL1PrivateL2CacheHierarchy`：經典 cache 層級
- `MESITwoLevelCacheHierarchy`：含 MESI 協議的 Ruby cache

#### 5. Simulator

`Simulator` 是模擬執行引擎。

**作用**：
- 管理模擬時間
- 控制模擬執行
- 管理 workload

**主要用法**：
```python
simulator = Simulator(board)
simulator.run()
```

#### 6. obtain_resource / Resources

`obtain_resource` 函數用於下載預構建的測試程式或系統鏡像。

**作用**：
- 支援本地快取，避免重複下載
- 簡化 workload 取得流程

### standard library 為什麼能降低配置負擔

**傳統方式的問題**：
- 需要手動建立 SimObject（如 Cache、CPU、Memory）
- 需要手動連接所有 port
- 需要手動定義記憶體位址範圍
- 易出錯且重複勞動

**standard library 的優勢**：
- Component 已預先整合，無需手動 port 連接
- 常見的系統配置已參數化
- 減少學習曲線，更適合教學
- 可重用常見 baseline，加快實驗迭代

---

## 第三部分：實驗 A - SE mode 最小可執行範例

### 目標

建立第一個 gem5 SE mode 配置，執行簡單的 hello world 程式，驗證 gem5 環境正常運作。

### 建立配置檔案

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/configs/se_hello.py <<'EOF'
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.caches.cache_hierarchy import CacheHierarchy
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

# Step 1: 建立 Processor
# CPUTypes.ATOMIC 是原子 CPU 模型，每條指令在一個週期完成
# 適合快速驗證功能，但不考慮微架構細節
processor = SimpleProcessor(
    cpu_type=CPUTypes.ATOMIC,
    num_cores=1
)

# Step 2: 建立 Memory
# SingleChannelDDR3_1600 預設為 2 GiB
from gem5.components.memory.multi_level_memory import MultiLevelMemoryHierarchy
from gem5.components.memory.memory import MemorySizeType
from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600

memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

# Step 3: 建立 Cache 層級
# NoCache 表示不使用 cache，直接連接到記憶體
from gem5.components.cachehierarchy.caches.cache_hierarchy import NoCache

cache_hierarchy = NoCache()

# Step 4: 建立 Board
# Board 自動連接 Processor、Memory 與 CacheHierarchy
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

# Step 5: 設定 Workload
# set_se_binary_workload 設定該程式為要模擬的任務
board.set_se_binary_workload(binary_path)

# Step 6: 建立 Simulator 並執行
simulator = Simulator(board=board)
simulator.run()
EOF
```

### 檔案詳解

**Import 說明**：

1. `sys.path.insert(0, '/home/a/HCSSimulation/gem5')`：將 gem5 路徑加入 Python 搜尋路徑
2. `from gem5.resources import obtain_resource`：用於下載測試程式
3. `from gem5.components.boards.simple_board import SimpleBoard`：簡單系統板卡
4. `from gem5.components.cachehierarchy.caches.cache_hierarchy import CacheHierarchy`：cache 管理
5. `from gem5.components.processors.simple_processor import SimpleProcessor`：簡單 CPU
6. `from gem5.components.processors.cpu_types import CPUTypes`：CPU 類型定義
7. `from gem5.components.processors.isa import ISA`：ISA 定義

**Component 建立說明**：

1. **Processor**：
   - `cpu_type=CPUTypes.ATOMIC`：選擇 ATOMIC CPU，每條指令原子執行（1 cycle）
   - 原因：ATOMIC 無需建模微架構，快速驗證程式是否能正確執行
   - `num_cores=1`：單核心

2. **Memory**：
   - `SingleChannelDDR3_1600`：預設單通道 DDR3-1600 記憶體
   - `size=MemorySizeType.GB_2`：設定為 2 GiB
   - 原因：hello world 程式執行所需記憶體很小，2 GiB 足夠

3. **CacheHierarchy**：
   - `NoCache()`：無 cache 層級
   - 原因：ATOMIC CPU 不需 cache 模型，cache 只在 TIMING 及以上 CPU 才有效

4. **Board**：
   - `clk_freq="3GHz"`：系統時鐘頻率 3 GHz
   - 自動連接 processor、memory、cachehierarchy
   - 原因：Board 簡化了 port 連接的複雜度

5. **Simulator**：
   - 執行模擬

### 執行實驗

首先進入 gem5 根目錄：

```bash
cd /home/a/HCSSimulation/gem5
```

執行模擬：

```bash
./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_hello \
  ../experiments/gem5-learning/configs/se_hello.py
```

**若找不到 `build/ALL/gem5.opt`**，改用：

```bash
  --outdir=../experiments/gem5-learning/results/se_hello \
  ../experiments/gem5-learning/configs/se_hello.py
```

### 預期輸出

模擬執行會產生以下文件在 `experiments/gem5-learning/results/se_hello/`：

- `config.ini`：完整配置
- `config.json`：JSON 格式配置
- `stats.txt`：模擬統計
- `simdebug.log`（可選）：debug 資訊

### 驗證成功

檢查輸出檔案是否存在且包含有效內容：

```bash
ls -la ../experiments/gem5-learning/results/se_hello/
cat ../experiments/gem5-learning/results/se_hello/stats.txt | head -50
grep "Hello" ../experiments/gem5-learning/results/se_hello/simdebug.log
```

若最後一個指令找到 "Hello" 或類似輸出，表示程式成功執行。

### 常見錯誤排查


原因：gem5 resources 需下載或本地資源不存在

解決：
```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.resources import obtain_resource
print(f'Resource at: {path}')
"
```

**錯誤 2**：`No module named 'gem5'`

原因：Python 路徑未正確設定

解決：
```bash
export PYTHONPATH=/home/a/HCSSimulation/gem5:$PYTHONPATH
```

**錯誤 3**：`build/ALL/gem5.opt: command not found`

原因：gem5 未編譯

解決：編譯 gem5（見前置檢查）

---

## 第四部分：實驗 B - SE mode + Classic Cache Hierarchy

### 目標

加入 cache 層級，使用 TIMING CPU 進行微架構級別的模擬，觀察 cache 行為對性能的影響。

### 概念介紹

**為什麼要加入 cache**：
- ATOMIC CPU 忽略微架構細節，無法觀察 cache 効果
- TIMING CPU 精確模擬 cache 存取，可觀察 cache hit/miss

**為什麼 CPU 要從 ATOMIC 改成 TIMING**：
- ATOMIC 每條指令 1 cycle，cache 無意義
- TIMING CPU 考慮記憶體延遲，cache 能減少平均存取時間

**Classic cache hierarchy**：
- gem5 提供的經典 cache 層級模型
- 由 L1I（指令 cache）、L1D（資料 cache）、L2（二級 cache）組成
- 使用 MOESI 一致性協議（四態，不如 MESI 複雜但夠用）

**Cache 參數意義**：
- **大小**：cache 能存儲多少資料（KiB、MiB）
- **Associativity（關聯度）**：一個位址可能存放在 cache 中的位置數
  - Fully associative：任何位置（最靈活，延遲高）
  - Direct mapped：唯一位置（快速，衝突多）
  - Set associative：N 個位置（折中）

### 建立配置檔案

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/configs/se_classic_cache.py <<'EOF'
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.caches.l1_caches import L1Cache
from gem5.components.cachehierarchy.caches.l2_cache import L2Cache
from gem5.components.cachehierarchy.caches.cache_hierarchy import CacheHierarchy
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

# Step 1: 建立 Processor，使用 TIMING CPU
# TIMING CPU 精確模擬記憶體訪問延遲，適合觀察 cache 效果
processor = SimpleProcessor(
    cpu_type=CPUTypes.TIMING,
    num_cores=1
)

# Step 2: 建立 Memory
from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600

memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

# Step 3: 建立 Cache 層級
# PrivateL1PrivateL2CacheHierarchy：每個核有私有 L1I/L1D，共享 L2
from gem5.components.cachehierarchy.caches.cache_hierarchy import (
    PrivateL1PrivateL2CacheHierarchy
)

# 定義 L1I cache：32 KiB，8-way associative
l1i_cache = L1Cache(
    size="32KiB",
    assoc=8,
    tag_latency=1,
    response_latency=1,
    mshrs=16,
    tgts_per_mshr=20,
    is_icache=True  # 指令 cache
)

# 定義 L1D cache：32 KiB，8-way associative
l1d_cache = L1Cache(
    size="32KiB",
    assoc=8,
    tag_latency=2,
    response_latency=2,
    mshrs=16,
    tgts_per_mshr=20,
    is_icache=False  # 資料 cache
)

# 定義 L2 cache：256 KiB，16-way associative
l2_cache = L2Cache(
    size="256KiB",
    assoc=16,
    tag_latency=20,
    response_latency=20,
    mshrs=20,
    tgts_per_mshr=12
)

# 建立 cache hierarchy
cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1d_cache=l1d_cache,
    l1i_cache=l1i_cache,
    l2_cache=l2_cache
)

# Step 4: 建立 Board
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

# Step 5: 設定 Workload
board.set_se_binary_workload(binary_path)

# Step 6: 執行模擬
simulator = Simulator(board=board)
simulator.run()
EOF
```

### 檔案詳解

**新增 Component**：

1. **L1Cache（指令 cache）**：
   - `size="32KiB"`：32 KB 指令 cache
   - `assoc=8`：8-way 關聯，減少衝突
   - `is_icache=True`：指標為指令 cache

2. **L1Cache（資料 cache）**：
   - `size="32KiB"`：32 KB 資料 cache
   - `is_icache=False`：標記為資料 cache

3. **L2Cache**：
   - `size="256KiB"`：256 KB L2 cache
   - `assoc=16`：16-way 關聯，減少衝突
   - 延遲比 L1 高（tag_latency=20 vs 1-2），反映遠程 cache 的延遲

4. **PrivateL1PrivateL2CacheHierarchy**：
   - 將 L1I、L1D、L2 組織成層級
   - "Private" 意為每個核心有私有 L1/L2，但 L3 或以上可能共享

### 執行實驗

```bash
cd /home/a/HCSSimulation/gem5

./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_classic_cache \
  ../experiments/gem5-learning/configs/se_classic_cache.py
```

### 統計分析

提取關鍵性能指標：

```bash
grep -E "simInsts|simTicks|numCycles|hostSeconds|ipc" \
  ../experiments/gem5-learning/results/se_classic_cache/stats.txt | head -30
```

查看 cache 統計：

```bash
grep -iE "system.cpu.dcache|system.cpu.icache|system.l2|miss|hit" \
  ../experiments/gem5-learning/results/se_classic_cache/stats.txt | head -50
```

### 關鍵統計欄位解釋

| 欄位 | 意義 |
|---|---|
| `simInsts` | 模擬執行的指令數 |
| `simTicks` | 模擬時鐘週期數 |
| `numCycles` | CPU 執行週期（不同 CPU 類型定義可能不同） |
| `hostSeconds` | 實際 host 計算機消耗的時間（秒） |
| `ipc` | Instructions Per Cycle（指令/週期），性能指標 |
| `dcache.misses` | L1D cache miss 數 |
| `dcache.hits` | L1D cache hit 數 |
| `icache.misses` | L1I cache miss 數 |
| `l2.misses` | L2 cache miss 數 |

### 與 Experiment A 比較

執行以下指令比較 ATOMIC (no cache) 與 TIMING (with cache) 的效能差異：

```bash
echo "=== SE Hello (ATOMIC, no cache) ==="
grep "simTicks\|ipc" ../experiments/gem5-learning/results/se_hello/stats.txt | head -5

echo "=== SE Classic Cache (TIMING, with cache) ==="
grep "simTicks\|ipc" ../experiments/gem5-learning/results/se_classic_cache/stats.txt | head -5
```

### 常見錯誤排查

**錯誤 1**：`AttributeError: no attribute 'TIMING'`

原因：gem5 版本差異，TIMING CPU 型別名稱不同

解決：檢查可用 CPU 類型：
```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.components.processors.cpu_types import CPUTypes
print([attr for attr in dir(CPUTypes) if not attr.startswith('_')])
"
```

**錯誤 2**：`Cache miss not found in stats`

原因：stats key 名稱因 gem5 版本而異

解決：檢查 stats 檔案中實際 key 名稱：
```bash
grep -i "cache\|miss" ../experiments/gem5-learning/results/se_classic_cache/stats.txt | head -20
```

---

## 第五部分：CPU Model 教學與實驗

### CPU Model 概述

gem5 提供多種 CPU 模型，不同模型在模擬精度與速度間權衡。

| CPU Model | 模型 | 精度 | 速度 | 適用場景 |
|---|---|---|---|---|
| **AtomicSimpleCPU** | `CPUTypes.ATOMIC` | 低 | 快 | 功能驗證 |
| **TimingSimpleCPU** | `CPUTypes.TIMING` | 中 | 中 | Cache 行為、基本性能 |
| **O3CPU** | `CPUTypes.O3` | 高 | 慢 | 微架構優化、乱序執行 |
| **KVMCPU** | `CPUTypes.KVM` | 低* | 快** | 結合真實 CPU 與虛擬化 |

*KVM 精度低是因為大部分指令由 host CPU 直接執行，只截獲特定事件

**KVM 在某些情況下甚至比真實 CPU 執行更快

### CPU Model 詳解

#### 1. ATOMIC（AtomicSimpleCPU）

**特點**：
- 每條指令在 1 個週期完成（無記憶體延遲）
- 無 cache 模型
- 無管線、無乱序

**用途**：
- 快速驗證程式功能正確性
- SE mode 初期測試
- functional verification

**優缺點**：
- 優點：超快速
- 缺點：無法觀察微架構現象

#### 2. TIMING（TimingSimpleCPU）

**特點**：
- 模擬記憶體延遲
- 支援 cache 模型
- 簡單管線（不支援亂序執行）

**用途**：
- cache 行為研究
- 基本性能分析
- SE mode 標準選擇

**優缺點**：
- 優點：精度 vs 速度的好平衡
- 缺點：無法模擬亂序執行效果

#### 3. O3CPU

**特點**：
- 完整超標量亂序執行核心模型
- 支援指令重排、猜測執行
- 複雜管線、預測器

**用途**：
- 微架構優化研究
- 現代 CPU 行為模擬
- 高精度性能預測

**優缺點**：
- 優點：高精度
- 缺點：執行非常慢，DSE 時間成本大

#### 4. KVMCPU

**特點**：
- 使用 Linux KVM 加速模擬
- 大部分指令由 host CPU 直接執行
- 只模擬指定的 SimObject 行為

**用途**：
- FS mode 快速啟動
- 驅動程式測試
- 系統級行為驗證

**局限**：
- WSL2 上 KVM 支援受限（nested virtualization）
- 無法精確模擬微架構細節

### SE mode 選擇建議

1. **初期探索**：ATOMIC（快速驗證功能）
2. **性能分析**：TIMING（cache 與基本延遲）
3. **微架構優化**：O3（需要時間成本）
4. **系統行為**：TIMING 或 O3（SE mode 較少用）

### CPU Model 選擇配置檔案

建立支援參數選擇 CPU 類型的配置：

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/configs/se_cpu_select.py <<'EOF'
import sys
import argparse
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.caches.cache_hierarchy import (
    NoCache,
    PrivateL1PrivateL2CacheHierarchy
)
from gem5.components.cachehierarchy.caches.l1_caches import L1Cache
from gem5.components.cachehierarchy.caches.l2_cache import L2Cache
from gem5.components.memory.memory import MemorySizeType
from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

# 命令行參數解析
parser = argparse.ArgumentParser(
    description="SE mode configuration with selectable CPU type"
)
parser.add_argument(
    "--cpu",
    choices=["ATOMIC", "TIMING", "O3"],
    default="ATOMIC",
    help="CPU type: ATOMIC (fast, no details), TIMING (medium), O3 (slow, detailed)"
)
parser.add_argument(
    "--cores",
    type=int,
    default=1,
    help="Number of CPU cores"
)

args = parser.parse_args()

# 根據參數選擇 CPU 類型
cpu_type_map = {
    "ATOMIC": CPUTypes.ATOMIC,
    "TIMING": CPUTypes.TIMING,
    "O3": CPUTypes.O3,
}

cpu_type = cpu_type_map[args.cpu]

# 建立 Processor
processor = SimpleProcessor(
    cpu_type=cpu_type,
    num_cores=args.cores
)

# 建立 Memory
memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

# 根據 CPU 類型選擇 cache hierarchy
if args.cpu == "ATOMIC":
    # ATOMIC CPU 不需 cache
    cache_hierarchy = NoCache()
else:
    # TIMING 和 O3 需要 cache
    l1i = L1Cache(
        size="32KiB",
        assoc=8,
        tag_latency=1,
        response_latency=1,
        mshrs=16,
        tgts_per_mshr=20,
        is_icache=True
    )
    l1d = L1Cache(
        size="32KiB",
        assoc=8,
        tag_latency=2,
        response_latency=2,
        mshrs=16,
        tgts_per_mshr=20,
        is_icache=False
    )
    l2 = L2Cache(
        size="256KiB",
        assoc=16,
        tag_latency=20,
        response_latency=20,
        mshrs=20,
        tgts_per_mshr=12
    )
    cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
        l1i_cache=l1i,
        l1d_cache=l1d,
        l2_cache=l2
    )

# 建立 Board
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

# 設定 Workload
board.set_se_binary_workload(binary_path)

# 執行
simulator = Simulator(board=board)
simulator.run()
EOF
```

### argparse 參數詳解

1. **`--cpu`**：
   - 選項：ATOMIC、TIMING、O3
   - 預設：ATOMIC
   - 目的：指定 CPU 模型，影響模擬精度與速度

2. **`--cores`**：
   - 類型：整數
   - 預設：1
   - 目的：設定 CPU 核心數，影響並行性

### 執行實驗

執行不同 CPU 型別比較：

**Case 1: ATOMIC（快速，低精度）**
```bash
cd /home/a/HCSSimulation/gem5

./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_cpu_atomic \
  ../experiments/gem5-learning/configs/se_cpu_select.py \
  --cpu ATOMIC --cores 1
```

**Case 2: TIMING（中等速度，中等精度）**
```bash
./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_cpu_timing \
  ../experiments/gem5-learning/configs/se_cpu_select.py \
  --cpu TIMING --cores 1
```

**Case 3: O3（慢，高精度）**
```bash
./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_cpu_o3 \
  ../experiments/gem5-learning/configs/se_cpu_select.py \
  --cpu O3 --cores 1
```

**多核測試（TIMING）**
```bash
./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_cpu_timing_2cores \
  ../experiments/gem5-learning/configs/se_cpu_select.py \
  --cpu TIMING --cores 2
```

### 比較性能指標

提取關鍵指標比較：

```bash
echo "=== ATOMIC CPU ==="
grep "simTicks\|hostSeconds\|ipc" ../experiments/gem5-learning/results/se_cpu_atomic/stats.txt | head -5

echo "=== TIMING CPU ==="
grep "simTicks\|hostSeconds\|ipc" ../experiments/gem5-learning/results/se_cpu_timing/stats.txt | head -5

echo "=== O3 CPU ==="
grep "simTicks\|hostSeconds\|ipc" ../experiments/gem5-learning/results/se_cpu_o3/stats.txt | head -5
```

### 解釋統計差異

- **simTicks**：模擬週期數
  - ATOMIC 最少（每指令 1 cycle）
  - TIMING 較多（考慮記憶體延遲）
  - O3 可能相近（管線與並行隱藏延遲）

- **hostSeconds**：host 計算機耗時
  - ATOMIC 最快
  - TIMING 適中
  - O3 最慢（複雜度最高）

- **ipc（Instructions Per Cycle）**：
  - ATOMIC 通常為 1（定義上每指令 1 cycle）
  - TIMING 可能 < 1（記憶體延遲）
  - O3 可能 > 1（多發射亂序執行）

---

## 第六部分：Memory System 教學與實驗

### Memory System 角色

gem5 中的 memory system 負責：
1. 建模記憶體層級（DRAM、HBM 等）
2. 模擬記憶體控制器與通道
3. 計算記憶體存取延遲
4. 支援多通道並行存取

### 為什麼記憶體模型會影響 Timing Simulation

在 TIMING CPU 及以上：
- 每條指令的執行延遲受記憶體訪問時間影響
- memory bandwidth 限制影響並行性
- latency 直接影響 IPC 與總執行時間

### Memory 配置詳解

#### SingleChannelDDR3_1600

`SingleChannelDDR3_1600` 是預設的單通道 DDR3-1600 記憶體配置。

**含義**：
- **Single Channel**：單一記憶體通道
- **DDR3**：Double Data Rate 3 DRAM 標準
- **1600**：1600 MHz 記憶體時鐘

**預設參數**：
- 大小：2 GiB
- latency：約 30-50 ns（根據不同層級）

#### Memory Size 與 Address Range

- gem5 自動將記憶體映射到虛擬位址空間
- SE mode 中程式的虛擬位址由 loader 決定
- 通常程式佔用 memory 的小部分（hello world < 1 MiB）

### Memory System 配置檔案

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/configs/se_memory_select.py <<'EOF'
import sys
import argparse
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.caches.cache_hierarchy import (
    PrivateL1PrivateL2CacheHierarchy
)
from gem5.components.cachehierarchy.caches.l1_caches import L1Cache
from gem5.components.cachehierarchy.caches.l2_cache import L2Cache
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

# 命令行參數解析
parser = argparse.ArgumentParser(
    description="SE mode with configurable memory size"
)
parser.add_argument(
    "--mem-size",
    default="2GiB",
    help="Memory size (e.g., 512MiB, 1GiB, 2GiB, 4GiB)"
)

args = parser.parse_args()

# 解析記憶體大小字符串為 MemorySizeType
def parse_mem_size(size_str):
    """
    將字符串（如 '1GiB'）轉換為 gem5 MemorySizeType
    """
    size_lower = size_str.lower()
    
    # 支援常見單位
    if size_lower.endswith("gib"):
        size_gb = int(size_str[:-3])
        if size_gb == 1:
            return MemorySizeType.GB_1
        elif size_gb == 2:
            return MemorySizeType.GB_2
        elif size_gb == 4:
            return MemorySizeType.GB_4
        else:
            # 不支援的大小，使用自訂方式
            # （依 gem5 版本，可能需要直接設定）
            print(f"Warning: {size_str} not in standard MemorySizeType")
            return MemorySizeType.GB_2
    
    elif size_lower.endswith("mib"):
        size_mb = int(size_str[:-3])
        if size_mb == 512:
            return MemorySizeType.MB_512
        else:
            print(f"Warning: {size_str} not in standard MemorySizeType")
            return MemorySizeType.GB_1
    
    else:
        print(f"Warning: Unknown size format {size_str}, using default 2GiB")
        return MemorySizeType.GB_2

mem_size = parse_mem_size(args.mem_size)

# 建立 Processor
processor = SimpleProcessor(
    cpu_type=CPUTypes.TIMING,
    num_cores=1
)

# 嘗試使用 DDR3_1600，若失敗則嘗試 DDR4
try:
    from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600
    memory = SingleChannelDDR3_1600(size=mem_size)
    print(f"Using DDR3-1600 memory, size: {args.mem_size}")
except ImportError:
    try:
        from gem5.components.memory.single_channel_ddr4_2400 import SingleChannelDDR4_2400
        memory = SingleChannelDDR4_2400(size=mem_size)
        print(f"DDR3 not available, using DDR4-2400, size: {args.mem_size}")
    except ImportError:
        print("Error: Neither DDR3 nor DDR4 memory class found")
        sys.exit(1)

# 建立 Cache hierarchy
l1i = L1Cache(
    size="32KiB",
    assoc=8,
    tag_latency=1,
    response_latency=1,
    mshrs=16,
    tgts_per_mshr=20,
    is_icache=True
)
l1d = L1Cache(
    size="32KiB",
    assoc=8,
    tag_latency=2,
    response_latency=2,
    mshrs=16,
    tgts_per_mshr=20,
    is_icache=False
)
l2 = L2Cache(
    size="256KiB",
    assoc=16,
    tag_latency=20,
    response_latency=20,
    mshrs=20,
    tgts_per_mshr=12
)
cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1i_cache=l1i,
    l1d_cache=l1d,
    l2_cache=l2
)

# 建立 Board
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

# 設定 Workload
board.set_se_binary_workload(binary_path)

# 執行
simulator = Simulator(board=board)
simulator.run()
EOF
```

### 檔案詳解

**記憶體大小解析函數**：

`parse_mem_size()` 將命令行字符串轉換為 gem5 MemorySizeType：
- 支援 512MiB、1GiB、2GiB、4GiB
- 返回標準 MemorySizeType 列舉值

**版本兼容性處理**：

使用 try/except 处理 DDR3/DDR4 差異：
- 優先嘗試 DDR3_1600
- 若不可用，fallback 到 DDR4_2400
- 目的：適應不同 gem5 版本

### 執行實驗

測試不同記憶體大小：

**1GiB 記憶體**：
```bash
cd /home/a/HCSSimulation/gem5

./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/mem_1g \
  ../experiments/gem5-learning/configs/se_memory_select.py \
  --mem-size 1GiB
```

**4GiB 記憶體**：
```bash
./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/mem_4g \
  ../experiments/gem5-learning/configs/se_memory_select.py \
  --mem-size 4GiB
```

### 記憶體大小對性能的影響

對於 hello world 這樣的小程式，記憶體大小影響最小。但在大型應用中：

- **larger memory**：降低 memory pressure，減少衝突
- **smaller memory**：增加 memory contention，但可能提高 cache 效率

---

## 第七部分：Ruby Coherence 實驗 - MESI Two Level

### Ruby 概念

**Ruby** 是 gem5 中的一個內存模型框架，用於模擬複雜的緩存一致性協議。

**Ruby 與 Classic Cache 差異**：

| 特性 | Classic Cache | Ruby |
|---|---|---|
| **模型複雜度** | 簡單，預定義 | 複雜，可自訂協議 |
| **一致性支援** | 簡單 MOESI | MESI、MOESI、KMESI 等 |
| **適用場景** | 單核、簡單多核 | 複雜多核、高度並行 |
| **模擬精度** | 中等 | 高等 |
| **執行速度** | 快 | 較慢 |

### Cache 一致性協議

**什麼是 Cache 一致性**：

在多核系統中，多個核可能擁有同一記憶體位置的複本。一致性協議確保所有核心看到相同的資料順序。

**MESI 協議狀態**：

- **M (Modified)**：此核擁有唯一複本，且資料已被修改，未回寫到記憶體
- **E (Exclusive)**：此核擁有唯一複本，資料未被修改
- **S (Shared)**：多個核共有此複本，資料與記憶體一致
- **I (Invalid)**：此複本無效

**MESI Two Level**：

- 私有 L1I、L1D cache（每核一份）
- 共享 L2 cache
- directory 協議控制多核間的一致性

### Ruby MESI 配置檔案

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/configs/se_ruby_mesi.py <<'EOF'
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.cache_hierarchy import CacheHierarchy
from gem5.components.cachehierarchy.coherence_protocol import CoherenceProtocol
from gem5.components.cachehierarchy.caches.mesi_two_level import MESITwoLevelCacheHierarchy
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

# 檢查系統是否支援 MESI_TWO_LEVEL
from gem5.components.cachehierarchy.coherence_protocol import CoherenceProtocol

def requires(protocol):
    """
    檢查系統是否支援指定的一致性協議
    某些 gem5 版本可能不支援特定協議
    """
    print(f"Checking for {protocol} support...")

# 建立 Processor，使用 TIMING，2 核心
processor = SimpleProcessor(
    cpu_type=CPUTypes.TIMING,
    num_cores=2  # 2 核心才能觀察一致性協議效果
)

# 建立 Memory
from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600

memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

# 建立 MESI Two Level Cache Hierarchy
# Ruby 框架自動處理一致性協議
try:
    cache_hierarchy = MESITwoLevelCacheHierarchy(
        l1d_size="32KiB",
        l1i_size="32KiB",
        l1d_assoc=8,
        l1i_assoc=8,
        l2_size="256KiB",
        l2_assoc=16,
        num_l2_banks=2  # 2 核心對應 2 個 L2 bank
    )
    print("MESI Two Level cache hierarchy created successfully")
except Exception as e:
    print(f"Error creating MESI cache: {e}")
    print("This may be due to gem5 version differences or missing Ruby support")
    sys.exit(1)

# 建立 Board
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

# 設定 Workload
board.set_se_binary_workload(binary_path)

# 執行
simulator = Simulator(board=board)
simulator.run()
EOF
```

### 檔案詳解

**為什麼使用 2 核心**：
- MESI 協議主要解決多核間的一致性
- 單核下無需一致性協議
- 2 核心是最小的多核配置

**Ruby 一致性對多核的重要性**：
- 確保多核共享記憶體的正確性
- 影響核間通信延遲
- 影響 cache 效率

**requires 函數**：
- 檢查系統是否支援特定協議
- 不同 gem5 版本支援的協議不同
- 可用於版本兼容性檢查

**num_l2_banks**：
- L2 cache 分為多個 bank 並行服務多核
- 2 核 -> 2 banks，避免 L2 成為瓶頸

### 執行實驗

```bash
cd /home/a/HCSSimulation/gem5

./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_ruby_mesi \
  ../experiments/gem5-learning/configs/se_ruby_mesi.py
```

### Ruby 統計分析

提取 Ruby 相關統計：

```bash
grep -iE "ruby|mesi|coherence|directory" \
  ../experiments/gem5-learning/results/se_ruby_mesi/stats.txt | head -50
```

查看 L1/L2 cache 統計：

```bash
grep -E "l1|l2|cache|miss|hit" \
  ../experiments/gem5-learning/results/se_ruby_mesi/stats.txt | head -80
```

### 關鍵 Ruby 統計欄位

| 欄位 | 意義 |
|---|---|
| `ruby_cycles` | Ruby 協議執行的週期數 |
| `l1d_misses` | L1D cache miss 數 |
| `l1i_misses` | L1I cache miss 數 |
| `directory_accesses` | directory 被訪問次數 |
| `directory_conflicts` | directory 衝突次數 |
| `coherence_messages` | 核間一致性消息數 |

### 常見錯誤排查

**錯誤 1**：`ImportError: cannot import name 'MESITwoLevelCacheHierarchy'`

原因：gem5 版本不支援或模組名稱不同

解決：檢查可用 cache hierarchy：
```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.components import cachehierarchy
print([x for x in dir(cachehierarchy) if 'MESI' in x or 'Cache' in x])
"
```

**錯誤 2**：`requires check failed for MESI_TWO_LEVEL`

原因：系統無此協議支援

解決：檢查 gem5 編譯選項，確認包含 Ruby 模組

---

## 第八部分：FS mode 實驗

### FS mode 概念回顧

FS mode（Full System mode）模擬完整系統，包括：
- OS kernel
- Device drivers
- System calls
- 其他 OS 級別設施

### FS mode 資源需求

FS mode 需要：
1. **Kernel image**：OS 核心（如 Linux kernel）
2. **Disk image**：文件系統（含 rootfs）
3. **Bootloader**：啟動程式

### gem5 Resources 的角色

`gem5.resources` 提供預先準備的 kernel 與 disk image，可自動下載：

```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.resources import list_resources
resources = list_resources()
for r in resources:
        print(r)
"
```

### 官方 FS mode 範例查找

首先檢查官方提供的 FS mode 範例：

```bash
cd /home/a/HCSSimulation/gem5
```


```bash
```

### FS mode 注意事項（WSL2 環境）

1. **KVM 支援受限**：
   - WSL2 上 nested virtualization 不完全支援
   - KVM CPU 可能無法使用
   - 建議使用 ATOMIC 或 TIMING CPU

2. **啟動時間**：
   - FS mode 啟動非常慢（可能數小時）
   - 初期開發不建議 FS mode
   - 先用 SE mode 驗證邏輯

3. **磁盤映像大小**：
   - 映像通常數 GB
   - 網路下載時間較長
   - 建議提前下載或在高速網路環境進行

### FS mode 模板配置

```bash
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.cachehierarchy.caches.cache_hierarchy import (
    PrivateL1PrivateL2CacheHierarchy
)
from gem5.components.cachehierarchy.caches.l1_caches import L1Cache
from gem5.components.cachehierarchy.caches.l2_cache import L2Cache
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

# 警告：FS mode 在 WSL2 上執行非常慢
print("Warning: FS mode simulation is very slow on WSL2")
print("Estimated boot time: 30 minutes to several hours")
print("This is a functional example. For actual experiments, consider using SE mode.")

# CPU 型別選擇
# 注意：在 WSL2 上 KVM 可能不可用
CPU_TYPE = CPUTypes.TIMING  # 建議使用 TIMING 而非 KVM

# 建立 Processor
processor = SimpleProcessor(
    cpu_type=CPU_TYPE,
    num_cores=1
)

# 建立 Memory
from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600

memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

# 建立 Cache hierarchy
l1i = L1Cache(
    size="32KiB",
    assoc=8,
    tag_latency=1,
    response_latency=1,
    mshrs=16,
    tgts_per_mshr=20,
    is_icache=True
)
l1d = L1Cache(
    size="32KiB",
    assoc=8,
    tag_latency=2,
    response_latency=2,
    mshrs=16,
    tgts_per_mshr=20,
    is_icache=False
)
l2 = L2Cache(
    size="256KiB",
    assoc=16,
    tag_latency=20,
    response_latency=20,
    mshrs=20,
    tgts_per_mshr=12
)
cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1i_cache=l1i,
    l1d_cache=l1d,
    l2_cache=l2
)

    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

# 設定 FS workload
# 注意：kernel 與 disk_image resource ID 需依 gem5 版本確認
# 常見配置（需驗證是否存在）：

try:
    # 嘗試下載資源（可能需要網路）
    
    # 建立啟動命令
    # m5 exit 用於在指定點結束模擬
    readfile_contents = """
    echo "Simulation started..."
    m5 exit
    """
    
    board.set_kernel_disk_workload(
        kernel=kernel_path,
        disk_image=disk_image_path,
        readfile_contents=readfile_contents
    )
    
except Exception as e:
    print(f"Error loading FS resources: {e}")
    print("Possible causes:")
    print("1. Network unavailable for resource download")
    print("2. Resource ID outdated for this gem5 version")
    print("3. Insufficient disk space")
    print("\nSuggestion: Use SE mode instead for faster iteration")
    sys.exit(1)

# 執行
simulator = Simulator(board=board)
print("Starting FS mode simulation... (this will take a long time)")
simulator.run()
EOF
```

### 檔案詳解

- FS mode 專用 board
- 自動配置 bootloader、BIOS 等

**set_kernel_disk_workload**：
- 設定 kernel 與 disk image
- readfile_contents 允許在模擬中執行命令

**m5 exit**：
- gem5 提供的特殊指令，用於結束模擬
- 允許在指定點停止，避免長時間執行

**資源版本兼容性**：
- kernel 與 image resource ID 因 gem5 版本異
- 需驗證確切 ID
- 建議查詢官方文件

### 查詢正確的 Resource ID

```bash
cd /home/a/HCSSimulation/gem5

python3 -c "
import sys
sys.path.insert(0, '.')
from gem5.resources import list_resources

resources = list_resources()
for r in sorted(resources):
        print(f'  - {r}')
"
```

### FS mode 錯誤排查

**錯誤 1**：`Resource ... not found`

原因：資源不存在或 ID 不正確

解決：
1. 查詢正確的 resource ID
2. 檢查網路連接（資源需下載）
3. 查詢 gem5 官方文件確認版本支援

**錯誤 2**：`KVM not available`

原因：WSL2 嵌套虛擬化不支援

解決：
1. 改用 TIMING CPU
2. 檢查 WSL2 設定中的虛擬化支援
3. 或在支援 KVM 的 Linux 環境執行

**錯誤 3**：`Boot timeout`

原因：模擬過程超時

解決：
1. 縮短 readfile 中的指令
2. 增加 timeout 設定
3. 使用 `m5 exit` 提前結束

**錯誤 4**：`Insufficient disk space`

原因：disk image 下載後佔用空間大

解決：
1. 檢查可用磁盤空間
2. 清理不需要的舊實驗結果

---

## 第九部分：Statistics Parsing

### 為什麼需要 Statistics Parsing

gem5 產生的 `stats.txt` 包含數百條統計記錄，手動查看不實用。自動解析統計能：
1. 快速提取關鍵指標
2. 比較多個實驗結果
3. 生成 DSE 報告
4. 簡化數據處理

### 統計檔案格式

**config.ini**：
- 完整系統配置
- 所有 SimObject 參數
- 用於重現實驗

**config.json**：
- JSON 格式配置
- 便於程式化解析

**stats.txt**：
- 模擬統計數據
- 以 `key value` 格式存儲
- 包含註解（以 `#` 開頭）

### Statistics Parser 實現

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/scripts/parse_stats.py <<'EOF'
#!/usr/bin/env python3
"""
gem5 統計解析工具

功能：
- 從多個 gem5 結果目錄讀取 stats.txt
- 提取關鍵統計指標
- 輸出 CSV 格式

用法：
  python3 parse_stats.py results/exp1 results/exp2 ... --out summary.csv
"""

import os
import sys
import argparse
import csv
from pathlib import Path

def parse_stats_file(stats_file):
    """
    解析 gem5 stats.txt 檔案
    返回 dict 格式的統計數據
    
    Args:
        stats_file: stats.txt 檔案路徑
        
    Returns:
        dict: {key: value} 統計數據
    """
    stats = {}
    
    try:
        with open(stats_file, 'r') as f:
            for line in f:
                line = line.strip()
                
                # 跳過空行和註解
                if not line or line.startswith('#'):
                    continue
                
                # 解析 "key value" 格式
                parts = line.split()
                if len(parts) >= 2:
                    key = parts[0]
                    value = parts[1]
                    
                    # 嘗試將 value 轉換為數字
                    try:
                        # 優先嘗試 float
                        if '.' in value:
                            stats[key] = float(value)
                        else:
                            stats[key] = int(value)
                    except ValueError:
                        # 非數值保持字符串
                        stats[key] = value
    
    except FileNotFoundError:
        print(f"Error: stats file not found: {stats_file}", file=sys.stderr)
        return None
    
    return stats

def extract_key_metrics(stats_dict):
    """
    從統計數據中提取關鍵指標
    
    Args:
        stats_dict: 統計數據字典
        
    Returns:
        dict: {metric_name: value}
    """
    metrics = {}
    
    # 定義關鍵指標及其可能的 key 名稱
    key_mapping = {
        'simInsts': ['simInsts', 'system.cpu.simInsts', 'simulated_instructions'],
        'simTicks': ['simTicks', 'system.cpu.simTicks', 'simulated_ticks'],
        'hostSeconds': ['hostSeconds', 'host_seconds'],
        'hostTickRate': ['hostTickRate', 'host_tick_rate'],
        'hostInstRate': ['hostInstRate', 'host_inst_rate'],
        'ipc': ['simulated_ipc', 'system.cpu.ipc', 'ipc'],
        'numCycles': ['numCycles', 'system.cpu.numCycles', 'simulated_cycles'],
        'dcache_misses': ['system.cpu.dcache.misses', 'dcache.misses'],
        'dcache_hits': ['system.cpu.dcache.hits', 'dcache.hits'],
        'icache_misses': ['system.cpu.icache.misses', 'icache.misses'],
        'icache_hits': ['system.cpu.icache.hits', 'icache.hits'],
        'l2_misses': ['system.l2.misses', 'l2.misses'],
        'l2_hits': ['system.l2.hits', 'l2.hits'],
    }
    
    # 查詢每個指標
    for metric_name, key_names in key_mapping.items():
        for key in key_names:
            if key in stats_dict:
                metrics[metric_name] = stats_dict[key]
                break
    
    return metrics

def main():
    # 命令行參數解析
    parser = argparse.ArgumentParser(
        description="Parse gem5 statistics from multiple results directories"
    )
    parser.add_argument(
        'results_dirs',
        nargs='+',
        help="One or more result directories containing stats.txt"
    )
    parser.add_argument(
        '--out',
        default='summary.csv',
        help="Output CSV file (default: summary.csv)"
    )
    
    args = parser.parse_args()
    
    # 收集所有統計數據
    all_metrics = []
    
    for result_dir in args.results_dirs:
        stats_file = os.path.join(result_dir, 'stats.txt')
        
        if not os.path.exists(stats_file):
            print(f"Warning: {stats_file} not found, skipping {result_dir}")
            continue
        
        print(f"Parsing {stats_file}...")
        stats = parse_stats_file(stats_file)
        
        if stats is None:
            continue
        
        metrics = extract_key_metrics(stats)
        metrics['result_dir'] = result_dir
        
        all_metrics.append(metrics)
    
    if not all_metrics:
        print("Error: No valid stats files found", file=sys.stderr)
        sys.exit(1)
    
    # 收集所有可能的欄位
    fieldnames = set(['result_dir'])
    for metrics in all_metrics:
        fieldnames.update(metrics.keys())
    fieldnames = sorted(list(fieldnames))
    
    # 寫入 CSV
    try:
        with open(args.out, 'w', newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(all_metrics)
        
        print(f"Statistics written to {args.out}")
        
        # 顯示摘要
        print(f"\nSummary: Parsed {len(all_metrics)} result(s), {len(fieldnames)} metrics")
        
    except IOError as e:
        print(f"Error writing CSV: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()
EOF

chmod +x /home/a/HCSSimulation/experiments/gem5-learning/scripts/parse_stats.py
```

### Parser 詳解

**parse_stats_file 函數**：
- 逐行讀取 stats.txt
- 跳過註解（#）與空行
- 解析 "key value" 格式
- 自動將值轉為數字或字符串

**extract_key_metrics 函數**：
- 從統計字典提取關鍵指標
- 支援多個可能的 key 名稱（因版本差異）
- 返回簡化的 metric 字典

**主程序邏輯**：
- 遍歷所有結果目錄
- 對每個目錄解析 stats.txt
- 收集所有指標
- 寫入 CSV

### 執行 Parser

提取前面實驗的統計：

```bash
python3 /home/a/HCSSimulation/experiments/gem5-learning/scripts/parse_stats.py \
  /home/a/HCSSimulation/experiments/gem5-learning/results/se_hello \
  /home/a/HCSSimulation/experiments/gem5-learning/results/se_classic_cache \
  /home/a/HCSSimulation/experiments/gem5-learning/results/se_cpu_atomic \
  /home/a/HCSSimulation/experiments/gem5-learning/results/se_cpu_timing \
  --out /home/a/HCSSimulation/experiments/gem5-learning/results/summary.csv
```

查看生成的 CSV：

```bash
cat /home/a/HCSSimulation/experiments/gem5-learning/results/summary.csv
```

### CSV 輸出示例

```
result_dir,simInsts,simTicks,hostSeconds,ipc,dcache_misses
experiments/gem5-learning/results/se_hello,1000,1000,0.5,1.0,0
experiments/gem5-learning/results/se_classic_cache,1000,1500,1.2,0.67,50
```

---

## 第十部分：Design Space Exploration (DSE)

### DSE 概念

**Design Space Exploration** 是指通過系統化地變換硬體參數，評估不同設計的性能影響。

**為什麼需要 DSE**：
1. 設計決策有太多參數組合
2. 無法逐一手動測試
3. 自動化掃描能發現最佳點
4. 識別參數間的相互影響

**DSE 的挑戰**：
- 參數數量指數級增長（curse of dimensionality）
- 模擬時間長（尤其 O3 CPU）
- 結果分析複雜

### 參數掃描策略

**初期探索**：
- 小範圍參數掃描
- 使用快速 CPU（TIMING）
- 少量核心（1-2）

**深入分析**：
- 根據初期發現微調參數範圍
- 使用更精確的 CPU（O3）
- 增加核心數

### DSE 配置檔案

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/configs/se_dse_config.py <<'EOF'
#!/usr/bin/env python3
"""
可參數化的 SE mode 配置，用於 DSE

支持的參數：
  --cpu: CPU 型別 (ATOMIC, TIMING, O3)
  --cores: 核心數
  --l1d: L1D cache 大小 (KiB)
  --l1i: L1I cache 大小 (KiB)
  --l2: L2 cache 大小 (KiB)
  --l1-assoc: L1 cache 關聯度
  --l2-assoc: L2 cache 關聯度
  --mem-size: 記憶體大小 (GiB)
"""

import sys
import argparse
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.caches.cache_hierarchy import (
    NoCache,
    PrivateL1PrivateL2CacheHierarchy
)
from gem5.components.cachehierarchy.caches.l1_caches import L1Cache
from gem5.components.cachehierarchy.caches.l2_cache import L2Cache
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

def main():
    parser = argparse.ArgumentParser(
        description="Configurable SE mode for design space exploration"
    )
    
    # CPU 參數
    parser.add_argument(
        '--cpu',
        choices=['ATOMIC', 'TIMING', 'O3'],
        default='TIMING',
        help='CPU type'
    )
    parser.add_argument(
        '--cores',
        type=int,
        default=1,
        help='Number of cores'
    )
    
    # Cache 參數
    parser.add_argument(
        '--l1d',
        type=int,
        default=32,
        help='L1D cache size in KiB'
    )
    parser.add_argument(
        '--l1i',
        type=int,
        default=32,
        help='L1I cache size in KiB'
    )
    parser.add_argument(
        '--l2',
        type=int,
        default=256,
        help='L2 cache size in KiB'
    )
    parser.add_argument(
        '--l1-assoc',
        type=int,
        default=8,
        help='L1 cache associativity'
    )
    parser.add_argument(
        '--l2-assoc',
        type=int,
        default=16,
        help='L2 cache associativity'
    )
    
    # 記憶體參數
    parser.add_argument(
        '--mem-size',
        type=int,
        default=2,
        help='Memory size in GiB'
    )
    
    args = parser.parse_args()
    
    # 建立 CPU 類型對應
    cpu_type_map = {
        'ATOMIC': CPUTypes.ATOMIC,
        'TIMING': CPUTypes.TIMING,
        'O3': CPUTypes.O3,
    }
    
    cpu_type = cpu_type_map[args.cpu]
    
    # 建立 Processor
    processor = SimpleProcessor(
        cpu_type=cpu_type,
        num_cores=args.cores
    )
    
    # 建立 Memory
    from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600
    
    # 對應記憶體大小到 MemorySizeType
    if args.mem_size == 1:
        mem_size = MemorySizeType.GB_1
    elif args.mem_size == 2:
        mem_size = MemorySizeType.GB_2
    elif args.mem_size == 4:
        mem_size = MemorySizeType.GB_4
    else:
        print(f"Warning: Memory size {args.mem_size} GiB not supported, using 2 GiB")
        mem_size = MemorySizeType.GB_2
    
    memory = SingleChannelDDR3_1600(size=mem_size)
    
    # 選擇 cache hierarchy
    if args.cpu == 'ATOMIC':
        cache_hierarchy = NoCache()
    else:
        l1i = L1Cache(
            size=f"{args.l1i}KiB",
            assoc=args.l1_assoc,
            tag_latency=1,
            response_latency=1,
            mshrs=16,
            tgts_per_mshr=20,
            is_icache=True
        )
        l1d = L1Cache(
            size=f"{args.l1d}KiB",
            assoc=args.l1_assoc,
            tag_latency=2,
            response_latency=2,
            mshrs=16,
            tgts_per_mshr=20,
            is_icache=False
        )
        l2 = L2Cache(
            size=f"{args.l2}KiB",
            assoc=args.l2_assoc,
            tag_latency=20,
            response_latency=20,
            mshrs=20,
            tgts_per_mshr=12
        )
        cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
            l1i_cache=l1i,
            l1d_cache=l1d,
            l2_cache=l2
        )
    
    # 建立 Board
    board = SimpleBoard(
        clk_freq="3GHz",
        processor=processor,
        memory=memory,
        cachehierarchy=cache_hierarchy
    )
    
    # 設定 Workload
    board.set_se_binary_workload(binary_path)
    
    # 執行
    simulator = Simulator(board=board)
    simulator.run()

if __name__ == '__main__':
    main()
EOF

chmod +x /home/a/HCSSimulation/experiments/gem5-learning/configs/se_dse_config.py
```

### DSE Runner 實現

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/scripts/run_dse.py <<'EOF'
#!/usr/bin/env python3
"""
Design Space Exploration Runner

自動執行參數掃描實驗，生成結果報告

掃描參數：
  - CPU: TIMING, O3
  - L1D: 16KiB, 32KiB
  - L2: 256KiB, 512KiB
  - Cores: 1, 2
"""

import os
import sys
import subprocess
from pathlib import Path

def run_experiment(config_path, gem5_binary, cpu, l1d, l1i, l2, cores, mem_size, outdir):
    """
    執行單一實驗
    
    Args:
        config_path: 配置檔案路徑
        gem5_binary: gem5.opt 路徑
        cpu: CPU 型別
        l1d: L1D 大小
        l1i: L1I 大小
        l2: L2 大小
        cores: 核心數
        mem_size: 記憶體大小
        outdir: 輸出目錄
    
    Returns:
        bool: 成功返回 True
    """
    
    # 建立輸出目錄
    os.makedirs(outdir, exist_ok=True)
    
    # 構建執行指令
    cmd = [
        gem5_binary,
        f"--outdir={outdir}",
        config_path,
        f"--cpu={cpu}",
        f"--cores={cores}",
        f"--l1d={l1d}",
        f"--l1i={l1i}",
        f"--l2={l2}",
        f"--mem-size={mem_size}",
    ]
    
    print(f"\n{'='*60}")
    print(f"Running: {' '.join(cmd)}")
    print(f"Output: {outdir}")
    print(f"{'='*60}")
    
    try:
        # check=True 表示非零返回碼會拋出異常，中止實驗
        result = subprocess.run(cmd, check=True)
        print(f"Success: {outdir}")
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error: Experiment failed with return code {e.returncode}")
        print(f"Check output in {outdir}")
        return False

def main():
    # 路徑設定
    gem5_root = "/home/a/HCSSimulation/gem5"
    
    # 嘗試找到 gem5.opt
    gem5_binary = None
        path = os.path.join(gem5_root, build_type)
        if os.path.exists(path):
            gem5_binary = path
            break
    
    if gem5_binary is None:
        print("Error: gem5.opt not found. Please compile gem5 first.")
        sys.exit(1)
    
    print(f"Using gem5 binary: {gem5_binary}")
    
    config_path = os.path.join(
        gem5_root,
        "../experiments/gem5-learning/configs/se_dse_config.py"
    )
    results_base = os.path.join(
        gem5_root,
        "../experiments/gem5-learning/results"
    )
    
    if not os.path.exists(config_path):
        print(f"Error: config file not found: {config_path}")
        sys.exit(1)
    
    # 定義掃描參數
    cpus = ["TIMING", "O3"]
    l1d_sizes = [16, 32]
    l1i_sizes = [16, 32]
    l2_sizes = [256, 512]
    core_counts = [1, 2]
    mem_size = 2  # GiB
    
    # 追蹤實驗結果
    results = []
    failed = []
    
    # 執行實驗
    exp_count = 0
    for cpu in cpus:
        for l1d in l1d_sizes:
            for l1i in l1i_sizes:
                for l2 in l2_sizes:
                    for cores in core_counts:
                        exp_count += 1
                        
                        # 建立輸出目錄名
                        dirname = f"dse_cpu{cpu}_cores{cores}_l1d{l1d}_l1i{l1i}_l2{l2}"
                        outdir = os.path.join(results_base, dirname)
                        
                        # 執行實驗
                        success = run_experiment(
                            config_path, gem5_binary,
                            cpu, l1d, l1i, l2, cores, mem_size,
                            outdir
                        )
                        
                        if success:
                            results.append(outdir)
                        else:
                            failed.append(outdir)
                        
                        print(f"Progress: {len(results)} succeeded, {len(failed)} failed")
    
    # 摘要
    print(f"\n{'='*60}")
    print(f"DSE Complete: {len(results)} experiments succeeded, {len(failed)} failed")
    print(f"Total: {exp_count} experiments")
    print(f"{'='*60}")
    
    if failed:
        print("\nFailed experiments:")
        for f in failed:
            print(f"  - {f}")
    
    # 解析統計
    print("\nParsing statistics...")
    parse_script = os.path.join(
        gem5_root,
        "../experiments/gem5-learning/scripts/parse_stats.py"
    )
    
    summary_csv = os.path.join(results_base, "dse_summary.csv")
    
    parse_cmd = [
        "python3", parse_script,
        *results,
        "--out", summary_csv
    ]
    
    try:
        subprocess.run(parse_cmd, check=True)
        print(f"Summary saved to: {summary_csv}")
        
        # 顯示摘要前幾行
        print("\nSummary (first 10 rows):")
        with open(summary_csv, 'r') as f:
            for i, line in enumerate(f):
                if i < 10:
                    print(f"  {line.rstrip()}")
                else:
                    break
    
    except subprocess.CalledProcessError as e:
        print(f"Error parsing statistics: {e}")
    
    return 0 if len(failed) == 0 else 1

if __name__ == '__main__':
    sys.exit(main())
EOF

chmod +x /home/a/HCSSimulation/experiments/gem5-learning/scripts/run_dse.py
```

### DSE Runner 詳解

**run_experiment 函數**：
- 構建 gem5 執行命令
- 建立輸出目錄
- `check=True` 確保失敗時中止，避免浪費時間

**參數掃描**：
- 嵌套循環遍歷所有參數組合
- CPU x L1D x L1I x L2 x Cores
- 總共 2 x 2 x 2 x 2 x 2 = 32 個實驗

**結果追蹤**：
- 記錄成功和失敗的實驗
- 執行後自動解析統計

### 執行 DSE

```bash
cd /home/a/HCSSimulation/gem5

python3 ../experiments/gem5-learning/scripts/run_dse.py
```

**預期執行時間**：
- TIMING CPU: 數分鐘到數小時
- O3 CPU: 數小時到數十小時
- 取決於系統硬體

### DSE 結果分析

查看生成的 CSV：

```bash
head -20 ../experiments/gem5-learning/results/dse_summary.csv
```

分析性能差異：

```bash
python3 -c "
import csv

with open('../experiments/gem5-learning/results/dse_summary.csv', 'r') as f:
    reader = csv.DictReader(f)
    
    best_ipc = 0
    best_config = None
    
    for row in reader:
        try:
            ipc = float(row.get('ipc', 0))
            if ipc > best_ipc:
                best_ipc = ipc
                best_config = row
        except:
            pass
    
    if best_config:
        print('Best configuration:')
        print(f'  IPC: {best_ipc}')
        print(f'  Config: {best_config[\"result_dir\"]}')
"
```

---

## 第十一部分：實驗記錄模板

### 為什麼需要實驗記錄

實驗記錄用於：
1. 追蹤實驗配置與結果
2. 便於重現過去的實驗
3. 記錄觀察與發現
4. 文件化排查過程

### 實驗記錄模板

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning/notes/experiment_log.md <<'EOF'
# gem5 實驗記錄

## 實驗 1：SE mode Hello World

### 基本資訊
- **日期**：2024-XX-XX
- **目的**：驗證 gem5 基本功能
- **gem5 版本**：git commit (執行 `cd gem5 && git log -1 --oneline` 確認)
- **WSL2 版本**：Ubuntu 22.04 或 24.04 (執行 `cat /etc/os-release` 確認)

### 系統配置
- **gem5 binary**：`build/ALL/gem5.opt`
- **Configuration file**：`configs/se_hello.py`

### 硬體設置
| 參數 | 值 |
|---|---|
| CPU 型別 | ATOMIC |
| 核心數 | 1 |
| L1I Cache | 無 |
| L1D Cache | 無 |
| L2 Cache | 無 |
| Memory | 2 GiB DDR3-1600 |

### 執行指令

```bash
./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_hello \
  ../experiments/gem5-learning/configs/se_hello.py
```

### 重要統計

| 指標 | 值 |
|---|---|
| simInsts | XXX |
| simTicks | XXX |
| hostSeconds | XXX |
| IPC | XXX |

### 觀察

- hello world 程式成功執行
- 無 cache miss（預期，因為使用 ATOMIC + NoCache）

### 問題與排查

- 若出現 "Resource not found"：需要下載 gem5 resources，見排查指南

---

## 實驗 2：SE mode + Classic Cache

### 基本資訊
- **日期**：2024-XX-XX
- **目的**：觀察 cache 對性能的影響

### 硬體設置
| 參數 | 值 |
|---|---|
| CPU 型別 | TIMING |
| L1I Cache | 32 KiB, 8-way |
| L1D Cache | 32 KiB, 8-way |
| L2 Cache | 256 KiB, 16-way |

### 重要統計

| 指標 | SE Hello | SE Cache |
|---|---|---|
| simInsts | XXX | XXX |
| simTicks | XXX | XXX |
| IPC | XXX | XXX |
| L1D Miss | 0 | XXX |
| L1I Miss | 0 | XXX |

### 觀察

- simTicks 增加（因為 TIMING CPU 考慮記憶體延遲）
- cache miss 數據顯示 hello world cache 友好

### 與 Experiment 1 的比較

- IPC 下降（預期，因為 TIMING CPU 考慮延遲）
- 但對於小程式，差異不大

---

## 實驗 3：CPU Model 比較

### 基本資訊
- **日期**：2024-XX-XX
- **目的**：比較不同 CPU 型別的性能

### 配置
- 使用 se_cpu_select.py 配置

### 結果

| CPU 型別 | simTicks | hostSeconds | IPC |
|---|---|---|---|
| ATOMIC | XXX | XXX | XXX |
| TIMING | XXX | XXX | XXX |
| O3 | XXX | XXX | XXX |

### 觀察

- ATOMIC 執行最快（host time）
- O3 simTicks 可能低於 TIMING（亂序執行効果）
- host 計算時間：ATOMIC < TIMING < O3

### 結論

- 初期探索用 ATOMIC 最快
- 性能分析用 TIMING 最實用
- 微架構研究才用 O3

---

## 實驗 4：Memory Size 影響

### 基本資訊
- **日期**：2024-XX-XX
- **目的**：測試記憶體大小對 hello world 的影響

### 配置

| Memory Size | simTicks | IPC |
|---|---|---|
| 1 GiB | XXX | XXX |
| 2 GiB | XXX | XXX |
| 4 GiB | XXX | XXX |

### 觀察

- hello world 不受記憶體大小影響（工作集很小）
- 只有大程式才會看到明顯差異

---

## 實驗 5：Ruby MESI 多核

### 基本資訊
- **日期**：2024-XX-XX
- **目的**：觀察多核間的 cache 一致性

### 配置
- CPU 型別：TIMING
- 核心數：2
- Cache：MESI Two Level

### 重要統計

| 指標 | 值 |
|---|---|
| Ruby coherence messages | XXX |
| Directory hits | XXX |
| Directory miss | XXX |

### 觀察

- 2 核心執行 hello world 時一致性開銷較小（工作集不共享）
- coherence message 數量低（預期）

---

## 實驗 6：Design Space Exploration

### 基本資訊
- **日期**：2024-XX-XX
- **目的**：參數掃描找最佳配置

### DSE 參數範圍

- CPU：TIMING、O3
- L1D：16 KiB、32 KiB
- L2：256 KiB、512 KiB
- Cores：1、2

### 最佳配置

（從 dse_summary.csv 提取）

| 參數 | 值 |
|---|---|
| CPU | XXX |
| L1D | XXX |
| L2 | XXX |
| Cores | XXX |
| **IPC** | **XXX** |

### 觀察

- 不同參數組合的 IPC 差異
- 哪些參數影響最大

---

## 排查記錄

### Issue 1：Resource 不存在


**原因**：gem5 resources 需下載

**解決**：
```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.resources import obtain_resource
"
```

### Issue 2：gem5.opt 不存在

**現象**：`./build/ALL/gem5.opt: command not found`

**原因**：gem5 未編譯

**解決**：
```bash
cd /home/a/HCSSimulation/gem5
```

### Issue 3：O3 執行超級慢

**現象**：O3 模擬花費數小時

**原因**：O3 CPU 複雜度高

**建議**：
- 初期使用 TIMING CPU
- 只在最終驗證時用 O3
- 用 ATOMIC 快速測試邏輯

EOF
```

---

## 第十二部分：常見錯誤排查指南

### 錯誤 1：`Resource 'xxx' not found`

**現象**：
```
gem5.resources.resource_manager.ResourceManager: Attempting to download...
```

**可能原因**：
1. 網路未連接或 gem5 resources 服務不可用
2. Resource ID 不正確
3. gem5 版本差異

**檢查指令**：

```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.resources import list_resources
resources = list_resources()
print([r for r in resources if 'hello' in r.lower()])
"
```

**解決方案**：
1. 確認網路連接
2. 查詢正確的 resource ID
3. 手動下載後指定路徑

---

### 錯誤 2：`requires check failed for MESI_TWO_LEVEL`

**現象**：
```
gem5.components.cachehierarchy.coherence_protocol: requires check failed for MESI_TWO_LEVEL
```

**原因**：系統未支援該協議

**檢查指令**：

```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.components.cachehierarchy.coherence_protocol import CoherenceProtocol
print([x for x in dir(CoherenceProtocol) if not x.startswith('_')])
"
```

**解決方案**：
1. 確認 gem5 編譯包含 Ruby 模組
2. 使用支援的協議
3. 升級 gem5 版本

---

### 錯誤 3：`build/ALL/gem5.opt` 不存在

**現象**：
```
bash: ./build/ALL/gem5.opt: No such file or directory
```

**原因**：gem5 未編譯

**檢查指令**：

```bash
cd /home/a/HCSSimulation/gem5
ls -la build/*/gem5.opt 2>/dev/null || echo "gem5 not compiled"
```

**解決方案**：

編譯 gem5：

```bash
cd /home/a/HCSSimulation/gem5
```

**編譯需求**：
- 時間：15-30 分鐘
- 磁盤空間：5-10 GB
- CPU 核心：越多越快

---

### 錯誤 4：`No module named 'gem5'`

**現象**：
```
ModuleNotFoundError: No module named 'gem5'
```

**原因**：Python 路徑未包含 gem5

**檢查指令**：

```bash
python3 -c "import sys; print(sys.path)"
```

**解決方案**：

在配置檔開頭加入：

```python
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
```

或設定環境變數：

```bash
export PYTHONPATH=/home/a/HCSSimulation/gem5:$PYTHONPATH
```

---

### 錯誤 5：`AttributeError: no attribute 'TIMING'`

**現象**：
```
AttributeError: type object 'CPUTypes' has no attribute 'TIMING'
```

**原因**：gem5 版本差異，CPU 類型名稱不同

**檢查指令**：

```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.components.processors.cpu_types import CPUTypes
print([x for x in dir(CPUTypes) if not x.startswith('_')])
"
```

**解決方案**：
使用可用的 CPU 類型替代

---

### 錯誤 6：FS mode `KVM not available`

**現象**：
```
Error: KVM CPU not available on this system
```

**原因**：WSL2 nested virtualization 不支援或未啟用

**檢查指令**：

```bash
grep -c vmx /proc/cpuinfo  # Intel
grep -c svm /proc/cpuinfo  # AMD
```

若返回 0，表示 KVM 不可用。

**解決方案**：
1. 改用 TIMING 或 ATOMIC CPU（較慢但功能正常）
2. 檢查 WSL2 設定中的虛擬化選項
3. 在原生 Linux 環境執行

---

### 錯誤 7：FS mode `Boot timeout`

**現象**：
```
Exiting @ tick XXX because simulate() limit reached
```

**原因**：FS mode 啟動超時

**解決方案**：
1. 增加模擬週期上限（--work-end-ckpt-count 參數）
2. 使用更快的 CPU（KVM 如可用）
3. 縮短 readfile 命令
4. 或使用 SE mode 代替

---

### 錯誤 8：`stats key 找不到`

**現象**：
```
grep: simInsts: No such key found
```

**原因**：stats key 名稱因版本而異

**檢查指令**：

```bash
grep -i "sim" results/stats.txt | head -20
```

**解決方案**：
1. 檢查實際 stats 檔案中的 key 名稱
2. 更新 parser 以支援多個別名
3. 查詢 gem5 文件確認 key 命名

---

### 錯誤 9：O3 CPU 執行超級慢

**現象**：
```
O3 simulation running... waiting... (still waiting)
```

**原因**：O3 CPU 模型複雜度極高

**預期時間**：
- ATOMIC：秒級
- TIMING：分鐘級
- O3：數小時或更長

**解決方案**：
1. 只在必要時使用 O3
2. 減少 workload 複雜度
3. 使用並行化運行多個實驗
4. 或在高性能計算集群執行

---

### 錯誤 10：`outdir 被覆蓋`

**現象**：
```
Warning: Overwriting existing output directory
```

**原因**：輸出目錄已存在

**解決方案**：
1. 確保每次實驗使用唯一 outdir
2. 使用時間戳記或實驗 ID
3. 或備份舊結果後再覆蓋

---

## 第十三部分：學習路線圖

推薦按以下順序進行實驗：

### 第 1 階段：基礎設置

1. **完成前置檢查**
   - 確認 gem5 根目錄
   - 確認 gem5.opt 存在或編譯
   - 確認 Python 版本
   - 確認 resources 可用

2. **建立實驗目錄**
   ```bash
   mkdir -p experiments/gem5-learning/{configs,scripts,results,workloads,notes}
   ```

### 第 2 階段：SE mode 基礎（1-2 小時）

3. **實驗 A：SE + ATOMIC + NoCache**
   - 執行 se_hello.py
   - 驗證 gem5 功能正常
   - 檢查 stats.txt 輸出

4. **實驗 B：SE + TIMING + Classic Cache**
   - 執行 se_classic_cache.py
   - 觀察 cache 統計
   - 與 Exp A 比較

### 第 3 階段：CPU Model 理解（30 分鐘 - 2 小時）

5. **實驗 C：CPU Model 比較**
   - 執行 se_cpu_select.py 測試 ATOMIC、TIMING、O3
   - 觀察 hostSeconds 與 simTicks 差異
   - 記錄發現

### 第 4 階段：記憶體與 Cache（1 小時）

6. **實驗 D：記憶體大小影響**
   - 執行 se_memory_select.py 測試 1GiB、2GiB、4GiB
   - 對於 hello world 觀察不到差異（正常）

7. **實驗 E：Cache 參數掃描**
   - 手動執行不同 cache 配置
   - 觀察 cache miss rate 變化

### 第 5 階段：多核與一致性（1-2 小時）

8. **實驗 F：Ruby MESI 多核**
   - 執行 se_ruby_mesi.py
   - 觀察多核協議統計

### 第 6 階段：自動化工具（30 分鐘）

9. **統計解析**
   - 執行 parse_stats.py
   - 生成 CSV 報告

### 第 7 階段：設計空間探索（數小時）

10. **DSE 初期探索**
    - 執行 run_dse.py（減少參數範圍）
    - 生成 dse_summary.csv
    - 分析性能差異

11. **DSE 細化**
    - 根據初期發現調整參數範圍
    - 使用 O3 CPU 進行高精度探索

### 第 8 階段：FS mode（可選，需數小時或更長）

12. **FS mode 功能驗證**
    - 查詢官方範例
    - 等待啟動（可能很慢）

13. **FS mode 深度實驗**
    - 如果初期實驗成功，進行更複雜的 FS mode 設定

### 第 9 階段：進階話題（可選）

14. **自訂 Workload**
    - 編寫或編譯自訂程式
    - 使用 SE mode 測試

15. **自訂 Component**
    - 修改 cache 模型或記憶體模型
    - 創建自訂 component

---

## 最終檢查清單

完成以下所有項目後，表示已掌握 gem5 基本使用：

- [ ] 建立 `/home/a/HCSSimulation/experiments/gem5-learning/` 目錄結構
- [ ] 執行 se_hello.py（ATOMIC，無 cache）
- [ ] 驗證 hello world 成功輸出
- [ ] 執行 se_classic_cache.py（TIMING，有 cache）
- [ ] 觀察 cache 統計差異
- [ ] 使用 se_cpu_select.py 比較 ATOMIC、TIMING、O3
- [ ] 執行 se_memory_select.py 測試記憶體大小
- [ ] 執行 se_ruby_mesi.py（多核 MESI）
- [ ] 使用 parse_stats.py 生成 CSV 報告
- [ ] 執行 run_dse.py 進行參數掃描
- [ ] 分析 dse_summary.csv 結果
- [ ] 填寫 experiment_log.md 記錄實驗
- [ ] 瞭解並排查常見錯誤（至少 5 個）
- [ ] 嘗試 FS mode（可選但建議）
- [ ] 理解 standard library 的 component 組織
- [ ] 能夠修改配置參數並執行新實驗

---

## 補充資源

### 官方文件
- gem5 官方文件：https://www.gem5.org/
- Standard Library 指南：https://www.gem5.org/documentation/

### 常用指令速查

**進入 gem5 目錄**：
```bash
cd /home/a/HCSSimulation/gem5
```

**編譯 gem5**：
```bash
```

**執行 SE mode**：
```bash
```

**查看統計**：
```bash
grep "KEY" OUTDIR/stats.txt
```

**生成報告**：
```bash
python3 scripts/parse_stats.py OUTDIR1 OUTDIR2 --out report.csv
```

**清理結果**：
```bash
rm -rf experiments/gem5-learning/results/*
```

### 進階學習建議

1. **閱讀 gem5 論文**：
   - "The gem5 Simulator" (ISCA 2011)

2. **研究 standard library 源碼**：
   - `gem5/src/components/` 目錄
   - 理解 Component 設計模式

3. **自訂 component 開發**：
   - 修改 cache 參數
   - 實現自訂記憶體控制器

4. **參與 gem5 社區**：
   - gem5 論壇
   - GitHub issues 與 pull requests

---

## 結論

本教學涵蓋了 gem5 從基礎到進階的完整學習路徑：

1. **基礎**：SE mode、ATOMIC CPU、無 cache
2. **進階**：TIMING CPU、Classic cache、多核
3. **高級**：Ruby 一致性、FS mode、DSE
4. **工具**：統計解析、自動化實驗、實驗記錄

通過按順序完成上述實驗，使用者將全面理解 gem5 的架構設計、模擬能力與使用方法。

gem5 standard library 提供的可重用 component（Board、Processor、Memory、CacheHierarchy）能大幅降低系統配置的複雜度，是現代 gem5 使用的最佳實踐。

---

**教學完成。祝您的 gem5 學習之旅順利！**
EOF
```

Now let me verify that the tutorial file was created successfully and has comprehensive content:
<function_calls>
<invoke name="bash">
<parameter name="command">cd /home/a/HCSSimulation && wc -l prompt_generated_gem5_tutorial.md && head -100 prompt_generated_gem5_tutorial.md | tail -50