# MP2: Set-Associative Expert Tag Prototype

## 1. Research Question
Does a set-associative BRAM tag array mitigate the timing and routing overhead of fully associative CAMs for expert cache tag lookups?

## 2. Methodology
We implement `expert_tag_cache_2way.sv`, `expert_tag_cache_4way.sv`, and `tag_replacement_lru_approx.sv` in SystemVerilog. We sweep cache capacity (entries) and compare CAM against direct-mapped, 2-way, and 4-way set-associative BRAM-backed Tag arrays.

## 3. Experimental Matrix
- **Cache Entries**: 16, 32, 64, 128
- **Tag Design**: Fully Associative CAM, Direct Mapped, 2-Way Set-Associative, 4-Way Set-Associative

## 4. Raw Results

### Cache Tag Scaling Matrix

| Cache Entries | Tag Design Type | BRAM blocks | LUTs | FFs | $F_{\text{max}}$ (MHz) | Lookup Latency | Hit Rate Degradation |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **16** | Fully Assoc CAM | 0 | 185 | 128 | 208 | 0 cycles | 0.0% (Baseline) |
| **64** | Fully Assoc CAM | 0 | 920 | 512 | 128 | 0 cycles | 0.0% |
| **64** | Direct Mapped | 1 | 45 | 96 | 312 | 1 cycle | 8.2% |
| **64** | 2-Way Set-Assoc | 1 | 112 | 160 | 263 | 1 cycle | 1.8% |
| **64** | 4-Way Set-Assoc | 1 | 240 | 290 | 235 | 1 cycle | 0.4% |

## 5. Interpretation
1. **Timing Closure Resolution**: Changing from a fully associative CAM to a 2-way set-associative BRAM tag array increases $F_{\text{max}}$ from 128 MHz to 263 MHz (+105% speedup) for a 64-entry cache.
2. **Hit Rate Trade-off**: The 2-way set-associative Tag array introduces a minimal **1.8% hit rate degradation** compared to fully associative CAM, while reducing LUT resources by **87.8%**. This confirms set-associative BRAM tags as a highly efficient alternative.

## 6. Threats to Validity
- **ASIC vs FPGA**: The BRAM structures are FPGA-specific. An ASIC implementation would use standard dual-port register files with similar but slightly lower latency.
