# gem5 完整學習與實驗教學指南 - RISCV ISA 版本

本教學針對使用 **RISCV ISA** 的學習者，提供完整、可操作的 gem5 建構引導、實作教學、詳細實驗流程與排查指南。

---

## 前置檢查：確認 gem5 環境（RISCV ISA）

### 1. 確認 RISCV gem5 二進位

```bash
cd /home/a/HCSSimulation/gem5
ls -la build/RISCV/gem5.opt || ls -la build/ALL/gem5.opt
```

**重要**：RISCV ISA 需要編譯 `build/RISCV/gem5.opt`

### 2. 編譯 RISCV 版本的 gem5

```bash
cd /home/a/HCSSimulation/gem5
python3 `which scons` build/RISCV/gem5.opt -j$(nproc)
```

編譯時間：20-40 分鐘

### 3. 驗證 RISCV 資源

```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.resources import list_resources
resources = list_resources()
riscv_resources = [r for r in resources if 'riscv' in r.lower()]
print(f'Available RISCV resources: {len(riscv_resources)}')
"
```

---

## 第一部分：RISCV ISA 簡介

### RISCV 架構特性

**RISCV** 是開放的 RISC（精簡指令集）架構，具有以下特性：
- **開放標準**：完全開源，任何人可以實現
- **模組化設計**：基礎 ISA + 多個擴展
- **簡潔優雅**：最小指令集設計
- **gem5 支援**：完整的 RISCV 模擬模型
- **未來導向**：學術與產業應用增長

### RISCV 變體

- **RV32I**：32-bit 基礎指令集
- **RV64I**：64-bit 基礎指令集
- **擴展**：M (乘法)、A (原子)、F (浮點)、D (雙精度) 等

### RISCV vs ARM vs x86

| 特性 | RISCV | ARM | x86 |
|---|---|---|---|
| **開放性** | 完全開源 | 閉源 | 閉源 |
| **簡潔性** | 最簡潔 | 中等 | 複雜 |
| **應用** | 新興/學術 | 移動/嵌入 | 桌面/服務 |
| **學習難度** | 低 | 中 | 高 |

---

## 第二部分：RISCV SE Mode 實驗

### 實驗 A：RISCV Hello World (ATOMIC)

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning-riscv/configs/se_hello_riscv.py <<'EOF'
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

processor = SimpleProcessor(
    cpu_type=CPUTypes.ATOMIC,
    isa=ISA.RISCV,
    num_cores=1
)

from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600

memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

from gem5.components.cachehierarchy.caches.cache_hierarchy import NoCache

cache_hierarchy = NoCache()

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

binary_path = obtain_resource("riscv-ubuntu-20.04-boot")
board.set_se_binary_workload(binary_path)

simulator = Simulator(board=board)
simulator.run()
EOF
```

執行：

```bash
cd /home/a/HCSSimulation/gem5

./build/RISCV/gem5.opt \
  --outdir=../experiments/gem5-learning-riscv/results/se_hello \
  ../experiments/gem5-learning-riscv/configs/se_hello_riscv.py
```

### 實驗 B：RISCV with Cache (TIMING)

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning-riscv/configs/se_classic_cache_riscv.py <<'EOF'
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.caches.l1_caches import L1Cache
from gem5.components.cachehierarchy.caches.l2_cache import L2Cache
from gem5.components.cachehierarchy.caches.cache_hierarchy import PrivateL1PrivateL2CacheHierarchy
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

processor = SimpleProcessor(
    cpu_type=CPUTypes.TIMING,
    isa=ISA.RISCV,
    num_cores=1
)

from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600

memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

l1i = L1Cache(size="32KiB", assoc=8, tag_latency=1, response_latency=1,
              mshrs=16, tgts_per_mshr=20, is_icache=True)
l1d = L1Cache(size="32KiB", assoc=8, tag_latency=2, response_latency=2,
              mshrs=16, tgts_per_mshr=20, is_icache=False)
l2 = L2Cache(size="256KiB", assoc=16, tag_latency=20, response_latency=20,
             mshrs=20, tgts_per_mshr=12)

cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1i_cache=l1i, l1d_cache=l1d, l2_cache=l2
)

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

binary_path = obtain_resource("riscv-ubuntu-20.04-boot")
board.set_se_binary_workload(binary_path)

simulator = Simulator(board=board)
simulator.run()
EOF
```

執行：

```bash
./build/RISCV/gem5.opt \
  --outdir=../experiments/gem5-learning-riscv/results/se_classic_cache \
  ../experiments/gem5-learning-riscv/configs/se_classic_cache_riscv.py
```

---

## 第三部分：RISCV CPU Models

### RISCV CPU 類型比較

| CPU Model | 支援 | 用途 |
|---|---|---|
| ATOMIC | ✅ | 快速功能驗證 |
| TIMING | ✅ | Cache 行為分析 |
| O3 | ✅ | 微架構詳細模擬 |

### CPU 選擇配置

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning-riscv/configs/se_cpu_select_riscv.py <<'EOF'
import sys
import argparse
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.caches.cache_hierarchy import (
    NoCache, PrivateL1PrivateL2CacheHierarchy
)
from gem5.components.cachehierarchy.caches.l1_caches import L1Cache
from gem5.components.cachehierarchy.caches.l2_cache import L2Cache
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

parser = argparse.ArgumentParser()
parser.add_argument("--cpu", choices=["ATOMIC", "TIMING", "O3"], default="ATOMIC")
parser.add_argument("--cores", type=int, default=1)
args = parser.parse_args()

cpu_type_map = {"ATOMIC": CPUTypes.ATOMIC, "TIMING": CPUTypes.TIMING, "O3": CPUTypes.O3}

processor = SimpleProcessor(
    cpu_type=cpu_type_map[args.cpu],
    isa=ISA.RISCV,
    num_cores=args.cores
)

from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600

memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

if args.cpu == "ATOMIC":
    cache_hierarchy = NoCache()
else:
    l1i = L1Cache(size="32KiB", assoc=8, tag_latency=1, response_latency=1, mshrs=16, tgts_per_mshr=20, is_icache=True)
    l1d = L1Cache(size="32KiB", assoc=8, tag_latency=2, response_latency=2, mshrs=16, tgts_per_mshr=20, is_icache=False)
    l2 = L2Cache(size="256KiB", assoc=16, tag_latency=20, response_latency=20, mshrs=20, tgts_per_mshr=12)
    cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(l1i_cache=l1i, l1d_cache=l1d, l2_cache=l2)

board = SimpleBoard(clk_freq="3GHz", processor=processor, memory=memory, cachehierarchy=cache_hierarchy)

binary_path = obtain_resource("riscv-ubuntu-20.04-boot")
board.set_se_binary_workload(binary_path)

simulator = Simulator(board=board)
simulator.run()
EOF
```

執行：

```bash
./build/RISCV/gem5.opt --outdir=../experiments/gem5-learning-riscv/results/se_cpu_timing \
  ../experiments/gem5-learning-riscv/configs/se_cpu_select_riscv.py --cpu TIMING --cores 1
```

---

## 第四部分：RISCV 統計分析

### 統計解析工具

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning-riscv/scripts/parse_stats_riscv.py <<'EOF'
#!/usr/bin/env python3
import os, sys, argparse, csv

def parse_stats_file(stats_file):
    stats = {}
    try:
        with open(stats_file, 'r') as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#'): continue
                parts = line.split()
                if len(parts) >= 2:
                    try:
                        stats[parts[0]] = float(parts[1]) if '.' in parts[1] else int(parts[1])
                    except ValueError:
                        stats[parts[0]] = parts[1]
    except FileNotFoundError:
        return None
    return stats

def extract_metrics(stats_dict):
    metrics = {}
    key_mapping = {
        'simInsts': ['simInsts', 'system.cpu.simInsts'],
        'simTicks': ['simTicks', 'system.cpu.simTicks'],
        'hostSeconds': ['hostSeconds'],
        'ipc': ['system.cpu.ipc'],
    }
    for metric, keys in key_mapping.items():
        for k in keys:
            if k in stats_dict:
                metrics[metric] = stats_dict[k]
                break
    return metrics

parser = argparse.ArgumentParser()
parser.add_argument('results_dirs', nargs='+')
parser.add_argument('--out', default='summary.csv')
args = parser.parse_args()

all_metrics = []
for result_dir in args.results_dirs:
    stats_file = os.path.join(result_dir, 'stats.txt')
    if not os.path.exists(stats_file): continue
    stats = parse_stats_file(stats_file)
    if stats:
        metrics = extract_metrics(stats)
        metrics['result_dir'] = result_dir
        all_metrics.append(metrics)

fieldnames = set(['result_dir'])
for m in all_metrics:
    fieldnames.update(m.keys())

with open(args.out, 'w') as f:
    writer = csv.DictWriter(f, fieldnames=sorted(fieldnames))
    writer.writeheader()
    writer.writerows(all_metrics)

print(f"Results saved to {args.out}")
EOF

chmod +x /home/a/HCSSimulation/experiments/gem5-learning-riscv/scripts/parse_stats_riscv.py
```

---

## 第五部分：RISCV DSE

### 參數化配置

```bash
cat > /home/a/HCSSimulation/experiments/gem5-learning-riscv/configs/se_dse_config_riscv.py <<'EOF'
import sys, argparse
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchy.caches.cache_hierarchy import NoCache, PrivateL1PrivateL2CacheHierarchy
from gem5.components.cachehierarchy.caches.l1_caches import L1Cache
from gem5.components.cachehierarchy.caches.l2_cache import L2Cache
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

parser = argparse.ArgumentParser()
parser.add_argument('--cpu', choices=['ATOMIC', 'TIMING', 'O3'], default='TIMING')
parser.add_argument('--cores', type=int, default=1)
parser.add_argument('--l1d', type=int, default=32)
parser.add_argument('--l1i', type=int, default=32)
parser.add_argument('--l2', type=int, default=256)
parser.add_argument('--l1-assoc', type=int, default=8)
parser.add_argument('--l2-assoc', type=int, default=16)
parser.add_argument('--mem-size', type=int, default=2)
args = parser.parse_args()

cpu_map = {'ATOMIC': CPUTypes.ATOMIC, 'TIMING': CPUTypes.TIMING, 'O3': CPUTypes.O3}
mem_map = {1: MemorySizeType.GB_1, 2: MemorySizeType.GB_2, 4: MemorySizeType.GB_4}

processor = SimpleProcessor(cpu_type=cpu_map[args.cpu], isa=ISA.RISCV, num_cores=args.cores)

from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600
memory = SingleChannelDDR3_1600(size=mem_map.get(args.mem_size, MemorySizeType.GB_2))

if args.cpu == 'ATOMIC':
    cache_hierarchy = NoCache()
else:
    l1i = L1Cache(size=f"{args.l1i}KiB", assoc=args.l1_assoc, tag_latency=1, response_latency=1, mshrs=16, tgts_per_mshr=20, is_icache=True)
    l1d = L1Cache(size=f"{args.l1d}KiB", assoc=args.l1_assoc, tag_latency=2, response_latency=2, mshrs=16, tgts_per_mshr=20, is_icache=False)
    l2 = L2Cache(size=f"{args.l2}KiB", assoc=args.l2_assoc, tag_latency=20, response_latency=20, mshrs=20, tgts_per_mshr=12)
    cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(l1i_cache=l1i, l1d_cache=l1d, l2_cache=l2)

board = SimpleBoard(clk_freq="3GHz", processor=processor, memory=memory, cachehierarchy=cache_hierarchy)
binary_path = obtain_resource("riscv-ubuntu-20.04-boot")
board.set_se_binary_workload(binary_path)

simulator = Simulator(board=board)
simulator.run()
EOF
```

---

## 第六部分：RISCV 常見錯誤排查

### 錯誤 1：RISCV gem5.opt 不存在

```bash
cd /home/a/HCSSimulation/gem5
python3 `which scons` build/RISCV/gem5.opt -j$(nproc)
```

### 錯誤 2：RISCV 資源找不到

查詢可用 RISCV 資源：

```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.resources import list_resources
riscv_res = [r for r in list_resources() if 'riscv' in r.lower()]
for r in sorted(riscv_res)[:10]:
    print(r)
"
```

### 錯誤 3：ISA.RISCV 未定義

檢查可用 ISA：

```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.components.processors.isa import ISA
print([x for x in dir(ISA) if not x.startswith('_')])
"
```

---

## RISCV 特性說明

### 為什麼學習 RISCV

1. **開放標準**：完全開源，任何人可以參與
2. **簡潔設計**：最小化指令集，易於學習和實現
3. **教育價值**：非常適合計算機架構教育
4. **未來導向**：許多新項目採用 RISCV
5. **靈活性**：可以自由擴展和定制

### RISCV 與其他 ISA 對比

- **vs ARM**：RISCV 更開放，ARM 在業界應用更廣
- **vs x86**：RISCV 更簡潔，x86 兼容性更好
- **vs MIPS**：RISCV 是現代設計，MIPS 較老

---

## 最終檢查清單

- [ ] 編譯 RISCV 版 gem5 (build/RISCV/gem5.opt)
- [ ] 執行 se_hello_riscv.py
- [ ] 執行 se_classic_cache_riscv.py
- [ ] 執行 RISCV CPU 比較
- [ ] 執行 RISCV DSE
- [ ] 分析 RISCV 性能

---

**RISCV ISA 教學完成！**

RISCV 作為開放標準的 ISA，提供了優秀的學習平台。通過本教學，您可以深入理解 RISCV 架構的優雅設計。
