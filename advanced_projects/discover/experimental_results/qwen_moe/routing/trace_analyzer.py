import os
import json
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import argparse

def calculate_gini(array):
    """Calculate the Gini coefficient of a numpy array (skewness measure)."""
    # Array cannot be empty and must be 1D
    if len(array) == 0:
        return 0.0
    array = np.array(array, dtype=np.float64)
    if np.sum(array) == 0:
        return 0.0
    # Sort the array
    array = np.sort(array)
    index = np.arange(1, array.shape[0] + 1)
    n = array.shape[0]
    return ((2 * index - n - 1) * array).sum() / (n * array.sum())

def analyze_trace(args):
    trace_file = args.trace_file
    output_dir = args.output_dir
    os.makedirs(output_dir, exist_ok=True)
    
    if not os.path.exists(trace_file):
        raise FileNotFoundError(f"Trace file {trace_file} does not exist.")
        
    df = pd.read_csv(trace_file)
    print(f"Loaded routing trace: {len(df)} rows")
    
    num_layers = df["layer_id"].nunique()
    num_experts = df["expert_id"].nunique()
    print(f"Trace contains {num_layers} layers and {num_experts} experts.")
    
    # 1. Expert frequency distribution & Gini coefficient
    exp_freqs = df.groupby(["layer_id", "expert_id"]).size().unstack(fill_value=0)
    
    layer_gini = {}
    layer_entropy = {}
    for layer in range(num_layers):
        if layer in exp_freqs.index:
            freq = exp_freqs.loc[layer].values
            p = freq / (freq.sum() + 1e-9)
            # Entropy
            entropy = -np.sum(p * np.log2(p + 1e-9))
            layer_entropy[int(layer)] = float(entropy)
            # Gini
            gini = calculate_gini(freq)
            layer_gini[int(layer)] = float(gini)
        else:
            layer_entropy[int(layer)] = 0.0
            layer_gini[int(layer)] = 0.0
            
    # 2. Reuse Distance
    # Calculate per-layer, per-request expert activation reuse distance
    reuse_distances = []
    
    for (req_id, layer_id), group in df.groupby(["request_id", "layer_id"]):
        # Sort by token_id and topk_rank
        group = group.sort_values(by=["token_id", "topk_rank"])
        # For each expert, track their last seen token_id
        last_seen = {}
        for idx, row in group.iterrows():
            token_id = row["token_id"]
            exp_id = row["expert_id"]
            if exp_id in last_seen:
                dist = token_id - last_seen[exp_id] - 1
                reuse_distances.append(dist)
            last_seen[exp_id] = token_id
            
    avg_reuse_dist = float(np.mean(reuse_distances)) if reuse_distances else 0.0
    median_reuse_dist = float(np.median(reuse_distances)) if reuse_distances else 0.0
    
    # 3. Consecutive token expert overlap
    # Percentage of experts shared between token t and t+1
    overlaps = []
    for (req_id, layer_id), group in df.groupby(["request_id", "layer_id"]):
        group = group.sort_values(by=["token_id", "topk_rank"])
        # Group by token_id to get set of experts per token
        tok_experts = group.groupby("token_id")["expert_id"].apply(set).sort_index()
        tokens = tok_experts.index.tolist()
        for i in range(len(tokens) - 1):
            t1 = tokens[i]
            t2 = tokens[i+1]
            exp1 = tok_experts[t1]
            exp2 = tok_experts[t2]
            overlap_ratio = len(exp1.intersection(exp2)) / len(exp1)
            overlaps.append(overlap_ratio)
            
    avg_consecutive_overlap = float(np.mean(overlaps)) if overlaps else 0.0
    
    # 4. Top-K transition matrix
    # Probability of transitioning from expert A (at step t) to expert B (at step t+1) in the same layer
    transition_matrix = np.zeros((num_experts, num_experts))
    for (req_id, layer_id), group in df.groupby(["request_id", "layer_id"]):
        group = group.sort_values(by=["token_id", "topk_rank"])
        tok_experts = group.groupby("token_id")["expert_id"].apply(list).sort_index()
        tokens = tok_experts.index.tolist()
        for i in range(len(tokens) - 1):
            exp1_list = tok_experts[tokens[i]]
            exp2_list = tok_experts[tokens[i+1]]
            # We add transition counts for all pairs of consecutive step experts
            for e1 in exp1_list:
                for e2 in exp2_list:
                    transition_matrix[e1, e2] += 1
                    
    # Normalize rows
    row_sums = transition_matrix.sum(axis=1, keepdims=True)
    transition_prob = np.where(row_sums > 0, transition_matrix / row_sums, 0.0)
    
    # 5. Hot expert ratio: fraction of experts that account for 80% of activations
    total_activations = len(df)
    exp_counts = df["expert_id"].value_counts().sort_values(ascending=False)
    cum_sum = exp_counts.cumsum().values
    hot_expert_count = np.searchsorted(cum_sum, 0.8 * total_activations) + 1
    hot_expert_ratio = float(hot_expert_count / num_experts)
    
    # 6. Routing stability over decoding steps
    # Measured by consecutive overlap rolling average
    routing_stability = avg_consecutive_overlap
    
    # Compile summary dict
    analysis_results = {
        "layer_entropy": layer_entropy,
        "layer_gini": layer_gini,
        "average_reuse_distance": avg_reuse_dist,
        "median_reuse_distance": median_reuse_dist,
        "consecutive_overlap": avg_consecutive_overlap,
        "hot_expert_ratio": hot_expert_ratio,
        "routing_stability": routing_stability,
        "gini_coefficient": float(calculate_gini(df["expert_id"].value_counts().values))
    }
    
    # Save results as JSON
    with open(os.path.join(output_dir, "trace_analysis.json"), "w") as f:
        json.dump(analysis_results, f, indent=4)
        
    print("\n--- Trace Analysis Results ---")
    print(f"Gini Coefficient (Global Skew): {analysis_results['gini_coefficient']:.4f}")
    print(f"Avg Reuse Distance: {analysis_results['average_reuse_distance']:.2f}")
    print(f"Median Reuse Distance: {analysis_results['median_reuse_distance']:.1f}")
    print(f"Avg Consecutive Expert Overlap: {analysis_results['consecutive_overlap']:.4f}")
    print(f"Hot Expert Ratio (80% actions): {analysis_results['hot_expert_ratio']:.4f}")
    
    # Plotting
    # 1. Expert Frequency Distribution per Layer (Stacked Bar)
    plt.figure(figsize=(10, 6))
    exp_freqs.T.plot(kind="bar", stacked=True, colormap="tab20", ax=plt.gca())
    plt.title("Expert Activation Frequency per Layer")
    plt.xlabel("Expert ID")
    plt.ylabel("Activation Count")
    plt.legend(title="Layer ID", bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, "expert_frequency.png"))
    plt.close()
    
    # 2. Layer Entropy and Gini
    fig, ax1 = plt.subplots(figsize=(8, 4))
    layers = list(layer_entropy.keys())
    entropies = list(layer_entropy.values())
    ginis = list(layer_gini.values())
    
    color = 'tab:blue'
    ax1.set_xlabel('Layer ID')
    ax1.set_ylabel('Entropy (bits)', color=color)
    ax1.plot(layers, entropies, color=color, marker='o', label='Entropy')
    ax1.tick_params(axis='y', labelcolor=color)
    
    ax2 = ax1.twinx()
    color = 'tab:red'
    ax2.set_ylabel('Gini Coefficient', color=color)
    ax2.plot(layers, ginis, color=color, marker='s', linestyle='--', label='Gini')
    ax2.tick_params(axis='y', labelcolor=color)
    
    plt.title("Entropy and Gini Coefficient per Layer")
    fig.tight_layout()
    plt.savefig(os.path.join(output_dir, "layer_metrics.png"))
    plt.close()
    
    # 3. Reuse Distance Histogram
    if reuse_distances:
        plt.figure(figsize=(8, 4))
        max_dist = int(np.max(reuse_distances))
        plt.hist(reuse_distances, bins=range(0, min(50, max_dist + 2)), color='purple', edgecolor='black', alpha=0.7)
        plt.title("Expert Reuse Distance Distribution")
        plt.xlabel("Reuse Distance (tokens)")
        plt.ylabel("Frequency")
        plt.grid(axis='y', linestyle='--', alpha=0.7)
        plt.tight_layout()
        plt.savefig(os.path.join(output_dir, "reuse_distance.png"))
        plt.close()
        
    # 4. Transition Heatmap
    plt.figure(figsize=(8, 6))
    plt.imshow(transition_prob, cmap="YlOrRd", interpolation="nearest")
    plt.colorbar(label="Transition Probability")
    plt.title("Expert Transition Probability Matrix")
    plt.xlabel("Next Expert ID")
    plt.ylabel("Current Expert ID")
    plt.xticks(range(num_experts))
    plt.yticks(range(num_experts))
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, "transition_heatmap.png"))
    plt.close()
    
    print(f"Plots saved to {output_dir}")
    return analysis_results

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--trace_file", type=str, required=True)
    parser.add_argument("--output_dir", type=str, default="/home/a/discover/routing")
    args = parser.parse_args()
    
    analyze_trace(args)
