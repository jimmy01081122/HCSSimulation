import sys
sys.path.insert(0, '/home/a/HCSSimulation/gem5')

from gem5.resources import obtain_resource
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.memory.memory import MemorySizeType
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.isa import ISA
from gem5.sim.simulator import Simulator

processor = SimpleProcessor(
    cpu_type=CPUTypes.ATOMIC,
    isa=ISA.X86,
    num_cores=1
)

from gem5.components.memory.single_channel_ddr3_1600 import SingleChannelDDR3_1600

memory = SingleChannelDDR3_1600(size=MemorySizeType.GB_2)

from gem5.components.cachehierarchy.caches.cache_hierarchy import NoCache

cache_hierarchy = NoCache()

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cachehierarchy=cache_hierarchy
)

binary_path = obtain_resource("x86-hello64-static")
board.set_se_binary_workload(binary_path)

simulator = Simulator(board=board)
simulator.run()
