"""
簡述: 使用 gem5 標準函式庫建置一個包含 Private L1-L2 快取階層的 RISC-V SE 系統。
"""
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.private_l1_private_l2_cache_hierarchy import PrivateL1PrivateL2CacheHierarchy
from gem5.components.memory import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# TODO: 1. 宣告所需 ISA
requires(isa_required=ISA.RISCV)

# TODO: 2. 建立 Private L1 (32KiB) + L2 (256KiB) 快取階層
cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1d_size="32KiB",
    l1i_size="32KiB",
    l2_size="256KiB"
)

# TODO: 3. 建立 512MiB 的 SingleChannelDDR3_1600 記憶體系統
memory = SingleChannelDDR3_1600(size="512MiB")

# TODO: 4. 建立單核心 TIMING 處理器
processor = SimpleProcessor(cpu_type=CPUTypes.TIMING, isa=ISA.RISCV, num_cores=1)

# TODO: 5. 建立 SimpleBoard，時脈設定為 3GHz
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

# TODO: 6. 設定 SE 模式的工作負載為 RISC-V Hello World 二進位檔
board.set_se_binary_workload(
    BinaryResource(local_path="/work/gem5/tests/test-progs/hello/bin/riscv/linux/hello")
)

# TODO: 7. 開始進行模擬
simulator = Simulator(board=board)
simulator.run()
print("RISC-V Standard Library simulation finished.")
