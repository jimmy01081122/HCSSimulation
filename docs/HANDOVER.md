# HCSSimulation 專案交接與操作指南

## 1. 專案目標
本專案透過 gem5 模擬器，實作了三個異構計算的基礎實驗：
- **方向一 (DMA)**: 評估記憶體數據搬運的通訊開銷。
- **方向二 (MoE)**: 比較 CPU 執行混合專家模型路由邏輯的效能，並練習管線視覺化。
- **方向三 (KV Cache)**: 透過雙核協作模擬軟體預取 (Software Prefetch) 對減少 Memory Stall 的效果。

## 2. 目錄結構
- `dir*/configs/`: 存放執行用 Python 腳本（純英文）。
- `dir*/configs/*_tutorial.py`: 存放詳細繁體中文註解版腳本（教學與查閱用）。
- `dir*/src/`: 存放 C/C++ 測試負載原始碼。
- `dir*/run.sh`: 自動化編譯與執行腳本。
- `docs/`: 存放各實驗的詳細教學與視覺化工具使用指南。

## 3. 核心操作流程
所有操作必須在 Docker 容器內執行，以確保環境一致性。

### A. 啟動環境
```bash
# 在 MacBook 主機端執行
cd /Users/jimmychang/space/projectspace
docker run --rm -it -v "$(pwd)":/work gem5-env
```

### B. 執行實驗 (以方向二為例)
```bash
# 在容器內執行
cd /work/HCSSimulation/dir2_moe_routing
./run.sh
```

## 4. 關鍵注意事項
- **編碼規範**: gem5 內部 Python 載入器不支援 UTF-8 字元。若需修改配置檔，請確保執行檔 (`.py`) 為純英文。中文註解請寫在對應的 `_tutorial.py` 中。
- **視覺化產物**: 執行完畢後，請到 `m5out/` 目錄查看 `config.dot.pdf` (系統拓樸) 與 `pipeview.out` (管線圖)。
