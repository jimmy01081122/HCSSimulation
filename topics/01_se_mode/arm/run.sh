#!/bin/bash
# 執行 ARM SE 模式模擬
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
GEM5=/work/gem5/build/ARM/gem5.opt
CONFIG=${SCRIPT_DIR}/config.py
OUTDIR=${SCRIPT_DIR}/m5out

echo "Running gem5 ARM SE Mode simulation..."
if [ ! -f "${GEM5}" ]; then
    echo "Error: ${GEM5} not found. Please build gem5 first."
    exit 1
fi

${GEM5} --outdir=${OUTDIR} ${CONFIG}
echo "ARM Simulation completed. Results in ${OUTDIR}/"
