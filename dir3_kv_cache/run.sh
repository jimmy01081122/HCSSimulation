#!/bin/bash

echo "正在編譯 KV Cache 預取測試程式..."
g++ -static -I/work/gem5-project/gem5/include \
    src/kv_prefetch.cpp \
    /work/gem5-project/gem5/util/m5/build/arm64/out/libm5.a \
    -lpthread \
    -o src/kv_prefetch

mkdir -p m5out

# 【解決方案】生成純淨版 Python 腳本
grep -v '#' configs/dual_core_prefetch.py > configs/dual_core_prefetch_exec.py

echo "啟動 gem5 雙核 (Shared L2) 模擬..."
export LC_ALL=C.UTF-8
export PYTHONUTF8=1
/work/gem5-project/gem5/build/ARM/gem5.opt -d m5out/ configs/dual_core_prefetch_exec.py src/kv_prefetch

echo "模擬完成！"
