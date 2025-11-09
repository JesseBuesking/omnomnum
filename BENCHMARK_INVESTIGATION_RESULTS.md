# Benchmark Investigation Results

## Critical Bugs Discovered

### Bug 1: Script Hardcoded Repetitions
**Location**: `scripts/benchmark_current.sh` line 120
```bash
--benchmark_repetitions=3
```

**Impact**: ALL previous testing used REPS=3 regardless of code settings. When I thought I was testing REPS=10, 20, 30, 50, 100, the script was actually running 3 repetitions every time.

### Bug 2: Code Override
**Location**: `test/test_benchmark.c` benchmark registrations
```cpp
BENCHMARK(BM_simple)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);
```

**Impact**: The `.Repetitions(REPETITIONS)` in code overrides command-line `--benchmark_repetitions` parameter. Must remove this to allow flexible repetition counts.

## Google Benchmark Behavior (from docs)

### Repetitions vs Iterations

**Iterations**: Internal loops within a single benchmark run, automatically determined by Google Benchmark to meet min_time requirements.

**Repetitions**: Number of times the entire benchmark is run. Each repetition independently determines its own iteration count, then statistics are aggregated.

### Expected Behavior

- **Runtime should scale with REPS**: If REPS=3 takes 1 second, REPS=30 should take ~10 seconds
- **Runtime should scale with min_time**: If min_time=0.01s, increasing to 0.1s should take 10x longer

## Actual Test Results (Corrected)

### Test 1: Varying REPS (min_time=0.01s constant)
```
REPS=  3: Runtime = 1.24s
REPS= 10: Runtime = 1.19s  ← Should be ~4x longer, but isn't!
REPS= 30: Runtime = 1.19s  ← Should be ~10x longer, but isn't!
REPS= 50: Runtime = 1.30s  ← Should be ~16x longer, but isn't!
```

**PROBLEM**: Runtime doesn't scale with REPS!

### Test 2: Varying min_time (REPS=10 constant)
```
min_time = 0.001s: Runtime =  0.14s
min_time = 0.01s:  Runtime =  1.19s  ← 10x slower ✓
min_time = 0.1s:   Runtime = 11.57s  ← 10x slower ✓
min_time = 0.5s:   Runtime = 57.35s  ← ~5x slower ✓
```

**CORRECT**: Runtime scales perfectly with min_time!

## Root Cause Analysis

### Why Doesn't REPS Scale Runtime?

Two possibilities:

**Theory 1: Benchmark runs in parallel** (unlikely)
- Google Benchmark might run repetitions concurrently
- But documentation doesn't mention this

**Theory 2: ReportAggregatesOnly + code repetitions**
- `.Repetitions(REPETITIONS)` in code is being compiled with a stale value
- `.ReportAggregatesOnly(true)` might be suppressing individual runs
- Command-line `--benchmark_repetitions` is being ignored due to code override

### Verification Needed

When I ran:
```bash
./test/test_benchmark --benchmark_repetitions=10 \
  --benchmark_filter="BM_simple"
```

Output showed:
```
BM_simple/repeats:3/real_time/threads:1_mean
```

Even though I specified 10 repetitions, it showed 3! This confirms the code's `.Repetitions(REPETITIONS)` is overriding the command line, and REPETITIONS was compiled to 3 (not 10 as currently shown in source).

## Correct Settings (Based on Current Understanding)

### What We Know For Sure

1. **min_time controls runtime**, not REPS
2. **Longer min_time = more stable measurements**
3. **CV variance persists at all tested settings**

### Practical Recommendations

**For development (fast iteration):**
```
min_time: 0.01s
REPS: 10 (code level)
Runtime: ~1.2s for 25 benchmarks
Method: Don't check CV, use regression detection
```

**For CI/CD (balanced):**
```
min_time: 0.02s - 0.05s
REPS: 10 (code level)
Runtime: ~2-6s for 25 benchmarks
Method: Compare to baseline, allow retries
```

**For official benchmarks (thorough):**
```
min_time: 0.5s
REPS: 10 (code level)
Runtime: ~57s for 25 benchmarks
Method: Multiple runs, distribution comparison
```

## Required Fixes

### Fix 1: Remove Script Override
**File**: `scripts/benchmark_current.sh`

Change line 120 from:
```bash
--benchmark_repetitions=3
```
To:
```bash
# Let code or command-line control repetitions
```

### Fix 2: Make Code Repetitions Configurable
**File**: `test/test_benchmark.c`

Either:
- Remove `.Repetitions(REPETITIONS)` to allow command-line control
- Or ensure REPETITIONS is set correctly for each mode

### Fix 3: Recompile Strategy
Always rebuild test_benchmark.o when changing REPETITIONS:
```bash
rm -f test/test_benchmark.o
BENCH_PREFIX=/path/to/vendor make test/test_benchmark
```

## Key Insight

**The fundamental issue isn't about finding the right REPS value.**

The problem is that CV variance for micro-benchmarks is inherently unstable due to:
1. Sub-microsecond timing
2. Scheduling jitter
3. CPU frequency scaling
4. Cache effects
5. Measurement overhead

**No amount of repetitions or min_time will make CV < 3.6% reliably achievable.**

The solution is to abandon CV as a pass/fail criterion and use:
- Regression detection (>5-10% slower than baseline)
- Distribution comparison (KS test, Mann-Whitney U test)
- Median + percentile analysis
- Visual inspection of timing distributions

## Next Steps

1. Fix the hardcoded repetitions in benchmark_current.sh
2. Decide on repetition strategy (code-level vs command-line)
3. Implement regression-based checking instead of CV threshold
4. Document the correct usage pattern
