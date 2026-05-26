#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
GEM5=/work/gem5/build/RISCV/gem5.opt
CONFIG=${SCRIPT_DIR}/config.py

echo "Checking gem5 Ruby compilation..."
if [ ! -f "${GEM5}" ]; then
    echo "Error: ${GEM5} not found. Please build gem5 first."
    exit 1
fi

echo "Running Ruby Cache Coherence simulation..."
echo "Notice: This requires gem5 compiled with PROTOCOL=MESI_Two_Level. If it fails, compile gem5 via scons with PROTOCOL=MESI_Two_Level."
${GEM5} --outdir=${SCRIPT_DIR}/m5out ${CONFIG}
