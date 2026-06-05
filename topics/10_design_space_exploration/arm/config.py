"""
簡述: 本腳本實作參數化的 ARM 模擬組態，支援由 DSE 掃描腳本動態傳入 CPU 模型、L1 大小與 L2 大小。
"""
import argparse
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.private_l1_private_l2_cache_hierarchy import PrivateL1PrivateL2CacheHierarchy
from gem5.components.memory import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

parser = argparse.ArgumentParser(description="DSE configuration")
parser.add_argument("--cpu", type=str, choices=["timing", "minor", "o3"], default="timing")
parser.add_argument("--l1-size", type=str, default="32KiB")
parser.add_argument("--l2-size", type=str, default="256KiB")
args = parser.parse_args()

requires(isa_required=ISA.ARM)

cpu_map = {
    "timing": CPUTypes.TIMING,
    "minor": CPUTypes.MINOR,
    "o3": CPUTypes.O3
}

cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1d_size=args.l1_size,
    l1i_size=args.l1_size,
    l2_size=args.l2_size
)

memory = SingleChannelDDR3_1600(size="512MiB")
processor = SimpleProcessor(cpu_type=cpu_map[args.cpu], isa=ISA.ARM, num_cores=1)

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
simulator.run()
