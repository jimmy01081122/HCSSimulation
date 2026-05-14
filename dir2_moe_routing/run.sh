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

echo "啟動 gem5 O3CPU 模擬 (包含管線追蹤)..."
# --debug-flags=O3PipeView : 啟動管線追蹤標籤
# --debug-file=trace.out   : 將追蹤結果存至 m5out/trace.out
/work/gem5/build/ARM/gem5.opt -d m5out/ \
    --debug-flags=O3PipeView \
    --debug-file=trace.out \
    configs/routing_test.py src/moe_routing

echo "正在轉換管線追蹤檔為可讀格式 (pipeview.out)..."
# 使用 gem5 內建工具將二進位或原始追蹤文字轉換為視覺化矩陣格式
# -c 500 表示轉換前 500 個指令，避免輸出檔過大
python3 /work/gem5/util/o3-pipeview.py -c 500 -o m5out/pipeview.out m5out/trace.out

echo "模擬與追蹤轉換完成！"
echo "拓樸圖: m5out/config.dot.pdf"
echo "管線圖: m5out/pipeview.out"
