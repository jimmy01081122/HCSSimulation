#!/bin/bash
# 執行 RISC-V FS 模擬
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
GEM5=/work/gem5/build/RISCV/gem5.opt
CONFIG=${SCRIPT_DIR}/config.py
OUTDIR=${SCRIPT_DIR}/m5out

echo "Running gem5 RISC-V FS Mode simulation..."
echo "Notice: FS mode requires internet access to download resources and may run for a very long time."
if [ ! -f "${GEM5}" ]; then
    echo "Error: ${GEM5} not found. Please build gem5 first."
    exit 1
fi
# 限制最高 ticks 避免模擬過長
${GEM5} --outdir=${OUTDIR} ${CONFIG} --max-ticks 10000000000
