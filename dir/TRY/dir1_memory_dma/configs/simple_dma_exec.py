
import m5
from m5.objects import *

system = System()

system.clk_domain = SrcClockDomain()
system.clk_domain.voltage_domain = VoltageDomain()


system.cpu = TimingSimpleCPU()

system.membus = SystemXBar()

system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = system.membus.cpu_side_ports

system.cpu.createInterruptController()

system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

import sys

process = Process()
process.executable = binary
process.cmd = [binary]
system.cpu.workload = process
system.cpu.createThreads()

system.system_port = system.membus.cpu_side_ports

root = Root(full_system = False, system = system)
m5.instantiate()

print("開始啟動 gem5 模擬...")
exit_event = m5.simulate()

print('模擬結束於 tick %i，原因: %s' % (m5.curTick(), exit_event.getCause()))
