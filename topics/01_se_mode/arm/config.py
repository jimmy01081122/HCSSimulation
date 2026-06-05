"""
簡述: 本組態檔案使用 gem5 標準函式庫建置一個基礎的 ARM SE 模式單核心系統。
學生需要理解 Board, Processor, Memory, Cache 的連接方式。
"""
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.no_cache import NoCache
from gem5.components.memory import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# TODO: 1. 宣告所需的 ISA 為 ARM
requires(isa_required=ISA.ARM)

# TODO: 2. 建立無快取階層 (NoCache)
cache_hierarchy = NoCache()

# TODO: 3. 建立 32MiB 大小的 SingleChannelDDR3_1600 記憶體系統
memory = SingleChannelDDR3_1600(size="32MiB")

# TODO: 4. 建立 SimpleProcessor，微架構為 TIMING, 核心數 1, 架構為 ARM
processor = SimpleProcessor(cpu_type=CPUTypes.TIMING, isa=ISA.ARM, num_cores=1)

# TODO: 5. 建立 SimpleBoard 連接上述三者，時脈設為 3GHz
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

# TODO: 6. 設定 SE 模式工作負載為 ARM Hello World 二進位檔
# 檔案路徑: /work/gem5/tests/test-progs/hello/bin/arm/linux/hello
board.set_se_binary_workload(
    BinaryResource(local_path="/work/gem5/tests/test-progs/hello/bin/arm/linux/hello")
)

# TODO: 7. 初始化 Simulator 並開始執行
simulator = Simulator(board=board)
simulator.run()
print("ARM SE simulation finished successfully.")
