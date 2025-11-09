# BM_simple Benchmark Discrepancy Investigation

## The Problem
- opt2_full_benchmark.json shows: BM_simple = 652ns
- Current benchmarks show: BM_simple = 188-194ns
- Baseline (commit 25f9203) shows: 194ns

## Root Cause: Measurement Artifact

The 652ns result was from a benchmark run with:
- Only 10 iterations total
- Possible cold cache effects
- No minimum time requirement
- Tree in dirty state during measurement

Current benchmarks use:
- `--benchmark_min_time=1s` (runs thousands of iterations)
- Multiple repetitions for statistical validity
- Warm cache

## Conclusion
The **true baseline was always ~194ns**. The 652ns measurement in opt2_full_benchmark.json 
was an artifact of insufficient iterations/warmup. The optimizations (OPT1+OPT2) improved
BM_simple from 194ns → 188ns (+3%), not from 652ns.

No performance mystery - just measurement methodology difference.
