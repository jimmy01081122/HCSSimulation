# 開發維護日誌：錯誤修復與環境配置紀錄

## 1. 系統環境與依賴項

### 基本環境
- **作業系統**: Ubuntu 24.04 (Docker Container)
- **硬體架構**: ARM64 (Apple Silicon M4 虛擬化)
- **模擬器**: gem5 v23.0.0.1

### 關鍵依賴套件 (Apt Packages)
- `build-essential`, `scons`, `python3-dev`: 編譯核心需求。
- `libgoogle-perftools-dev`, `libprotobuf-dev`, `libboost-all-dev`: gem5 必要的第三方庫。
- `graphviz`, `python3-pydot`: 用於生成系統拓樸圖 (`config.dot.pdf`)。

### gem5 指令庫依賴
- **libm5.a**: 測試程式需連結此庫以使用 `m5_work_begin()` 等指令。
  - 編譯路徑: `gem5-project/gem5/util/m5/build/arm64/out/libm5.a`

---

## 2. 錯誤修復紀錄 (Issue Tracking)

### 【Issue 01】UnicodeDecodeError: 'ascii' codec can't decode...
- **現象**: 執行 Python 配置檔時崩潰。
- **原因**: gem5 內部 Python 載入器無法處理 UTF-8 (中文) 字元。
- **修復**: 將配置檔拆分為「純英文執行版 (`.py`)」與「詳細中文教學版 (`_tutorial.py`)」。

### 【Issue 02】fatal: Couldn't find appropriate workload object
- **現象**: 模擬啟動時發生 Fatal 錯誤。
- **原因**: gem5 23.0 要求 SE 模式必須顯示定義 `system.workload`。
- **修復**: 在配置檔加入 `system.workload = SEWorkload.init_compatible(binary)`。

### 【Issue 03】NameError: name 'L1ICache' is not defined
- **現象**: 配置檔讀取失敗。
- **原因**: 模擬腳本中直接使用了快取物件，但未導入 `configs.common`。
- **修復**: 在配置檔中顯式定義 `L1Cache`, `L1ICache`, `L1DCache`, `L2Cache` 類別，確保不依賴外部複雜腳本。

### 【Issue 04】instruction 'bti' unimplemented
- **現象**: 終端機出現大量黃色警告。
- **原因**: Ubuntu 預設編譯器開啟了 ARM v8.5 的 Branch Target Identification 保护，但 gem5 舊核心不支援。
- **修復**: 在 `run.sh` 編譯指令加入 `-mbranch-protection=none`。

### 【Issue 05】libm5.a Not Found
- **現象**: `ld` 連結器報錯。
- **原因**: 容器內部尚未編譯實體指令庫。
- **修復**: 在容器內執行 `scons build/arm64/out/libm5.a`。

---

## 3. 環境設置 SOP
1. **重新建構 Docker**: `docker build -t gem5-env .`
2. **啟動掛載**: 必須掛載 `projectspace` 父目錄，確保容器內可同時存取 `/work/gem5-project` 與 `/work/HCSSimulation`。
3. **編譯驗證**: 每次修改 C++ Workload 後需重新執行各目錄的 `./run.sh`。
