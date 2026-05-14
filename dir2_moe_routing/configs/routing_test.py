# -*- coding: utf-8 -*-
import m5
from m5.objects import *
import sys

# 1. 建立系統
system = System()
system.clk_domain = SrcClockDomain(clock = '2GHz', voltage_domain = VoltageDomain())
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('1GB')]

# 2. 建立 O3 (Out-of-Order) CPU
# O3CPU 模擬了分支預測、指令重排序等複雜行為，適合觀察計算密集型邏輯。
system.cpu = DerivO3CPU()

# 3. 配置 Cache (為了簡化，我們先直接連總線，但 O3CPU 通常搭配 Cache 效果更佳)
# 這裡建立一個簡單的 L1 Cache 來支援 O3CPU 的高效能。
system.cpu.icache = L1ICache(size='32kB')
system.cpu.dcache = L1DCache(size='32kB')

# 連接 Cache 到 CPU
system.cpu.icache_port = system.cpu.icache.cpu_side
system.cpu.dcache_port = system.cpu.dcache.cpu_side

# 建立 L2 Bus
system.l2bus = L2XBar()

# 連接 L1 Cache 到 L2 Bus
system.cpu.icache.mem_side = system.l2bus.cpu_side_ports
system.cpu.dcache.mem_side = system.l2bus.cpu_side_ports

# 建立系統總線
system.membus = SystemXBar()

# 連接 L2 Bus 到系統總線
system.l2bus.mem_side = system.membus.cpu_side_ports

# 4. 其他基礎設定
system.cpu.createInterruptController()
system.mem_ctrl = MemCtrl(dram = DDR4_2400_8x8(range = system.mem_ranges[0]))
system.mem_ctrl.port = system.membus.mem_side_ports
system.system_port = system.membus.cpu_side_ports

# 5. 設定 Workload
binary = sys.argv[1]
process = Process()
process.executable = binary
process.cmd = [binary]
system.cpu.workload = process
system.cpu.createThreads()

# 6. 啟動
root = Root(full_system = False, system = system)
m5.instantiate()

print("開始啟動 MoE Routing 模擬...")
exit_event = m5.simulate()
print('模擬結束於 tick %i，原因: %s' % (m5.curTick(), exit_event.getCause()))
