#!/bin/bash
set -e

# 定義路徑
GEM5_BIN="/home/a/gem5/build/ARM/gem5.opt"
SE_CONFIG="/home/a/gem5/configs/deprecated/example/se.py"
WORK_DIR="/work/advanced_projects/hetero_moe_scheduling"

echo "=== 1. 編譯 C++ 基準測試 (ARM64 靜態連結) ==="
aarch64-linux-gnu-g++ -static -O3 \
    -o ${WORK_DIR}/gem5_sim/cpu_sched_bench_arm \
    ${WORK_DIR}/gem5_sim/cpu_sched_bench.cpp

echo "=== 2. 執行 Centralized Global Queue 模擬 ==="
mkdir -p ${WORK_DIR}/reports/gem5_centralized
${GEM5_BIN} --outdir=${WORK_DIR}/reports/gem5_centralized ${SE_CONFIG} \
    --cpu-type=O3CPU \
    --caches --l2cache \
    --l1d_size=64kB --l1i_size=64kB --l2_size=512kB \
    --cmd=${WORK_DIR}/gem5_sim/cpu_sched_bench_arm \
    --options="central"

echo "=== 3. 執行 Distributed Expert-Affinity Queue 模擬 ==="
mkdir -p ${WORK_DIR}/reports/gem5_distributed
${GEM5_BIN} --outdir=${WORK_DIR}/reports/gem5_distributed ${SE_CONFIG} \
    --cpu-type=O3CPU \
    --caches --l2cache \
    --l1d_size=64kB --l1i_size=64kB --l2_size=512kB \
    --cmd=${WORK_DIR}/gem5_sim/cpu_sched_bench_arm \
    --options="dist"

echo "=== 4. 提取關鍵微架構數據 ==="
python3 ${WORK_DIR}/gem5_sim/parse_gem5_stats.py
