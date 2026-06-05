#!/bin/bash
# run_smoke_test.sh
# End-to-end smoke test for trace generation, cache simulation, stimulus conversion, and RTL testing.

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR="$DIR/.."

cd "$PROJECT_DIR"

echo "=== Step 1: Generating Synthetic Traces ==="
python3 python/generate_synthetic_moe_trace.py \
  --num-experts 8 \
  --top-k 1 \
  --num-layers 1 \
  --num-tokens 64 \
  --distribution zipf \
  --zipf-s 1.2 \
  --prefetch-distance 4 \
  --output traces/smoke_trace.csv \
  --output-hint traces/smoke_hint.csv \
  --seed 42
if [ $? -ne 0 ]; then
    echo "FAIL: Trace generation failed."
    exit 1
fi

echo "=== Step 2: Running Cache Simulation ==="
python3 python/expert_cache_sim.py \
  --trace traces/smoke_trace.csv \
  --hint-trace traces/smoke_hint.csv \
  --policy routing_aware_next \
  --cache-size 4 \
  --dma-latency 10 \
  --score-threshold 0.1 \
  --output results/smoke_sim.csv
if [ $? -ne 0 ]; then
    echo "FAIL: Cache simulation failed."
    exit 1
fi

echo "=== Step 3: Converting Trace to RTL Testbench Stimulus ==="
python3 python/trace_to_testbench.py \
  --trace traces/smoke_trace.csv \
  --hint-trace traces/smoke_hint.csv \
  --output-req tb/stimulus_req.hex \
  --output-hint tb/stimulus_hint.hex
if [ $? -ne 0 ]; then
    echo "FAIL: Trace conversion to RTL stimulus failed."
    exit 1
fi

echo "=== Step 4: Running Verilog RTL Testbenches ==="
bash scripts/run_iverilog_tests.sh
if [ $? -ne 0 ]; then
    echo "FAIL: RTL simulation testbenches failed."
    exit 1
fi

echo "SMOKE TEST PASSED"
exit 0
