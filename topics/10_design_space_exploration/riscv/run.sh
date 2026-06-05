#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
GEM5=/work/gem5/build/RISCV/gem5.opt
CONFIG=${SCRIPT_DIR}/config.py

echo "Starting Design Space Exploration parameter sweeps on RISC-V..."
# 定義欲掃描的硬體組態空間
for cpu in timing minor; do
    for l1 in 16KiB 32KiB; do
        for l2 in 128KiB 256KiB; do
            OUTDIR=${SCRIPT_DIR}/m5out_${cpu}_${l1}_${l2}
            echo "Running config: CPU=${cpu}, L1=${l1}, L2=${l2}"
            ${GEM5} --outdir=${OUTDIR} ${CONFIG} --cpu ${cpu} --l1-size ${l1} --l2-size ${l2}
        done
    done
done

echo "Aggregating DSE results..."
cd ${SCRIPT_DIR}
python3 collect_results.py
