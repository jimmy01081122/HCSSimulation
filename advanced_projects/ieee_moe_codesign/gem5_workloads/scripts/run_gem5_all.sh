#!/bin/bash
# run_gem5_all.sh
# Automates the execution of all queue benchmarks under different cache configurations.
# Calls gem5's se.py directly with O3CPU and cache parameters.

set -e

GEM5_BIN="/gem5/build/ARM/gem5.opt"
SE_CONFIG="/gem5/configs/deprecated/example/se.py"
RESULT_DIR="experiments/results/gem5"

mkdir -p ${RESULT_DIR}

L1D_SIZES=("32kB" "64kB" "128kB")
L2_SIZES=("256kB" "512kB" "1MB")
QUEUES=("centralized" "distributed" "lock_free")

echo "Starting gem5 Microarchitecture Sweep..."

for l1d in "${L1D_SIZES[@]}"; do
    for l2 in "${L2_SIZES[@]}"; do
        for queue in "${QUEUES[@]}"; do
            OUT_DIR="${RESULT_DIR}/l1d_${l1d}_l2_${l2}_${queue}"
            mkdir -p "${OUT_DIR}"

            BIN="gem5_workloads/src/${queue}_arm"

            echo "Running: L1D=${l1d}, L2=${l2}, Queue=${queue}"

            # Run gem5 with se.py directly
            ${GEM5_BIN} --outdir="${OUT_DIR}" "${SE_CONFIG}" \
                --cpu-type=TimingSimpleCPU \
                --caches \
                --l2cache \
                --l1d_size="${l1d}" \
                --l1i_size=64kB \
                --l2_size="${l2}" \
                --cmd="${BIN}"
        done
    done
done

echo "gem5 Sweep Complete. Parsing statistics..."
python3 gem5_workloads/scripts/parse_stats.py \
    --input-dir "${RESULT_DIR}" \
    --output "${RESULT_DIR}/gem5_summary.csv"
