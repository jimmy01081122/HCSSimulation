#!/bin/bash

# 這個腳本會自動化處理編碼問題，並執行模擬。

echo "正在編譯測試程式 src/dma_test.c..."
gcc -static src/dma_test.c -o src/dma_test

mkdir -p m5out

# 【解決方案】生成純淨版 Python 腳本以避開 gem5 編碼問題
# 移除所有含有中文字元的註解行，僅保留程式邏輯
echo "正在準備執行環境 (處理編碼相容性)..."
grep -v '#' configs/simple_dma.py > configs/simple_dma_exec.py

echo "啟動 gem5 模擬器..."
export LC_ALL=C.UTF-8
export PYTHONUTF8=1
/work/gem5-project/gem5/build/ARM/gem5.opt -d m5out/ configs/simple_dma_exec.py src/dma_test

echo "模擬完成！請查看 m5out/stats.txt 獲取詳細數據。"
