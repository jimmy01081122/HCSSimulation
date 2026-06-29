import re
import os
import json

def parse_stats_file(file_path):
    metrics = {
        "simSeconds": 0.0,
        "simTicks": 0,
        "cpuCycles": 0,
        "dcacheMisses": 0,
        "ipc": 0.0
    }
    
    if not os.path.exists(file_path):
        print(f"Warning: {file_path} does not exist.")
        return metrics

    with open(file_path, "r") as f:
        content = f.read()

    # 正則表達式匹配 gem5 stat 行
    # 格式如: simSeconds                                 0.000109                       # Number of seconds simulated (second)
    def find_val(name):
        pattern = rf"^\s*{re.escape(name)}\s+([0-9\.e\-+]+)"
        match = re.search(pattern, content, re.MULTILINE)
        return float(match.group(1)) if match else None

    # 有時 CPU 名稱會是 system.cpu.numCycles 或者是 system.cpu.num_cycles
    # 或是 system.cpu0.numCycles 等。我們做模糊搜索。
    sim_seconds = find_val("simSeconds")
    sim_ticks = find_val("simTicks")
    
    # 搜尋 CPU Cycles
    cpu_cycles_match = re.search(r"system\.cpu\.numCycles\s+(\d+)", content)
    if not cpu_cycles_match:
        cpu_cycles_match = re.search(r"system\.cpu\d+\.numCycles\s+(\d+)", content)
    cpu_cycles = int(cpu_cycles_match.group(1)) if cpu_cycles_match else 0

    # 搜尋 L1 D-Cache Misses
    dcache_misses_match = re.search(r"system\.cpu\.dcache\.overallMisses::total\s+(\d+)", content)
    if not dcache_misses_match:
        dcache_misses_match = re.search(r"system\.cpu\d+\.dcache\.overallMisses::total\s+(\d+)", content)
    dcache_misses = int(dcache_misses_match.group(1)) if dcache_misses_match else 0

    # 搜尋 IPC
    ipc_match = re.search(r"system\.cpu\.ipc\s+([0-9\.]+)", content)
    if not ipc_match:
        ipc_match = re.search(r"system\.cpu\d+\.ipc\s+([0-9\.]+)", content)
    ipc = float(ipc_match.group(1)) if ipc_match else 0.0

    if sim_seconds: metrics["simSeconds"] = sim_seconds
    if sim_ticks: metrics["simTicks"] = int(sim_ticks)
    if cpu_cycles: metrics["cpuCycles"] = cpu_cycles
    if dcache_misses: metrics["dcacheMisses"] = dcache_misses
    if ipc: metrics["ipc"] = ipc

    return metrics

def main():
    base_dir = "/work/advanced_projects/hetero_moe_scheduling/reports"
    cent_stats = os.path.join(base_dir, "gem5_centralized", "stats.txt")
    dist_stats = os.path.join(base_dir, "gem5_distributed", "stats.txt")

    print("\n=============================================")
    print("        gem5 微架構模擬結果對齊與分析")
    print("=============================================")
    
    cent_metrics = parse_stats_file(cent_stats)
    dist_metrics = parse_stats_file(dist_stats)

    print(f"{'指標 (Metrics)':<30} | {'集中式佇列 (Central)':<20} | {'分散式佇列 (Distributed)':<20} | {'改進 (Improvement)'}")
    print("-" * 90)
    
    def print_row(name, key, fmt):
        v1 = cent_metrics[key]
        v2 = dist_metrics[key]
        if key == "dcacheMisses" or key == "cpuCycles" or key == "simTicks":
            diff = v1 - v2
            pct = (diff / v1 * 100) if v1 > 0 else 0
            imp = f"{pct:.1f}% 降低" if diff > 0 else f"{-pct:.1f}% 增加"
        elif key == "ipc":
            diff = v2 - v1
            pct = (diff / v1 * 100) if v1 > 0 else 0
            imp = f"{pct:.1f}% 提升" if diff > 0 else f"{-pct:.1f}% 降低"
        else:
            diff = v1 - v2
            pct = (diff / v1 * 100) if v1 > 0 else 0
            imp = f"{pct:.1f}% 降低" if diff > 0 else f"{-pct:.1f}% 增加"
        
        val1 = fmt.format(v1)
        val2 = fmt.format(v2)
        print(f"{name:<30} | {val1:<20} | {val2:<20} | {imp}")

    print_row("模擬總時延 (simSeconds)", "simSeconds", "{:.6f} s")
    print_row("模擬總週期 (simTicks)", "simTicks", "{} ticks")
    print_row("CPU 總時鐘週期 (Cycles)", "cpuCycles", "{} cycles")
    print_row("L1 D-Cache 缺失數 (Misses)", "dcacheMisses", "{} misses")
    print_row("指令執行率 (IPC)", "ipc", "{:.3f} ipc")

    # 將數據寫入 json，以便系統級模擬器讀取
    summary = {
        "centralized": cent_metrics,
        "distributed": dist_metrics
    }
    
    with open(os.path.join(base_dir, "gem5_metrics_summary.json"), "w") as f:
        json.dump(summary, f, indent=4)
        
    print(f"\n[INFO] 已將微架構數據摘要寫入 {os.path.join(base_dir, 'gem5_metrics_summary.json')}")

if __name__ == "__main__":
    main()
