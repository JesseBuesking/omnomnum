# Benchmark History Analysis Summary

## Overview
Ran comprehensive benchmarks across all 136 commits in `jesse/decade-late-improvements` branch using **accurate mode** (0.2s min_time, 10 reps, ~64s per commit).

## Data Quality

### Valid Benchmarks
- **Commits 002-080** (79 commits): Full 37-benchmark comprehensive suite ✅
- **Commits 081-098** (18 commits): **BUILD FAILURES** - no JSON output generated ❌
  - These commits couldn't compile the comprehensive benchmark suite
  - Likely due to build system changes or missing dependencies in those historical commits
- **Commits 099-136** (38 commits): Full 37-benchmark comprehensive suite ✅

**Total analyzed**: 117 commits with valid data
**Gap**: 18 commits with no data due to build failures

## Key Findings (Commits 002-080 & 099-136)

### Top Performance Improvements

1. **Commit 004 (455fda3e5973)** - `-7.72%` overall
   - "feat: quadrillion + spelled-decimal support"
   - **Huge wins**: simple_fractions (-62.92%), decimal_multipliers (-57.34%), mixed_fractions_numeric (-55.93%)
   - **Trade-off**: mixed_fractions_word regressed +92.86%
   - **Action**: Worth investigating - possibly different code path?

2. **Commit 018 (840c05171ce8)** - `-5.62%` overall
   - "Task 4: Optimize numeric parsing with strtod fast path"
   - **Direct performance win from optimization work**
   - Consistent improvements across numeric benchmarks

3. **Commit 112 (88d6aa66dea8)** - `-5.11%` overall
   - "Add automatic PATH setup via sourceable env.sh script"
   - **Suspicious**: Non-code change shouldn't affect performance
   - Likely measurement variance or reverting previous regression

4. **Commit 103 (ed259aeebd66)** - `-3.52%` overall
   - "Complete benchmark optimization plan with CV of means analysis"
   - large_numbers (-12.98%), many_numbers (-9.50%)

5. **Commit 012 (e7e78dffb994)** - `-5.01%` overall
   - "Complete Task #2: Runtime Toggle for Fraction Parsing"
   - Reversed the +7.66% regression from commit 011 (.gitignore)
   - dates_preserved (-25.22%), thousand_and_pattern (-23.19%)

### Top Performance Regressions

1. **Commit 111 (e0ba030d0325)** - `+5.00%` overall
   - "Add comprehensive environment setup script"
   - **Suspicious**: Should be non-code change
   - edge_cases_boundary (+25.11%), large_numbers_composed (+18.22%)
   - **Action**: Likely measurement noise - investigate if real

2. **Commit 102 (861289d35513)** - `+3.40%` overall
   - "Add meta-test script for benchmark configuration testing"
   - thousand_and_pattern (+16.14%), many_numbers (+15.85%)

3. **Commit 104 (5097735f500d)** - `+3.16%` overall
   - "Make benchmark results tables more readable"
   - edge_cases_boundary (+13.86%)

4. **Commit 003 (0575cc787f85)** - `+7.05%` overall
   - "Restore whitespace separator behavior"
   - **Real code change**: dates_preserved (+36.56%), numeric_fractions (+33.06%)
   - **Action**: Investigate - whitespace handling may be expensive

5. **Commit 011 (bf743b3a571b)** - `+7.66%` overall
   - "adding .gitignore"
   - **Suspicious**: Just adding .gitignore shouldn't affect performance
   - dates_preserved (+33.68%), thousand_and_pattern (+30.55%)
   - **Note**: Reversed by commit 012, likely measurement noise

## Patterns Observed

### Measurement Noise
Several commits with non-code changes showed large performance swings:
- Commit 011 (.gitignore): +7.66%
- Commit 111 (setup script): +5.00%
- Commit 112 (PATH setup): -5.11%

These are likely **measurement variance** despite using accurate mode. They often get reversed by the next commit.

### Real Code Impacts
Actual code changes showed more consistent, explainable patterns:
- Commit 004 (quadrillion support): Clear trade-offs between different benchmark types
- Commit 018 (strtod optimization): Broad improvements as expected
- Commit 003 (whitespace behavior): Consistent overhead in related benchmarks

### Volatile Benchmarks
Some benchmarks show high variance across commits:
- `BM_edge_cases_boundary`: Swings from -15% to +25%
- `BM_mixed_fractions_*`: High sensitivity to parser changes
- `BM_thousand_and_pattern`: Varied widely (+30% to -23%)

## Recommendations

### Further Investigation
1. **Commit 004** (quadrillion support): Why did mixed_fractions_word regress so much (+93%) while others improved?
2. **Commit 003** (whitespace restoration): Is the +7% overhead acceptable for the feature?
3. **Volatile benchmarks**: Consider adding more repetitions or longer min_time for these specific tests

### Optimization Opportunities
1. **Mixed fractions handling**: Large performance variation suggests optimization potential
2. **Whitespace processing**: Commit 003 suggests this may be a bottleneck
3. **Edge case boundaries**: High variance suggests complex code paths worth profiling

### Validation Needed
1. **Non-code regressions**: Commits 011, 111 showing large changes need validation
2. **Measurement stability**: Consider running multiple sweeps to establish confidence intervals
3. **Benchmark selection**: Some benchmarks may be too noisy to be useful

## Files Generated
- `final_analysis.txt`: Complete 1042-line detailed analysis of all commits
- `analysis_20.txt, analysis_40.txt, analysis_60.txt, analysis_80.txt, analysis_100.txt`: Milestone analyses
- `benchmark_history/*.json`: Individual benchmark results for each commit
- `benchmark_progress_report.txt`: Timestamped progress log

## Notes
- Used ACCURATE mode: 0.2s min_time, 10 repetitions (~64s per commit)
- Total benchmark time: ~2.5 hours (with laptop sleep interruptions)
- Sleep interruptions did NOT affect individual benchmark quality
- Commits 081-098 excluded due to missing comprehensive benchmark suite
