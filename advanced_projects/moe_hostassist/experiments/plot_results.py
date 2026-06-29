# plot_results.py
# Generates high-quality publication-ready academic figures from the simulation results.

import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

RESULTS_DIR = "results"
FIGURES_DIR = "figures"
os.makedirs(FIGURES_DIR, exist_ok=True)

# Set style for academic publications
sns.set_theme(style="ticks")
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'figure.titlesize': 14,
    'pdf.fonttype': 42,
    'ps.fonttype': 42
})

def plot_fig1_bottleneck_breakdown():
    print(">>> Plotting Figure 1: Bottleneck Breakdown vs Concurrency...")
    csv_path = os.path.join(RESULTS_DIR, "e1_bottleneck_interaction.csv")
    if not os.path.exists(csv_path):
        print(f"Error: {csv_path} not found.")
        return
        
    df = pd.read_csv(csv_path)
    
    # Select breakdown categories
    categories = [
        "host_scheduling_ms", "expert_cache_miss_ms", "activation_transfer_ms",
        "queue_wait_ms", "gpu_bubble_ms", "cpu_stall_ms", "gpu_exec_ms", "cpu_exec_ms"
    ]
    
    labels = [
        "Host Scheduling", "Cache Miss Mgmt", "PCIe Transfer",
        "GPU Queue Wait", "GPU Bubble (PCIe wait)", "CPU Stall",
        "GPU Exec", "CPU Exec"
    ]
    
    # Normalize breakdowns to percent for percentage breakdown plot, and also make absolute latency plot
    df_percent = df.copy()
    total = df_percent[categories].sum(axis=1)
    for col in categories:
        df_percent[col] = (df_percent[col] / total) * 100.0
        
    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    
    # Color palette
    colors = sns.color_palette("tab10", len(categories))
    
    # Subplot 1: Absolute Latency (Log Scale)
    bottom = np.zeros(len(df))
    for i, col in enumerate(categories):
        axes[0].bar(df["concurrency"].astype(str), df[col], bottom=bottom, label=labels[i], color=colors[i])
        bottom += df[col]
    axes[0].plot(df["concurrency"].astype(str), df["e2e_latency_ms"], color="black", marker="o", linestyle="--", label="Total E2E Latency")
    axes[0].set_yscale("log")
    axes[0].set_xlabel("Concurrency (Concurrent Streams)")
    axes[0].set_ylabel("Mean Latency (ms) [Log Scale]")
    axes[0].set_title("Absolute Latency Breakdown")
    axes[0].legend(loc="upper left", frameon=True, fontsize=9)
    
    # Subplot 2: Relative Percentage Breakdown
    bottom = np.zeros(len(df_percent))
    for i, col in enumerate(categories):
        axes[1].bar(df_percent["concurrency"].astype(str), df_percent[col], bottom=bottom, label=labels[i], color=colors[i])
        bottom += df_percent[col]
    axes[1].set_xlabel("Concurrency (Concurrent Streams)")
    axes[1].set_ylabel("Percentage of Total E2E Latency (%)")
    axes[1].set_title("Relative Latency Breakdown")
    axes[1].set_ylim(0, 100)
    
    plt.tight_layout()
    plt.savefig(os.path.join(FIGURES_DIR, "fig1_bottleneck_breakdown.png"), dpi=300)
    plt.savefig(os.path.join(FIGURES_DIR, "fig1_bottleneck_breakdown.pdf"), dpi=300)
    plt.close()

def plot_fig2_dispatch_jitter():
    print(">>> Plotting Figure 2: Dispatch Jitter vs GPU Bubble...")
    csv_path = os.path.join(RESULTS_DIR, "e3_dispatch_jitter.csv")
    if not os.path.exists(csv_path):
        print(f"Error: {csv_path} not found.")
        return
        
    df = pd.read_csv(csv_path)
    
    plt.figure(figsize=(8, 6))
    concurrencies = sorted(df["concurrency"].unique())
    colors = sns.color_palette("Set1", len(concurrencies))
    
    for i, N in enumerate(concurrencies):
        sub_df = df[df["concurrency"] == N].sort_values("dispatch_jitter_us")
        plt.plot(sub_df["dispatch_jitter_us"], sub_df["gpu_bubble_ms"], 
                 marker="o", linestyle="-", label=f"N = {N} Streams", color=colors[i])
                 
    plt.xlabel("Host Dispatch Jitter (us)")
    plt.ylabel("Mean GPU Idle Bubble (ms)")
    plt.title("Dispatch Jitter to GPU Idle Bubble Transfer Function")
    plt.grid(True, linestyle=":", alpha=0.6)
    plt.legend(frameon=True)
    
    plt.tight_layout()
    plt.savefig(os.path.join(FIGURES_DIR, "fig2_dispatch_jitter.png"), dpi=300)
    plt.savefig(os.path.join(FIGURES_DIR, "fig2_dispatch_jitter.pdf"), dpi=300)
    plt.close()

def plot_fig3_prefetch_benefit():
    print(">>> Plotting Figure 3: Prefetch Benefit vs Accuracy...")
    csv_path = os.path.join(RESULTS_DIR, "e4_prefetch_tradeoff.csv")
    if not os.path.exists(csv_path):
        print(f"Error: {csv_path} not found.")
        return
        
    df = pd.read_csv(csv_path)
    
    plt.figure(figsize=(8, 6))
    windows = sorted(df["prefetch_window"].unique())
    colors = sns.color_palette("muted", len(windows))
    
    # Plot baseline E2E without prefetch (which is equivalent to W=1, P=0.0)
    baseline_e2e = df[(df["prefetch_window"] == 1) & (df["prediction_accuracy"] == 0.0)]["e2e_latency_ms"].values[0]
    plt.axhline(y=baseline_e2e, color="black", linestyle="--", label="Reactive baseline (No prefetch)")
    
    for i, W in enumerate(windows):
        sub_df = df[df["prefetch_window"] == W].sort_values("prediction_accuracy")
        plt.plot(sub_df["prediction_accuracy"] * 100.0, sub_df["e2e_latency_ms"], 
                 marker="s", linestyle="-", label=f"Prefetch Window W = {W}", color=colors[i])
                 
    plt.xlabel("Expert Selection Prediction Accuracy P (%)")
    plt.ylabel("Mean E2E Request Latency (ms)")
    plt.title("Prefetching Benefit vs. Prediction Accuracy")
    
    # Highlight the P_min threshold region (where prefetching outperforms reactive baseline)
    plt.axvspan(50.0, 65.0, color="green", alpha=0.15, label="P_min Threshold Region (50-65%)")
    
    plt.grid(True, linestyle=":", alpha=0.6)
    plt.legend(frameon=True)
    
    plt.tight_layout()
    plt.savefig(os.path.join(FIGURES_DIR, "fig3_prefetch_benefit.png"), dpi=300)
    plt.savefig(os.path.join(FIGURES_DIR, "fig3_prefetch_benefit.pdf"), dpi=300)
    plt.close()

def plot_fig4_pareto_frontier():
    print(">>> Plotting Figure 4: Priority Scheduling Pareto Frontier...")
    csv_path = os.path.join(RESULTS_DIR, "e5_priority_scheduling.csv")
    if not os.path.exists(csv_path):
        print(f"Error: {csv_path} not found.")
        return
        
    df = pd.read_csv(csv_path)
    
    # We will plot for agentic fraction = 0.25 (typical concurrent mix)
    sub_df = df[df["agentic_fraction"] == 0.25]
    
    plt.figure(figsize=(8, 6))
    
    # Define markers and colors for policies
    policies = sub_df["policy"].unique()
    colors = sns.color_palette("Set2", len(policies))
    markers = ["o", "v", "^", "s", "D", "*"]
    
    for i, policy in enumerate(policies):
        p_df = sub_df[sub_df["policy"] == policy]
        plt.scatter(p_df["batch_throughput_tps"], p_df["agentic_p99_latency_ms"], 
                    s=120, color=colors[i], marker=markers[i % len(markers)], 
                    label=policy, edgecolors='black', linewidths=1.2)
                    
    plt.xlabel("Batch Stream Throughput (req/sec)")
    plt.ylabel("Agentic Stream P99 Latency (ms)")
    plt.title("Scheduling Policy Pareto Frontier (Agentic Fraction = 25%)")
    plt.grid(True, linestyle=":", alpha=0.6)
    plt.legend(frameon=True)
    
    plt.tight_layout()
    plt.savefig(os.path.join(FIGURES_DIR, "fig4_pareto_frontier.png"), dpi=300)
    plt.savefig(os.path.join(FIGURES_DIR, "fig4_pareto_frontier.pdf"), dpi=300)
    plt.close()

def main():
    print("============================================================")
    print("             Plotting MoE-HostAssist Figures")
    print("============================================================")
    
    plot_fig1_bottleneck_breakdown()
    plot_fig2_dispatch_jitter()
    plot_fig3_prefetch_benefit()
    plot_fig4_pareto_frontier()
    
    print("============================================================")
    print("             All Publication Figures Generated")
    print("============================================================")

if __name__ == "__main__":
    main()
