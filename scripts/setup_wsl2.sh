#!/bin/bash

# 這個腳本用於在 WSL2 或任何已安裝 Docker 的 Linux 環境中初始化 gem5 開發環境。

echo "--- HCSSimulation 環境初始化 (WSL2/Docker) ---"

# 1. 檢查 Docker 是否運行
if ! docker info > /dev/null 2>&1; then
    echo "錯誤: Docker 並未運行。請啟動 Docker Desktop for Windows 並確認已開啟 WSL2 整合。"
    exit 1
fi

# 2. 建構 Docker 映像檔
echo "正在建構 gem5-env 映像檔 (這可能需要幾分鐘)..."
docker build -t gem5-env .

# 3. 提示使用者下載並編譯 gem5
echo "--- 初始化完成 ---"
echo "接下來請執行以下步驟來進入開發環境並編譯 gem5 (僅需執行一次):"
echo "1. 啟動容器: docker run --rm -it -v \$(pwd):/work gem5-env"
echo "2. 在容器內下載 gem5: git clone https://github.com/gem5/gem5"
echo "3. 編譯 gem5 (建議分配足夠記憶體): cd gem5 && scons build/ARM/gem5.opt -j\$(nproc)"
