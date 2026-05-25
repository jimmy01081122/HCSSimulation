# HCSSimulation: 異構系統架構模擬專案 (Heterogeneous Computing System Simulation)

本專案旨在透過 gem5 模擬器，實作並評估三種基礎的異構計算架構。這是一個針對新手設計的教學專案，重點在於架構的搭建、數據的採集與基礎效能分析。

## 專案目錄結構
- `dir`
   - `dir1_memory_dma/`: 記憶體頻寬與 DMA 數據搬運瓶頸分析。
   - `dir2_moe_routing/`: MoE (Mixture of Experts) 路由邏輯的運算延遲對比。
   - `dir3_kv_cache/`: KV Cache 分散收集與軟體預取模擬。
- `docs/`: 詳細的教學文件與實驗報告。

## 環境設定與啟動
### 啟動 Docker 容器
請確保已安裝 Docker Desktop 並在 `gem5-project` 目錄下執行：
```bash
docker run --rm -it -v "$(pwd)":/work gem5-env
```

## 跨電腦環境轉移指南 (Workflow)

為了在不同裝置上保持開發一致性，請遵循以下標準作業流程 (SOP)：

1. **程式碼同步**：
   - 僅將 `HCSSimulation/` 資料夾內的配置檔 (`.py`)、原始碼 (`.c`, `.cpp`) 與文件 (`.md`) 提交至 Git。
   - **不要** 提交 `gem5/` 主程式目錄或其編譯後的二進位檔，因為其體積龐大且具備平台依賴性。

2. **新環境設定步驟**：
   - 在新電腦上安裝 Docker Desktop 並分配足夠資源 (建議 12GB+ RAM)。
   - 克隆本 Repo：`git clone git@github.com:jimmy01081122/HCSSimulation.git`。
   - 依照專案根目錄的 `Dockerfile` 重新建構 Docker 映像檔（若尚未建構）。
   - 重新下載並編譯 gem5（若環境中無現成編譯結果）。

3. **執行模擬**：
   - 進入容器後，進入各方向目錄執行對應的 `run.sh`。

## 數據量測與視覺化

### 1. 基礎數據量測 (Standard Stats)
所有模擬結果預設會輸出至各執行目錄下的 `m5out/` 資料夾中。
- `stats.txt`: 包含所有效能統計數據 (IPC, Cycles, Memory Accesses 等)。
- `config.json`: 完整的系統硬體配置描述。

### 2. 系統拓樸圖 (Graphviz)
當模擬啟動後，gem5 會自動偵測環境中的 `pydot` 並在 `m5out/` 目錄生成 `config.dot.pdf`。
- **查看方式**：直接在本機 Finder 中進入 `m5out/` 資料夾，使用 PDF 閱讀器開啟。

### 3. O3 管線視覺化 (Pipeline Viewer)
針對亂序執行核心 (O3CPU)，可產生詳細的指令執行追蹤。
- **生成方式**：執行方向二的 `run.sh` 後，會在 `m5out/` 生成 `pipeview.out`。
- **查看方式**：使用 VS Code 等編輯器開啟 `pipeview.out`，觀察指令在管線各階段的流動。
