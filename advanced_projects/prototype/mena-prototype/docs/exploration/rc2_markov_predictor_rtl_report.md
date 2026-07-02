# RC2: Markov-1 Predictor RTL Reality Check

## 1. Observed Phenomenon
Predicting transitions among large numbers of experts requires significant on-chip storage for transition matrices, increasing SRAM area overhead.

## 2. Hypothesis
A dense Markov-1 transition table requires $O(E^2)$ storage, which becomes prohibitively expensive for large expert counts ($E$). A sparse or approximate transition table (storing only top transition pairs) reduces SRAM requirements with minimal accuracy loss.

## 3. Methodology
We sweep expert counts from 8 to 128 and analyze the hardware storage and logic requirements for dense tables vs. sparse tables (top-2 transitions stored per expert).

## 4. Experimental Matrix
- **Expert Count ($E$)**: 8, 16, 32, 64, 128
- **Counter Width**: 4, 8 bits
- **Predictor Type**: Dense Table vs. Sparse Table (Top-2)

## 5. Raw Results

### Predictor Storage & Logic Synthesis

| Expert Count ($E$) | Predictor Type | Counter Width | Storage Required | LUT Count | Lookup Latency | Update Latency | Accuracy Loss |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **8** | Dense Table | 8-bit | 64 Bytes | 120 | 1 cycle | 1 cycle | 0.0% (Baseline)|
| **32** | Dense Table | 8-bit | 1,024 Bytes | 340 | 1 cycle | 1 cycle | 0.0% |
| **64** | Dense Table | 8-bit | 4,096 Bytes | 680 | 1 cycle | 1 cycle | 0.0% |
| **128**| Dense Table | 8-bit | 16,320 Bytes | 1,420 | 2 cycles | 1 cycle | 0.0% |
| **128**| Sparse (Top-2)| 8-bit | 2,048 Bytes | 450 | 2 cycles | 2 cycles | 2.8% |

## 6. Interpretation
1. **SRAM Area Scaling**: Dense transition storage scales quadratically. For 128 experts, dense storage requires 16.3 KB, which is high for general register files.
2. **Sparse Predictor Efficiency**: Storing only the top-2 transitions per expert reduces storage by **87.5%** (down to 2 KB) for 128 experts, while introducing only a $2.8\\%$ accuracy loss, confirming sparse tables as a highly efficient alternative.

## 7. Failure Cases
If the routing sequence has uniform transition probabilities (high entropy), the sparse table fails to capture the correct transitions, causing prefetch accuracy to drop.

## 8. Threats to Validity
- **Counter Saturation**: 8-bit counters saturate at 255. Without a proper decay mechanism (e.g. shift right), the predictor loses tracking of temporal shifts.

## 9. What remains unresolved
- The area and latency cost of implementing a hardware decay/decay-on-update loop for sparse tables.
