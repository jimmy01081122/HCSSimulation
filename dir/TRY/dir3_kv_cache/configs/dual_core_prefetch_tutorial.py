# -*- coding: utf-8 -*-
import m5
from m5.objects import *
import sys

class L1Cache(Cache):
    assoc = 2
    tag_latency = 2
    data_latency = 2
    response_latency = 2
    mshrs = 4
    tgts_per_mshr = 20

class L1ICache(L1Cache):
    is_read_only = True

class L1DCache(L1Cache):
    pass

class L2Cache(Cache):
    assoc = 8
    tag_latency = 20
    data_latency = 20
    response_latency = 20
    mshrs = 20
    tgts_per_mshr = 12

# 1. 建立系統
system = System()
system.clk_domain = SrcClockDomain(clock = '1GHz', voltage_domain = VoltageDomain())
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('512MB')]

# 2. 建立雙核 CPU (TimingSimpleCPU)
# 我們需要兩個核心來模擬「計算」與「預取」的並行動作。
system.cpu = [TimingSimpleCPU(cpu_id=i) for i in range(2)]

# 3. 建立快取階層 (Cache Hierarchy)
# 建立一個共享的 L2 Bus
system.l2bus = L2XBar()

# 為每個核心建立私有的 L1 Cache
for cpu in system.cpu:
    cpu.icache = L1ICache(size='32kB')
    cpu.dcache = L1DCache(size='32kB')
    
    # 連接 L1 到 CPU
    cpu.icache_port = cpu.icache.cpu_side
    cpu.dcache_port = cpu.dcache.cpu_side
    
    # 連接 L1 到共享的 L2 Bus
    cpu.icache.mem_side = system.l2bus.cpu_side_ports
    cpu.dcache.mem_side = system.l2bus.cpu_side_ports
    
    cpu.createInterruptController()

# 建立共享的 L2 Cache
system.l2cache = L2Cache(size='256kB')
system.l2cache.cpu_side = system.l2bus.mem_side_ports

# 建立系統總線 (連結 L2 到 記憶體)
system.membus = SystemXBar()
system.l2cache.mem_side = system.membus.cpu_side_ports

# 4. 記憶體控制
system.mem_ctrl = MemCtrl(dram = DDR3_1600_8x8(range = system.mem_ranges[0]))
system.mem_ctrl.port = system.membus.mem_side_ports
system.system_port = system.membus.cpu_side_ports

# 5. 設定 Workload (雙核心在 SE 模式下共用同一個二進位檔，內部透過 pthread 區分任務)
binary = sys.argv[1]
process = Process()
process.executable = binary
process.cmd = [binary]
system.workload = SEWorkload.init_compatible(binary)
for cpu in system.cpu:
    cpu.workload = process
    cpu.createThreads()

# 6. 啟動模擬
root = Root(full_system = False, system = system)
m5.instantiate()

print("啟動雙核 KV Cache 模擬...")
exit_event = m5.simulate()
print('模擬結束於 tick %i，原因: %s' % (m5.curTick(), exit_event.getCause()))
