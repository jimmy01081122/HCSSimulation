#!/usr/bin/env python3
"""
簡述: 本分析腳本掃描所有 DSE 模擬的結果目錄，並彙整出 CSV 檔案。
"""
import os
import csv
import re

def extract_sim_seconds(stats_path):
    if not os.path.exists(stats_path):
        return None
    with open(stats_path, "r") as f:
        for line in f:
            if "simSeconds" in line:
                parts = line.split()
                if len(parts) >= 2:
                    return float(parts[1])
    return None

if __name__ == "__main__":
    output_csv = "dse_results.csv"
    fieldnames = ["cpu", "l1_size", "l2_size", "sim_seconds"]
    
    # 搜尋當前目錄下所有的 m5out_ 目錄
    results = []
    for dirname in os.listdir("."):
        if dirname.startswith("m5out_") and os.path.isdir(dirname):
            # TODO: 從目錄名解析配置參數 (格式: m5out_cpu_l1_l2)
            parts = dirname.split("_")
            if len(parts) >= 4:
                cpu = parts[1]
                l1 = parts[2]
                l2 = parts[3]
                
                stats_path = os.path.join(dirname, "stats.txt")
                sim_sec = extract_sim_seconds(stats_path)
                
                if sim_sec is not None:
                    results.append({
                        "cpu": cpu,
                        "l1_size": l1,
                        "l2_size": l2,
                        "sim_seconds": sim_sec
                    })
                    
    # 將彙整結果寫入 CSV
    with open(output_csv, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for r in results:
            writer.writerow(r)
            
    print(f"DSE aggregation completed. Output saved to {output_csv}")
