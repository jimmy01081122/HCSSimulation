# gem5 Learning Project - Documentation Index

## Recommended Reading Order

This project is divided into 10 topics to guide you through learning gem5. Please follow the topics in sequential order:

1. **[topics/01_se_mode/](topics/01_se_mode/)**: Introduction to Syscall Emulation mode.
2. **[topics/02_fs_mode/](topics/02_fs_mode/)**: Introduction to Full System mode.
3. **[topics/03_standard_library/](topics/03_standard_library/)**: Using the gem5 standard library (Board, Processor, etc.).
4. **[topics/04_python_config/](topics/04_python_config/)**: Deep dive into Python configurations in gem5.
5. **[topics/05_cpu_model/](topics/05_cpu_model/)**: Understanding different CPU models (Timing, O3, Minor).
6. **[topics/06_cache_hierarchy/](topics/06_cache_hierarchy/)**: Setting up and modifying Cache Hierarchy.
7. **[topics/07_memory_system/](topics/07_memory_system/)**: Exploring memory controllers and bandwidth.
8. **[topics/08_ruby_coherence/](topics/08_ruby_coherence/)**: Ruby memory system and Cache Coherence protocols.
9. **[topics/09_statistics_parsing/](topics/09_statistics_parsing/)**: Parsing and analyzing gem5 `stats.txt`.
10. **[topics/10_design_space_exploration/](topics/10_design_space_exploration/)**: Automating design space exploration (DSE).

## Quick Command Reference

### Setup and Clone gem5
```bash
git clone https://github.com/gem5/gem5
```

### Start Docker Container
This project uses Docker to provide the build and runtime environment.
```bash
docker run --rm -it -v "$(pwd)":/work gem5-env
```

### Compile gem5 (Inside Container)
The scripts and configurations in this repository are exclusively designed for ARM and RISC-V architectures.
```bash
cd /work/gem5
# Compile for ARM
scons build/ARM/gem5.opt -j$(nproc)

# Compile for RISC-V
scons build/RISCV/gem5.opt -j$(nproc)
```

## Running the Topics
Navigate to any topic folder to find the `README.md` and `run.sh` script. Complete the `TODO` sections inside the provided python skeleton and run the simulation using:
```bash
./run.sh
```

## File Structure Summary

```
HCSSimulation/
├── INDEX.md                              (THIS FILE - Start here!)
├── README.md                             (Project overview)
├── topics/                               (Core learning workspaces)
│   ├── 01_se_mode/
│   ├── 02_fs_mode/
│   ├── 03_standard_library/
│   ├── 04_python_config/
│   ├── 05_cpu_model/
│   ├── 06_cache_hierarchy/
│   ├── 07_memory_system/
│   ├── 08_ruby_coherence/
│   ├── 09_statistics_parsing/
│   └── 10_design_space_exploration/
└── gem5/                                 (gem5 source code - if cloned)
```

## Troubleshooting
- **Docker not available?** See `WSL2_GUIDE.md` for Windows WSL2 setups.
- **Out of memory during compile?** Ensure Docker is allocated enough RAM or decrease the `-j` factor.
