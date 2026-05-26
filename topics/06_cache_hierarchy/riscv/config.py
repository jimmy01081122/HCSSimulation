"""
簡述: 本腳本實作快取大小可調整的 RISC-V 模擬系統。
學生可以透過命令列參數修改 L1i, L1d 與 L2 的容量，藉以研究 Cache 參數對效能的影響。
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

# TODO: 1. 實作命令列參數解析
parser = argparse.ArgumentParser(description="gem5 Cache Hierarchy sweep")
parser.add_argument("--l1d-size", type=str, default="32KiB")
parser.add_argument("--l1i-size", type=str, default="32KiB")
parser.add_argument("--l2-size", type=str, default="256KiB")
args = parser.parse_args()

requires(isa_required=ISA.RISCV)

# TODO: 2. 初始化自訂容量的快取階層
cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1d_size=args.l1d_size,
    l1i_size=args.l1i_size,
    l2_size=args.l2_size
)

memory = SingleChannelDDR3_1600(size="512MiB")
processor = SimpleProcessor(cpu_type=CPUTypes.TIMING, isa=ISA.RISCV, num_cores=1)

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

board.set_se_binary_workload(
    BinaryResource(local_path="/work/gem5/tests/test-progs/hello/bin/riscv/linux/hello")
)

simulator = Simulator(board=board)
print(f"Running RISC-V sim with L1d={args.l1d_size}, L1i={args.l1i_size}, L2={args.l2_size}")
simulator.run()
