# Benchmark Reliability Analysis - Critical Findings

## Executive Summary

**Our initial approach was FUNDAMENTALLY FLAWED.** The low CV values (0.00-0.18%) we observed were NOT reliable. Through rigorous testing, we discovered:

1. **CV measurements themselves are highly unstable**
2. **We need to measure FAILURE RATE, not average CV**
3. **Minimum 20 repetitions required** for any reliability
4. **Even with optimal settings, 5-40% of runs exceed CV threshold**

## The Problem with Our Initial Analysis

### What We Thought
- CV < 3.6% threshold
- Tested min_time from 0.01s down to 0.000001s
- All showed "low" CV (~0.1-0.2%)
- Concluded we could go as low as 0.001s

### What We Missed
We only ran each configuration **1-5 times**. We measured the CV Google Benchmark reported, but didn't measure how STABLE that CV metric itself was across runs.

## Critical Discovery: CV Instability

### Meta-Analysis: "CV of the CVs"
Ran same config 10 times, measured variation in reported CV:

| REPS | Mean Reported CV | CV of the CVs | Status |
|------|------------------|---------------|---------|
| 2    | 0.03%           | **109.0%**    | ⚠️  UNRELIABLE |
| 3    | 0.04%           | **100.8%**    | ⚠️  UNRELIABLE |
| 5    | 0.04%           | **186.5%**    | ⚠️  UNRELIABLE |
| 10   | 0.02%           | **138.8%**    | ⚠️  UNRELIABLE |

**The CV metric itself varies by 100-186% between identical runs!**

## Correct Metric: Failure Rate

Instead of looking at average CV, we should measure: **What percentage of runs exceed the 3.6% threshold?**

### Results: min_time=0.01s, Different REPS

| REPS | Mean CV | CV Range      | Failure Rate | Status |
|------|---------|---------------|--------------|---------|
| 3    | 4.22%   | 0.07%-23.15%  | **8/20 (40%)** | ✗ UNRELIABLE |
| 5    | 3.46%   | 0.40%-13.27%  | **7/20 (35%)** | ✗ UNRELIABLE |
| 10   | 2.85%   | 0.52%-15.26%  | **4/20 (20%)** | ✗ UNRELIABLE |
| 20   | 1.55%   | 0.35%-3.97%   | **1/20 (5%)**  | ✓ RELIABLE |

### Results: REPS=20, Different min_time

| min_time | Mean CV | Failure Rate | Status |
|----------|---------|--------------|---------|
| 0.001s   | 3.44%   | **6/15 (40%)** | ✗ UNRELIABLE |
| 0.002s   | 3.34%   | **6/15 (40%)** | ✗ UNRELIABLE |
| 0.005s   | 3.62%   | **6/15 (40%)** | ✗ UNRELIABLE |
| 0.01s    | 4.20%   | **6/15 (40%)** | ✗ UNRELIABLE |
| 0.02s    | 2.88%   | **2/15 (13%)** | ⚠️  MARGINAL |

## Root Cause Analysis

### Why is BM_simple So Unstable?

BM_simple runs in ~600-700ns. At this timescale:

1. **Scheduling jitter dominates**: Context switches, CPU frequency scaling, cache effects
2. **Measurement overhead**: Clock resolution, system call overhead
3. **Statistical noise**: With such small absolute times, any variation creates huge relative variance

### Example: 20 Consecutive Runs (REPS=3, min_time=0.005s)

```
Run  1: CV = 7.08%  ← SPIKE!
Run  2: CV = 1.51%
Run  3: CV = 1.20%
Run  4: CV = 1.65%
Run  5: CV = 3.33%
Run  6: CV = 2.04%
Run  7: CV = 3.56%
Run  8: CV = 1.11%
Run  9: CV = 2.01%
Run 10: CV = 1.70%
...
Mean: 2.24%, Range: 0.69%-7.08%, Failures: 1/20 (5%)
```

The CV varies **10x** between runs (0.69% to 7.08%)!

## Recommendations

### For Development Benchmarking

**DO NOT rely on CV < 3.6% as a quality gate for fast benchmarks.**

Instead:
1. **Use absolute time comparisons**: "Was there a regression >5%?"
2. **Run multiple times**: Check if results are consistent across 5-10 runs
3. **Compare distributions**: Use median and percentiles, not just mean
4. **Accept higher variability**: Fast benchmarks (<1μs) will have inherent noise

### Minimum Settings for "Reliable" CV

If you MUST use CV as a metric:
- **Repetitions**: ≥20 (better: 50-100)
- **min_time**: ≥0.02s (better: 0.1s+)
- **Expected failure rate**: Still 5-10%

### Alternative: Use Slower Benchmarks

Benchmark **BM_many_numbers** (85μs) likely has more stable CV because:
- Longer runtime → scheduling noise less significant
- More iterations per measurement
- Better signal-to-noise ratio

## Conclusion

**CV < 3.6% is NOT a reliable criterion for micro-benchmarks (<1μs runtime).**

The "quick mode" optimization should focus on:
1. **Runtime reduction**: Fewer reps, lower min_time
2. **Consistency checking**: Run same config multiple times, verify results agree
3. **Regression detection**: Compare to baseline, not absolute CV threshold

**For micro-benchmarks, timing variance is inherent and unavoidable.**
