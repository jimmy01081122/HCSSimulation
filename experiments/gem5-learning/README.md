# gem5 Learning and Experimentation Framework

This directory contains a complete framework for learning and experimenting with gem5 simulator based on the specifications in `/home/a/HCSSimulation/prompt.md`.

## Directory Structure

```
gem5-learning/
├── configs/          # gem5 configuration files (Python scripts)
├── scripts/          # Utility scripts (statistics parsing, DSE runner)
├── results/          # Simulation output directories
├── workloads/        # Custom or downloaded test programs
├── notes/            # Experiment logs and documentation
└── README.md         # This file
```

## Quick Start

### 1. Environment Setup

Verify gem5 is installed and compiled:

```bash
cd /home/a/HCSSimulation/gem5
ls -la build/ALL/gem5.opt || ls -la build/X86/gem5.opt

# If not found, compile gem5:
python3 `which scons` build/X86/gem5.opt -j$(nproc)
```

### 2. Run Your First Experiment

Execute the hello world example:

```bash
cd /home/a/HCSSimulation/gem5

./build/ALL/gem5.opt \
  --outdir=../experiments/gem5-learning/results/se_hello \
  ../experiments/gem5-learning/configs/se_hello.py
```

Check the results:

```bash
ls ../experiments/gem5-learning/results/se_hello/
cat ../experiments/gem5-learning/results/se_hello/stats.txt | head -50
```

### 3. Parse Statistics

Extract key metrics from simulation results:

```bash
python3 ../experiments/gem5-learning/scripts/parse_stats.py \
  ../experiments/gem5-learning/results/se_hello \
  --out ../experiments/gem5-learning/results/summary.csv

cat ../experiments/gem5-learning/results/summary.csv
```

## Included Configuration Files

- **se_hello.py**: SE mode with ATOMIC CPU and no cache (fastest, least accurate)
- **se_classic_cache.py**: SE mode with TIMING CPU and classic cache hierarchy
- **se_cpu_select.py**: Parameterizable SE mode for CPU model comparison
- **se_memory_select.py**: SE mode with configurable memory size
- **se_ruby_mesi.py**: Multi-core SE mode with MESI coherence protocol
- **fs_x86_template.py**: FS mode template (experimental, very slow on WSL2)
- **se_dse_config.py**: Fully parameterized configuration for design space exploration

## Included Scripts

- **parse_stats.py**: Parse gem5 stats.txt files and generate CSV summaries
- **run_dse.py**: Automated design space exploration runner (parameter sweep)

## Learning Path

Follow this sequence to master gem5:

1. **Basics** (1-2 hours)
   - Run se_hello.py
   - Run se_classic_cache.py
   - Compare ATOMIC vs TIMING CPU

2. **CPU Models** (30 min - 2 hours)
   - Use se_cpu_select.py to compare O3, TIMING, ATOMIC
   - Analyze performance differences

3. **Memory Systems** (1 hour)
   - Run se_memory_select.py with different sizes
   - Observe memory impact on performance

4. **Multi-core & Coherence** (1-2 hours)
   - Run se_ruby_mesi.py
   - Study cache coherence statistics

5. **Automation** (30 min)
   - Use parse_stats.py to generate reports
   - Compare multiple experiments

6. **Design Space Exploration** (2-4 hours)
   - Run run_dse.py for parameter sweeps
   - Analyze DSE results

7. **Advanced** (optional)
   - Try FS mode (very slow on WSL2, not recommended for initial learning)
   - Create custom workloads
   - Extend gem5 components

## Common Commands

### Run a basic SE mode simulation

```bash
cd /home/a/HCSSimulation/gem5

./build/X86/gem5.opt \
  --outdir=../experiments/gem5-learning/results/my_exp \
  ../experiments/gem5-learning/configs/se_hello.py
```

### View simulation statistics

```bash
grep -E "simInsts|simTicks|ipc" \
  ../experiments/gem5-learning/results/my_exp/stats.txt
```

### Parse multiple results into CSV

```bash
python3 ../experiments/gem5-learning/scripts/parse_stats.py \
  result1 result2 result3 \
  --out summary.csv
```

### Run design space exploration

```bash
python3 ../experiments/gem5-learning/scripts/run_dse.py
# Warning: This may take several hours!
```

## Troubleshooting

### Resource not found

If you get "Resource 'x86-hello64-static' not found":

```bash
python3 -c "
import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')
from gem5.resources import obtain_resource
obtain_resource('x86-hello64-static')
print('Resource downloaded successfully')
"
```

### gem5.opt not found

Compile gem5:

```bash
cd /home/a/HCSSimulation/gem5
python3 `which scons` build/X86/gem5.opt -j$(nproc)
```

### Python module not found

Add gem5 to PYTHONPATH:

```bash
export PYTHONPATH=/home/a/HCSSimulation/gem5:$PYTHONPATH
```

### O3 simulation is too slow

This is expected! O3 CPU has very high complexity. Options:

1. Use TIMING CPU instead for faster iteration
2. Reduce workload complexity
3. Use ATOMIC to test logic first, then O3 for final validation

## Notes

- All scripts are CLI-friendly and can be copied-pasted directly
- Experiment with small parameter ranges first, then expand
- Use --outdir to save each experiment separately (avoid overwriting)
- Statistics are logged in stats.txt in each output directory
- Configuration parameters are documented in the main tutorial file

## Additional Resources

- Main tutorial: `/home/a/HCSSimulation/prompt_generated_gem5_tutorial.md`
- gem5 Official: https://www.gem5.org/
- Standard Library: https://www.gem5.org/documentation/

