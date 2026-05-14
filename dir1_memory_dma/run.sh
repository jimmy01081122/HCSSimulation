#!/bin/bash

# 這個腳本會自動化編譯與執行模擬的過程。

# 1. 編譯測試程式
# 使用 -static 是關鍵，因為 gem5 的 SE 模式無法輕易找到容器內的動態連結庫。
echo "正在編譯測試程式 src/dma_test.c..."
gcc -static src/dma_test.c -o src/dma_test

# 2. 建立輸出目錄
mkdir -p m5out

# 3. 執行 gem5 模擬
# 我們指定輸出目錄為本資料夾下的 m5out，方便查看 stats.txt。
echo "啟動 gem5 模擬器..."
/work/gem5/build/ARM/gem5.opt -d m5out/ configs/simple_dma.py src/dma_test

echo "模擬完成！請查看 m5out/stats.txt 獲取詳細數據。"
