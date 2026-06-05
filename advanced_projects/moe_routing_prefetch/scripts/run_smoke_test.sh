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

echo "=== Step 2: Converting Trace to RTL Testbench Stimulus ==="
python3 python/trace_to_testbench.py \
  --trace traces/smoke_trace.csv \
  --hint-trace traces/smoke_hint.csv \
  --output-req tb/stimulus_req.hex \
  --output-hint tb/stimulus_hint.hex
if [ $? -ne 0 ]; then
    echo "FAIL: Trace conversion to RTL stimulus failed."
    exit 1
fi

for repl in fifo lru; do
    echo "=== Running test for replacement policy: $repl ==="
    
    if [ "$repl" = "fifo" ]; then
        export REPL_POLICY_SEL=0
    else
        export REPL_POLICY_SEL=1
    fi
    
    echo "--- Step A: Running Cache Simulation ($repl) ---"
    python3 python/expert_cache_sim.py \
      --trace traces/smoke_trace.csv \
      --hint-trace traces/smoke_hint.csv \
      --policy routing_aware_next \
      --replacement-policy $repl \
      --cache-size 4 \
      --dma-latency 10 \
      --score-threshold 0.1 \
      --output results/smoke_sim.csv
    if [ $? -ne 0 ]; then
        echo "FAIL: Cache simulation for $repl failed."
        exit 1
    fi
    
    echo "--- Step B: Running Verilog RTL Testbench ($repl) ---"
    bash scripts/run_iverilog_tests.sh > rtl_run.log 2>&1
    RTL_STATUS=$?
    cat rtl_run.log
    if [ $RTL_STATUS -ne 0 ]; then
        echo "FAIL: RTL simulation testbenches for $repl failed."
        exit 1
    fi
    
    echo "--- Step C: Comparing Python vs RTL Counters ($repl) ---"
    python3 -c "
import csv
import re
import sys

csv_data = {}
with open('results/smoke_sim.csv', 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        csv_data = {
            'total_requests': int(row['total_requests']),
            'cache_hits': int(row['cache_hits']),
            'cache_misses': int(row['cache_misses']),
            'prefetch_issued_count': int(row['prefetch_issued_count']),
            'prefetch_useful_count': int(row['prefetch_useful_count']),
            'prefetch_useless_count': int(row['prefetch_useless_count']),
            'prefetch_dropped_count': int(row['prefetch_dropped_count']),
            'prefetch_filtered_count': int(row['prefetch_filtered_count']),
        }

rtl_data = {}
patterns = {
    'cnt_total_req': re.compile(r'cnt_total_req:\s*(\d+)'),
    'cnt_hit': re.compile(r'cnt_hit:\s*(\d+)'),
    'cnt_miss': re.compile(r'cnt_miss:\s*(\d+)'),
    'cnt_prefetch_issued': re.compile(r'cnt_prefetch_issued:\s*(\d+)'),
    'cnt_prefetch_hit': re.compile(r'cnt_prefetch_hit:\s*(\d+)'),
    'cnt_prefetch_useless': re.compile(r'cnt_prefetch_useless:\s*(\d+)'),
    'cnt_prefetch_dropped': re.compile(r'cnt_prefetch_dropped:\s*(\d+)'),
    'cnt_prefetch_filtered': re.compile(r'cnt_prefetch_filtered:\s*(\d+)'),
}

with open('rtl_run.log', 'r') as f:
    log_content = f.read()
    for key, pat in patterns.items():
        m = pat.search(log_content)
        if m:
            rtl_data[key] = int(m.group(1))

mapping = {
    'total_requests': 'cnt_total_req',
    'cache_hits': 'cnt_hit',
    'cache_misses': 'cnt_miss',
    'prefetch_issued_count': 'cnt_prefetch_issued',
    'prefetch_useful_count': 'cnt_prefetch_hit',
    'prefetch_useless_count': 'cnt_prefetch_useless',
    'prefetch_dropped_count': 'cnt_prefetch_dropped',
    'prefetch_filtered_count': 'cnt_prefetch_filtered',
}

print('=== Python vs RTL Counter Comparison ($repl) ===')
print('{:<30} | {:<8} | {:<8} | {:<6}'.format('Metric', 'Python', 'RTL', 'Status'))
print('-' * 60)

all_match = True
for py_field, rtl_field in mapping.items():
    py_val = csv_data.get(py_field, -1)
    rtl_val = rtl_data.get(rtl_field, -1)
    match = (py_val == rtl_val)
    if not match:
        all_match = False
    status = 'OK' if match else 'FAIL'
    print('{:<30} | {:<8} | {:<8} | {:<6}'.format(py_field, py_val, rtl_val, status))

print('-' * 60)
if all_match:
    print('COUNTER MATCH PASSED')
    sys.exit(0)
else:
    print('COUNTER MATCH FAILED')
    sys.exit(1)
"
    COMPARE_STATUS=$?
    if [ $COMPARE_STATUS -ne 0 ]; then
        echo "SMOKE TEST FAILED FOR POLICY: $repl"
        exit 1
    fi
done

echo "SMOKE TEST PASSED FOR BOTH FIFO AND LRU"
exit 0
