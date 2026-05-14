#!/bin/bash

# 這個腳本會編譯雙執行緒測試程式並執行雙核模擬。

echo "正在編譯 KV Cache 預取測試程式 (需連結 pthread)..."

g++ -static -I/work/gem5/include \
    src/kv_prefetch.cpp \
    /work/gem5/util/m5/build/arm64/out/libm5.a \
    -lpthread \
    -o src/kv_prefetch

mkdir -p m5out

echo "啟動 gem5 雙核 (Shared L2) 模擬..."
/work/gem5/build/ARM/gem5.opt -d m5out/ configs/dual_core_prefetch.py src/kv_prefetch

echo "模擬完成！請查看 m5out/stats.txt 觀察 cpu0 與 l2cache 的統計數據。"
