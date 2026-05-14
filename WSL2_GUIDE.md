# WSL2 環境建置與專案執行指南

本文件引導您如何在其他 Windows 電腦上透過 WSL2 與 Docker 重新建置此專案環境。

## 前置要求
1. 安裝 **Windows 11** 或 **Windows 10 (22H2+)**。
2. 安裝 **WSL2** (在 PowerShell 執行 `wsl --install`)。
3. 安裝 **Docker Desktop for Windows**。
   - 在設定中勾選 "Use the WSL 2 based engine"。
   - 在 "Resources > WSL Integration" 中勾選您使用的 Linux 發行版 (例如 Ubuntu)。

## 快速建置步驟

1. **克隆專案**:
   ```bash
   git clone git@github.com:jimmy01081122/HCSSimulation.git
   cd HCSSimulation
   ```

2. **執行初始化腳本**:
   ```bash
   chmod +x scripts/setup_wsl2.sh
   ./scripts/setup_wsl2.sh
   ```

3. **啟動開發容器**:
   ```bash
   docker run --rm -it -v $(pwd):/work gem5-env
   ```

4. **在容器內準備 gem5 (若為新環境)**:
   ```bash
   # 下載原始碼
   git clone https://gem5.googlesource.com/public/gem5
   
   # 編譯 (ARM 版本)
   cd gem5
   scons build/ARM/gem5.opt -j$(nproc)
   ```

## 執行模擬專案
編譯完成後，您可以回到專案子目錄執行測試：
```bash
cd /work/dir1_memory_dma
chmod +x run.sh
./run.sh
```

## 常見問題
- **記憶體不足**: 若編譯被 Killed，請在 Windows 使用者目錄下建立 `.wslconfig` 檔案，增加 `memory=12GB` 以上。
- **檔案權限**: 若在 WSL2 中遇到權限問題，請確保專案位於 WSL 檔案系統內 (例如 `~/HCSSimulation`)而非 `/mnt/c/`。
