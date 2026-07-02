# System Simulation Results Summary

| Experiment | Trace | Experts | Cache Size | Cache Policy | Double Buffering | Hit Rate | Misses | Transfer Cycles | Exec Cycles | Total Cycles | P50 Wait | P95 Wait |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 1. Uniform + Cache None | synth_uniform_E8_T128_K2.jsonl | 8 | 0 | none | False | 0.00% | 32 | 1000000.0 | 102400.0 | 1102400.0 | 167650.0 | 272500.0 |
| 2. Uniform + LRU (size 4) | synth_uniform_E8_T128_K2.jsonl | 8 | 4 | lru | False | 0.00% | 32 | 1000000.0 | 102400.0 | 1102400.0 | 167650.0 | 272500.0 |
| 3. Zipf + LRU (size 4) | synth_zipf_E8_T128_K2.jsonl | 8 | 4 | lru | False | 0.00% | 22 | 687500.0 | 102400.0 | 789900.0 | 75200.0 | 116450.0 |
| 4. Zipf + History (size 4) | synth_zipf_E8_T128_K2.jsonl | 8 | 4 | history | False | 59.09% | 9 | 281250.0 | 102400.0 | 383650.0 | 31250.0 | 75300.0 |
| 5. Hot + LRU (size 4) | synth_hot_E16_T512_K2.jsonl | 16 | 4 | lru | False | 75.00% | 2 | 62500.0 | 409600.0 | 472100.0 | 41225.0 | 113700.0 |
| 6. Hot + History (size 4) | synth_hot_E16_T512_K2.jsonl | 16 | 4 | history | False | 75.00% | 2 | 62500.0 | 409600.0 | 472100.0 | 41225.0 | 113700.0 |
| 7. Zipf + LRU + DB | synth_zipf_E8_T128_K2.jsonl | 8 | 4 | lru | True | 0.00% | 22 | 687500.0 | 102400.0 | 687900.0 | 62500.0 | 93750.0 |
| 8. Zipf + History + DB | synth_zipf_E8_T128_K2.jsonl | 8 | 4 | history | True | 59.09% | 9 | 281250.0 | 102400.0 | 281650.0 | 31250.0 | 62500.0 |
| 9. Hot + History + DB | synth_hot_E16_T512_K2.jsonl | 16 | 4 | history | True | 75.00% | 2 | 62500.0 | 409600.0 | 440850.0 | 41225.0 | 82450.0 |
