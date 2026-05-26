# Topic 02: Full System (FS) Mode 核心概念與系統建置

## 1. 學習目標
* 理解 Full System (FS) 全系統模擬的工作原理及其與 SE 模式的區別。
* 掌握如何使用 `ArmBoard` 與 `RiscvBoard` 來模擬完整的作業系統開機過程。
* 理解 gem5 Resources 中對於 Kernel、Disk Image 的管理機制。
* 探討全系統模擬中的虛擬硬體周邊設備配置與中斷處理。

## 2. 架構與設計思維
Full System (FS) 模式是計算機系統架構設計中，最貼近真實硬體運行狀態的模擬模式。
在 FS 模式下，模擬器將負責模擬整個硬體系統，包括中斷控制器、定時器、PCI 匯流排、磁碟控制器、網路卡、UART 串口調試端等。因為硬體結構被完整地模擬出來，我們可以在上面啟動一個未經修改的作業系統核心 (例如 Linux Kernel) 和完整的根檔案系統 (Root Filesystem)。

### FS 模式與 SE 模式對比
* **OS 行為**：SE 模式直接由模擬器轉譯 syscall；FS 模式運行真實的 OS Kernel。
* **硬體周邊**：SE 模式無周邊模擬；FS 模式有完整的虛擬硬體週邊與暫存器定義。
* **時間精確度**：FS 模式包含了作業系統核心的行程調度 (Scheduler)、分頁中斷 (Page Fault)、I/O 驅動程式執行時間，因此對於系統整體的效能評估極度精準，但模擬速度顯著變慢 (通常為數萬到數十萬分之一的真實速度)。
* **應用場景**：SE 模式適用於單純運算密集型演算法；FS 模式適用於涉及多行程同步、網路通訊、檔案系統 I/O、驅動程式開發或 OS 核心調校的硬體設計驗證。

### 資源下載機制
gem5 標準函式庫提供 `obtain_resource()` API，可自動從遠端伺服器上下載經過適配的 Linux 核心 (`riscv-boot-loader` / `arm-boot-loader`) 與根磁碟映像檔 (`ubuntu-disk-image`)。

## 3. 程式碼架構解析
* `ArmBoard` / `RiscvBoard`：這類板子繼承自 `AbstractSystemBoard`，具備連接虛擬周邊設備 (如 UART, Interrupt Controller, Real-time clock) 的邏輯。
* `obtain_resource`：自動化處理二進位資源的版本與快取路徑，使得 Python 程式碼能夠以一站式的方式進行雲端資源配置。

## 4. 學生練習 (TODO)
在 `config.py` 中，你需要：
1. 宣告所需的 ISA。
2. 配置記憶體子系統 (FS 模式記憶體建議設定大於 `512MiB`)。
3. 建立 CPU 核心 (為了使開機不至於太慢，可使用 `TIMING` 或配合 KVM 進行快速引導)。
4. 呼叫 `board.set_kernel_disk_workload`，並填入藉由 `obtain_resource` 取得的核心和磁碟資源。
