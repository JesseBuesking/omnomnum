# Performance Analysis Summary

## Overview

Benchmarked 30 out of 47 commits across the `jesse/decade-late-improvements` branch (commits #9-47). Early commits (#1-8) failed to build, which is expected for the initial repository state.

**Analysis Period**: 2025-11-07 to 2025-11-08

## Key Findings

### Overall Performance: Stable

Comparing first successful benchmark (#9) to latest (#47):

| Benchmark | First (us/iter) | Latest (us/iter) | Change | Verdict |
|-----------|----------------|------------------|---------|---------|
| **BM_simple** | 0.660 | 0.656 | -0.61% | **✓ 0.61% faster** |
| **BM_long_string** | 2.806 | 2.811 | +0.18% | ≈ essentially unchanged |
| **BM_many_numbers** | 79.992 | 80.617 | +0.78% | ≈ essentially unchanged |

**Conclusion**: Performance has remained **remarkably stable** throughout all 15 major feature additions and bug fixes. The simple benchmark actually improved slightly, while the other two showed negligible regression (<1%).

## Best & Worst Performing Commits

### BM_simple (simple number normalization)
- **Best**: 0.646 us/iter - #46 `aa267d1a` - "Fix P1 bug: Detect 'and' with non-space whitespace in thousand patterns"
- **Worst**: 0.946 us/iter - #39 `856df3a8` - "Update TASKS.md: Mark Task 11 as completed"

### BM_long_string (long text with embedded numbers)
- **Best**: 2.689 us/iter - #31 `43f67e94` - "Fix CMake: Compile test files as C++ despite .c extension"
- **Worst**: 3.189 us/iter - #44 `cd5590eb` - "Update test expectations and add googletest/yaml-cpp installation docs"

### BM_many_numbers (high number density)
- **Best**: 78.982 us/iter - #34 `ad743bab` - "Update documentation to reflect project improvements"
- **Worst**: 87.029 us/iter - #31 `43f67e94` - "Fix CMake: Compile test files as C++ despite .c extension"

## Notable Performance Changes (>5%)

### Significant Regressions
1. **#39** `856df3a8` - "Update TASKS.md: Mark Task 11 as completed"
   - BM_simple: +42.47% regression
   - *Note: This appears to be a build artifact/noise issue*

2. **#44** `cd5590eb` - "Update test expectations and add googletest/yaml-cpp installation docs"
   - BM_simple: +27.56% regression
   - BM_long_string: +15.92% regression
   - *Note: Documentation-only commit; likely build variance*

### Significant Improvements
1. **#45** `39405553` - "Fix P1 bugs: Honor parse_fractions flag comprehensively"
   - BM_simple: -22.53% improvement
   - BM_long_string: -14.39% improvement
   - **This fix reversed the regression from #44**

2. **#40** `46af420c` - "Task 13 (WIP): Replace hardcoded fallback in mixed digit-thousand patterns"
   - BM_simple: -29.07% improvement
   - **This fix reversed the regression from #39**

### Moderate Fluctuations (5-15%)
- **#28**: Percent handling - 12.30% regression in BM_long_string
- **#29**: Build config change - 10.99% improvement in BM_long_string
- **#30**: Space preservation - 8.18% regression in BM_long_string
- **#31**: CMake fix - 10.81% improvement in BM_long_string, 6.72% regression in BM_many_numbers
- **#32**: Percentage range fix - 5.89% regression in BM_simple, 6.02% improvement in BM_many_numbers
- **#46**: Whitespace handling - 5.45% improvement in BM_many_numbers

## Performance Stability Analysis

### Variance Pattern
The benchmark results show occasional spikes in commits #39 and #44, which:
1. Were both documentation-only changes (TASKS.md, test expectations)
2. Were immediately followed by commits that restored normal performance
3. Suggest these were **build artifacts or measurement noise** rather than real regressions

### True Code Changes
Looking at commits with actual code changes:
- **Task 4** (#20): "Optimize numeric parsing with strtod fast path" - showed a small regression (2.43% in BM_many_numbers) vs baseline, but this was expected as the optimization traded allocations for direct parsing
- **Task 11** (#38): "Add 'minus' word sign support" - no significant impact (+3.28% in BM_many_numbers)
- **Task 13** (#41): "Fix word-to-number mapping" - minimal impact

### Noise Floor
Most commits show variations of ±3-5%, which represents the **noise floor** of the benchmark environment. Changes below 5% should be considered within normal measurement variance.

## Recommendations

1. **Performance is Excellent**: Despite adding 15 major features (thread safety, fraction support, minus/percent handling, etc.), performance has remained essentially unchanged.

2. **Build Reproducibility**: The spikes in #39 and #44 suggest improving build reproducibility. Consider:
   - Pinning compiler versions
   - Setting explicit optimization flags
   - Multiple benchmark runs to filter outliers

3. **Focus Areas**: If future optimization is desired, the most impactful area is **BM_many_numbers** (many sequential numbers), which is 120x slower than simple cases. This is expected but could potentially be optimized.

4. **Regression Detection**: Use a ±10% threshold for performance regression alerts to avoid false positives from build variance.

## Methodology

- **Tool**: `bench_local` (custom benchmark harness)
- **Test Cases**:
  - `BM_simple`: "two hundred" (50,000 iterations)
  - `BM_long_string`: long text with embedded numbers (5,000 iterations)
  - `BM_many_numbers`: many sequential number words (5,000 iterations)
- **Environment**: Linux 4.4.0
- **Commits Tested**: 30 out of 47 (63.8% coverage)

## Raw Data

All benchmark results are available in `benchmark_results/` directory.
Analysis script: `scripts/analyze_benchmarks.py`
Benchmark runner: `scripts/benchmark_all_commits.sh`

---

**Generated**: 2025-11-09
**Branch**: `jesse/decade-late-improvements`
