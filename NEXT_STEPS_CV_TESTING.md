# Next Steps: Proper CV of CV Testing with Variable REPS

## What Was Done

### 1. Fixed Code to Allow CLI Repetition Control
**Status**: ✅ COMPLETED

Removed all `.Repetitions(REPETITIONS)` calls from test/test_benchmark.c:
- Before: `BENCHMARK(BM_simple)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);`
- After: `BENCHMARK(BM_simple)->ReportAggregatesOnly(true);`

This allows command-line `--benchmark_repetitions` to control repetition count.

### 2. Identified Script Issue
**File**: `scripts/benchmark_current.sh` line 120

The script hardcodes:
```bash
--benchmark_repetitions=3
```

This should either be:
- Removed (let code default or CLI control)
- Made configurable via environment variable

### 3. Build Environment Issue
**Status**: ❌ BLOCKED

Cannot rebuild benchmark binary because:
- `/home/user/omnomnum/vendor/` directory doesn't exist
- Google Benchmark library not found
- Need to reinstall or locate Google Benchmark

## What Needs to Be Done

### Step 1: Rebuild Benchmark Binary

Once Google Benchmark is available:

```bash
# Remove old binary
rm -f test/test_benchmark.o test/test_benchmark

# Rebuild with proper includes
BENCH_PREFIX=/path/to/benchmark bash rebuild_benchmark.sh
```

### Step 2: Verify REPS Parameter Works

Test that CLI repetitions actually vary:

```bash
# Test with different REPS
for REPS in 3 10 30; do
  echo "Testing REPS=$REPS:"

  # Run and check output
  ./test/test_benchmark --benchmark_min_time=0.005s \
    --benchmark_repetitions=$REPS \
    --benchmark_filter="BM_simple" \
    --benchmark_display_aggregates_only=false 2>&1 | grep "repeats:"

  # Should show "repeats:$REPS", not always "repeats:3"
done
```

Expected output:
```
Testing REPS=3:
BM_simple/repeats:3/...

Testing REPS=10:
BM_simple/repeats:10/...  ← Should change!

Testing REPS=30:
BM_simple/repeats:30/...  ← Should change!
```

### Step 3: Test Runtime Scaling

Verify that REPS actually affects runtime (it should now!):

```bash
for REPS in 3 10 30 50; do
  START=$(date +%s.%N)

  ./test/test_benchmark --benchmark_min_time=0.005s \
    --benchmark_repetitions=$REPS \
    >/dev/null 2>&1

  END=$(date +%s.%N)
  RUNTIME=$(echo "$END - $START" | bc)

  echo "REPS=$REPS: Runtime=${RUNTIME}s"
done
```

Expected: Runtime should scale with REPS (e.g., REPS=30 should take ~10x longer than REPS=3).

### Step 4: CV of CV Testing

Run the stability test with proper REPS variation:

```bash
# For each REPS level, run 10 times and measure CV variation
for REPS in 2 3 5 10 20 30 50; do
  echo "Testing REPS=$REPS (10 runs):"

  # Collect CVs from 10 independent runs
  CVS=()
  for run in {1..10}; do
    CV=$(./test/test_benchmark --benchmark_min_time=0.005s \
         --benchmark_repetitions=$REPS \
         --benchmark_filter="BM_simple" 2>&1 | \
         grep "BM_simple.*_cv" | awk '{print $2}')
    CVS+=($CV)
  done

  # Calculate "CV of the CVs"
  python3 << EOF
import statistics
cvs = [${CVS[@]}]
mean = statistics.mean(cvs)
stdev = statistics.stdev(cvs)
cv_of_cvs = (stdev / mean * 100) if mean > 0 else 0
print(f"  Mean CV: {mean:.2f}%")
print(f"  CV of CVs: {cv_of_cvs:.1f}%")
if cv_of_cvs > 50:
    print("  Status: UNRELIABLE")
elif cv_of_cvs > 25:
    print("  Status: UNSTABLE")
else:
    print("  Status: STABLE")
EOF

done
```

### Step 5: Failure Rate Testing

Measure how often CV exceeds 3.6% threshold:

```bash
for REPS in 3 10 20 30 50; do
  echo "REPS=$REPS, min_time=0.005s (20 runs):"

  FAILURES=0
  for run in {1..20}; do
    CV=$(./test/test_benchmark --benchmark_min_time=0.005s \
         --benchmark_repetitions=$REPS \
         --benchmark_filter="BM_simple" 2>&1 | \
         grep "BM_simple.*_cv" | awk '{print $2}')

    if (( $(echo "$CV >= 3.6" | bc -l) )); then
      ((FAILURES++))
    fi
  done

  FAIL_RATE=$((FAILURES * 5))
  echo "  Failures: $FAILURES/20 ($FAIL_RATE%)"

  if (( FAIL_RATE > 10 )); then
    echo "  Status: UNRELIABLE"
  elif (( FAIL_RATE > 5 )); then
    echo "  Status: MARGINAL"
  else
    echo "  Status: RELIABLE"
  fi
done
```

## Expected Results

Based on the corrected understanding:

### Runtime Scaling
- **Previous (broken)**: REPS=3→50 all took ~1.2s (script forced REPS=3)
- **Expected (fixed)**: REPS should scale roughly linearly
  - REPS=3: ~0.5s
  - REPS=10: ~1.5s
  - REPS=30: ~4.5s
  - REPS=50: ~7.5s

### CV Stability
Previous tests were invalid (all used REPS=3). Need to retest with actual varying REPS.

**Hypothesis**: Higher REPS should provide:
1. Lower "CV of CVs" (more stable measurements)
2. Lower failure rate (fewer runs exceeding 3.6%)

But may not eliminate the problem entirely due to:
- Sub-microsecond timing inherent variance
- Scheduling jitter
- Measurement overhead

## Key Questions to Answer

1. **Does REPS actually affect CV stability?**
   - If yes, what's the minimum REPS for reliable CV < 3.6%?
   - If no, confirms CV is not a usable metric

2. **What's the runtime cost of higher REPS?**
   - Is REPS=50 worth the extra time?
   - What's the sweet spot for reliability vs speed?

3. **Does min_time interact with REPS?**
   - Test combinations: low min_time + high REPS vs high min_time + low REPS

## Files Modified

- `test/test_benchmark.c`: Removed `.Repetitions(REPETITIONS)` calls
- `rebuild_benchmark.sh`: Created script to rebuild binary (needs benchmark lib path)

## Files to Modify

- `scripts/benchmark_current.sh`: Remove or make configurable `--benchmark_repetitions=3`

## Blocker

Cannot proceed without:
- Google Benchmark library installation
- Or path to existing benchmark installation
- Check if `/home/user/omnomnum/vendor/` needs to be recreated

## Once Unblocked

Run the test scripts above to generate proper data on:
- CV stability vs REPS
- Failure rate vs REPS
- Runtime vs REPS
- Optimal settings recommendation
