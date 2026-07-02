"""markov_predictor.py – First-order Markov routing predictor for MoE expert access."""

import numpy as np
from typing import List


class MarkovPredictor:
    """
    First-order Markov chain predictor for MoE expert routing.

    Learns transition probabilities P(next_expert | prev_expert) from
    observed (prev_expert, curr_expert) pairs and uses them to predict
    the most likely next expert(s) given the last routed expert.
    """

    def __init__(self, num_experts: int):
        self.num_experts = num_experts
        # Raw count matrix: transition[i][j] = # times i→j was observed
        self.transition = np.zeros((num_experts, num_experts), dtype=int)

    # ------------------------------------------------------------------
    def update(self, prev_expert: int, curr_expert: int) -> None:
        """Record one observed routing transition prev_expert → curr_expert."""
        assert 0 <= prev_expert < self.num_experts, f"prev_expert {prev_expert} out of range"
        assert 0 <= curr_expert < self.num_experts, f"curr_expert {curr_expert} out of range"
        self.transition[prev_expert, curr_expert] += 1

    # ------------------------------------------------------------------
    def predict(self, last_expert: int, top_k: int = 2) -> List[int]:
        """
        Return the top-k most likely next experts given *last_expert*.

        Falls back to uniform ranking (0, 1, …) when no transitions have
        been observed from *last_expert*.
        """
        row = self.transition[last_expert].copy().astype(float)
        total = row.sum()
        if total == 0:
            # No data → return lowest indices as tie-breaker
            return list(range(min(top_k, self.num_experts)))
        # Descending sort by count; stable so ties keep lower index first
        ranked = np.argsort(-row, kind="stable")
        return ranked[:top_k].tolist()

    # ------------------------------------------------------------------
    def prefetch_hit_rate(self, accesses: List[int], top_k: int = 2) -> float:
        """
        Simulate prefetching over an access sequence.

        For each consecutive pair (accesses[t], accesses[t+1]), check
        whether accesses[t+1] appears in the top-k prediction made from
        accesses[t].  Returns the fraction of hits (pairs where prediction
        was correct).

        The model is *not* updated during this evaluation (use training
        phase update() calls before calling this method).
        """
        if len(accesses) < 2:
            return 0.0
        hits = 0
        total = len(accesses) - 1
        for t in range(total):
            predicted = self.predict(accesses[t], top_k=top_k)
            if accesses[t + 1] in predicted:
                hits += 1
        return hits / total

    # ------------------------------------------------------------------
    def to_probability_matrix(self) -> np.ndarray:
        """
        Return the row-normalised transition probability matrix.

        Rows with zero total count are left as uniform (1/num_experts).
        """
        mat = self.transition.astype(float)
        row_sums = mat.sum(axis=1, keepdims=True)
        # Replace zero rows with uniform distribution to avoid divide-by-zero
        zero_rows = (row_sums == 0).flatten()
        mat[zero_rows] = 1.0 / self.num_experts
        row_sums[zero_rows] = 1.0
        return mat / row_sums
