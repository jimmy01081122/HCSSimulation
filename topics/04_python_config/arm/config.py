"""
簡述: 使用底層 m5.objects API 手動配置一個 ARM SE 模式系統，不使用 gem5 標準函式庫。
用於幫助學生理解 SimObject、埠 (Port) 連接與時脈電壓網域的設定方式。
"""
import m5
from m5.objects import *

# 1. 建立 System 根節點
system = System()

# 2. 建立時脈網域與電壓網域
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '2GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# 3. 設定模擬記憶體範圍 (512MB) 與模式
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('512MB')]

# 4. 建立 CPU 核心 (使用 ARM 專屬 CPU)
system.cpu = ArmTimingSimpleCPU()

# 5. 建立系統匯流排 (System Crossbar)
system.membus = SystemXBar()

# TODO: 1. 手動連接 CPU 的指令埠與資料埠到匯流排的 CPU 側
system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = system.membus.cpu_side_ports

# 6. 手動接線 Interrupt Controller
system.cpu.createInterruptController()

# 7. 建立記憶體控制器與實體通道 (DDR3)
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]

# TODO: 2. 手動連接記憶體控制器的埠到匯流排的記憶體側
system.mem_ctrl.port = system.membus.mem_side_ports

# 8. 設定工作負載與進程
process = Process()
process.executable = "/work/gem5/tests/test-progs/hello/bin/arm/linux/hello"
process.cmd = [process.executable]
system.cpu.workload = process
system.cpu.createThreads()

system.workload = SEWorkload.init_compatible(process.executable)

# 9. 連接系統連接埠，以供除錯與內部存取
system.system_port = system.membus.cpu_side_ports

# 10. 建立 Root 物件
root = Root(full_system=False, system=system)

# TODO: 3. 呼叫低階模擬 API 進行系統實例化與開始模擬
m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()

print(f"Exited loop at tick {m5.curTick()} because {exit_event.getCause()}")
