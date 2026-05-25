#!/bin/bash
set -e

echo "檢測系統..."

OS="$(uname)"

if [ "$OS" = "Darwin" ]; then
    echo "macOS 環境"

    echo "編譯 (x86/macOS)..."
    gcc -O2 src/dma_test.c -o src/dma_test

else
    echo "WSL / Linux 環境"

    echo "檢查 ARM cross compiler..."

    if ! command -v aarch64-linux-gnu-gcc >/dev/null 2>&1; then
        echo "錯誤：未安裝 aarch64-linux-gnu-gcc"
        echo "執行：sudo apt update && sudo apt install -y gcc-aarch64-linux-gnu"
        exit 1
    fi

    echo "編譯 (ARM64)..."

    aarch64-linux-gnu-gcc \
        -static \
        -O2 \
        -march=armv8-a \
        src/dma_test.c \
        -o src/dma_test

    echo "驗證 binary 架構:"
    file src/dma_test
fi

mkdir -p m5out

echo "啟動 gem5..."

export LC_ALL=C.UTF-8
export PYTHONUTF8=1

/work/gem5/build/ARM/gem5.opt \
    -d m5out \
    configs/simple_dma.py \
    src/dma_test

echo "完成 → m5out/stats.txt"