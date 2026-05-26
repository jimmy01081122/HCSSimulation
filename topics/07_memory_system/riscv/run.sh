#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
GEM5=/work/gem5/build/RISCV/gem5.opt
CONFIG=${SCRIPT_DIR}/config.py

echo "Starting memory sweeps on RISC-V..."
for gen in DDR3 DDR4 DDR5; do
    echo "Running DDR type: $gen"
    ${GEM5} --outdir=${SCRIPT_DIR}/m5out_${gen} ${CONFIG} --mem-type ${gen}
done
