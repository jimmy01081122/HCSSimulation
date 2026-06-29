import os
import pandas as pd
import matplotlib.pyplot as plt

def plot_experiment_results():
    csv_path = "/work/advanced_projects/hetero_moe_scheduling/reports/sweep_results.csv"
    output_dir = "/work/advanced_projects/hetero_moe_scheduling/reports"
    
    if not os.path.exists(csv_path):
        print(f"Error: CSV file {csv_path} not found.")
        return

    df = pd.read_csv(csv_path)

    # 設置繪圖風格
    plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in plt.style.available else 'default')
    
    # 策略的顏色和線條樣式設定
    strategy_styles = {
        "FCFS_TRANSFER": {"color": "#e74c3c", "marker": "o", "ls": "--", "label": "FCFS + Always Transfer (GPU)"},
        "ELAS_TRANSFER": {"color": "#e67e22", "marker": "s", "ls": "--", "label": "ELAS + Always Transfer (GPU)"},
        "ELAS_OFFLOAD": {"color": "#95a5a6", "marker": "^", "ls": ":", "label": "ELAS + Always Offload (CPU)"},
        "ELAS_DCMD_CO_DESIGN": {"color": "#2ecc71", "marker": "D", "ls": "-", "label": "ELAS + DCMD (Co-Design)"},
        "F_ELAS_DCMD_OPT": {"color": "#3498db", "marker": "X", "ls": "-", "label": "F-ELAS + DCMD (Paper Opt)"}
    }

    # 1. 繪製 Cache Capacity Sweep 圖表
    df_cache = df[df["sweep_type"] == "cache_capacity"]
    if not df_cache.empty:
        plt.figure(figsize=(10, 6))
        for strategy, group in df_cache.groupby("strategy"):
            style = strategy_styles.get(strategy, {"color": "black", "marker": "o", "ls": "-", "label": strategy})
            # 排序以防無序畫線
            group = group.sort_values("cache_capacity")
            plt.plot(
                group["cache_capacity"], 
                group["total_sim_time_ms"], 
                color=style["color"], 
                marker=style["marker"], 
                linestyle=style["ls"], 
                linewidth=2.5, 
                markersize=8,
                label=style["label"]
            )
        
        plt.title("Design Space Exploration: Impact of GPU Expert Cache Capacity", fontsize=14, fontweight="bold", pad=15)
        plt.xlabel("GPU Expert Cache Capacity (Number of Experts)", fontsize=12)
        plt.ylabel("Total Simulation Time (ms)", fontsize=12)
        plt.xticks(df_cache["cache_capacity"].unique())
        plt.legend(frameon=True, facecolor="white", edgecolor="none", fontsize=10)
        plt.tight_layout()
        
        fig_path = os.path.join(output_dir, "cache_capacity_sweep.png")
        plt.savefig(fig_path, dpi=300)
        plt.close()
        print(f"[SUCCESS] 已生成快取容量掃描圖表: {fig_path}")

    # 2. 繪製 PCIe Bandwidth Sweep 圖表
    df_pcie = df[df["sweep_type"] == "pcie_bandwidth"]
    if not df_pcie.empty:
        plt.figure(figsize=(10, 6))
        for strategy, group in df_pcie.groupby("strategy"):
            style = strategy_styles.get(strategy, {"color": "black", "marker": "o", "ls": "-", "label": strategy})
            group = group.sort_values("pcie_bandwidth_gbs")
            plt.plot(
                group["pcie_bandwidth_gbs"], 
                group["total_sim_time_ms"], 
                color=style["color"], 
                marker=style["marker"], 
                linestyle=style["ls"], 
                linewidth=2.5, 
                markersize=8,
                label=style["label"]
            )
        
        plt.title("Design Space Exploration: Impact of PCIe Bandwidth", fontsize=14, fontweight="bold", pad=15)
        plt.xlabel("PCIe Bandwidth (GB/s)", fontsize=12)
        plt.ylabel("Total Simulation Time (ms)", fontsize=12)
        plt.xticks(df_pcie["pcie_bandwidth_gbs"].unique())
        plt.legend(frameon=True, facecolor="white", edgecolor="none", fontsize=10)
        plt.tight_layout()
        
        fig_path = os.path.join(output_dir, "pcie_bandwidth_sweep.png")
        plt.savefig(fig_path, dpi=300)
        plt.close()
        print(f"[SUCCESS] 已生成 PCIe 頻寬掃描圖表: {fig_path}")

if __name__ == "__main__":
    plot_experiment_results()
