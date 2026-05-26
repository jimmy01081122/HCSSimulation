#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
RISC-V gem5 SE Mode - CPU Model Comparison Configuration
檔案功能簡述: 
  示範如何使用參數化配置進行不同 CPU 模型的比較
  學習目標:
    - 理解不同 CPU 模型的差異 (ATOMIC, TIMING, O3)
    - 學習如何進行設計空間探索 (DSE)
    - 分析 CPU 型號對效能的影響
    - 使用命令列參數進行配置變更

使用方式:
  gem5.opt --outdir=results/cpu_atomic configs/se_cpu_select_riscv.py --cpu=ATOMIC
  gem5.opt --outdir=results/cpu_timing configs/se_cpu_select_riscv.py --cpu=TIMING
  gem5.opt --outdir=results/cpu_o3 configs/se_cpu_select_riscv.py --cpu=O3

TODO 任務:
  1. 實現 --cpu 命令列參數解析
  2. 根據參數選擇 CPU 型號
  3. 運行三種 CPU 型號並收集統計
  4. 使用指令碼批量運行並比較結果
"""

import m5
from m5.objects import *
import argparse

# TODO: 導入 gem5 standard library
from gem5.utils.requires import requires
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.memory.memory import Memory
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.resources.resource import Resource, obtain_resource
from gem5.isas import ISA

def parse_arguments():
    """
    解析命令列參數
    
    TODO: 完成參數解析
    提示: 添加 --cpu 參數以選擇 CPU 型號 (ATOMIC, TIMING, O3)
    """
    parser = argparse.ArgumentParser(description="RISC-V CPU Comparison Config")
    
    # TODO: 添加 --cpu 參數
    # parser.add_argument('--cpu', choices=['ATOMIC', 'TIMING', 'O3'], default='TIMING',
    #                     help='CPU model to use')
    
    # TODO: 可選: 添加其他參數
    # parser.add_argument('--freq', default='3GHz', help='CPU frequency')
    # parser.add_argument('--cores', type=int, default=1, help='Number of cores')
    
    return parser.parse_args()

def get_cpu_type(cpu_name):
    """
    根據名稱取得對應的 CPU 型號
    
    TODO: 完成 CPU 型號對應
    """
    cpu_map = {
        'ATOMIC': CPUTypes.ATOMIC,
        'TIMING': CPUTypes.TIMING,
        'O3': CPUTypes.O3,
        # TODO: 添加其他 CPU 型號
    }
    
    if cpu_name not in cpu_map:
        raise ValueError(f"Unknown CPU type: {cpu_name}")
    
    return cpu_map[cpu_name]

def main():
    """
    主程式: 建立可配置的 gem5 RISC-V 系統
    """
    
    # TODO: 步驟 1 - 解析命令列參數
    args = parse_arguments()
    print(f"Using CPU model: {args.cpu if hasattr(args, 'cpu') else 'TIMING (default)'}")
    
    # TODO: 步驟 2 - 選擇 CPU 型號
    # cpu_type = get_cpu_type(args.cpu) if hasattr(args, 'cpu') else CPUTypes.TIMING
    
    # 建立 SimpleBoard 系統
    board = SimpleBoard(
        clk_freq="3GHz",
        processor=SimpleProcessor(
            cpu_type=CPUTypes.TIMING,  # TODO: 使用解析得到的 CPU 型號
            isa=ISA.RISCV,
            num_cores=1
        ),
        memory=Memory(
            addr_range="0GB:2GB",
            size="2GB"
        ),
    )
    
    # TODO: 步驟 3 - 載入程式
    # 提示: 根據 CPU 型號選擇合適的工作負載
    # riscv_binary = obtain_resource("riscv-ubuntu-20.04-hello")
    
    # TODO: 步驟 4 - 建立執行環境
    root = Root(full_system=False, system=board)
    
    # 建立模擬環境
    m5.instantiate()
    
    # 執行模擬
    exit_event = m5.simulate()
    
    print(f"Simulation finished at cycle {m5.curTick()}")
    print(f"Exit reason: {exit_event.getCause()}")
    print(f"CPU model: TIMING (TODO: update to use actual CPU model)")

if __name__ == "__main__":
    main()
