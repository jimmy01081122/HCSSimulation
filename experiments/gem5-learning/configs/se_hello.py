
from gem5.resources.resource import obtain_resource

from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.no_cache import NoCache
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor

from gem5.isas import ISA
from gem5.simulate.simulator import Simulator


# 建立 Processor。
# SimpleProcessor 代表所有核心使用同一種 CPU model。
# CPUTypes.ATOMIC 速度快，適合做第一個 smoke test。
# isa=ISA.RISCV 表示這個 processor 執行 RISC-V ISA。
processor = SimpleProcessor(
    cpu_type=CPUTypes.ATOMIC,
    isa=ISA.RISCV,
    num_cores=1,
)


# 建立 Memory。
# SingleChannelDDR3_1600 是 gem5 standard library 提供的單通道 DDR3 記憶體模型。
# gem5 23 不需要 MemorySizeType，直接使用 "2GiB" 字串即可。
memory = SingleChannelDDR3_1600(size="2GiB")


# 建立 CacheHierarchy。
# NoCache 表示不建立 cache，CPU 直接連到 memory system。
# 第一個 SE mode 實驗先不用 cache，可以降低除錯複雜度。
cache_hierarchy = NoCache()


# 建立 Board。
# SimpleBoard 適合 SE mode。
# Board 負責組合 Processor、Memory、CacheHierarchy。
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
)


# 取得 RISC-V hello workload。
# riscv-hello 是 gem5 resources 中的 RISC-V hello binary。
# 它必須搭配 ISA.RISCV 與 build/RISCV/gem5.opt 使用。
binary = obtain_resource(resource_id="riscv-hello")
board.set_se_binary_workload(binary)


# 建立並執行模擬器。
simulator = Simulator(board=board)
simulator.run()
