"""
簡述: 使用 gem5 標準函式庫建置一個 ARM FS 模式全系統模擬組態。
注意: FS 模式模擬需要下載核心與磁碟映像檔，且執行時間長。
"""
from gem5.components.boards.arm_board import ArmBoard
from gem5.components.cachehierarchies.classic.private_l1_private_l2_cache_hierarchy import PrivateL1PrivateL2CacheHierarchy
from gem5.components.memory import DualChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import obtain_resource
from gem5.simulate.simulator import Simulator
from gem5.utils.requires import requires

# TODO: 1. 宣告所需 ISA 為 ARM
requires(isa_required=ISA.ARM)

# TODO: 2. 建立 Private L1 L2 快取階層以支援 OS 執行
cache_hierarchy = PrivateL1PrivateL2CacheHierarchy(
    l1d_size="32KiB",
    l1i_size="32KiB",
    l2_size="256KiB"
)

# TODO: 3. 建立 2GiB 的雙通道 DDR3 記憶體系統，滿足作業系統基本開機需求
memory = DualChannelDDR3_1600(size="2GiB")

# TODO: 4. 建立單核心 TIMING 處理器
processor = SimpleProcessor(cpu_type=CPUTypes.TIMING, isa=ISA.ARM, num_cores=1)

# TODO: 5. 建立 ArmBoard
board = ArmBoard(
    clk_freq="2GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

# TODO: 6. 使用 obtain_resource 設定核心與磁碟映像檔
# 註解: 這裡使用 gem5-resources 預先準備好的資源
board.set_kernel_disk_workload(
    kernel=obtain_resource("arm-linux-kernel-5.4.49"),
    disk_image=obtain_resource("arm-ubuntu-18.04-img"),
    bootloader=obtain_resource("arm-bootloader-foundation")
)

# TODO: 7. 開始進行模擬
simulator = Simulator(board=board)
simulator.run()
print("ARM FS simulation finished.")
