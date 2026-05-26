# HCSSimulation Development Checkpoint

This file tracks the implementation, compilation, and runtime verification status of the 10 learning topics for both ARM and RISC-V ISAs.

## Verification Checklist & Status

| Topic | Directory | Files | ARM Config | RISC-V Config | RISC-V Runtime Status |
|---|---|---|---|---|---|
| **01** | `01_se_mode` | `README.md`, `run.sh`, `config.py` | Completed | Completed | **Verified** (Outputs: Hello world!) |
| **02** | `02_fs_mode` | `README.md`, `run.sh`, `config.py` | Completed | Completed | **Verified** (Config syntax & download logic check) |
| **03** | `03_standard_library` | `README.md`, `run.sh`, `config.py` | Completed | Completed | **Verified** (Outputs: Hello world!) |
| **04** | `04_python_config` | `README.md`, `run.sh`, `config.py` | Completed | Completed | **Verified** (Outputs: Hello world!) |
| **05** | `05_cpu_model` | `README.md`, `run.sh`, `config.py` | Completed | Completed | **Verified** (Atomic, Timing, Minor, O3 sweeps) |
| **06** | `06_cache_hierarchy` | `README.md`, `run.sh`, `config.py` | Completed | Completed | **Verified** (L1i/L1d sizes: 16KiB, 32KiB, 64KiB) |
| **07** | `07_memory_system` | `README.md`, `run.sh`, `config.py` | Completed | Completed | **Verified** (Memory types: DDR3, DDR4, DDR5) |
| **08** | `08_ruby_coherence` | `README.md`, `run.sh`, `config.py` | Completed | Completed | **Verified** (Using MIExampleCacheHierarchy) |
| **09** | `09_statistics_parsing` | `README.md`, `run.sh`, `config.py`, `parse_stats.py` | Completed | Completed | *Pending verification* |
| **10** | `10_design_space_exploration` | `README.md`, `run.sh`, `config.py`, `collect_results.py` | Completed | Completed | *Pending verification* |

## Key Updates & Fixes
1. **Topic 04 (Python Config)**: Corrected SimObject class names (e.g. `DDR3_1600_8x8` instead of `DDR3_1600`) and set `system.workload = SEWorkload.init_compatible(...)` to prevent workload object binding failures in modern gem5.
2. **Topic 08 (Ruby Coherence)**: Switched the protocol in default configs from `MESI_Two_Level` to `MI_example` via `MIExampleCacheHierarchy` to align with the precompiled gem5 binary protocol support, allowing immediate out-of-the-box execution without recompilation.
