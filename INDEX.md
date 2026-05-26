# gem5 Learning Project - Documentation Index

## Recommended Reading Order

### Phase 1: Quick Orientation (15 minutes)
1. [TUTORIAL_START_HERE.md](TUTORIAL_START_HERE.md) - Start here! Overview of available materials
2. [ISA_SELECTION_GUIDE.md](ISA_SELECTION_GUIDE.md) - Choose between ARM and RISC-V

### Phase 2: Core Learning (Choose ONE based on ISA preference)

**For ARM Architecture:**
- [prompt_generated_gem5_tutorial_ARM.md](prompt_generated_gem5_tutorial_ARM.md) - Complete ARM tutorial (13 sections, 3000+ lines)

**For RISC-V Architecture:**
- [prompt_generated_gem5_tutorial_RISCV.md](prompt_generated_gem5_tutorial_RISCV.md) - Complete RISC-V tutorial (13 sections, 3000+ lines)

### Phase 3: Advanced Topics (After completing Phase 2)

#### Research Directions
1. [docs/direction1_dma_tutorial.md](docs/direction1_dma_tutorial.md) - Memory Bandwidth & DMA Data Transfer
2. [docs/direction2_moe_routing_tutorial.md](docs/direction2_moe_routing_tutorial.md) - MoE Routing Latency Analysis
3. [docs/direction3_kv_cache_tutorial.md](docs/direction3_kv_cache_tutorial.md) - KV Cache Prefetch Strategy

#### Tools & Utilities
- [docs/visualization_tools_tutorial.md](docs/visualization_tools_tutorial.md) - Graphviz & O3 Pipeline Visualization

#### Reference Documents
- [docs/HANDOVER.md](docs/HANDOVER.md) - Project Status & Transition Notes

### Phase 4: Environment Setup (Optional)

- [WSL2_GUIDE.md](WSL2_GUIDE.md) - Windows Subsystem for Linux 2 Setup Guide

## Summary Documents

- [README.md](README.md) - Project overview and quick reference
- [MULTI_ISA_SUMMARY.md](MULTI_ISA_SUMMARY.md) - Multi-ISA implementation summary
- [FINAL_CHECKLIST.txt](FINAL_CHECKLIST.txt) - Project completion checklist
- [LINT_FIXES_LOG.md](LINT_FIXES_LOG.md) - Code quality improvements log

## Experiment Directories

### ARM Experiments
```
experiments/gem5-learning-arm/
├── README.md                 - ARM quick start
├── configs/                  - ARM configuration files
├── scripts/                  - ARM utility scripts
├── results/                  - ARM simulation outputs
├── workloads/                - ARM test programs
└── notes/                    - ARM experiment notes
```

### RISC-V Experiments
```
experiments/gem5-learning-riscv/
├── README.md                 - RISC-V quick start
├── configs/                  - RISC-V configuration files
├── scripts/                  - RISC-V utility scripts
├── results/                  - RISC-V simulation outputs
├── workloads/                - RISC-V test programs
└── notes/                    - RISC-V experiment notes
```

### Research Direction Experiments
```
dir/TRY/
├── dir1_memory_dma/          - Memory bandwidth & DMA
├── dir2_moe_routing/         - MoE routing analysis
└── dir3_kv_cache/            - KV cache prefetch
```

## Quick Command Reference

### Start Docker Container
```bash
docker run --rm -it -v "$(pwd)":/work gem5-env
```

### Compile gem5 (Inside Container)
```bash
# ARM
cd /work/gem5
scons build/ARM/gem5.opt -j$(nproc)

# RISC-V
scons build/RISCV/gem5.opt -j$(nproc)
```

### Run First Experiment (ARM)
```bash
cd /work/gem5
./build/ARM/gem5.opt \
  --outdir=../experiments/gem5-learning-arm/results/se_hello \
  ../experiments/gem5-learning-arm/configs/se_hello_arm.py
```

### Run First Experiment (RISC-V)
```bash
cd /work/gem5
./build/RISCV/gem5.opt \
  --outdir=../experiments/gem5-learning-riscv/results/se_hello \
  ../experiments/gem5-learning-riscv/configs/se_hello_riscv.py
```

### Parse Statistics (ARM)
```bash
python3 experiments/gem5-learning-arm/scripts/parse_stats_arm.py \
  experiments/gem5-learning-arm/results/se_hello \
  --out experiments/gem5-learning-arm/results/summary.csv
```

### Parse Statistics (RISC-V)
```bash
python3 experiments/gem5-learning-riscv/scripts/parse_stats_riscv.py \
  experiments/gem5-learning-riscv/results/se_hello \
  --out experiments/gem5-learning-riscv/results/summary.csv
```

## Learning Objectives by Phase

### Phase 1: Foundation (After TUTORIAL_START_HERE.md)
- Understand gem5 execution modes (SE vs FS)
- Know the difference between ARM and RISC-V
- Familiar with basic gem5 workflow

### Phase 2: Core Skills (After main tutorial)
- Configure gem5 with standard library
- Run SE mode experiments
- Understand CPU models (ATOMIC, TIMING, O3)
- Analyze simulation statistics

### Phase 3: Advanced Topics (After research directions)
- Analyze memory system performance
- Understand cache coherence
- Design space exploration
- Performance optimization techniques

## File Structure Summary

```
HCSSimulation/
├── INDEX.md                              (THIS FILE - Start here!)
├── TUTORIAL_START_HERE.md                (Phase 1)
├── ISA_SELECTION_GUIDE.md                (Phase 1)
├── README.md                             (Project overview)
├── prompt_generated_gem5_tutorial_ARM.md (Phase 2 - ARM)
├── prompt_generated_gem5_tutorial_RISCV.md (Phase 2 - RISC-V)
│
├── docs/
│   ├── direction1_dma_tutorial.md        (Phase 3)
│   ├── direction2_moe_routing_tutorial.md (Phase 3)
│   ├── direction3_kv_cache_tutorial.md   (Phase 3)
│   ├── visualization_tools_tutorial.md   (Phase 3)
│   └── HANDOVER.md                       (Reference)
│
├── experiments/
│   ├── gem5-learning-arm/                (ARM workspace)
│   ├── gem5-learning-riscv/              (RISC-V workspace)
│   └── gem5-learning/                    (Deprecated x86)
│
├── dir/TRY/                              (Research directions)
│   ├── dir1_memory_dma/
│   ├── dir2_moe_routing/
│   └── dir3_kv_cache/
│
├── gem5/                                 (gem5 source code)
│
└── scripts/
    └── setup_wsl2.sh
```

## Troubleshooting

- **Docker not available?** See [WSL2_GUIDE.md](WSL2_GUIDE.md)
- **gem5 won't compile?** Check [README.md](README.md) for memory requirements
- **Experiment fails?** Check the Phase 2 tutorial for your ISA
- **Need architecture info?** See [ISA_SELECTION_GUIDE.md](ISA_SELECTION_GUIDE.md)

## Last Updated
- Document Version: 1.0
- Framework: gem5 v24 (latest)
- Date: 2024-05-26
