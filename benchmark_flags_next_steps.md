# Benchmark Flags: Next Steps

## Background

Current testing revealed that CV is an unreliable metric for micro-benchmarks. Instead, we should use "CV of means" (measuring how much the mean time varies between runs).

**Current finding**: `min_time=0.1s, REPS=10` gives 1.59% variation in means (can detect 5.5% regressions)

## Tasks

### 1. Change Benchmarking Scripts to Stop Using CV and Compare Means

**Scripts to modify:**

#### `scripts/benchmark_current.sh`
- **Current behavior**: Runs benchmarks, outputs JSON
- **Needed change**: Add comparison mode that:
  - Runs benchmark twice (or N times)
  - Extracts mean times from each run
  - Calculates: `|mean1 - mean2| / mean1 * 100`
  - Reports: "Mean difference: X.XX%" instead of CV
  - Optional: Add `--compare` flag to enable comparison mode

#### Test scripts (optional cleanup):
These scripts use CV and may be obsolete:
- `test_cv_stability.sh` - uses CV of CVs
- `test_cv_reliability.sh` - uses CV
- `test_runtime_vs_reliability.sh` - uses CV

**Decision needed**: Keep these for historical reference or delete?

### 2. Check Various REPS and min_times

**Goal**: Find the sweet spot where:
- Benchmark runs fast (< 5 seconds total)
- Mean difference is consistently low

**Configurations to test:**

| min_time | REPS | Expected Runtime | Expected CV of Means | Notes |
|----------|------|------------------|----------------------|-------|
| 0.05s    | 3    | ~0.5s            | ~5%?                 | Very fast |
| 0.05s    | 5    | ~0.8s            | ~3%?                 | Fast |
| 0.05s    | 10   | ~1.5s            | ~2%?                 | Moderate |
| 0.1s     | 3    | ~1.0s            | 2.36% (known)        | Good balance |
| 0.1s     | 10   | ~2.5s            | 1.59% (known)        | Best known |
| 0.2s     | 3    | ~2.0s            | ~1%?                 | Slower |
| 0.2s     | 10   | ~5.0s            | ~0.5%?               | Very slow |

**Test each configuration** with the meta-test (step 3).

### 3. Find Configuration Where Mean Difference is CONSISTENTLY < 3%

**Meta-test of means** (analogous to "CV of CVs"):

**Procedure:**
1. Pick a configuration (e.g., `min_time=0.05s, REPS=5`)
2. Run the benchmark 20 times independently
3. For each run, extract the mean time
4. Calculate:
   - Average of all 20 means
   - Standard deviation of means
   - **CV of means** = (stddev / average) * 100
   - Maximum pairwise difference between any two means

**Success criteria:**
- **CV of means < 3%** (consistently stable)
- Runtime < 5 seconds (fast enough for development)
- Bonus: Maximum pairwise difference < 5% (good regression detection)

**Example test:**
```bash
# Test min_time=0.05s, REPS=5
for run in {1..20}; do
  ./test/test_benchmark \
    --benchmark_repetitions=5 \
    --benchmark_min_time=0.05s \
    --benchmark_filter=BM_simple \
    --benchmark_out=test/meta_t0.05_r5_run${run}.json \
    --benchmark_out_format=json
done

# Analyze: Calculate CV of means
python3 << 'PYEOF'
import json, glob, statistics

files = sorted(glob.glob("test/meta_t0.05_r5_run*.json"))
means = []
for f in files:
    with open(f) as file:
        data = json.load(file)
    mean_bench = next(b for b in data['benchmarks']
                     if 'BM_simple' in b['name'] and b.get('aggregate_name') == 'mean')
    means.append(mean_bench['real_time'])

avg = statistics.mean(means)
std = statistics.stdev(means)
cv_of_means = (std / avg * 100)

print(f"Average mean: {avg:.2f} ns")
print(f"CV of means:  {cv_of_means:.2f}%")
print(f"Max diff:     {(max(means) - min(means)) / avg * 100:.2f}%")

if cv_of_means < 3:
    print("✓ ACCEPTABLE: CV of means < 3%")
else:
    print("✗ UNSTABLE: CV of means >= 3%")
PYEOF
```

### 4. Document Final Recommendation

Once testing is complete, document:

**Recommended Settings:**
- `min_time=X.XXs`
- `REPS=N`
- **Runtime**: ~X.X seconds
- **CV of means**: X.X%
- **Can detect regressions >**: X.X%

**Update these files:**
- `BENCHMARK_RECOMMENDATIONS.md` - update with new findings
- `scripts/benchmark_current.sh` - set new defaults
- `.github/workflows/` (if using CI) - update benchmark commands

## Expected Outcome

A clear, data-driven answer to:
> "What benchmark settings should I use that are fast AND reliably detect performance regressions?"

**Hypothesis**:
- `min_time=0.05s, REPS=5` might be the sweet spot
- ~1 second runtime
- ~2-3% CV of means
- Can detect regressions > 5-7%

**But we need to test to confirm!**
