# Optimization Testing Methodology

This document describes the standardized process for testing performance optimizations in OmNomNum.

## Overview

Every optimization candidate must go through a 3-phase process:
1. **Baseline Profiling** - Understand current performance with valgrind + Google Benchmark
2. **Modified Profiling** - Measure changes with optimization applied
3. **Analysis & Decision** - Compare results and decide accept/reject

## Required Tools

```bash
# Profiling tools
apt-get install valgrind  # For callgrind and massif

# Benchmarking (already available)
# Google Benchmark v1.9.4+ with statistical analysis
```

## Phase 1: Baseline Profiling

### Step 1.1: Clean Build for Baseline

```bash
# Clean any previous builds
make clean

# Build optimized baseline (without the proposed optimization)
make test/test_benchmark
```

### Step 1.2: Run Valgrind Callgrind (CPU Profiling)

```bash
# Profile CPU instruction counts with callgrind
valgrind --tool=callgrind \
    --callgrind-out-file=baseline_callgrind.out \
    ./test/test_benchmark --benchmark_filter="BM_many_numbers" \
    --benchmark_min_time=0.1s

# Analyze results
callgrind_annotate baseline_callgrind.out > baseline_callgrind_analysis.txt

# Extract key metrics
grep -E "(realloc|malloc|free|insertYYSTYPE|ensureYYSTYPECapacity)" baseline_callgrind_analysis.txt > baseline_hotspots.txt
```

**What to look for:**
- Total instruction count (Ir column)
- Allocation function costs (malloc, realloc, free)
- Target function overhead (the code you're optimizing)
- Percentage of total time

**Example output:**
```
54,341,939  realloc                     [/lib/x86_64-linux-gnu/libc.so.6]
 7,222,000  insertYYSTYPE               [omnomnum]
1,292,956,650  PROGRAM TOTALS
```

### Step 1.3: Run Valgrind Massif (Memory Profiling)

```bash
# Profile heap allocations with massif
valgrind --tool=massif \
    --massif-out-file=baseline_massif.out \
    ./test/test_benchmark --benchmark_filter="BM_many_numbers" \
    --benchmark_min_time=0.1s

# Analyze results
ms_print baseline_massif.out > baseline_massif_analysis.txt
```

**What to look for:**
- Peak heap usage
- Allocation/deallocation patterns
- Stack vs heap breakdown
- Memory churn (frequent alloc/free cycles)

### Step 1.4: Run Google Benchmark (Real Performance)

```bash
# Run with statistical analysis
./test/test_benchmark \
    --benchmark_filter="BM_.*" \
    --benchmark_repetitions=5 \
    --benchmark_min_time=1s \
    --benchmark_report_aggregates_only=true \
    --benchmark_out=baseline_benchmark.json \
    --benchmark_out_format=json

# Extract mean times for comparison
cat baseline_benchmark.json | jq -r '.benchmarks[] | select(.run_type == "aggregate" and .aggregate_name == "mean") | "\(.name): \(.real_time) ns"'
```

**What to look for:**
- Mean real_time (wall clock time)
- Standard deviation (stddev)
- Coefficient of variation (cv) - should be <5% for reliable results
- All three benchmarks: BM_simple, BM_long_string, BM_many_numbers

**Save results:**
```bash
# Save for future reference
echo "=== Baseline Results ===" > baseline_summary.txt
echo "" >> baseline_summary.txt
echo "Callgrind:" >> baseline_summary.txt
cat baseline_hotspots.txt >> baseline_summary.txt
echo "" >> baseline_summary.txt
echo "Benchmark:" >> baseline_summary.txt
cat baseline_benchmark.json | jq -r '.benchmarks[] | select(.aggregate_name == "mean") | "\(.name): \(.real_time) ns"' >> baseline_summary.txt
```

## Phase 2: Modified Profiling

### Step 2.1: Apply Optimization

Make the code changes for your optimization. Document what you changed:

```bash
# Example: Document the change
echo "OPT: Buffer reuse in normalize()" > optimization_description.txt
echo "Files modified:" >> optimization_description.txt
echo "  - omnomnum.c lines 430-437, 538-544" >> optimization_description.txt
echo "  - scanner.def.c line 111" >> optimization_description.txt
```

### Step 2.2: Clean Build with Optimization

```bash
# Clean to ensure full rebuild
make clean

# Build with optimization applied
make test/test_benchmark
```

### Step 2.3: Run Valgrind Callgrind

```bash
valgrind --tool=callgrind \
    --callgrind-out-file=modified_callgrind.out \
    ./test/test_benchmark --benchmark_filter="BM_many_numbers" \
    --benchmark_min_time=0.1s

callgrind_annotate modified_callgrind.out > modified_callgrind_analysis.txt
grep -E "(realloc|malloc|free|insertYYSTYPE|ensureYYSTYPECapacity)" modified_callgrind_analysis.txt > modified_hotspots.txt
```

### Step 2.4: Run Valgrind Massif

```bash
valgrind --tool=massif \
    --massif-out-file=modified_massif.out \
    ./test/test_benchmark --benchmark_filter="BM_many_numbers" \
    --benchmark_min_time=0.1s

ms_print modified_massif.out > modified_massif_analysis.txt
```

### Step 2.5: Run Google Benchmark

```bash
./test/test_benchmark \
    --benchmark_filter="BM_.*" \
    --benchmark_repetitions=5 \
    --benchmark_min_time=1s \
    --benchmark_report_aggregates_only=true \
    --benchmark_out=modified_benchmark.json \
    --benchmark_out_format=json
```

## Phase 3: Analysis & Decision

### Step 3.1: Compare Callgrind Results

```bash
# Compare instruction counts
echo "=== Callgrind Comparison ===" > comparison.txt
echo "" >> comparison.txt
echo "Baseline hotspots:" >> comparison.txt
cat baseline_hotspots.txt >> comparison.txt
echo "" >> comparison.txt
echo "Modified hotspots:" >> comparison.txt
cat modified_hotspots.txt >> comparison.txt
```

**Manual analysis:**
1. Calculate instruction count reduction for target functions
2. Calculate percentage reduction
3. Verify the optimization mechanism worked as expected

**Example:**
```
Baseline realloc: 54,341,939 instructions (4.20%)
Modified realloc: 28,910,570 instructions (2.44%)
Reduction: 25,431,369 instructions (46.8% reduction) ✓ CONFIRMED
```

### Step 3.2: Compare Massif Results

```bash
# Extract peak memory usage
echo "=== Massif Comparison ===" >> comparison.txt
grep "peak" baseline_massif_analysis.txt >> comparison.txt
grep "peak" modified_massif_analysis.txt >> comparison.txt
```

### Step 3.3: Compare Benchmark Results

```bash
# Side-by-side comparison
echo "" >> comparison.txt
echo "=== Benchmark Comparison ===" >> comparison.txt
echo "" >> comparison.txt
echo "Baseline:" >> comparison.txt
cat baseline_benchmark.json | jq -r '.benchmarks[] | select(.aggregate_name == "mean") | "\(.name): \(.real_time) ns"' >> comparison.txt
echo "" >> comparison.txt
echo "Modified:" >> comparison.txt
cat modified_benchmark.json | jq -r '.benchmarks[] | select(.aggregate_name == "mean") | "\(.name): \(.real_time) ns"' >> comparison.txt
```

**Calculate improvements:**
```bash
# Python script to calculate percentage changes
cat > calculate_improvement.py << 'EOF'
import json
import sys

with open('baseline_benchmark.json') as f:
    baseline = json.load(f)
with open('modified_benchmark.json') as f:
    modified = json.load(f)

# Extract mean times
baseline_times = {b['name'].split('/')[0]: b['real_time']
                  for b in baseline['benchmarks']
                  if b.get('aggregate_name') == 'mean'}
modified_times = {b['name'].split('/')[0]: b['real_time']
                  for b in modified['benchmarks']
                  if b.get('aggregate_name') == 'mean'}

print("\n=== Performance Changes ===")
for name in sorted(baseline_times.keys()):
    if name in modified_times:
        base = baseline_times[name]
        mod = modified_times[name]
        change = ((base - mod) / base) * 100
        print(f"{name:20} {base:8.1f} -> {mod:8.1f} ns  ({change:+6.2f}%)")
EOF

python3 calculate_improvement.py >> comparison.txt
```

### Step 3.4: Decision Criteria

Apply these rules to decide whether to keep the optimization:

**ACCEPT if:**
- ✅ Callgrind shows expected mechanism (e.g., reduced realloc calls)
- ✅ All benchmarks show improvement OR
- ✅ Primary benchmark (BM_many_numbers) improves >5% with no regression >5% on others
- ✅ No unexpected memory increase

**INVESTIGATE FURTHER if:**
- ⚠️ Callgrind shows improvement but benchmark doesn't (or vice versa)
- ⚠️ Mixed results (some benchmarks improve, some regress <5%)
- ⚠️ Results don't match hypothesis

**REJECT if:**
- ❌ Any benchmark regresses >5%
- ❌ Callgrind shows optimization didn't work as expected
- ❌ Memory usage significantly increases
- ❌ Net performance is negative

### Step 3.5: Document Results

Create a results document (e.g., `OPT1_RESULTS.md`):

```markdown
# [Optimization Name] Results

## Optimization Description
[What was changed and why]

## Testing Date
[Date]

## Callgrind Results
| Metric | Baseline | Modified | Change |
|--------|----------|----------|--------|
| realloc instructions | X | Y | Z% |
| Total instructions | X | Y | Z% |

## Benchmark Results
| Benchmark | Baseline | Modified | Change |
|-----------|----------|----------|--------|
| BM_simple | X ns | Y ns | Z% |
| BM_long_string | X ns | Y ns | Z% |
| BM_many_numbers | X ns | Y ns | Z% |

## Analysis
[Why it worked/didn't work]

## Decision
**[ACCEPTED/REJECTED]**

## Next Steps
[If rejected, what to try next]
```

## Complete Example Workflow

```bash
#!/bin/bash
# Full optimization testing workflow

OPT_NAME="buffer_reuse"
BASELINE_DIR="baseline_${OPT_NAME}"
MODIFIED_DIR="modified_${OPT_NAME}"

# Create result directories
mkdir -p "$BASELINE_DIR" "$MODIFIED_DIR"

# Phase 1: Baseline
echo "=== Phase 1: Baseline Profiling ==="
make clean && make test/test_benchmark

valgrind --tool=callgrind --callgrind-out-file="$BASELINE_DIR/callgrind.out" \
    ./test/test_benchmark --benchmark_filter="BM_many_numbers" --benchmark_min_time=0.1s

valgrind --tool=massif --massif-out-file="$BASELINE_DIR/massif.out" \
    ./test/test_benchmark --benchmark_filter="BM_many_numbers" --benchmark_min_time=0.1s

./test/test_benchmark --benchmark_filter="BM_.*" --benchmark_repetitions=5 \
    --benchmark_min_time=1s --benchmark_report_aggregates_only=true \
    --benchmark_out="$BASELINE_DIR/benchmark.json" --benchmark_out_format=json

# Phase 2: Apply optimization and test
echo "=== Phase 2: Modified Profiling ==="
# [Apply your optimization here]
# git apply my_optimization.patch  # or edit files manually

make clean && make test/test_benchmark

valgrind --tool=callgrind --callgrind-out-file="$MODIFIED_DIR/callgrind.out" \
    ./test/test_benchmark --benchmark_filter="BM_many_numbers" --benchmark_min_time=0.1s

valgrind --tool=massif --massif-out-file="$MODIFIED_DIR/massif.out" \
    ./test/test_benchmark --benchmark_filter="BM_many_numbers" --benchmark_min_time=0.1s

./test/test_benchmark --benchmark_filter="BM_.*" --benchmark_repetitions=5 \
    --benchmark_min_time=1s --benchmark_report_aggregates_only=true \
    --benchmark_out="$MODIFIED_DIR/benchmark.json" --benchmark_out_format=json

# Phase 3: Compare and analyze
echo "=== Phase 3: Analysis ==="
callgrind_annotate "$BASELINE_DIR/callgrind.out" > "$BASELINE_DIR/analysis.txt"
callgrind_annotate "$MODIFIED_DIR/callgrind.out" > "$MODIFIED_DIR/analysis.txt"

# Generate comparison report
echo "Baseline vs Modified comparison saved to: comparison_${OPT_NAME}.txt"
```

## Tips and Best Practices

### 1. Always Test All Benchmarks
Don't optimize for just one benchmark - check all three:
- **BM_simple**: Catches overhead in simple cases
- **BM_long_string**: Represents typical usage
- **BM_many_numbers**: Stress test for allocation-heavy paths

### 2. Use Sufficient Iterations
- Callgrind: At least 0.1s min time (ensures warm cache)
- Benchmark: At least 1s min time with 5 repetitions (statistical validity)

### 3. Verify Mechanism
If callgrind shows no change but benchmark improves (or vice versa), investigate why:
- Cache effects?
- Branch prediction?
- Profiling overhead masking real improvement?

### 4. Document Rejections
Failed optimizations are valuable data:
- Document why it failed
- What was learned
- Alternative approaches to try

### 5. Baseline Comparison
Always compare against the most recent committed baseline, not against a previous optimization attempt.

### 6. Statistical Validity
Check coefficient of variation (cv):
- cv < 5%: Results are reliable
- cv > 10%: Re-run with more iterations or longer min_time

## Common Pitfalls

### ❌ Insufficient Warm-up
**Problem**: First benchmark run shows different results than subsequent runs.
**Solution**: Use `--benchmark_min_time=1s` to ensure sufficient warm-up.

### ❌ Comparing Different Build States
**Problem**: Comparing optimization+other_changes vs baseline.
**Solution**: Always `make clean` before rebuilding.

### ❌ Profiling with Release Optimizations
**Problem**: Compiler optimizations can obscure what's happening.
**Solution**: For callgrind, it's fine to use -O3 (matches production). For debugging, use -O1.

### ❌ Ignoring Regressions
**Problem**: Accepting optimization that improves one benchmark but regresses another.
**Solution**: All benchmarks matter - reject if any regress >5%.

### ❌ Not Verifying Mechanism
**Problem**: Benchmark shows improvement but don't understand why.
**Solution**: Always verify with callgrind that the expected change occurred.

## Summary Checklist

Before committing an optimization:

- [ ] Baseline callgrind profiling completed
- [ ] Baseline massif profiling completed
- [ ] Baseline benchmark with 5 reps × 1s completed
- [ ] Modified callgrind profiling completed
- [ ] Modified massif profiling completed
- [ ] Modified benchmark with 5 reps × 1s completed
- [ ] Callgrind confirms expected mechanism
- [ ] No benchmark shows >5% regression
- [ ] Results documented in `OPT_X_RESULTS.md`
- [ ] Decision documented with rationale
- [ ] Code changes documented in commit message

## File Naming Convention

Use consistent naming for all profiling outputs:

```
baseline_callgrind.out
baseline_callgrind_analysis.txt
baseline_massif.out
baseline_massif_analysis.txt
baseline_benchmark.json

modified_callgrind.out
modified_callgrind_analysis.txt
modified_massif.out
modified_massif_analysis.txt
modified_benchmark.json

comparison.txt
OPT_NAME_RESULTS.md
```

This makes it easy to track results across multiple optimization attempts.
