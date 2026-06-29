# generate_all_figures.py
# Generates IEEE-publication-quality visualization figures for MoE co-design results.
# Adjusts layout, DPI, and fonts to match IEEE standard guidelines.

import os
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib

# Set src directory in Python path to run simulator for CDF
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../simulator/src'))
from simulator import HMCSimulator
from workload_gen import WorkloadGenerator

# Set IEEE-style plotting parameters
matplotlib.rcParams.update({
    'font.size': 10,
    'font.family': 'sans-serif',
    'figure.dpi': 300,
    'axes.grid': True,
    'grid.alpha': 0.3,
    'xtick.labelsize': 8,
    'ytick.labelsize': 8,
    'axes.labelsize': 10,
    'legend.fontsize': 8,
})

RESULT_DIR = "experiments/results"
FIGURE_DIR = "experiments/figures"
os.makedirs(FIGURE_DIR, exist_ok=True)

# Colorblind-safe palette matching IEEE standard styling
PALETTE = {
    "FCFS_TRANSFER": "#d7191c",
    "ELAS_TRANSFER": "#fdae61",
    "ELAS_OFFLOAD": "#abd9e9",
    "ELAS_DCMD": "#2c7bb6",
    "F_ELAS_DCMD": "#31a354"
}

def plot_line_sweep(csv_path, sweep_var, x_label, y_label, title, output_name):
    """Plots a general line chart comparing strategies over a sweep variable."""
    if not os.path.exists(csv_path):
        print(f"Warning: CSV file {csv_path} not found. Skipping plot.")
        return
        
    df = pd.read_csv(csv_path)
    
    plt.figure(figsize=(4.5, 3.2))
    
    # Sort strategies to match consistent order
    strategies = df["strategy"].unique()
    
    for strat in strategies:
        strat_df = df[df["strategy"] == strat].sort_values("sweep_value")
        color = PALETTE.get(strat, "#333333")
        plt.plot(strat_df["sweep_value"], strat_df["total_sim_time_ms"], 
                 marker='o', markersize=4, label=strat, color=color, linewidth=1.5)
        
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(title, fontsize=10)
    plt.legend(loc='upper right', framealpha=0.9)
    plt.tight_layout()
    
    plt.savefig(os.path.join(FIGURE_DIR, f"{output_name}.png"), dpi=300)
    plt.savefig(os.path.join(FIGURE_DIR, f"{output_name}.pdf"), dpi=300)
    plt.close()
    print(f"Saved figure: {output_name}")

def plot_cdf_latencies():
    """Generates the CDF of request latencies under default parameters to evaluate tail latency."""
    num_experts = 16
    trace_path = "traces/workload_E16_alpha1.2.csv"
    if not os.path.exists(trace_path):
        print("Warning: Default trace not found. Skipping CDF plot.")
        return

    # Define strategies
    strategies_to_test = [
        ("fcfs", "transfer", "FCFS_TRANSFER"),
        ("elas", "transfer", "ELAS_TRANSFER"),
        ("elas", "offload", "ELAS_OFFLOAD"),
        ("elas", "dcmd", "ELAS_DCMD"),
        ("f-elas", "dcmd", "F_ELAS_DCMD")
    ]

    plt.figure(figsize=(4.5, 3.2))

    for sched, offload, name in strategies_to_test:
        # Re-load trace requests
        requests = WorkloadGenerator.load_trace_file(trace_path)
        
        sim = HMCSimulator(
            gpu_cache_capacity=4,
            pcie_bandwidth_gbs=16.0,
            cpu_expert_latency_ms=4.0,
            gpu_expert_latency_ms=0.2,
            gpu_dense_latency_ms=0.5,
            cpu_orch_latency_ms=0.1,
            pcie_dma_channels=2,
            cpu_cores=8,
            scheduling_policy=sched,
            offloading_policy=offload,
            cache_policy="lru",
            starvation_threshold=5,
            sched_epoch=0.1
        )
        sim.run(requests)
        
        # Calculate end-to-end request latencies
        latencies = [r.finish_time - r.arrival_time for r in sim.completed_requests]
        latencies_sorted = np.sort(latencies)
        p = np.linspace(0.0, 1.0, len(latencies_sorted))
        
        color = PALETTE.get(name, "#333333")
        plt.plot(latencies_sorted, p, label=name, color=color, linewidth=1.5)

    plt.xlabel("Request Latency (ms)")
    plt.ylabel("Cumulative Probability")
    plt.title("CDF of Request Latency (C=4, BW=16GB/s)", fontsize=10)
    plt.legend(loc='lower right')
    plt.tight_layout()
    
    plt.savefig(os.path.join(FIGURE_DIR, "fig7_latency_cdf.png"), dpi=300)
    plt.savefig(os.path.join(FIGURE_DIR, "fig7_latency_cdf.pdf"), dpi=300)
    plt.close()
    print("Saved figure: fig7_latency_cdf")

def plot_gem5_microarch():
    """Generates grouped bar charts comparing cycles and cache misses for both ARM and RISC-V."""
    csv_arm = os.path.join(RESULT_DIR, "gem5/gem5_summary.csv")
    csv_riscv = os.path.join(RESULT_DIR, "gem5_riscv/gem5_riscv_summary.csv")
    
    if not os.path.exists(csv_arm) or not os.path.exists(csv_riscv):
        print(f"Warning: Summary CSVs not found. Skipping gem5 plot.")
        return
        
    df_arm = pd.read_csv(csv_arm)
    df_riscv = pd.read_csv(csv_riscv)
    
    # Filter for default cache configuration (L1D=64kB, L2=512kB)
    df_arm_def = df_arm[(df_arm["l1d_size"] == "64kB") & (df_arm["l2_size"] == "512kB")]
    df_riscv_def = df_riscv[(df_riscv["l1d_size"] == "64kB") & (df_riscv["l2_size"] == "512kB")]
    
    if df_arm_def.empty or df_riscv_def.empty:
        print("Warning: No matching default gem5 cache sizes (64kB/512kB). Skipping plot.")
        return
        
    # Order by queue type
    df_arm_def = df_arm_def.set_index("queue_type").reindex(["centralized", "distributed", "lock_free"]).reset_index()
    df_riscv_def = df_riscv_def.set_index("queue_type").reindex(["centralized", "distributed", "lock_free"]).reset_index()
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(7.2, 3.2))
    
    x = np.arange(3)
    width = 0.35
    
    # 1. Cycles comparison (ARM vs RISC-V)
    bars_arm_c = ax1.bar(x - width/2, df_arm_def["cpu_cycles"] / 1e6, width, label="ARMv8-A",
                         color="#2c7bb6", edgecolor='black')
    bars_riscv_c = ax1.bar(x + width/2, df_riscv_def["cpu_cycles"] / 1e6, width, label="RISC-V (RV64)",
                           color="#fdae61", edgecolor='black')
    
    ax1.set_ylabel("CPU Cycles (Millions)")
    ax1.set_title("CPU Cycles Comparison")
    ax1.set_xticks(x)
    ax1.set_xticklabels(["Centralized", "Distributed", "Lock-Free"], fontsize=8)
    ax1.legend(loc="upper right")
    
    # Add values on top of bars
    for bar in bars_arm_c:
        yval = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width()/2.0, yval + 0.05, f"{yval:.2f}M", 
                 ha='center', va='bottom', fontsize=7, weight='bold')
    for bar in bars_riscv_c:
        yval = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width()/2.0, yval + 0.05, f"{yval:.2f}M", 
                 ha='center', va='bottom', fontsize=7, weight='bold')

    # 2. L2 Cache Misses comparison (ARM vs RISC-V)
    bars_arm_m = ax2.bar(x - width/2, df_arm_def["l2cache_misses"], width, label="ARMv8-A L2",
                         color="#abd9e9", edgecolor='black')
    bars_riscv_m = ax2.bar(x + width/2, df_riscv_def["l2cache_misses"], width, label="RISC-V L2",
                           color="#d7191c", edgecolor='black')
    
    ax2.set_ylabel("L2 Cache Misses")
    ax2.set_title("L2 Cache Performance")
    ax2.set_xticks(x)
    ax2.set_xticklabels(["Centralized", "Distributed", "Lock-Free"], fontsize=8)
    ax2.legend(loc="upper right")
    
    for bar in bars_arm_m:
        yval = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2.0, yval + 50, f"{int(yval)}", 
                 ha='center', va='bottom', fontsize=7, weight='bold')
    for bar in bars_riscv_m:
        yval = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2.0, yval + 50, f"{int(yval)}", 
                 ha='center', va='bottom', fontsize=7, weight='bold')
                 
    plt.suptitle("Microarchitectural Queue Comparison: ARMv8-A vs. RISC-V (gem5)", fontsize=10, weight='bold')
    plt.tight_layout()
    
    plt.savefig(os.path.join(FIGURE_DIR, "fig8_gem5_microarch.png"), dpi=300)
    plt.savefig(os.path.join(FIGURE_DIR, "fig8_gem5_microarch.pdf"), dpi=300)
    plt.close()
    print("Saved figure: fig8_gem5_microarch")

def main():
    print("\n=============================================")
    print("        Generating IEEE Figures...")
    print("=============================================")
    
    # Fig 2: Cache capacity sweep
    plot_line_sweep(
        csv_path=os.path.join(RESULT_DIR, "exp1_cache_sweep.csv"),
        sweep_var="sweep_value",
        x_label="GPU Expert Cache Capacity (C)",
        y_label="Total Simulation Time (ms)",
        title="Cache Capacity vs. Simulation Time (BW=16GB/s)",
        output_name="fig2_cache_sweep"
    )
    
    # Fig 3: PCIe bandwidth sweep
    plot_line_sweep(
        csv_path=os.path.join(RESULT_DIR, "exp2_pcie_sweep.csv"),
        sweep_var="sweep_value",
        x_label="PCIe Bandwidth (GB/s)",
        y_label="Total Simulation Time (ms)",
        title="PCIe Bandwidth vs. Simulation Time (C=4)",
        output_name="fig3_pcie_sweep"
    )

    # Fig 4: Concurrency scaling
    plot_line_sweep(
        csv_path=os.path.join(RESULT_DIR, "exp3_concurrency.csv"),
        sweep_var="sweep_value",
        x_label="Number of Concurrent Requests",
        y_label="Total Simulation Time (ms)",
        title="Concurrency Scaling (C=4, BW=16GB/s)",
        output_name="fig4_concurrency"
    )

    # Fig 5: Zipf alpha sensitivity
    plot_line_sweep(
        csv_path=os.path.join(RESULT_DIR, "exp4_zipf_alpha.csv"),
        sweep_var="sweep_value",
        x_label="Zipf Alpha (Distribution Skewness)",
        y_label="Total Simulation Time (ms)",
        title="Workload Skewness Sensitivity (C=4, BW=16GB/s)",
        output_name="fig5_zipf_alpha"
    )

    # Fig 6: CPU cores scaling
    plot_line_sweep(
        csv_path=os.path.join(RESULT_DIR, "exp5_cpu_cores.csv"),
        sweep_var="sweep_value",
        x_label="Number of CPU Cores",
        y_label="Total Simulation Time (ms)",
        title="CPU Core Scaling (C=4, BW=16GB/s)",
        output_name="fig6_cpu_cores"
    )

    # Fig 7: Latency CDF
    plot_cdf_latencies()

    # Fig 8: gem5 microarchitectural queues
    plot_gem5_microarch()

if __name__ == "__main__":
    main()
