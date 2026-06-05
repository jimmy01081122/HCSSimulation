#!/bin/bash
# run_iverilog_tests.sh
# Compiles and runs all Icarus Verilog testbenches.

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR="$DIR/.."

cd "$PROJECT_DIR"

echo "Compiling cache tag array testbench..."
iverilog -o tb/tb_expert_cache_tag_array.vvp \
  rtl/expert_cache_tag_array.v \
  tb/tb_expert_cache_tag_array.v
if [ $? -ne 0 ]; then
    echo "FAIL: Compilation of tb_expert_cache_tag_array failed."
    exit 1
fi

echo "Compiling top prefetch system testbench..."
iverilog -o tb/tb_top_moe_prefetch_system.vvp \
  rtl/expert_cache_tag_array.v \
  rtl/fifo_replacement.v \
  rtl/simple_dma_model.v \
  rtl/top_moe_prefetch_system.v \
  tb/tb_top_moe_prefetch_system.v
if [ $? -ne 0 ]; then
    echo "FAIL: Compilation of tb_top_moe_prefetch_system failed."
    exit 1
fi

echo "Running Cache Tag Array unit test..."
OUTPUT_TAG=$(vvp tb/tb_expert_cache_tag_array.vvp)
echo "$OUTPUT_TAG"
if [[ ! "$OUTPUT_TAG" =~ "PASS tb_expert_cache_tag_array" ]]; then
    echo "FAIL: tb_expert_cache_tag_array test failed."
    exit 1
fi

echo "Running Top Prefetch System simulation..."
OUTPUT_TOP=$(vvp tb/tb_top_moe_prefetch_system.vvp)
echo "$OUTPUT_TOP"
if [[ ! "$OUTPUT_TOP" =~ "PASS tb_top_moe_prefetch_system" ]]; then
    echo "FAIL: tb_top_moe_prefetch_system test failed."
    exit 1
fi

echo "ALL RTL TESTS PASSED"
exit 0
