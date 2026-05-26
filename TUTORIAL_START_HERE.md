# gem5 Complete Learning Guide - Start Here

Welcome! This document will guide you through the complete gem5 learning materials. Choose your preferred ISA (ARM or RISC-V) and begin learning.

## What Has Been Created

A comprehensive, CLI-friendly gem5 teaching framework including:

1. **Main Tutorials**: 
   - `prompt_generated_gem5_tutorial_ARM.md` - ARM architecture
   - `prompt_generated_gem5_tutorial_RISCV.md` - RISC-V architecture

2. **Experiment Framework**: 
   - `experiments/gem5-learning-arm/` - ARM experiments
   - `experiments/gem5-learning-riscv/` - RISC-V experiments

## Quick Access Guide

### Choose Your ISA First

**Option A: ARM Architecture**
- Tutorial: `prompt_generated_gem5_tutorial_ARM.md`
- Framework: `experiments/gem5-learning-arm/`
- Best for: Mobile/embedded systems research

**Option B: RISC-V Architecture**
- Tutorial: `prompt_generated_gem5_tutorial_RISCV.md`
- Framework: `experiments/gem5-learning-riscv/`
- Best for: Open-source ISA learning

### For Absolute Beginners

Start with these steps (using ARM as example):

1. **Read the Introduction** in `prompt_generated_gem5_tutorial_ARM.md`:
   - Part 1: gem5 Execution Modes Overview
   - Part 2: Standard Library Components

2. **Run Your First Experiment**:
   ```bash
   cd /home/a/HCSSimulation/gem5
   ./build/ARM/gem5.opt \
     --outdir=../experiments/gem5-learning-arm/results/se_hello \
     ../experiments/gem5-learning-arm/configs/se_hello_arm.py
   ```

3. **Verify Success**:
   ```bash
   ls ../experiments/gem5-learning-arm/results/se_hello/
   cat ../experiments/gem5-learning-arm/results/se_hello/stats.txt | head -30
   ```

### For Users with gem5 Experience

Jump directly to sections of interest in your chosen tutorial:

- **Part 5**: CPU Model Teaching (ATOMIC, TIMING, O3)
- **Part 7**: Ruby Coherence (MESI Two Level)
- **Part 10**: Design Space Exploration

### For Those Ready for Automation

Use the provided scripts (ARM example):

```bash
# Parse statistics from experiments
python3 experiments/gem5-learning-arm/scripts/parse_stats_arm.py \
  experiments/gem5-learning-arm/results/se_hello \
  --out experiments/gem5-learning-arm/results/summary.csv
```

## Tutorial Structure

### Part 1-2: Foundations
- SE mode vs FS mode comparison
- Standard library components
- Why standard library reduces configuration burden

### Part 3-4: SE Mode Basics
- Experiment A: Minimal SE mode (ATOMIC, no cache)
- Experiment B: SE mode with classic cache (TIMING)

### Part 5-6: Architecture Exploration
- CPU models (ATOMIC, TIMING, O3, KVM)
- Memory systems (DDR3, sizes, channels)

### Part 7: Multi-core
- Ruby coherence protocols
- MESI Two Level cache hierarchy
- Multi-core synchronization

### Part 8: Full System
- FS mode experimentation
- Kernel and disk images
- System-level simulation

### Part 9-10: Automation
- Statistics parsing (parse_stats.py)
- Design space exploration runner (run_dse.py)

### Part 11-12: Practice & Troubleshooting
- Experiment log templates
- Common errors and solutions
- 10+ error cases with fixes

### Part 13: Learning Roadmap
- Recommended learning sequence
- Skill progression
- Advanced topics

## File Locations (ARM Example)

```
/home/a/HCSSimulation/
├── prompt_generated_gem5_tutorial_ARM.md      # ARM tutorial (READ THIS!)
├── prompt_generated_gem5_tutorial_RISCV.md    # RISC-V tutorial
├── ISA_SELECTION_GUIDE.md                     # Choose your ISA
├── TUTORIAL_START_HERE.md                     # This file
├── experiments/gem5-learning-arm/
│   ├── README.md                              # Quick start guide
│   ├── configs/
│   │   ├── se_hello_arm.py                    # Hello world example
│   │   ├── se_classic_cache_arm.py            # Cache hierarchy example
│   │   ├── se_cpu_select_arm.py               # CPU model selection
│   │   └── se_dse_config_arm.py               # DSE configuration
│   ├── scripts/
│   │   └── parse_stats_arm.py                 # Statistics parser
│   ├── results/                               # Output directory
│   ├── workloads/                             # Custom programs
│   └── notes/
│       └── experiment_log.md                  # Experiment template
└── experiments/gem5-learning-riscv/
    └── (Similar structure for RISC-V)
```

## Learning Path (Recommended)

### Beginner (1-2 hours)
1. Read `ISA_SELECTION_GUIDE.md` to choose your ISA
2. Read Part 1-2 of your chosen tutorial
3. Run `se_hello_*.py` - verify gem5 works
4. Run `se_classic_cache_*.py` - observe cache effects
5. Compare ATOMIC vs TIMING - understand CPU models

### Intermediate (3-4 hours)
1. CPU model comparison (ATOMIC, TIMING, O3)
2. Memory system exploration
3. Ruby MESI multi-core
4. Statistics parsing

### Advanced (4+ hours)
1. Design space exploration
2. Custom workload development
3. Component customization
4. Cross-ISA comparison

## Pre-requisites

Before starting, ensure:

```bash
# 1. Check gem5 installation
cd /home/a/HCSSimulation/gem5
ls -la build/ALL/gem5.opt || ls -la build/X86/gem5.opt

# 2. If gem5 not compiled, compile it
python3 `which scons` build/X86/gem5.opt -j$(nproc)

# 3. Check Python version
python3 --version  # Should be 3.6 or later

# 4. Verify WSL2 CPUs
nproc  # Check available cores
```

## How to Read the Tutorial

1. **First Reading**: Skim through entire tutorial to understand scope
2. **Focused Reading**: Read each part relevant to your current learning stage
3. **Practical Work**: Implement experiments as you read
4. **Reference**: Keep tutorial handy for command syntax and explanations

### Key Features of the Tutorial

✓ **CLI-Friendly**: All commands can be directly copy-pasted  
✓ **Complete Code**: No abbreviated code snippets  
✓ **Step-by-Step**: Each experiment has clear execution steps  
✓ **Explanation**: Every concept linked to gem5 architecture  
✓ **Verification**: How to confirm each step succeeded  
✓ **Troubleshooting**: 10+ common errors with solutions  

## Quick Command Reference

```bash
# Environment setup
cd /home/a/HCSSimulation/gem5
export PYTHONPATH=/home/a/HCSSimulation/gem5:$PYTHONPATH

# Run SE mode
./build/X86/gem5.opt --outdir=OUTPUT_DIR CONFIG.py [ARGS]

# Parse results
python3 ../experiments/gem5-learning/scripts/parse_stats.py \
  result_dir1 result_dir2 ... --out summary.csv

# Check statistics
grep "KEY_NAME" output_dir/stats.txt
```

## Common Mistakes to Avoid

1. **Using FS mode too early** - Start with SE mode, it's 100x faster
2. **Using O3 CPU for DSE** - Use TIMING first, O3 is extremely slow
3. **Overwriting results** - Always use unique --outdir
4. **Not reading error messages** - gem5 error messages are informative
5. **Running on Windows directly** - Use WSL2 as specified

## Next Steps

1. **Read**: `prompt_generated_gem5_tutorial.md` (start with Part 1-2)
2. **Setup**: Verify gem5 environment (`cd /home/a/HCSSimulation/gem5`)
3. **Run**: Execute `experiments/gem5-learning/configs/se_hello.py`
4. **Explore**: Follow the learning path in tutorial Part 13

## Getting Help

### Tutorial-Related Questions
- Refer to Part 12 (Troubleshooting) in the main tutorial
- Check the experiment templates in `notes/experiment_log.md`

### gem5-Specific Questions
- Official gem5 documentation: https://www.gem5.org/
- Standard Library guide: https://www.gem5.org/documentation/

### WSL2 Issues
- Refer to `WSL2_GUIDE.md` in the repository root

## Estimated Learning Time

- **Part 1-2 (Concepts)**: 30 minutes
- **Part 3-4 (SE Mode)**: 1-2 hours
- **Part 5-7 (Architecture)**: 2-3 hours
- **Part 8 (FS Mode)**: 1-2 hours (optional, slow)
- **Part 9-10 (Automation)**: 1 hour
- **Part 11-13 (Practice)**: 2-4 hours
- **Total**: 8-15 hours for comprehensive mastery

## Final Checklist

After completing the tutorial, you should be able to:

- [ ] Explain the difference between SE mode and FS mode
- [ ] Create a basic gem5 configuration using standard library
- [ ] Run SE mode simulations with different CPU models
- [ ] Interpret and parse gem5 statistics
- [ ] Perform design space exploration
- [ ] Debug common gem5 configuration errors
- [ ] Document and reproduce experiments

---

**Happy learning with gem5!**

For questions or issues, refer back to the comprehensive tutorial in `prompt_generated_gem5_tutorial.md`.
