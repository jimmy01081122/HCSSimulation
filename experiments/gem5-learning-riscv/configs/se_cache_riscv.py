#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
RISC-V gem5 SE Mode - Classic Cache Configuration Example
檔案功能簡述: 
  示範如何在 RISC-V gem5 中建立具有經典快取層級的系統
  學習目標:
    - 理解快取階層的配置方式
    - 觀察 TIMING CPU 與快取的互動
    - 分析不同快取大小對效能的影響
    - 比較 ARM 與 RISC-V 快取效能差異

使用方式:
  gem5.opt --outdir=results/se_cache configs/se_cache_riscv.py

TODO 任務:
  1. 修改 L1 快取大小 (目前: 32KB) 並觀察效能變化
  2. 修改 L2 快取大小 (目前: 256KB) 並測試
  3. 使用 parse_stats_riscv.py 比較結果
  4. 與 ARM 版本的結果進行對比
"""

import m5
from m5.objects import *

# TODO: 導入 gem5 standard library
from gem5.utils.requires import requires
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.memory.memory import Memory
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.resources.resource import Resource, obtain_resource
from gem5.isas import ISA
from gem5.coherence_protocol import CoherenceProtocol

def create_riscv_cache_system():
    """
    建立 RISC-V SE mode 系統,包含經典快取架構
    
    快取配置:
      L1 指令快取: 32 KB
      L1 資料快取: 32 KB  
      L2 統一快取: 256 KB
    """
    
    # TODO: 完成快取配置
    # 提示: RISC-V 的快取配置與 ARM 類似
    # 參考: https://gem5.org/documentation/general_docs/memory_system/caches/
    
    # 建立 L1 指令快取 (32 KB)
    l1i_cache = Cache()
    l1i_cache.size = '32KB'
    l1i_cache.assoc = 2
    # TODO: 設定其他 L1 指令快取參數
    
    # 建立 L1 資料快取 (32 KB)
    l1d_cache = Cache()
    l1d_cache.size = '32KB'
    l1d_cache.assoc = 2
    # TODO: 設定其他 L1 資料快取參數
    
    # 建立 L2 快取 (256 KB)
    l2_cache = Cache()
    l2_cache.size = '256KB'
    l2_cache.assoc = 8
    # TODO: 設定其他 L2 快取參數
    
    return l1i_cache, l1d_cache, l2_cache

def main():
    """
    主程式: 建立完整的 gem5 RISC-V 系統
    """
    
    # TODO: 步驟 1 - 建立 SimpleBoard (完成)
    board = SimpleBoard(
        clk_freq="3GHz",
        processor=SimpleProcessor(
            cpu_type=CPUTypes.TIMING,  # 使用 TIMING CPU 以觀察快取互動
            isa=ISA.RISCV,
            num_cores=1
        ),
        memory=Memory(
            addr_range="0GB:2GB",
            size="2GB"
        ),
    )
    
    # TODO: 步驟 2 - 整合快取架構
    # l1i, l1d, l2 = create_riscv_cache_system()
    # 提示: 需要將快取連接到 CPU 與系統匯流排
    
    # TODO: 步驟 3 - 載入並執行程式
    # 提示: 使用 obtain_resource() 取得預編譯的 RISC-V 程式
    # 範例: riscv_binary = obtain_resource("riscv-ubuntu-20.04-hello")
    
    # TODO: 步驟 4 - 建立執行環境並執行模擬
    # 提示: 設定模擬選項, 執行程式, 輸出統計資訊
    
    # 建立根物件
    root = Root(full_system=False, system=board)
    
    # 建立模擬環境
    m5.instantiate()
    
    # 執行模擬
    exit_event = m5.simulate()
    
    print(f"Simulation finished at cycle {m5.curTick()}")
    print(f"Exit reason: {exit_event.getCause()}")

if __name__ == "__main__":
    main()
