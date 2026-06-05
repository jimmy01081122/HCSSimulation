"""
簡述: 本腳本實作可配置實體記憶體時序模型的 ARM 模擬系統。
透過自訂 mem-type，學生可觀察 DDR3, DDR4, 與 DDR5 之間的模擬效能差異。
"""
import argparse
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.private_l1_private_l2_cache_hierarchy import PrivateL1PrivateL2CacheHierarchy
from gem5.components.memory import (
    SingleChannelDDR3_1600,
    SingleChannelDDR4_2400,
    DIMM_DDR5_4400
)
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# TODO: 1. 實作參數解析，選擇記憶體世代
parser = argparse.ArgumentParser(description="gem5 Memory System sweep")
parser.add_argument(
    "--mem-type",
    type=str,
    choices=["DDR3", "DDR4", "DDR5"],
    default="DDR3",
    help="Select DDR generation"
)
args = parser.parse_args()

requires(isa_required=ISA.ARM)

cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1d_size="32KiB",
    l1i_size="32KiB",
    l2_size="256KiB"
)

# TODO: 2. 根據參數動態對應實體記憶體控制器對象
mem_map = {
    "DDR3": SingleChannelDDR3_1600,
    "DDR4": SingleChannelDDR4_2400,
    "DDR5": DIMM_DDR5_4400
}

memory = mem_map[args.mem-type](size="1GiB")

processor = SimpleProcessor(cpu_type=CPUTypes.TIMING, isa=ISA.ARM, num_cores=1)

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

board.set_se_binary_workload(
    BinaryResource(local_path="/work/gem5/tests/test-progs/hello/bin/arm/linux/hello")
)

simulator = Simulator(board=board)
print(f"Running ARM simulation with memory type: {args.mem_type}")
simulator.run()
