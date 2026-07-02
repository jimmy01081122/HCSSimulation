# MENA: MoE Expert-transfer and NPU Assist Processor Prototype

MENA is a hardware-software co-design prototyping framework for Mixture-of-Experts (MoE) LLM inference routing, token dispatch, expert caching, and system-level modeling.

---

## Project Structure

```text
mena-prototype/
  README.md
  report.md
  requirements.txt
  scripts/
    check_env.sh            # Script for verifying development environment
    gen_synthetic_trace.py   # Synthetic MoE routing trace generator
    gen_toy_moe_trace.py     # Toy PyTorch MoE router trace generator
    analyze_traces.py        # Utility to compute trace stats
    run_experiments.py       # Helper script to run simulator sweeps
    golden_topk.py          # Python reference model for Top-k selector
    golden_dispatch.py      # Python reference model for Dispatcher
    gen_topk_vectors.py     # Generator for Top-k test vectors
    gen_dispatch_vectors.py  # Generator for Dispatcher test vectors
    test_expert_cache.py    # Unit tests for expert cache simulator
  traces/
    *.jsonl                 # Generated MoE routing trace files
  sim/
    expert_cache.py         # Modular cache model (LRU, LFU, History)
    mena_sim.py             # Event-driven system timing simulator
    results_summary.csv     # Compiled simulator run results (CSV)
    results_summary.md      # Compiled simulator run results (Markdown table)
  rtl/
    topk_selector.sv        # SystemVerilog combinational top-2 selector
    token_dispatcher.sv      # SystemVerilog batch expert-major dispatcher
    expert_queue.sv         # SystemVerilog parameterizable token FIFO
    mena_top_stub.sv        # Top-level wrapper integrating top-k and dispatcher
  tb/
    topk_vectors.json       # Top-k test inputs & golden outputs
    dispatch_vectors.json   # Dispatcher test inputs & golden outputs
  verilator/
    topk_tb.cpp             # Verilator testbench harness for top-k selector
    dispatch_tb.cpp         # Verilator testbench harness for token dispatcher
    topk_vectors.h          # Compiled static test cases for top-k
    dispatch_vectors.h      # Compiled static test cases for dispatcher
    Makefile                # Verilator compilation Makefile
  gem5_model/
    mena_gem5_plan.md       # Integration specifications for gem5 simulator
  fpga/
    pynqz2_demo_plan.md     # Prototyping blueprint for PYNQ-Z2 overlay
  docs/
    lightweight_npu_spec.md # Microarchitecture draft for NPU module
```

---

## Setup & Run Instructions

### 1. Requirements & Dependencies
Ensure Python 3.10+ is installed. Install required packages using pip:
```bash
pip install -r requirements.txt
```

### 2. Generate Traces
To generate synthetic routing traces:
```bash
# Uniform skew
python3 scripts/gen_synthetic_trace.py --num_layers 4 --num_tokens 128 --num_experts 8 --top_k 2 --skew_type uniform --output traces/synth_uniform_E8_T128_K2.jsonl

# Zipf skew
python3 scripts/gen_synthetic_trace.py --num_layers 4 --num_tokens 128 --num_experts 8 --top_k 2 --skew_type zipf --output traces/synth_zipf_E8_T128_K2.jsonl

# Toy PyTorch MoE router trace
python3 scripts/gen_toy_moe_trace.py --output traces/toy_moe_E8_T128_K2.jsonl
```

### 3. Run Simulator Experiments
To run the full suite of system-level simulations and compile performance summaries:
```bash
python3 scripts/run_experiments.py
```
This updates `sim/results_summary.csv` and `sim/results_summary.md`.

To run the unit tests for the expert cache module:
```bash
python3 scripts/test_expert_cache.py
```

### 4. Compile and Run Verilator Tests
Verilator tests are executed using the pre-built `mccpu-dev:latest` Docker image via the provided wrapper script:

```bash
# 1. Generate test vectors
python3 scripts/gen_topk_vectors.py
python3 scripts/gen_dispatch_vectors.py

# 2. Build the Verilator testbenches inside the Docker container
./scripts/run_verilator_docker.sh make clean
./scripts/run_verilator_docker.sh make all

# 3. Run the Verilator simulations inside the Docker container
./scripts/run_verilator_docker.sh ./topk_sim
./scripts/run_verilator_docker.sh ./dispatch_sim
```

### 5. Build and Run gem5 inside Isolated gem5-dev Container
To prevent polluting the host or MCCPU toolchain, gem5 is built and executed inside a dedicated Docker Compose environment.

The gem5 source at `/home/a/gem5-new` is mounted to `/gem5` in the container.

```bash
# 1. Build the gem5-dev Docker image and spin up the environment
docker compose build

# 2. Rebuild gem5 inside the container (e.g. ARM build)
./scripts/run_gem5_docker.sh scons build/ARM/gem5.opt -j$(nproc)

# 3. Execute the dummy MENA test model inside the container
./scripts/run_gem5_docker.sh ./build/ARM/gem5.opt configs/example/mena_dummy_test.py
```


---

## Current Limitations

1.  **RTX 3050 GPU Constraints**: Low VRAM (6GB) precludes running full-scale weights. Workloads rely entirely on synthetic trace modeling.
2.  **RTL and Verilator Status**: Top-K selector and token dispatcher RTL are fully verified using Verilator inside the Docker environment. Tests pass successfully.
3.  **GEMM Logic**: The lightweight NPU specification is defined as a paper blueprint; physical verilog matrix accelerators are not implemented in this version.
