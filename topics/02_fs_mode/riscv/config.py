"""
簡述: 使用 gem5 標準函式庫建置一個 RISC-V FS 模式全系統模擬組態。
注意: FS 模式模擬需要下載核心與磁碟映像檔，且執行時間長。
"""
from gem5.components.boards.riscv_board import RiscvBoard
from gem5.components.cachehierarchies.classic.private_l1_private_l2_cache_hierarchy import PrivateL1PrivateL2CacheHierarchy
from gem5.components.memory import DualChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import obtain_resource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# TODO: 1. 宣告所需 ISA 為 RISCV
requires(isa_required=ISA.RISCV)

# TODO: 2. 建立快取階層
cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1d_size="32KiB",
    l1i_size="32KiB",
    l2_size="256KiB"
)

# TODO: 3. 建立 2GiB 記憶體系統
memory = DualChannelDDR3_1600(size="2GiB")

# TODO: 4. 建立單核心 TIMING 處理器
processor = SimpleProcessor(cpu_type=CPUTypes.TIMING, isa=ISA.RISCV, num_cores=1)

# TODO: 5. 建立 RiscvBoard
board = RiscvBoard(
    clk_freq="2GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

# TODO: 6. 設定核心與磁碟映像檔
board.set_kernel_disk_workload(
    kernel=obtain_resource("riscv-boot-loader-vmlinux-5.10"),
    disk_image=obtain_resource("riscv-ubuntu-20.04-img")
)

# TODO: 7. 開始模擬
simulator = Simulator(board=board)
simulator.run()
print("RISC-V FS simulation finished.")
