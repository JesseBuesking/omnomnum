# Benchmark Configuration Recommendations

## TL;DR: The Harsh Truth

**CV < 3.6% is not achievable as a reliable threshold for micro-benchmarks.**

Even with 100 repetitions, 20-47% of runs randomly exceed the threshold due to inherent timing variability at sub-microsecond scales.

## Tested Configurations

We tested REPS from 3 to 100, min_time from 0.005s to 0.5s, running 15-30 iterations of each config:

| REPS | min_time | Runtime | Failure Rate | Status |
|------|----------|---------|--------------|---------|
| 3    | 0.01s    | ~1.2s   | 20-40%       | ✗ UNRELIABLE |
| 10   | 0.01s    | ~1.2s   | 20-40%       | ✗ UNRELIABLE |
| 20   | 0.01s    | ~1.2s   | 5-20%        | ✗ UNRELIABLE |
| 30   | 0.01s    | ~1.2s   | 17-37%       | ✗ UNRELIABLE |
| 50   | 0.01s    | ~1.5s   | 27%          | ✗ UNRELIABLE |
| 100  | 0.01s    | ~2s     | 40%          | ✗ UNRELIABLE |
| 10   | 0.5s     | ~57s    | 0% (small sample) | ⚠️  Probably unreliable |

**Key finding**: Runtime scales with min_time, not REPS. But reliability doesn't improve predictably.

## Three Practical Approaches

### Option 1: Abandon CV Threshold (RECOMMENDED)

**Best for: CI/CD, regression detection, general development**

**Settings**:
- REPS: 10 (good balance)
- min_time: 0.01s
- Runtime: ~1.2 seconds for 25 benchmarks

**Don't check CV**. Instead:
1. **Baseline comparison**: Is current run >5-10% slower than baseline?
2. **Trend analysis**: Are times increasing over multiple PRs?
3. **Median + P95**: Compare median and 95th percentile times

**Example check**:
```bash
# Run benchmark
./test/test_benchmark --benchmark_min_time=0.01s --benchmark_out=current.json

# Compare to baseline (separate tool/script)
python compare_benchmarks.py baseline.json current.json --threshold=0.05  # 5% regression threshold
```

### Option 2: Accept High Failure Rate

**Best for: When you must have a CV check**

**Settings**:
- REPS: 30
- min_time: 0.01s
- Runtime: ~1.2 seconds

**Accept that**:
- 15-40% of runs will spuriously fail CV threshold
- Need to re-run when it fails
- This is NOT a bug - it's inherent to micro-benchmarking

**CI/CD strategy**:
```bash
# Run benchmark up to 3 times until it passes
for attempt in {1..3}; do
  if ./test/test_benchmark passes_cv_check; then
    break
  fi
  echo "Attempt $attempt failed CV check (expected), retrying..."
done
```

### Option 3: Slow But Thorough

**Best for: Official releases, performance validation**

**Settings**:
- REPS: 10
- min_time: 0.5s
- Runtime: ~57 seconds for 25 benchmarks

**Why this helps (a little)**:
- Longer runs average out micro-jitter
- Still not 100% reliable but better than fast modes
- Expected failure rate: ~5-10%

**Use when**:
- Generating official benchmark reports
- Validating performance claims
- You have time to spare

## Detailed Recommendations by Use Case

### Daily Development
```
REPS: 10
min_time: 0.01s
Runtime: ~1.2s
Check: Regression >10% from baseline (not CV)
```

### Pre-commit Hook
```
REPS: 10
min_time: 0.01s
Runtime: ~1.2s
Check: Median time within 5% of baseline
Allow: 1 retry if check fails
```

### CI/CD Pipeline
```
REPS: 10
min_time: 0.02s
Runtime: ~2s
Check: Mean time within 5% of baseline + trending analysis
Allow: 2 retries if check fails
```

### Performance Validation
```
REPS: 10-20
min_time: 0.1s
Runtime: ~10s
Check: Distribution comparison (KS test or similar)
Run: 5-10 times, compare distributions
```

### Official Release Benchmarks
```
REPS: 20
min_time: 0.5s
Runtime: ~60s
Check: Comprehensive report with percentiles
Run: 10 times, report median, P50, P90, P95, P99
```

## Why Doesn't Higher REPS Help?

Counter-intuitively, REPS=50 or REPS=100 performed WORSE than REPS=30. Why?

1. **Longer total runtime**: More opportunities for system interference
2. **Thermal effects**: CPU throttling during long runs
3. **Cache pollution**: Between repetitions
4. **Scheduler decisions**: More context switches over longer periods

The sweet spot appears to be REPS=10-30, not higher.

## Alternative Quality Metrics

Instead of CV < 3.6%, use:

### 1. Coefficient of Variation of the Median (CVM)
Run 10 benchmarks, take median of each, compute CV of the medians.
More stable than CV of individual runs.

### 2. Interquartile Range (IQR)
`IQR = P75 - P25`
Less sensitive to outliers than stddev.

### 3. Relative Standard Error (RSE)
`RSE = (stddev / mean) / sqrt(n)`
Accounts for sample size.

### 4. Visual Inspection
Plot distribution of times across runs.
Look for bimodal distributions or outliers.

## The Bottom Line

**For micro-benchmarks (<1μs), timing variance is INHERENT, not fixable.**

Choose settings based on your priorities:
- **Fast development iteration**: REPS=10, min_time=0.01s, skip CV check
- **Balanced**: REPS=20, min_time=0.02s, allow retries
- **Thorough**: REPS=10, min_time=0.5s, statistical comparison

**Do NOT** expect CV < 3.6% to be reliable at any setting.
