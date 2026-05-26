#!/usr/bin/env python3
"""
簡述: 本腳本用於讀取並解析 gem5 輸出的 stats.txt 檔案，抓取關鍵效能計數器指標並進行格式化輸出。
學生需要完成 TODO 中的正則匹配或值提取。
"""
import sys
import os
import re

def parse_gem5_stats(stats_path):
    if not os.path.exists(stats_path):
        print(f"Error: {stats_path} does not exist.")
        return None

    # TODO: 定義正則匹配規則以尋找關鍵指標
    # 指標包含: simSeconds, simInsts, system.cpu.numCycles, system.cpu.ipc
    patterns = {
        "simSeconds": re.compile(r"simSeconds\s+([0-9.e+-]+)"),
        "simInsts": re.compile(r"simInsts\s+(\d+)"),
        "cpuCycles": re.compile(r"(?:board\.processor\.cores\.core|system\.cpu)\.numCycles\s+(\d+)"),
        "ipc": re.compile(r"(?:board\.processor\.cores\.core|system\.cpu)\.ipc\s+([0-9.e+-]+)")
    }
    
    results = {}
    with open(stats_path, "r") as f:
        for line in f:
            for name, pattern in patterns.items():
                match = pattern.search(line)
                if match:
                    # TODO: 抓取數值並儲存
                    results[name] = float(match.group(1))

    return results

if __name__ == "__main__":
    stats_file = "m5out/stats.txt"
    if len(sys.argv) > 1:
        stats_file = sys.argv[1]
        
    metrics = parse_gem5_stats(stats_file)
    if metrics:
        print("\n=== gem5 Simulation Performance Report (ARM) ===")
        print(f"Simulated Seconds : {metrics.get('simSeconds', 'N/A')} s")
        print(f"Executed Insts    : {metrics.get('simInsts', 'N/A')}")
        print(f"CPU Cycles        : {metrics.get('cpuCycles', 'N/A')}")
        print(f"Calculated IPC    : {metrics.get('ipc', 'N/A')}")
        # 計算 CPI
        if "cpuCycles" in metrics and "simInsts" in metrics and metrics["simInsts"] > 0:
            cpi = metrics["cpuCycles"] / metrics["simInsts"]
            print(f"Calculated CPI    : {cpi:.4f}")
        print("================================================\n")
