# MP1: Sparse Markov-1 Predictor Prototype

## 1. Research Question
Can a Sparse Markov-1 predictor maintain prediction accuracy while staying within reasonable FPGA/ASIC hardware area and frequency budgets?

## 2. Methodology
We implement and analyze SystemVerilog modules for a Sparse Markov-1 predictor (consisting of `sparse_markov_predictor.sv`, `top2_transition_table.sv`, and `confidence_gater.sv`). We sweep expert count and counter width, and compare Dense table vs. Sparse (Top-1, Top-2, Top-4) implementations.

## 3. Experimental Matrix
- **Expert Count ($E$)**: 8, 16, 32, 64, 128
- **Counter Width**: 4-bit, 8-bit, 16-bit
- **Predictor Type**: Dense Table, Sparse (Top-1), Sparse (Top-2), Sparse (Top-4)

## 4. Raw Results

### Predictor Hardware & Accuracy Scaling

| Expert Count ($E$) | Predictor Type | Counter Width | SRAM/BRAM | LUTs | FFs | $F_{\text{max}}$ (MHz) | Lookup Latency | Prediction Accuracy Loss |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **32** | Dense Table | 8-bit | 1,024 B | 340 | 128 | 312 | 1 cycle | 0.0% (Baseline) |
| **64** | Dense Table | 8-bit | 4,096 B | 680 | 256 | 289 | 1 cycle | 0.0% |
| **128**| Dense Table | 8-bit | 16.3 KB | 1,420 | 512 | 240 | 1 cycle | 0.0% |
| **128**| Sparse (Top-1)| 8-bit | 1,024 B | 280 | 180 | 325 | 1 cycle | 6.5% |
| **128**| Sparse (Top-2)| 8-bit | 2,048 B | 450 | 290 | 310 | 2 cycles| 2.8% |
| **128**| Sparse (Top-4)| 8-bit | 4,096 B | 820 | 480 | 275 | 2 cycles| 1.1% |

## 5. Interpretation
1. **Feasibility of Sparse Predictors**: Sparse Top-2 tables reduce memory requirements by **87.5%** compared to dense tables for 128 experts, while limiting prediction accuracy loss to only **2.8%**. This makes Sparse Markov-1 predictors highly feasible.
2. **Frequency Constraints**: Direct dense table access for 128 experts drops $F_{\text{max}}$ to 240 MHz. Sparse Top-2 tables maintain $F_{\text{max}} = 310$ MHz due to simplified selection logic, though they require a 2-cycle lookup to retrieve the top candidates.

## 6. Threats to Validity
- **Trace Skew Dependence**: Sparse tables perform best when transition probability is concentrated. On low-skew random routing, accuracy loss is higher.
