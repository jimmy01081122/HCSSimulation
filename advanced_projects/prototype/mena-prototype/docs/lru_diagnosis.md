# LRU Cache Diagnostic Sweep Report

**Trace parameters**: num_experts=8, num_tokens=128, num_layers=4, top_k=2, seed=42

## Hit Rate Matrix (rows=alpha, cols=cache_size)

| α \ Cache | 1 | 2 | 4 | 8 |
| :---: | :---: | :---: | :---: | :---: |
| **1.1** | 8.8% | 69.8% | 95.4% | 99.2% |
| **1.3** | 6.2% | 75.7% | 96.8% | 99.2% |
| **1.5** | 4.1% | 78.9% | 96.8% | 99.3% |
| **2.0** | 1.8% | 89.4% | 99.2% | 99.4% |

## Reuse Distance Statistics (per alpha, first cache_size column)

| α | min | mean | p50 | p95 | max |
| :---: | :---: | :---: | :---: | :---: | :---: |
| 1.1 | 0 | 1.4 | 1.0 | 3.0 | 7 |
| 1.3 | 0 | 1.3 | 1.0 | 3.0 | 5 |
| 1.5 | 0 | 1.3 | 1.0 | 3.0 | 5 |
| 2.0 | 0 | 1.1 | 1.0 | 2.0 | 4 |

## First 20 Expert Accesses per Alpha

**α=1.1**: `[0, 2, 0, 2, 0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1]`

**α=1.3**: `[0, 2, 0, 2, 0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1]`

**α=1.5**: `[0, 2, 0, 2, 0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1]`

**α=2.0**: `[0, 2, 0, 2, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1]`

## Automated Diagnosis

✅  12 configurations achieved >50% hit rate (max=99.4%).
   → The LRU cache itself is functional.

### Reuse Distance Analysis

  α=1.1:  median reuse dist=1.0, p95=3.0
    cache=1: (hit=8.8% ✓)
    cache=2: (hit=69.8% ✓)
    cache=4: (hit=95.4% ✓)
    cache=8: (hit=99.2% ✓)

  α=1.3:  median reuse dist=1.0, p95=3.0
    cache=1: (hit=6.2% ✓)
    cache=2: (hit=75.7% ✓)
    cache=4: (hit=96.8% ✓)
    cache=8: (hit=99.2% ✓)

  α=1.5:  median reuse dist=1.0, p95=3.0
    cache=1: (hit=4.1% ✓)
    cache=2: (hit=78.9% ✓)
    cache=4: (hit=96.8% ✓)
    cache=8: (hit=99.3% ✓)

  α=2.0:  median reuse dist=1.0, p95=2.0
    cache=1: (hit=1.8% ✓)
    cache=2: (hit=89.4% ✓)
    cache=4: (hit=99.2% ✓)
    cache=8: (hit=99.4% ✓)

### Root-Cause Summary

No near-zero hit rate cases found – the **LRU cache implementation itself is correct and functional**.

---

## Cross-Validation: Why `mena_sim.py` Shows 0% Hit Rate

The diagnostic sweep tests LRU **access-by-access** (one call per expert per token).
`mena_sim.py` operates **layer-granularly**: it calls `cache.request(expert_id)` once per
*expert* per layer (not once per token), iterating experts in **sorted numeric order [0,1,…,7]**.

This creates **deterministic cache thrashing**:

```
Layer 0: request experts 0→7 sequentially. Cache (size=4) after = {4,5,6,7}.
Layer 1: request expert 0 → miss, evict 4. Request expert 1 → miss, evict 5. ...
         Cache after = {4,5,6,7} again. 0 hits.
Layer 2: Same pattern. 0 hits.
```

This is confirmed by `sim/results_summary.md`:

| Experiment | Trace | Cache | Policy | Hit Rate | Verdict |
|:---|:---|:---:|:---|:---:|:---|
| Uniform + LRU (size 4) | uniform | 4 | lru | **0.00%** | Thrash (not Zipf-specific!) |
| Zipf + LRU (size 4) | zipf | 4 | lru | **0.00%** | Thrash — same root cause |
| Zipf + History (size 4) | zipf | 4 | history | **67.74%** | Prefetch compensates |
| Hot + LRU (size 4) | hot | 4 | lru | **75.00%** | Hot trace has tiny working set |
| Zipf + LRU + DB | zipf | 4 | lru | **0.00%** | Double buffering doesn't fix policy |

**The fact that Uniform+LRU also gets 0% rules out any Zipf-specific bug.**
The issue is simulation architecture: sorted expert-major dispatch × LRU = deterministic thrashing.

## Final Diagnosis: THRASHING (Not a Bug)

**The LRU cache is correct.** The `diag_lru_sweep.py` analysis confirms:
- Cache size=2 achieves 69–89% hit rate depending on alpha
- Cache size=4 achieves 95–99% hit rate

The 0% in `mena_sim.py` is caused by the **simulation's layer-batch processing with sorted dispatch order**.

### Recommended Fixes

1. **Per-layer caches**: Instantiate one `ExpertCache` per layer in `mena_sim.py`.
2. **Randomize dispatch order**: Use `random.shuffle(expert_order)` to break determinism.
3. **Use History policy**: 67.74% hit rate vs. 0% for LRU on identical Zipf traces.
4. **Higher `--zipf_alpha`**: Use α ≥ 1.5 (now default via `--zipf_alpha` in `gen_synthetic_trace.py`).
