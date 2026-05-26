"""
簡述: 本腳本實作可自訂 CPU 模型的 RISC-V 模擬系統。
透過 argparse 命令列參數，允許使用者在 atomic, timing, minor, o3 之間進行切換。
"""
import argparse
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.no_cache import NoCache
from gem5.components.cachehierarchies.classic.private_l1_private_l2_cache_hierarchy import PrivateL1PrivateL2CacheHierarchy
from gem5.components.memory import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# TODO: 1. 解析命令列參數 --cpu
parser = argparse.ArgumentParser(description="gem5 CPU model sweep")
parser.add_argument(
    "--cpu",
    type=str,
    choices=["atomic", "timing", "minor", "o3"],
    default="timing",
    help="Select CPU model"
)
args = parser.parse_args()

requires(isa_required=ISA.RISCV)

# TODO: 2. 依據 CPU 模型動態選擇快取階層
cpu_map = {
    "atomic": (CPUTypes.ATOMIC, NoCache()),
    "timing": (CPUTypes.TIMING, PrivateL1PrivateL2CacheHierarchy(l1d_size="32KiB", l1i_size="32KiB", l2_size="256KiB")),
    "minor": (CPUTypes.MINOR, PrivateL1PrivateL2CacheHierarchy(l1d_size="32KiB", l1i_size="32KiB", l2_size="256KiB")),
    "o3": (CPUTypes.O3, PrivateL1PrivateL2CacheHierarchy(l1d_size="32KiB", l1i_size="32KiB", l2_size="256KiB")),
}

cpu_type, cache_hierarchy = cpu_map[args.cpu]

memory = SingleChannelDDR3_1600(size="512MiB")

# TODO: 3. 建立 CPU
processor = SimpleProcessor(cpu_type=cpu_type, isa=ISA.RISCV, num_cores=1)

board = SimpleBoard(
    clk_freq="2.5GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

board.set_se_binary_workload(
    BinaryResource(local_path="/work/gem5/tests/test-progs/hello/bin/riscv/linux/hello")
)

# 4. 開始模擬
simulator = Simulator(board=board)
print(f"Starting RISC-V simulation with {args.cpu} CPU model...")
simulator.run()
print("Simulation finished.")
