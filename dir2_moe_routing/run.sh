#!/bin/bash

# 這個腳本會編譯包含 m5 ops 的 C++ 程式並執行模擬。

echo "正在編譯 MoE Routing 測試程式..."

# 注意：我們需要連結 gem5 的 libm5.a。
# 在 Ubuntu 24.04 容器中，預期路徑在 /work/gem5/util/m5/build/arm64/out/libm5.a
# 如果路徑不同，請根據容器內部的編譯結果調整。
g++ -static -I/work/gem5/include \
    src/moe_routing.cpp \
    /work/gem5/util/m5/build/arm64/out/libm5.a \
    -o src/moe_routing

mkdir -p m5out

echo "啟動 gem5 O3CPU 模擬..."
/work/gem5/build/ARM/gem5.opt -d m5out/ configs/routing_test.py src/moe_routing

echo "模擬完成！請查看 m5out/stats.txt 中的 work_item 相關數據。"
