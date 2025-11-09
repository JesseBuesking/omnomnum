# Benchmark Configuration Recommendations

## UPDATED: The Right Metric Changes Everything

**Previous finding**: CV of individual times < 3.6% was unreliable.
**New finding**: **CV of means across runs** < 3% is ACHIEVABLE and reliable!

The key insight: We should measure how much the MEAN time varies between runs, not how much individual samples vary within a run.

## New Methodology: CV of Means

Instead of checking CV within a single benchmark run, we now:
1. Run the benchmark N times independently (N=5 in our tests)
2. Extract the mean time from each run
3. Calculate CV of those N means: `(stddev(means) / avg(means)) × 100`

This measures **reproducibility** - how consistent are results across runs?

## Tested Configurations (CV of Means)

Tested 9 configurations with 5 independent comparison runs each:

### Fast Configs

| Config | Single Run | Meta-Test* | CV | Max Diff | Status |
|--------|------------|------------|-------|----------|---------|
| 0.05s × 3 reps  | ~7s  | 26s  | 3.71% | 8.79% | ✗ FAIL (too unstable) |
| 0.05s × 5 reps  | ~9s  | 41s  | 2.11% | 4.99% | ✓ PASS ⚡ **Fast dev** |
| 0.05s × 10 reps | ~16s | 81s  | 2.02% | 5.00% | ✓ PASS |

### Balanced Configs

| Config | Single Run | Meta-Test* | CV | Max Diff | Status |
|--------|------------|------------|-------|----------|---------|
| 0.1s × 3 reps   | ~11s | 52s  | 2.33% | 5.54% | ✓ PASS |
| 0.1s × 5 reps   | ~16s | 81s  | 2.32% | 5.79% | ✓ PASS |
| 0.1s × 10 reps  | ~32s | 161s | 2.02% | 4.86% | ✓ PASS ⭐ **Recommended** |

### Thorough Configs

| Config | Single Run | Meta-Test* | CV | Max Diff | Status |
|--------|------------|------------|-------|----------|---------|
| 0.2s × 3 reps   | ~21s | 104s | 2.82% | 6.90% | ✓ PASS |
| 0.2s × 10 reps  | ~64s | 319s | 1.40% | 3.45% | ✓ PASS 🏆 **Most reliable** |
| 0.5s × 5 reps   | ~82s | 410s | 1.76% | 4.29% | ✓ PASS (baseline) |

\* Meta-test runtime includes 5 comparison runs for CV of means calculation

**Key findings**:
- **CV of means < 3%** is achievable and stable!
- Only the fastest config (0.05s, 3 reps) fails
- Best balance: `0.1s × 10 reps` (2.7min, 2.02% CV, detects >4.86% regressions)
- Most reliable: `0.2s × 10 reps` (5.3min, 1.40% CV, detects >3.45% regressions)

## Recommended Settings

### For Development (Speed Priority) ⚡

**min_time=0.05s, reps=5**

**Pros**:
- Fast: ~9 seconds
- Reliable: 2.11% CV of means
- Can detect regressions >5%

**Cons**:
- Less precise regression detection

**Use for**: Quick iteration during development

**Example**:
```bash
# Using BENCH_MODE
BENCH_MODE=fast bash scripts/benchmark_current.sh

# Or legacy QUICK_BENCH
QUICK_BENCH=1 bash scripts/benchmark_current.sh
```

### For CI/Testing (Balanced) ⭐ RECOMMENDED

**min_time=0.1s, reps=10**

**Pros**:
- Good speed: ~32 seconds
- Excellent reliability: 2.02% CV of means
- Can detect regressions >4.86%
- Best balance of speed vs precision

**Cons**:
- Takes half a minute

**Use for**: CI/CD pipelines, pre-merge testing

**Example**:
```bash
# Default mode (no flags needed)
bash scripts/benchmark_current.sh

# Or explicitly
BENCH_MODE=default bash scripts/benchmark_current.sh
```

### For Releases (Maximum Reliability) 🏆

**min_time=0.2s, reps=10**

**Pros**:
- Best reliability: 1.40% CV of means
- Best regression detection: >3.45%
- Most consistent results

**Cons**:
- Slower: ~64 seconds

**Use for**: Official releases, performance validation, benchmarking reports

**Example**:
```bash
BENCH_MODE=accurate bash scripts/benchmark_current.sh
```

## How to Use Mean Comparison Mode

The `benchmark_current.sh` script now supports comparing means across multiple runs:

```bash
# Run comparison mode with 5 independent runs
COMPARE_MODE=1 COMPARE_RUNS=5 \
BENCH_MIN_TIME=0.1s BENCH_REPS=10 \
bash scripts/benchmark_current.sh test/comparison.json
```

This will:
1. Run the benchmark 5 times independently
2. Calculate CV of means for each benchmark
3. Report overall statistics
4. Pass/fail based on <3% CV threshold

### Example Output
```
================================================================================
OVERALL STATISTICS
================================================================================
Average CV of means:      2.02%
Average max pairwise diff: 4.86%

✓ PASS: Average CV of means < 3%
✓ PASS: Can detect regressions > 5%
================================================================================
```

## Testing Different Configurations

Use the meta-test script to evaluate configurations on your hardware:

```bash
# Test all recommended configurations
BENCH_PREFIX=/path/to/google-benchmark bash scripts/test_benchmark_configs.sh
```

Results are saved to `test/meta_results/` with a summary showing which configs pass.

## Why This Works Better

**Old approach (CV within run)**:
- Measured variance of individual timing samples
- Highly susceptible to micro-jitter, cache effects, scheduler noise
- Required 50-100 reps but still unreliable

**New approach (CV of means)**:
- Measures reproducibility: do runs give consistent mean times?
- Averages out micro-jitter within each run
- Only 5-10 reps needed with much better reliability
- Directly answers: "Can I detect a 5% regression?"

## The Bottom Line

**Using CV of means, reliable benchmarking IS achievable:**

- ⚡ **Fast**: `min_time=0.05s, reps=5` (~9s, 2.11% CV) - `BENCH_MODE=fast`
- ⭐ **Default**: `min_time=0.1s, reps=10` (~32s, 2.02% CV, detects >4.86%) - **recommended**
- 🏆 **Accurate**: `min_time=0.2s, reps=10` (~64s, 1.40% CV, detects >3.45%) - `BENCH_MODE=accurate`

All of these achieve < 3% CV of means and provide reliable regression detection.

**Usage**:
```bash
# Fast (~9s)
BENCH_MODE=fast bash scripts/benchmark_current.sh

# Default (~32s) - recommended
bash scripts/benchmark_current.sh

# Accurate (~64s)
BENCH_MODE=accurate bash scripts/benchmark_current.sh
```
