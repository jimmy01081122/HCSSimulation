
from pathlib import Path

from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.no_cache import NoCache
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor

from gem5.isas import ISA
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator


processor = SimpleProcessor(
    cpu_type=CPUTypes.ATOMIC,
    isa=ISA.RISCV,
    num_cores=1,
)

memory = SingleChannelDDR3_1600(size="2GiB")

cache_hierarchy = NoCache()

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
)

binary_path = Path("tests/test-progs/hello/bin/riscv/linux/hello").resolve()
binary = BinaryResource(local_path=str(binary_path))

board.set_se_binary_workload(binary)

simulator = Simulator(board=board)
simulator.run()
