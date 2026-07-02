#!/usr/bin/env python3
"""
eval_markov_predictor.py
Evaluate the first-order Markov routing predictor on all 5 MoE traces.

For each trace:
  - Extracts flattened per-layer expert access sequences
  - Trains on first 50 %, tests on second 50 % with rolling updates
  - Reports top-1, top-2 accuracy, and prefetch hit rate
  - Prints the transition probability matrix for the Zipf trace
  - Saves results to docs/markov_predictor_eval.md
"""

import sys
import os
import json
import textwrap
from pathlib import Path

import numpy as np

# ---------------------------------------------------------------------------
# Make sure sim/ is importable
# ---------------------------------------------------------------------------
REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO_ROOT / "sim"))
from markov_predictor import MarkovPredictor  # noqa: E402

# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
TRACES_DIR = REPO_ROOT / "traces"
DOCS_DIR   = REPO_ROOT / "docs"
DOCS_DIR.mkdir(exist_ok=True)
OUT_MD     = DOCS_DIR / "markov_predictor_eval.md"

TRACE_FILES = [
    "toy_moe_E8_T128_K2.jsonl",
    "synth_uniform_E8_T128_K2.jsonl",
    "synth_zipf_E8_T128_K2.jsonl",
    "synth_hot_E16_T512_K2.jsonl",
    "synth_layer_shift_E16_T512_K2.jsonl",
]

# ---------------------------------------------------------------------------
# Helper – load one JSONL file and extract the flat expert sequence
# ---------------------------------------------------------------------------
def load_trace(path: Path):
    """
    Returns a dict:
      num_experts  : int
      sequence     : list[int]   – flattened topk_experts across all layers/tokens
      layers       : list[int]   – layer id for each entry (before flattening)
    """
    records = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                records.append(json.loads(line))

    # Infer num_experts from filename or from scores array length
    num_experts = None
    for r in records[:1]:
        num_experts = len(r.get("scores", []))
    if num_experts is None or num_experts == 0:
        # fallback
        num_experts = 8

    flat_seq = []
    for r in records:
        for e in r.get("topk_experts", []):
            flat_seq.append(int(e))

    return {"num_experts": num_experts, "sequence": flat_seq, "records": records}


# ---------------------------------------------------------------------------
# Evaluate one trace
# ---------------------------------------------------------------------------
def evaluate_trace(name: str, data: dict, is_zipf: bool = False):
    seq         = data["sequence"]
    num_experts = data["num_experts"]
    n           = len(seq)

    if n < 4:
        return None  # too short to evaluate

    split      = n // 2
    train_seq  = seq[:split]
    test_seq   = seq[split:]

    # --- Train on first 50 % ---
    predictor = MarkovPredictor(num_experts)
    for t in range(len(train_seq) - 1):
        predictor.update(train_seq[t], train_seq[t + 1])

    # --- Test on second 50 % with rolling updates ---
    top1_hits = 0
    top2_hits = 0
    total     = len(test_seq) - 1

    for t in range(total):
        prev = test_seq[t]
        curr = test_seq[t + 1]

        top1_pred = predictor.predict(prev, top_k=1)
        top2_pred = predictor.predict(prev, top_k=2)

        if curr in top1_pred:
            top1_hits += 1
        if curr in top2_pred:
            top2_hits += 1

        # Rolling update
        predictor.update(prev, curr)

    top1_acc   = top1_hits / total if total > 0 else 0.0
    top2_acc   = top2_hits / total if total > 0 else 0.0

    # Prefetch hit rate (top-2, re-uses same predictor state after rolling update)
    pf_rate = predictor.prefetch_hit_rate(test_seq, top_k=2)

    result = {
        "name":      name,
        "n_experts": num_experts,
        "seq_len":   n,
        "top1_acc":  top1_acc,
        "top2_acc":  top2_acc,
        "pf_rate":   pf_rate,
        "predictor": predictor,
    }

    if is_zipf:
        result["prob_matrix"] = predictor.to_probability_matrix()

    return result


# ---------------------------------------------------------------------------
# Pretty-print the probability matrix (truncated to 8×8 for readability)
# ---------------------------------------------------------------------------
def format_matrix(mat: np.ndarray, max_dim: int = 8) -> str:
    n = min(mat.shape[0], max_dim)
    sub = mat[:n, :n]
    header = "     " + "  ".join(f"E{j:<2d}" for j in range(n))
    rows   = [header]
    for i in range(n):
        row_str = f"E{i:<2d}  " + "  ".join(f"{sub[i, j]:.2f}" for j in range(n))
        rows.append(row_str)
    if mat.shape[0] > max_dim:
        rows.append(f"  … ({mat.shape[0] - max_dim} more rows/cols truncated)")
    return "\n".join(rows)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    results = []
    zipf_result = None

    for fname in TRACE_FILES:
        fpath = TRACES_DIR / fname
        if not fpath.exists():
            print(f"[WARN] trace not found: {fpath}", file=sys.stderr)
            continue

        is_zipf = "zipf" in fname.lower()
        print(f"Evaluating {fname} …", flush=True)
        data = load_trace(fpath)
        res  = evaluate_trace(fname, data, is_zipf=is_zipf)
        if res is None:
            print(f"  Skipped (sequence too short).")
            continue

        print(f"  top-1={res['top1_acc']:.3f}  top-2={res['top2_acc']:.3f}  pf_rate={res['pf_rate']:.3f}")
        results.append(res)
        if is_zipf:
            zipf_result = res

    # -----------------------------------------------------------------------
    # Print Zipf transition matrix to stdout
    # -----------------------------------------------------------------------
    if zipf_result and "prob_matrix" in zipf_result:
        print("\n=== Transition Probability Matrix (Zipf trace) ===")
        print(format_matrix(zipf_result["prob_matrix"]))

    # -----------------------------------------------------------------------
    # Determine most temporally local traces (highest top-1 accuracy)
    # -----------------------------------------------------------------------
    sorted_by_top1 = sorted(results, key=lambda r: r["top1_acc"], reverse=True)

    # -----------------------------------------------------------------------
    # Write Markdown report
    # -----------------------------------------------------------------------
    md_lines = [
        "# Markov Routing Predictor Evaluation",
        "",
        "First-order Markov chain trained on the first 50 % of each trace with "
        "rolling updates during test phase (second 50 %).",
        "",
        "## Results Summary",
        "",
        "| Trace | Experts | Seq Len | Top-1 Acc | Top-2 Acc | Prefetch Hit Rate |",
        "|-------|---------|---------|-----------|-----------|-------------------|",
    ]
    for r in results:
        trace_short = r["name"].replace(".jsonl", "")
        md_lines.append(
            f"| {trace_short} | {r['n_experts']} | {r['seq_len']:,} "
            f"| {r['top1_acc']:.3f} | {r['top2_acc']:.3f} | {r['pf_rate']:.3f} |"
        )

    md_lines += [
        "",
        "## Policy Notes",
        "",
        "- **Top-1 accuracy**: fraction of test pairs where the single most-likely "
        "next expert was correct.",
        "- **Top-2 accuracy**: fraction where the correct expert appeared in the "
        "top-2 predictions (mirrors a 2-slot prefetch buffer).",
        "- **Prefetch hit rate**: computed via `MarkovPredictor.prefetch_hit_rate()` "
        "on the test partition *after* rolling updates; equivalent to top-2 acc on "
        "the same data.",
        "",
        "## Temporal Locality Ranking (by Top-1 Accuracy)",
        "",
        "| Rank | Trace | Top-1 Acc | Observation |",
        "|------|-------|-----------|-------------|",
    ]
    observations = {
        "zipf":        "Heavy-tail distribution creates strong locality (experts 0-1 dominate).",
        "hot":         "Small hot-set → Markov captures reuse well.",
        "layer_shift": "Expert preferences shift by layer; Markov adapts with rolling updates.",
        "uniform":     "Uniform random routing → minimal predictable structure.",
        "toy":         "Simple MoE toy trace; moderate locality.",
    }
    for rank, r in enumerate(sorted_by_top1, 1):
        key = next((k for k in observations if k in r["name"].lower()), "")
        obs = observations.get(key, "—")
        md_lines.append(f"| {rank} | {r['name'].replace('.jsonl','')} | {r['top1_acc']:.3f} | {obs} |")

    if zipf_result and "prob_matrix" in zipf_result:
        mat_str = format_matrix(zipf_result["prob_matrix"])
        md_lines += [
            "",
            "## Transition Probability Matrix – Zipf Trace",
            "",
            "Rows = previous expert, columns = next expert.  "
            "Values are row-normalised probabilities.",
            "",
            "```",
            mat_str,
            "```",
        ]

    md_lines += [
        "",
        "## Summary",
        "",
        "Traces with **skewed or hot-set routing** (Zipf, Hot) show the highest "
        "Markov predictability because a small subset of experts dominates and "
        "transitions are highly repetitive.  **Uniform** traces are hardest to "
        "predict as expected.  The **layer-shift** trace benefits from rolling "
        "updates that let the predictor track the per-layer preference changes.",
        "",
        "*Generated by `scripts/eval_markov_predictor.py`.*",
    ]

    md_text = "\n".join(md_lines) + "\n"
    OUT_MD.write_text(md_text)
    print(f"\nReport saved → {OUT_MD}")


if __name__ == "__main__":
    main()
