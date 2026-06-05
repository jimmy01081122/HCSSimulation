# MoE Routing-aware Prefetcher 環境建置與安裝指南

本指南旨在幫助使用者在全新環境中（包括實體工作站、虛擬主機與 Docker 容器）快速部署與運行本專案，確保系統具備高度的可移植性。

---

## 1. 系統需求與前提條件

### 1.1 作業系統
- **推薦**：Linux (Debian/Ubuntu 20.04 LTS 或更新版本)
- **相容**：Windows Subsystem for Linux (WSL2) 或 macOS (需自備 homebrew 安裝 iverilog)

### 1.2 軟體工具
- **Icarus Verilog**：硬體模擬編譯器，建議版本 v10.0 或更新版本。
- **Python**：版本需為 3.8 或更新版本，並備有 `pip` 套件管理器。

---

## 2. 依賴套件管理

本專案將依賴套件區分為 **硬體編譯工具** 與 **Python 分析與建模套件**。

### 2.1 系統級工具 (Hardware Simulation)
- `iverilog` (Icarus Verilog)：編譯 Verilog 原始碼並生成模擬可執行檔。
- `vvp` (Icarus Verilog runtime)：運行編譯產出的 vvp 仿真檔。

### 2.2 Python 軟體套件 (Analysis & Modeling)
這些依賴均列於 [requirements.txt](file:///home/a/HCSSimulation/advanced_projects/moe_routing_prefetch/requirements.txt) 中：
- `pandas`：用於讀取與整理 CSV 路由 trace。
- `numpy`：用於各項數據運算與敏感度指標計算。
- `matplotlib`：用於產生敏感度 Sweeps 實驗折線圖與專家啟動熱力圖。
- `torch` (PyTorch)：用於建構 Toy MoE 類神經網路結構，做 forward 推理並提取真實 Softmax 路由權重。

---

## 3. 一鍵自動安裝 (Linux Debian/Ubuntu)

專案附帶了自動安裝指令碼。請在專案根目錄下執行以下指令：

```bash
# 賦予執行權限並運行安裝腳本
chmod +x scripts/install_dependencies.sh
./scripts/install_dependencies.sh
```

> [!NOTE]
> 該指令碼會嘗試檢測 `apt-get` 以安裝 `iverilog` 與 `pip3`（可能需要提供 `sudo` 密碼），並自動調用 `pip` 安裝 [requirements.txt](file:///home/a/HCSSimulation/advanced_projects/moe_routing_prefetch/requirements.txt) 中定義的軟體套件。

---

## 4. 手動與自定義安裝

如果您身處於特殊的網路限制環境中，或希望精準控制依賴包大小（特別是避免下載 CUDA 版本之 PyTorch），可以參考以下自定義指令：

### 4.1 手動安裝 Icarus Verilog
- **Ubuntu/Debian**:
  ```bash
  sudo apt-get update && sudo apt-get install -y iverilog
  ```
- **macOS (using Homebrew)**:
  ```bash
  brew install icarus-verilog
  ```

### 4.2 安裝輕量化 CPU-only 版本 PyTorch (推薦)
為避免在無 GPU 的工作站下載了數 GB 大小的 CUDA 驅動依賴，推薦指定 PyTorch 的 CPU 索引：
```bash
python3 -m pip install torch --index-url https://download.pytorch.org/whl/cpu
python3 -m pip install pandas numpy matplotlib
```

---

## 5. 可移植性驗證 (驗收測試)

為確保您的環境已成功對齊且沒有任何依賴缺漏，請在安裝後執行 **End-to-End Smoke Test**：

```bash
# 切換至專案目錄並執行煙霧測試
cd advanced_projects/moe_routing_prefetch
bash scripts/run_smoke_test.sh
```

### 5.1 預期通過輸出
煙霧測試將會：
1. 調用 Python 腳本動態生成專家 demand trace。
2. 進行 cache 仿真，確認統計 counters 自洽。
3. 將 trace 轉換為十六進位 stimulus 檔。
4. 自動調用 `iverilog` 編譯與運行 RTL 測試（支援 FIFO 與 LRU 雙重測試）。
5. 自動比對 Python 與 RTL 的計數器，輸出以下字樣即代表環境移植成功：
   ```text
   COUNTER MATCH PASSED
   SMOKE TEST PASSED FOR BOTH FIFO AND LRU
   ```
