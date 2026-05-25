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

system = System()
system.clk_domain = SrcClockDomain(clock = '1GHz', voltage_domain = VoltageDomain())
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('512MB')]

system.cpu = [TimingSimpleCPU(cpu_id=i) for i in range(2)]

system.l2bus = L2XBar()

for cpu in system.cpu:
    cpu.icache = L1ICache(size='32kB')
    cpu.dcache = L1DCache(size='32kB')
    
    cpu.icache_port = cpu.icache.cpu_side
    cpu.dcache_port = cpu.dcache.cpu_side
    
    cpu.icache.mem_side = system.l2bus.cpu_side_ports
    cpu.dcache.mem_side = system.l2bus.cpu_side_ports
    
    cpu.createInterruptController()

system.l2cache = L2Cache(size='256kB')
system.l2cache.cpu_side = system.l2bus.mem_side_ports

system.membus = SystemXBar()
system.l2cache.mem_side = system.membus.cpu_side_ports

system.mem_ctrl = MemCtrl(dram = DDR3_1600_8x8(range = system.mem_ranges[0]))
system.mem_ctrl.port = system.membus.mem_side_ports
system.system_port = system.membus.cpu_side_ports

binary = sys.argv[1]
process = Process()
process.executable = binary
process.cmd = [binary]
system.workload = SEWorkload.init_compatible(binary)
for cpu in system.cpu:
    cpu.workload = process
    cpu.createThreads()

root = Root(full_system = False, system = system)
m5.instantiate()

print("Beginning Dual-Core KV Cache simulation...")
exit_event = m5.simulate()
print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))
