#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
GEM5=/work/gem5/build/ARM/gem5.opt
CONFIG=${SCRIPT_DIR}/config.py

echo "Starting Cache size sweeps on ARM..."
for l1 in 16KiB 32KiB 64KiB; do
    echo "Running with L1d/i = $l1"
    ${GEM5} --outdir=${SCRIPT_DIR}/m5out_L1_${l1} ${CONFIG} --l1d-size ${l1} --l1i-size ${l1} --l2-size 256KiB
done
