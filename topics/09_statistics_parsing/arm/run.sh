#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
GEM5=/work/gem5/build/ARM/gem5.opt
CONFIG=${SCRIPT_DIR}/config.py
OUTDIR=${SCRIPT_DIR}/m5out
PARSER=${SCRIPT_DIR}/parse_stats.py

echo "Running simulation to generate stats..."
if [ ! -f "${GEM5}" ]; then
    echo "Error: ${GEM5} not found. Please build gem5 first."
    exit 1
fi

${GEM5} --outdir=${OUTDIR} ${CONFIG}

echo "Invoking Python statistics parser..."
python3 ${PARSER} ${OUTDIR}/stats.txt
