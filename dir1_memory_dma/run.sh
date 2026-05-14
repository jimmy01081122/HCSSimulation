#!/bin/bash

# 這個腳本會編譯並執行方向一的模擬。

echo "正在編譯測試程式 src/dma_test.c..."
gcc -static src/dma_test.c -o src/dma_test

mkdir -p m5out

echo "啟動 gem5 模擬器 (使用純英文配置檔以避免編碼錯誤)..."
# 注意：configs/simple_dma.py 已改為純英文。
# 詳細中文註解請參考 configs/simple_dma_tutorial.py。
export LC_ALL=C.UTF-8
export PYTHONUTF8=1
/work/gem5-project/gem5/build/ARM/gem5.opt -d m5out/ configs/simple_dma.py src/dma_test

echo "模擬完成！請查看 m5out/stats.txt 獲取詳細數據。"
