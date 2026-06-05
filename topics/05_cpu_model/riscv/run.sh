#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
GEM5=/work/gem5/build/RISCV/gem5.opt
CONFIG=${SCRIPT_DIR}/config.py

echo "Starting CPU model sweeps on RISC-V..."
for model in atomic timing minor o3; do
    echo "============================================="
    echo "Running model: $model"
    echo "============================================="
    ${GEM5} --outdir=${SCRIPT_DIR}/m5out_${model} ${CONFIG} --cpu ${model}
done
