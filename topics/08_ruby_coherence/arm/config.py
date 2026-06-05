"""
簡述: 本腳本實作雙核心的 ARM Ruby 模擬系統。
本實驗使用 Ruby 記憶體系統與 MESI 二級快取協議。
注意: 執行此模擬前，gem5 必須已編譯 PROTOCOL=MESI_Two_Level。
"""
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.ruby.mi_example_cache_hierarchy import MIExampleCacheHierarchy
from gem5.components.memory import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# TODO: 1. 宣告所需 ISA 與快取協議 MI_example
requires(
    isa_required=ISA.ARM
)

# TODO: 2. 建立 MI_example 快取階層
cache_hierarchy = MIExampleCacheHierarchy(
    size="32KiB",
    assoc=8
)

memory = SingleChannelDDR3_1600(size="512MiB")

# TODO: 3. 建立雙核心 (num_cores=2) TIMING 處理器
processor = SimpleProcessor(cpu_type=CPUTypes.TIMING, isa=ISA.ARM, num_cores=2)

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

# 在雙核 SE 模式下，載入多個工作負載
board.set_se_binary_workload(
    BinaryResource(local_path="/work/gem5/tests/test-progs/hello/bin/arm/linux/hello")
)

simulator = Simulator(board=board)
print("Running Multi-core ARM Ruby Cache Coherence simulation...")
simulator.run()
print("Simulation finished.")
