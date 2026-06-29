#!/bin/bash
# run_all_experiments.sh
# Runs all 5 DSE sweep experiments sequentially and triggers figure generation.

set -e

CONFIG_DIR="experiments/configs"
SCRIPT_DIR="experiments/scripts"
RESULT_DIR="experiments/results"

mkdir -p ${RESULT_DIR}

echo "=== Running Experiment 1: Cache Capacity Sweep ==="
python3 ${SCRIPT_DIR}/run_single_exp.py --config ${CONFIG_DIR}/exp1_cache_sweep.yaml

echo "=== Running Experiment 2: PCIe Bandwidth Sweep ==="
python3 ${SCRIPT_DIR}/run_single_exp.py --config ${CONFIG_DIR}/exp2_pcie_sweep.yaml

echo "=== Running Experiment 3: Concurrency Scaling Sweep ==="
python3 ${SCRIPT_DIR}/run_single_exp.py --config ${CONFIG_DIR}/exp3_concurrency.yaml

echo "=== Running Experiment 4: Zipf Alpha Sensitivity Sweep ==="
python3 ${SCRIPT_DIR}/run_single_exp.py --config ${CONFIG_DIR}/exp4_zipf_alpha.yaml

echo "=== Running Experiment 5: CPU Core Scaling Sweep ==="
python3 ${SCRIPT_DIR}/run_single_exp.py --config ${CONFIG_DIR}/exp5_cpu_cores.yaml

echo "=== DSE sweeps complete. Generating figures... ==="
python3 experiments/figures/generate_all_figures.py

echo "=== All DSE experiments and visualization complete ==="
