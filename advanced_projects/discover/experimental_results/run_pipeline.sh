#!/bin/bash
# ==============================================================================
# MoE-LLM Co-Design 實驗運行管線與硬體驗證指令集 (run_pipeline.sh)
# 使用指引：本腳本整理了本研究所提之模擬與驗證全流程指令，附帶詳細參數註解。
# ==============================================================================

set -e # 遇錯即停

echo "======================================================================"
echo "          LLM/MoE Co-Design 實驗管線一鍵運行與驗證工具集               "
echo "======================================================================"

# ------------------------------------------------------------------------------
# 準備工作：定義路徑與確認工具鏈
# ------------------------------------------------------------------------------
BASE_DIR=$(pwd)
echo "目前工作路徑: $BASE_DIR"

# 檢查相依工具鏈是否安裝
command -v python3 >/dev/null 2>&1 || { echo "錯誤: 系統未安裝 python3"; exit 1; }
command -v iverilog >/dev/null 2>&1 || { echo "警告: 系統未安裝 iverilog (RTL 編譯將被跳過)"; }
command -v yosys >/dev/null 2>&1 || { echo "警告: 系統未安裝 yosys (RTL 綜合將被跳過)"; }

# ------------------------------------------------------------------------------
# 第一階段：運行 Qwen-MoE 模型分析與聯合調度模擬
# ------------------------------------------------------------------------------
echo -e "
>>> [階段一] 運行 Qwen-MoE 模型分析與聯合調度模擬..."

# 1. 運行路由軌跡分析 (Gini 係數、時間重用距離、相鄰步重疊率統計)
echo "運行 trace_analyzer.py..."
python3 qwen_moe/routing/trace_analyzer.py \
    --trace_file qwen_moe/profiling/qwen_moe_routing_trace.csv \
    --output_dir qwen_moe/routing

# 2. 運行專家加載緩存模擬 (包含 LRU, LFU, 靜態熱點緩存, 跨層親和預取緩存)
echo "運行 expert_cache_sim.py..."
python3 qwen_moe/expert_cache/expert_cache_sim.py \
    --trace_file qwen_moe/profiling/qwen_moe_routing_trace.csv \
    --output_dir qwen_moe/expert_cache

# 3. 運行協同記憶體調度消融模擬 (模擬 PCIe 頻寬爭用、注意力計算 Slack 與丟棄預取)
echo "運行 joint_scheduler_sim.py..."
python3 qwen_moe/joint_scheduler/joint_scheduler_sim.py \
    --trace_file qwen_moe/profiling/qwen_moe_routing_trace.csv \
    --output_dir qwen_moe/joint_scheduler

# ------------------------------------------------------------------------------
# 第二階段：運行 Phi-2 KV Cache 壓縮模擬
# ------------------------------------------------------------------------------
echo -e "
>>> [階段二] 運行 Phi-2 KV Cache 壓縮與頁面回收模擬..."

# 運行 KV Cache 模擬器，設定為 Phi-2 結構維度 (32層, 32頭, 80維度, batch 4, seq 640)
echo "運行 kv_cache_sim.py..."
python3 phi2_kv/kv_cache/kv_cache_sim.py \
    --batch_size 4 \
    --prompt_len 512 \
    --gen_len 128 \
    --num_layers 32 \
    --num_heads 32 \
    --head_dim 80 \
    --memory_budget_mb 256.0 \
    --output_dir phi2_kv/kv_cache

# ------------------------------------------------------------------------------
# 第三階段：運行 RTL 硬體加速器前端仿真與 Yosys 綜合
# ------------------------------------------------------------------------------
echo -e "
>>> [階段三] 運行 RTL 硬體加速器前端仿真與週期驗證..."

if command -v iverilog >/dev/null 2>&1; then
    # 1. 使用 Icarus Verilog 編譯 SystemVerilog 設計檔與測試平台
    echo "正在編譯 SystemVerilog 硬體前端設計..."
    iverilog -g2012 -o qwen_moe/rtl/moe_sim_proposed \
        qwen_moe/rtl/moe_accelerator_frontend_proposed.sv \
        qwen_moe/rtl/tb_moe_accelerator_proposed.sv
        
    # 2. 執行硬體模擬生成週期日誌 rtl_cycle_log.txt 與 sim_trace_proposed.txt
    echo "執行 vvp 硬體波形仿真..."
    vvp qwen_moe/rtl/moe_sim_proposed
    
    # 3. 運行 Python 黃金模型與 RTL 週期日誌對齊校驗
    echo "運行 verify_rtl_proposed.py 完成週期級對齊校驗..."
    python3 qwen_moe/rtl/verify_rtl_proposed.py
else
    echo "跳過 RTL 編譯與仿真 (未偵測到 iverilog)。"
fi

if command -v yosys >/dev/null 2>&1; then
    # 4. 運行 Yosys 進行閘級邏輯電路綜合，評估片上觸發器與邏輯閘個數
    echo "正在執行 Yosys 邏輯綜合..."
    yosys -s qwen_moe/rtl/synth_proposed.ys > qwen_moe/rtl/synth_proposed.log
    echo "綜合完成，報告儲存於 qwen_moe/rtl/synth_proposed.log"
else
    echo "跳過 Yosys 閘級綜合 (未偵測到 yosys)。"
fi

echo -e "
======================================================================"
echo "         全部模擬與硬體驗證流程運行完畢！結果儲存於對應子資料夾內        "
echo "======================================================================"
