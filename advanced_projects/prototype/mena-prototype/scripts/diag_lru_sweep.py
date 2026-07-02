#!/usr/bin/env python3
"""
diag_lru_sweep.py — Diagnostic sweep for Zipf+LRU 0% hit rate investigation.

Generates synthetic Zipf traces inline (no file I/O for traces), simulates
an LRU cache, computes hit rates and reuse distances, and writes a diagnosis
report to docs/lru_diagnosis.md.
"""
import os
import sys
import math
import numpy as np
from collections import OrderedDict

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------
ALPHA_VALUES  = [1.1, 1.3, 1.5, 2.0]
CACHE_SIZES   = [1, 2, 4, 8]
NUM_EXPERTS   = 8          # number of distinct experts (pool size)
NUM_TOKENS    = 128        # tokens per layer
NUM_LAYERS    = 4          # layers (same as default trace)
TOP_K         = 2          # experts per token
SEED          = 42
REPORT_PATH   = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", "docs", "lru_diagnosis.md")
)

# ---------------------------------------------------------------------------
# Trace generation (inline – no file I/O)
# ---------------------------------------------------------------------------
def generate_zipf_sequence(alpha: float, num_experts: int, num_tokens: int,
                            num_layers: int, top_k: int, seed: int) -> list[int]:
    """Return a flat list of expert IDs accessed in order (all layers, all tokens)."""
    rng = np.random.default_rng(seed)
    accesses: list[int] = []

    ranks = np.arange(1, num_experts + 1, dtype=float)
    zipf_probs = 1.0 / (ranks ** alpha)
    zipf_probs /= zipf_probs.sum()
    base_logits = np.log(zipf_probs + 1e-9)

    for _layer in range(num_layers):
        for _token in range(num_tokens):
            noise  = rng.normal(0, 0.5, size=num_experts)
            logits = base_logits + noise
            exp_l  = np.exp(logits - logits.max())
            scores = exp_l / exp_l.sum()
            topk   = np.argsort(scores)[::-1][:top_k].tolist()
            accesses.extend(topk)

    return accesses


# ---------------------------------------------------------------------------
# LRU cache simulator
# ---------------------------------------------------------------------------
class SimpleLRU:
    def __init__(self, capacity: int):
        self.capacity = capacity
        self._cache: OrderedDict[int, None] = OrderedDict()

    def access(self, key: int) -> bool:
        """Return True on hit, False on miss."""
        if self.capacity == 0:
            return False
        if key in self._cache:
            self._cache.move_to_end(key)          # mark recently used
            return True
        # miss
        if len(self._cache) >= self.capacity:
            self._cache.popitem(last=False)        # evict LRU
        self._cache[key] = None
        return False


# ---------------------------------------------------------------------------
# Reuse distance computation  (exact stack-distance / LRU distance)
# ---------------------------------------------------------------------------
def compute_reuse_distances(accesses: list[int]) -> list[int]:
    """
    For each access, compute reuse distance = number of *unique* experts
    seen since the previous access to the same expert.
    Returns only finite distances (skips first-time accesses = cold misses).
    """
    last_seen: dict[int, int] = {}  # expert -> index of last access
    unique_since: list[set] = []    # track sets of unique experts seen between reuses

    # We maintain a sliding window of unique experts seen
    # Using the position-based approach:
    distances: list[int] = []
    window: list[int] = []  # stack of unique accesses since last occurrence

    # Standard stack-distance via a list (O(n^2) – fine for n≤4096)
    seen_order: list[int] = []
    for exp in accesses:
        if exp in seen_order:
            dist = len(set(seen_order[seen_order.index(exp):]))
            distances.append(dist - 1)            # -1: exclude the element itself
            seen_order.remove(exp)
        seen_order.append(exp)

    return distances


# ---------------------------------------------------------------------------
# Run one sweep cell
# ---------------------------------------------------------------------------
def run_sweep_cell(alpha: float, cache_size: int) -> dict:
    accesses = generate_zipf_sequence(
        alpha, NUM_EXPERTS, NUM_TOKENS, NUM_LAYERS, TOP_K, SEED
    )
    lru   = SimpleLRU(capacity=cache_size)
    hits  = 0
    misses = 0
    for exp in accesses:
        if lru.access(exp):
            hits += 1
        else:
            misses += 1

    total     = hits + misses
    hit_rate  = hits / total if total else 0.0

    reuse_dists = compute_reuse_distances(accesses)
    if reuse_dists:
        rd_min  = int(np.min(reuse_dists))
        rd_max  = int(np.max(reuse_dists))
        rd_p50  = float(np.percentile(reuse_dists, 50))
        rd_p95  = float(np.percentile(reuse_dists, 95))
        rd_mean = float(np.mean(reuse_dists))
    else:
        rd_min = rd_max = rd_p50 = rd_p95 = rd_mean = float("nan")

    return {
        "alpha":     alpha,
        "cache_size": cache_size,
        "total_accesses": total,
        "hits":      hits,
        "misses":    misses,
        "hit_rate":  hit_rate,
        "rd_min":    rd_min,
        "rd_max":    rd_max,
        "rd_p50":    rd_p50,
        "rd_p95":    rd_p95,
        "rd_mean":   rd_mean,
        "first20":   accesses[:20],
    }


# ---------------------------------------------------------------------------
# Diagnosis helper
# ---------------------------------------------------------------------------
def diagnose(results: list[dict]) -> str:
    """
    Produce a human-readable diagnosis of whether 0% hit rate is a bug or thrashing.
    """
    lines = []

    # --- check: does cache_size ≥ NUM_EXPERTS ever give ~100%? ---
    best_by_alpha: dict[float, float] = {}
    for r in results:
        alpha = r["alpha"]
        if alpha not in best_by_alpha or r["hit_rate"] > best_by_alpha[alpha]:
            best_by_alpha[alpha] = r["hit_rate"]

    # --- check reuse distances vs cache sizes ---
    low_hit_cases  = [r for r in results if r["hit_rate"] < 0.01]
    high_hit_cases = [r for r in results if r["hit_rate"] > 0.50]

    lines.append("## Automated Diagnosis\n")

    # 1. Check if ANY configuration achieves hits
    if not high_hit_cases:
        lines.append("⚠️  **WARNING**: No configuration achieved >50% hit rate.")
        lines.append("   → This strongly suggests a **bug** in the cache or trace generator,")
        lines.append("     NOT just natural thrashing.  Investigate ExpertCache.request().\n")
    else:
        hr_vals = [r["hit_rate"] for r in high_hit_cases]
        lines.append(f"✅  {len(high_hit_cases)} configurations achieved >50% hit rate (max={max(hr_vals):.1%}).")
        lines.append("   → The LRU cache itself is functional.\n")

    # 2. Check reuse distance vs cache thresholds
    lines.append("### Reuse Distance Analysis\n")
    for alpha in ALPHA_VALUES:
        # gather all reuse distances for this alpha (any cache size, same access sequence)
        sample = next((r for r in results if r["alpha"] == alpha), None)
        if sample is None:
            continue
        p50 = sample["rd_p50"]
        p95 = sample["rd_p95"]
        # A cache of size C can hit if reuse distance ≤ C
        lines.append(f"  α={alpha:.1f}:  median reuse dist={p50:.1f}, p95={p95:.1f}")
        for cs in CACHE_SIZES:
            expected_r = next(
                (r for r in results if r["alpha"] == alpha and r["cache_size"] == cs), None
            )
            if expected_r is None:
                continue
            # If reuse dist p50 > cache_size → expected thrashing
            verdict = ""
            if p50 > cs:
                verdict = f"(expected thrashing: p50 dist {p50:.0f} > cache {cs})"
            elif expected_r["hit_rate"] < 0.01 and p50 <= cs:
                verdict = f"(⚠️ SUSPICIOUS: p50 dist {p50:.0f} ≤ cache {cs} but hit={expected_r['hit_rate']:.1%})"
            else:
                verdict = f"(hit={expected_r['hit_rate']:.1%} ✓)"
            lines.append(f"    cache={cs}: {verdict}")
        lines.append("")

    # 3. Root-cause summary
    lines.append("### Root-Cause Summary\n")
    # Compute fraction of 0% cases that are explainable by reuse dist
    explainable = 0
    suspicious  = 0
    for r in low_hit_cases:
        if r["rd_p50"] > r["cache_size"]:
            explainable += 1
        else:
            suspicious  += 1

    if suspicious > 0:
        lines.append(f"**BUG DETECTED**: {suspicious} configuration(s) show near-zero hit rate "
                     f"even though median reuse distance ≤ cache size.")
        lines.append("Likely cause: the cache key space in `mena_sim.py` is NOT per-layer —")
        lines.append("experts from all layers share one cache, so expert-0-layer-0 and")
        lines.append("expert-0-layer-1 are the same key, but the cross-layer access pattern")
        lines.append("creates artificially large effective reuse distances.")
        lines.append("**Fix**: instantiate one `ExpertCache` per layer in `mena_sim.py`.\n")
    elif explainable > 0 and not suspicious:
        lines.append(f"**THRASHING**: all {explainable} near-zero-hit cases are explained by")
        lines.append(f"reuse distance > cache size.  No bug detected in LRU logic itself.")
        lines.append("Increase cache size or use a prefetch policy (e.g., `--policy history`).\n")
    else:
        lines.append("No near-zero hit rate cases found – cache is working correctly.\n")

    return "\n".join(lines)


# ---------------------------------------------------------------------------
# Markdown report builder
# ---------------------------------------------------------------------------
def build_report(results: list[dict], diagnosis: str) -> str:
    md = []
    md.append("# LRU Cache Diagnostic Sweep Report\n")
    md.append(f"**Trace parameters**: num_experts={NUM_EXPERTS}, num_tokens={NUM_TOKENS}, "
              f"num_layers={NUM_LAYERS}, top_k={TOP_K}, seed={SEED}\n")

    # --- Hit-rate matrix ---
    md.append("## Hit Rate Matrix (rows=alpha, cols=cache_size)\n")
    header = "| α \\ Cache | " + " | ".join(str(c) for c in CACHE_SIZES) + " |"
    sep    = "| :---: | " + " | ".join(":---:" for _ in CACHE_SIZES) + " |"
    md.append(header)
    md.append(sep)
    for alpha in ALPHA_VALUES:
        row_parts = [f"**{alpha}**"]
        for cs in CACHE_SIZES:
            r = next((x for x in results if x["alpha"] == alpha and x["cache_size"] == cs), None)
            if r:
                row_parts.append(f"{r['hit_rate']:.1%}")
            else:
                row_parts.append("—")
        md.append("| " + " | ".join(row_parts) + " |")
    md.append("")

    # --- Reuse distance table ---
    md.append("## Reuse Distance Statistics (per alpha, first cache_size column)\n")
    md.append("| α | min | mean | p50 | p95 | max |")
    md.append("| :---: | :---: | :---: | :---: | :---: | :---: |")
    for alpha in ALPHA_VALUES:
        r = next((x for x in results if x["alpha"] == alpha), None)
        if r:
            md.append(
                f"| {alpha} | {r['rd_min']} | {r['rd_mean']:.1f} | "
                f"{r['rd_p50']:.1f} | {r['rd_p95']:.1f} | {r['rd_max']} |"
            )
    md.append("")

    # --- First-20 sequences ---
    md.append("## First 20 Expert Accesses per Alpha\n")
    for alpha in ALPHA_VALUES:
        r = next((x for x in results if x["alpha"] == alpha), None)
        if r:
            md.append(f"**α={alpha}**: `{r['first20']}`\n")

    # --- Diagnosis ---
    md.append(diagnosis)

    return "\n".join(md)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    print("=" * 60)
    print("  Zipf + LRU Diagnostic Sweep")
    print("=" * 60)
    print(f"  Experts={NUM_EXPERTS}, Tokens={NUM_TOKENS}, Layers={NUM_LAYERS}, TopK={TOP_K}\n")

    results: list[dict] = []

    for alpha in ALPHA_VALUES:
        print(f"\n--- Zipf α={alpha} ---")
        for cache_size in CACHE_SIZES:
            r = run_sweep_cell(alpha, cache_size)
            results.append(r)
            print(
                f"  cache={cache_size:2d}: hit_rate={r['hit_rate']:.2%}  "
                f"(rd_p50={r['rd_p50']:.1f}, rd_p95={r['rd_p95']:.1f})"
            )

    # Print first-20 for one reference alpha
    ref = next(r for r in results if r["alpha"] == 1.5)
    print(f"\nFirst 20 accesses (α=1.5): {ref['first20']}")

    # Diagnosis
    print("\n" + "=" * 60)
    diagnosis = diagnose(results)
    print(diagnosis)

    # Save report
    os.makedirs(os.path.dirname(REPORT_PATH), exist_ok=True)
    report_text = build_report(results, diagnosis)
    with open(REPORT_PATH, "w") as f:
        f.write(report_text)
    print(f"\n✅  Report saved to: {REPORT_PATH}")


if __name__ == "__main__":
    main()
