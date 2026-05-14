# -*- coding: utf-8 -*-
# 這個檔案是 gem5 的系統配置腳本，用來定義我們的虛擬硬體架構。

import m5
from m5.objects import *

# 1. 建立系統實體 (System)
# 這是所有硬體設備的容器。
system = System()

# 設定時脈與電壓領域
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz' # 設定 CPU 頻率為 1GHz
system.clk_domain.voltage_domain = VoltageDomain()

# 2. 設定記憶體模式與範圍
system.mem_mode = 'timing' # 使用 timing 模式以獲得精確的延遲模擬
system.mem_ranges = [AddrRange('512MB')] # 設定實體記憶體大小為 512MB

# 3. 建立 CPU
# 使用 TimingSimpleCPU，它是一個簡單的順序執行 (In-order) 模型，適合新手學習。
system.cpu = TimingSimpleCPU()

# 4. 建立系統總線 (System Bus)
# 所有的數據交換都會經過這條總線。
system.membus = SystemXBar()

# 5. 連接 CPU 的埠 (Ports) 到總線
# CPU 有指令埠 (icache_port) 與數據埠 (dcache_port)。
system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = system.membus.cpu_side_ports

# 6. 建立中斷控制器 (Interrupt Controller)
# 雖然我們沒用到複雜中斷，但這是 CPU 啟動的基本組件。
system.cpu.createInterruptController()

# 7. 建立記憶體控制器 (Memory Controller)
# 這裡使用 DDR3 控制器。
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8() # 指定使用 DDR3-1600 記憶體
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

# 8. 設定 Process 並指定執行檔
# 這裡會讀取外部傳入的參數來決定執行哪個程式。
import sys
binary = sys.argv[1] # 從命令列取得執行檔路徑

process = Process()
process.executable = binary
process.cmd = [binary]
system.cpu.workload = process
system.cpu.createThreads()

# 9. 設定實體記憶體與系統的連接
system.system_port = system.membus.cpu_side_ports

# 10. 初始化與啟動模擬
root = Root(full_system = False, system = system)
m5.instantiate()

print("開始啟動 gem5 模擬...")
exit_event = m5.simulate()

print('模擬結束於 tick %i，原因: %s' % (m5.curTick(), exit_event.getCause()))
