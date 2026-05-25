import m5
from m5.objects import *
import sys

# Define simple cache classes
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

system = System()
system.clk_domain = SrcClockDomain(clock = '2GHz', voltage_domain = VoltageDomain())
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('1GB')]

system.cpu = DerivO3CPU()

system.cpu.icache = L1ICache(size='32kB')
system.cpu.dcache = L1DCache(size='32kB')

system.cpu.icache_port = system.cpu.icache.cpu_side
system.cpu.dcache_port = system.cpu.dcache.cpu_side

system.l2bus = L2XBar()
system.cpu.icache.mem_side = system.l2bus.cpu_side_ports
system.cpu.dcache.mem_side = system.l2bus.cpu_side_ports

system.membus = SystemXBar()
system.l2bus.mem_side_ports = system.membus.cpu_side_ports

system.cpu.createInterruptController()
system.mem_ctrl = MemCtrl(dram = DDR4_2400_8x8(range = system.mem_ranges[0]))
system.mem_ctrl.port = system.membus.mem_side_ports
system.system_port = system.membus.cpu_side_ports

binary = sys.argv[1]
process = Process()
process.executable = binary
process.cmd = [binary]
system.workload = SEWorkload.init_compatible(binary)
system.cpu.workload = process
system.cpu.createThreads()

root = Root(full_system = False, system = system)
m5.instantiate()

print("Beginning MoE Routing simulation...")
exit_event = m5.simulate()
print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))
