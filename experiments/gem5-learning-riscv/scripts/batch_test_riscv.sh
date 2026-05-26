#!/bin/bash
# Batch Testing Script for gem5 RISC-V Experiments
# 用途: 批量執行 RISC-V gem5 實驗並收集結果
# 使用: ./batch_test_riscv.sh

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Configuration
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/../.." && pwd )"
GEM5_DIR="$PROJECT_ROOT/gem5"
EXPERIMENT_DIR="$SCRIPT_DIR"

log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_section() {
    echo -e "\n${BLUE}========== $1 ==========${NC}\n"
}

# TODO: 實現測試任務
run_hello_test() {
    log_section "執行 Hello World 測試"
    
    local output_dir="$EXPERIMENT_DIR/results/hello"
    mkdir -p "$output_dir"
    
    cd "$GEM5_DIR"
    ./build/RISCV/gem5.opt \
        --outdir="$output_dir" \
        "$EXPERIMENT_DIR/configs/se_hello_riscv.py"
    
    log_info "Hello World 測試完成"
}

# TODO: 實現快取測試
run_cache_test() {
    log_section "執行快取大小比較測試"
    
    # TODO: 添加快取測試邏輯
    # 1. 執行 32KB L1 + 256KB L2 基準測試
    # 2. 執行 64KB L1 + 256KB L2 測試
    # 3. 執行 32KB L1 + 512KB L2 測試
    # 4. 收集統計資訊
    
    log_info "快取測試完成 (TODO: 實現此功能)"
}

# TODO: 實現 CPU 模型比較測試
run_cpu_comparison_test() {
    log_section "執行 CPU 模型比較測試"
    
    # TODO: 添加 CPU 型號比較邏輯
    # 1. ATOMIC CPU 測試
    # 2. TIMING CPU 測試
    # 3. O3 CPU 測試
    # 4. 統計結果並生成報告
    
    log_info "CPU 模型比較測試完成 (TODO: 實現此功能)"
}

# TODO: 實現結果分析
analyze_results() {
    log_section "分析測試結果"
    
    # TODO: 添加結果分析邏輯
    # 1. 讀取統計文件
    # 2. 提取關鍵指標
    # 3. 生成比較報告
    
    log_info "結果分析完成 (TODO: 實現此功能)"
}

# Main execution
main() {
    log_info "開始執行 RISC-V gem5 批量測試"
    
    run_hello_test
    run_cache_test
    run_cpu_comparison_test
    analyze_results
    
    log_info "所有測試完成!"
    echo ""
    echo "結果位置: $EXPERIMENT_DIR/results/"
    echo "實驗筆記: $EXPERIMENT_DIR/notes/experiment_log.md"
}

main "$@"
