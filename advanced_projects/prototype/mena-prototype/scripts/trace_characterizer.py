#!/usr/bin/env python3
"""
trace_characterizer.py — Full characterization of MoE routing traces.

Analyzes JSONL traces and computes:
  1. Expert Popularity Histogram (per-expert token count)
  2. Reuse Distance: min/max/p50/p95
  3. Expert Transition Matrix (N×N)
  4. Hot Expert Ratio (top-20% experts traffic fraction)
  5. Per-layer Routing Entropy (Shannon entropy of expert selection)
  6. Routing Locality Score (fraction of consecutive same-expert accesses)

Saves results to docs/trace_characterization.md
"""
import os
import sys
import json
import math
import glob
import numpy as np
from collections import defaultdict, OrderedDict
from typing import Any

TRACES_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "traces"))
REPORT_PATH = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", "docs", "trace_characterization.md")
)


# ---------------------------------------------------------------------------
# Trace loader
# ---------------------------------------------------------------------------
def load_trace(path: str) -> list[dict]:
    """Load JSONL trace. Each record: {layer, token, topk_experts, ...}"""
    records = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                records.append(json.loads(line))
    return records


# ---------------------------------------------------------------------------
# 1. Expert Popularity Histogram
# ---------------------------------------------------------------------------
def expert_popularity(records: list[dict]) -> dict[int, int]:
    """Count total tokens routed to each expert (each topk mention = 1 token)."""
    counts: dict[int, int] = defaultdict(int)
    for r in records:
        for exp in r["topk_experts"]:
            counts[exp] += 1
    return dict(sorted(counts.items()))


# ---------------------------------------------------------------------------
# 2. Reuse Distance
# ---------------------------------------------------------------------------
def compute_reuse_distances(access_seq: list[int]) -> list[int]:
    """
    Stack-distance (unique-expert reuse distance) for each access.
    Returns list of finite distances (cold-miss accesses omitted).
    Uses O(n²) approach – fine for trace sizes here.
    """
    distances: list[int] = []
    seen_order: list[int] = []   # ordered history of recent accesses (no dups)

    for exp in access_seq:
        if exp in seen_order:
            idx  = seen_order.index(exp)
            dist = len(set(seen_order[idx:]))    # unique experts since last access
            distances.append(dist - 1)           # exclude self
            seen_order.remove(exp)
        seen_order.append(exp)

    return distances


def reuse_distance_stats(distances: list[int]) -> dict:
    if not distances:
        return {"min": None, "max": None, "p50": None, "p95": None, "mean": None, "n": 0}
    return {
        "min":  int(np.min(distances)),
        "max":  int(np.max(distances)),
        "p50":  float(np.percentile(distances, 50)),
        "p95":  float(np.percentile(distances, 95)),
        "mean": float(np.mean(distances)),
        "n":    len(distances),
    }


# ---------------------------------------------------------------------------
# 3. Expert Transition Matrix
# ---------------------------------------------------------------------------
def transition_matrix(access_seq: list[int], num_experts: int) -> np.ndarray:
    """NxN matrix: mat[i][j] = times expert j followed expert i."""
    mat = np.zeros((num_experts, num_experts), dtype=int)
    for a, b in zip(access_seq[:-1], access_seq[1:]):
        if 0 <= a < num_experts and 0 <= b < num_experts:
            mat[a][b] += 1
    return mat


# ---------------------------------------------------------------------------
# 4. Hot Expert Ratio
# ---------------------------------------------------------------------------
def hot_expert_ratio(popularity: dict[int, int]) -> tuple[float, float]:
    """
    Returns (hot_expert_fraction, traffic_fraction).
    hot_expert_fraction = top-20% of experts by count (ceil).
    traffic_fraction    = their share of total traffic.
    """
    if not popularity:
        return 0.0, 0.0
    total_experts = len(popularity)
    top_k = max(1, math.ceil(total_experts * 0.20))
    sorted_counts = sorted(popularity.values(), reverse=True)
    top_traffic   = sum(sorted_counts[:top_k])
    total_traffic = sum(sorted_counts)
    return top_k / total_experts, top_traffic / total_traffic if total_traffic else 0.0


# ---------------------------------------------------------------------------
# 5. Per-layer Routing Entropy
# ---------------------------------------------------------------------------
def per_layer_entropy(records: list[dict]) -> dict[int, float]:
    """Shannon entropy of expert selection distribution per layer."""
    layer_counts: dict[int, dict[int, int]] = defaultdict(lambda: defaultdict(int))
    for r in records:
        for exp in r["topk_experts"]:
            layer_counts[r["layer"]][exp] += 1

    entropies = {}
    for layer, counts in sorted(layer_counts.items()):
        total = sum(counts.values())
        probs = np.array(list(counts.values()), dtype=float) / total
        probs = probs[probs > 0]
        entropies[layer] = float(-np.sum(probs * np.log2(probs)))
    return entropies


# ---------------------------------------------------------------------------
# 6. Routing Locality Score
# ---------------------------------------------------------------------------
def routing_locality(access_seq: list[int]) -> float:
    """Fraction of consecutive accesses targeting the same expert."""
    if len(access_seq) < 2:
        return 0.0
    same = sum(1 for a, b in zip(access_seq[:-1], access_seq[1:]) if a == b)
    return same / (len(access_seq) - 1)


# ---------------------------------------------------------------------------
# Characterize one trace file
# ---------------------------------------------------------------------------
def characterize(path: str) -> dict[str, Any]:
    records = load_trace(path)
    if not records:
        return {"error": "empty trace"}

    num_experts = max(
        exp for r in records for exp in r["topk_experts"]
    ) + 1

    # Flat access sequence (all layers interleaved in file order)
    access_seq = [exp for r in records for exp in r["topk_experts"]]

    # 1. Popularity
    popularity = expert_popularity(records)

    # 2. Reuse distance
    rd = compute_reuse_distances(access_seq)
    rd_stats = reuse_distance_stats(rd)

    # 3. Transition matrix
    tmat = transition_matrix(access_seq, num_experts)

    # Top-5 transitions
    flat_tmat = [
        (tmat[i, j], i, j) for i in range(num_experts) for j in range(num_experts) if tmat[i, j] > 0
    ]
    top5_trans = sorted(flat_tmat, reverse=True)[:5]

    # 4. Hot expert ratio
    hot_frac, hot_traffic = hot_expert_ratio(popularity)

    # 5. Per-layer entropy
    layer_ent = per_layer_entropy(records)

    # 6. Locality
    locality = routing_locality(access_seq)

    return {
        "path":           path,
        "filename":       os.path.basename(path),
        "num_records":    len(records),
        "num_experts":    num_experts,
        "total_accesses": len(access_seq),
        "popularity":     popularity,
        "rd_stats":       rd_stats,
        "tmat":           tmat,
        "top5_trans":     top5_trans,
        "hot_expert_frac": hot_frac,
        "hot_traffic_frac": hot_traffic,
        "layer_entropies": layer_ent,
        "locality_score": locality,
    }


# ---------------------------------------------------------------------------
# Markdown report
# ---------------------------------------------------------------------------
def format_tmat(tmat: np.ndarray, n: int) -> str:
    """Format transition matrix as a compact markdown table."""
    lines = []
    header = "| from\\to | " + " | ".join(str(j) for j in range(n)) + " |"
    sep    = "| :---: | " + " | ".join(":---:" for _ in range(n)) + " |"
    lines.append(header)
    lines.append(sep)
    for i in range(n):
        row = [f"**{i}**"] + [str(tmat[i, j]) for j in range(n)]
        lines.append("| " + " | ".join(row) + " |")
    return "\n".join(lines)


def build_report(all_results: list[dict]) -> str:
    md = []
    md.append("# MoE Trace Characterization Report\n")
    md.append("Traces analyzed from `traces/` directory.\n")

    # --- Summary table ---
    md.append("## Summary Table\n")
    md.append("| Trace | Experts | Accesses | RD p50 | RD p95 | Hot 20% Traffic | Locality | Avg Entropy |")
    md.append("| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |")
    for res in all_results:
        if "error" in res:
            md.append(f"| {res.get('filename','?')} | — | — | — | — | — | — | — |")
            continue
        rd    = res["rd_stats"]
        entrs = list(res["layer_entropies"].values())
        avg_e = f"{np.mean(entrs):.2f}" if entrs else "—"
        md.append(
            f"| {res['filename']} | {res['num_experts']} | {res['total_accesses']} | "
            f"{rd['p50']:.1f} | {rd['p95']:.1f} | "
            f"{res['hot_traffic_frac']:.1%} | {res['locality_score']:.3f} | {avg_e} |"
        )
    md.append("")

    # --- Per-trace detailed sections ---
    for res in all_results:
        if "error" in res:
            continue
        md.append(f"---\n\n## {res['filename']}\n")
        md.append(f"- **Records**: {res['num_records']}  (tokens × layers)")
        md.append(f"- **Num experts**: {res['num_experts']}")
        md.append(f"- **Total expert accesses**: {res['total_accesses']}\n")

        # 1. Popularity
        md.append("### 1. Expert Popularity Histogram\n")
        pop = res["popularity"]
        total_acc = res["total_accesses"]
        md.append("| Expert | Count | Fraction |")
        md.append("| :---: | :---: | :---: |")
        for exp, cnt in sorted(pop.items(), key=lambda x: -x[1]):
            md.append(f"| {exp} | {cnt} | {cnt/total_acc:.1%} |")
        md.append("")

        # 2. Reuse distance
        md.append("### 2. Reuse Distance Statistics\n")
        rd = res["rd_stats"]
        md.append(f"| min | mean | p50 | p95 | max | n |")
        md.append(f"| :---: | :---: | :---: | :---: | :---: | :---: |")
        md.append(
            f"| {rd['min']} | {rd['mean']:.1f} | {rd['p50']:.1f} | "
            f"{rd['p95']:.1f} | {rd['max']} | {rd['n']} |"
        )
        md.append("")

        # 3. Transition matrix
        md.append("### 3. Expert Transition Matrix\n")
        md.append("*(row = from-expert, col = to-expert, value = count)*\n")
        n = res["num_experts"]
        md.append(format_tmat(res["tmat"], n))
        md.append("")
        md.append("**Top-5 transitions:**")
        for cnt, src, dst in res["top5_trans"]:
            md.append(f"  - Expert {src} → Expert {dst}: {cnt} times")
        md.append("")

        # 4. Hot expert ratio
        md.append("### 4. Hot Expert Ratio\n")
        md.append(
            f"- Top-**{res['hot_expert_frac']:.0%}** of experts "
            f"handle **{res['hot_traffic_frac']:.1%}** of total traffic."
        )
        md.append("")

        # 5. Per-layer entropy
        md.append("### 5. Per-Layer Routing Entropy\n")
        md.append("| Layer | Entropy (bits) |")
        md.append("| :---: | :---: |")
        for layer, ent in res["layer_entropies"].items():
            md.append(f"| {layer} | {ent:.4f} |")
        md.append("")

        # 6. Locality score
        md.append("### 6. Routing Locality Score\n")
        md.append(
            f"Fraction of consecutive accesses targeting the same expert: "
            f"**{res['locality_score']:.4f}** ({res['locality_score']:.1%})\n"
        )

    return "\n".join(md)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    trace_files = sorted(glob.glob(os.path.join(TRACES_DIR, "*.jsonl")))
    if not trace_files:
        print(f"No JSONL files found in {TRACES_DIR}")
        sys.exit(1)

    print(f"Found {len(trace_files)} trace file(s) in {TRACES_DIR}\n")

    all_results = []
    for path in trace_files:
        print(f"Characterizing: {os.path.basename(path)} ...", end=" ", flush=True)
        try:
            res = characterize(path)
            all_results.append(res)
            rd = res["rd_stats"]
            print(
                f"experts={res['num_experts']}, accesses={res['total_accesses']}, "
                f"rd_p50={rd['p50']:.1f}, hot_traffic={res['hot_traffic_frac']:.1%}, "
                f"locality={res['locality_score']:.3f}"
            )
        except Exception as e:
            print(f"ERROR: {e}")
            all_results.append({"filename": os.path.basename(path), "error": str(e)})

    # Build and save report
    os.makedirs(os.path.dirname(REPORT_PATH), exist_ok=True)
    report = build_report(all_results)
    with open(REPORT_PATH, "w") as f:
        f.write(report)
    print(f"\n✅  Report saved to: {REPORT_PATH}")


if __name__ == "__main__":
    main()
