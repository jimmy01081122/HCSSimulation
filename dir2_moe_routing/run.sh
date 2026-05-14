#!/bin/bash

echo "正在編譯 MoE Routing 測試程式..."
g++ -static -mbranch-protection=none -I/work/gem5-project/gem5/include \
    src/moe_routing.cpp \
    /work/gem5-project/gem5/util/m5/build/arm64/out/libm5.a \
    -o src/moe_routing

mkdir -p m5out

echo "啟動 gem5 O3CPU 模擬 (包含管線追蹤)..."
export LC_ALL=C.UTF-8
export PYTHONUTF8=1
/work/gem5-project/gem5/build/ARM/gem5.opt -d m5out/ \
    --debug-flags=O3PipeView \
    --debug-file=trace.out \
    configs/routing_test.py src/moe_routing

echo "正在轉換管線追蹤檔為可讀格式 (pipeview.out)..."
python3 /work/gem5-project/gem5/util/o3-pipeview.py -c 500 -o m5out/pipeview.out m5out/trace.out

echo "模擬與追蹤轉換完成！"
