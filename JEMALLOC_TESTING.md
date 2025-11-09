# jemalloc Allocator Testing Results

## Executive Summary

Tested jemalloc as an external optimization (zero code changes required). Results are **MIXED** - significant improvement on allocation-heavy workload but regression on mid-complexity case.

**Recommendation**: DO NOT enable jemalloc by default. Consider it as optional deployment flag for high-throughput scenarios.

## Test Methodology

Compared system malloc (ptmalloc2) vs jemalloc using `LD_PRELOAD` with Google Benchmark:

```bash
# Baseline (system malloc)
./test/test_benchmark --benchmark_filter="BM_.*" --benchmark_repetitions=5 --benchmark_min_time=1s

# jemalloc test
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libjemalloc.so.2 ./test/test_benchmark [same args]
```

**Build State**: OPT1+OPT2 applied (buffer reuse + YYSTYPE capacity=128)

## Results

| Benchmark        | System malloc | jemalloc | Difference | Change  |
|------------------|---------------|----------|------------|---------|
| BM_simple        | 184 ns        | 180 ns   | +4 ns      | +2.2%   |
| BM_long_string   | 1362 ns       | 1421 ns  | -59 ns     | **-4.3%** |
| BM_many_numbers  | 31862 ns      | 30416 ns | +1446 ns   | **+4.5%** |

## Analysis

### Why BM_many_numbers Improved (+4.5%)

**Theory**: BM_many_numbers is allocation-heavy (~90 number conversions per iteration). jemalloc's thread-local caches and better size-class segregation reduce allocation overhead.

**Mechanism**:
- System malloc: Global arena locks for some allocations
- jemalloc: Per-thread tcaches eliminate most locking
- Better locality for repeated allocations of same size

**Result**: 1446 ns faster (4.5% improvement) on stress test.

### Why BM_long_string Regressed (-4.3%)

**Hypothesis 1**: Cache Effects
- BM_long_string has moderate allocation count with varied sizes
- jemalloc's size-class segregation may hurt cache locality here
- System malloc's simple bump allocation might be more cache-friendly for sequential processing

**Hypothesis 2**: Metadata Overhead
- jemalloc maintains more metadata per allocation
- For medium-sized workloads, overhead exceeds benefit

**Hypothesis 3**: Thread-local Cache Misses
- BM_long_string pattern doesn't benefit from tcache warmth
- Cold tcache lookups add latency

**Needs Investigation**: Profile with callgrind to see where the 59ns went.

### Why BM_simple Neutral (+2.2%)

BM_simple is too small to show significant allocator differences. 4ns improvement is within noise margin.

## Trade-off Analysis

**Best Case (High-Throughput Server)**:
- Thousands of calls per second
- Allocation-heavy workloads
- BM_many_numbers represents this well
- **jemalloc wins: +4.5%**

**Worst Case (Varied Input Complexity)**:
- Mix of simple and complex strings
- BM_long_string represents medium complexity
- **jemalloc loses: -4.3%**

**Average Case**:
If workload is 33% each benchmark type:
- Weighted average: (2.2% + (-4.3%) + 4.5%) / 3 = **+0.8%**

Barely positive, not worth the dependency.

## Comparison to Profiling Predictions

From REMAINING_ALLOCATION_ANALYSIS.md:
- **Predicted**: 5-10% improvement with jemalloc
- **Actual**: +4.5% on stress test, -4.3% on mid-complexity

**Verdict**: Prediction was optimistic. Real-world workloads show mixed results.

## Recommendation

### DO NOT Apply jemalloc by Default

**Reasons**:
1. Regression on BM_long_string is unacceptable
2. Net benefit is marginal (+0.8% average)
3. Adds external dependency
4. Different memory usage patterns may surprise users

### Optional Deployment Flag

Document jemalloc as **optional performance tuning** for specific scenarios:

**Good fit for jemalloc**:
- High-throughput stream processing
- Many short-lived allocations
- Multi-threaded environments
- Workloads similar to BM_many_numbers

**Poor fit for jemalloc**:
- Single-threaded CLI tools
- Varied input complexity
- Memory-constrained environments

**How to enable**:
```bash
# Linux
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libjemalloc.so.2 your_app

# Or link directly
gcc -o your_app your_app.c -lomnomnum -ljemalloc
```

## Next Steps

Based on REMAINING_ALLOCATION_ANALYSIS.md priorities:

1. ✅ **jemalloc**: TESTED - Mixed results, optional only
2. ⏳ **Profile-Guided Optimization (PGO)**: Try this next (5-15% predicted)
3. ⏳ **Fast-path check for process_percent()**: Small win, easy implementation
4. ⏳ **Pool sub-ParserState**: Medium effort, 2-3% potential

**Recommended next step**: Try PGO before making more code changes.

## PGO Quick Test

```bash
# Step 1: Build with instrumentation
make clean
make CFLAGS="-O3 -fprofile-generate"

# Step 2: Generate profile data
./test/test_benchmark --benchmark_filter="BM_.*" --benchmark_min_time=5s

# Step 3: Rebuild with profile optimization
make clean
make CFLAGS="-O3 -fprofile-use"

# Step 4: Benchmark optimized build
./test/test_benchmark --benchmark_filter="BM_.*" --benchmark_repetitions=5 --benchmark_min_time=1s
```

This will optimize branch prediction and inlining based on actual runtime behavior.
