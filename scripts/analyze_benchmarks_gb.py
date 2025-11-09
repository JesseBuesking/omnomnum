#!/usr/bin/env python3
"""
Parse and analyze Google Benchmark results from all commits.
"""

import os
import json
import re
from pathlib import Path
from collections import defaultdict

def parse_benchmark_json(filepath):
    """Parse a single Google Benchmark JSON file."""
    try:
        with open(filepath, 'r') as f:
            data = json.load(f)
    except:
        return None

    # Get the filename to extract commit info
    filename = Path(filepath).stem  # e.g., "48_29bc590c"
    match = re.match(r'(\d+)_([0-9a-f]+)', filename)
    if not match:
        return None

    commit_index = int(match.group(1))
    commit_short = match.group(2)

    # Get commit metadata from context
    context = data.get('context', {})

    # Parse benchmarks
    benchmarks = data.get('benchmarks', [])

    # Extract mean values for each benchmark
    results = {}
    for bm in benchmarks:
        if bm.get('aggregate_name') == 'mean':
            # Extract benchmark family name (BM_simple, BM_long_string, BM_many_numbers)
            name = bm.get('name', '')
            # Get the base name before any slashes
            base_name = name.split('/')[0]
            real_time = bm.get('real_time', 0)  # in nanoseconds
            results[base_name] = real_time

    # Get stddev values
    stddevs = {}
    for bm in benchmarks:
        if bm.get('aggregate_name') == 'stddev':
            name = bm.get('name', '')
            base_name = name.split('/')[0]
            real_time = bm.get('real_time', 0)
            stddevs[base_name] = real_time

    return {
        'index': commit_index,
        'short': commit_short,
        'git_sha': context.get('git_sha', commit_short),
        'build_time': context.get('build_time', ''),
        'benchmarks': results,
        'stddevs': stddevs
    }

def main():
    results_dir = Path('/home/user/omnomnum/benchmark_results_gb')

    # Parse all JSON files
    all_results = []
    for filepath in sorted(results_dir.glob('*.json')):
        result = parse_benchmark_json(filepath)
        if result and result['benchmarks']:
            all_results.append(result)

    if not all_results:
        print("No benchmark results found!")
        return

    # Sort by index
    all_results.sort(key=lambda x: x['index'])

    print("=" * 120)
    print("GOOGLE BENCHMARK PERFORMANCE ANALYSIS ACROSS ALL COMMITS")
    print("=" * 120)
    print()

    # Get first and last commit for comparison
    first = all_results[0]
    last = all_results[-1]

    print(f"First successful benchmark:")
    print(f"  Commit #{first['index']}: {first['short']}")
    print(f"  Build time: {first['build_time']}")
    print()

    print(f"Latest benchmark:")
    print(f"  Commit #{last['index']}: {last['short']}")
    print(f"  Build time: {last['build_time']}")
    print()

    # Print detailed results table
    print("=" * 120)
    print("DETAILED BENCHMARK RESULTS (nanoseconds - mean real_time)")
    print("=" * 120)
    print()
    print(f"{'#':<4} {'Commit':<12} {'BM_simple (ns)':<16} {'BM_long_string (ns)':<20} {'BM_many_numbers (ns)':<22}")
    print("-" * 120)

    for result in all_results:
        bm = result['benchmarks']
        simple = bm.get('BM_simple', 0)
        long_str = bm.get('BM_long_string', 0)
        many = bm.get('BM_many_numbers', 0)

        print(f"{result['index']:<4} {result['short']:<12} {simple:>14.2f}   {long_str:>18.2f}   {many:>20.2f}")

    print()
    print("=" * 120)
    print("PERFORMANCE COMPARISON: First vs Latest")
    print("=" * 120)
    print()

    for bm_name in ['BM_simple', 'BM_long_string', 'BM_many_numbers']:
        first_val = first['benchmarks'].get(bm_name, 0)
        last_val = last['benchmarks'].get(bm_name, 0)

        if first_val > 0 and last_val > 0:
            diff = last_val - first_val
            pct_change = (diff / first_val) * 100
            direction = "slower" if diff > 0 else "faster"

            # Get stddev for context
            first_std = first['stddevs'].get(bm_name, 0)
            last_std = last['stddevs'].get(bm_name, 0)

            print(f"{bm_name}:")
            print(f"  First:       {first_val:>10.2f} ns ± {first_std:>8.2f} ns")
            print(f"  Latest:      {last_val:>10.2f} ns ± {last_std:>8.2f} ns")
            print(f"  Change:      {diff:>+10.2f} ns ({pct_change:+.2f}%) - {abs(pct_change):.2f}% {direction}")
            print()

    # Find best and worst commits for each benchmark
    print("=" * 120)
    print("BEST & WORST COMMITS BY BENCHMARK")
    print("=" * 120)
    print()

    for bm_name in ['BM_simple', 'BM_long_string', 'BM_many_numbers']:
        # Filter results that have this benchmark
        with_bm = [r for r in all_results if bm_name in r['benchmarks']]
        if not with_bm:
            continue

        # Find best (lowest time) and worst (highest time)
        best = min(with_bm, key=lambda x: x['benchmarks'][bm_name])
        worst = max(with_bm, key=lambda x: x['benchmarks'][bm_name])

        best_val = best['benchmarks'][bm_name]
        worst_val = worst['benchmarks'][bm_name]
        best_std = best['stddevs'].get(bm_name, 0)
        worst_std = worst['stddevs'].get(bm_name, 0)

        print(f"{bm_name}:")
        print(f"  Best:  {best_val:>10.2f} ns ± {best_std:>8.2f} ns - #{best['index']:02d} {best['short']}")
        print(f"  Worst: {worst_val:>10.2f} ns ± {worst_std:>8.2f} ns - #{worst['index']:02d} {worst['short']}")
        print()

    # Find commits with notable changes
    print("=" * 120)
    print("NOTABLE PERFORMANCE CHANGES (>5% improvement or regression)")
    print("=" * 120)
    print()

    for i in range(1, len(all_results)):
        prev = all_results[i-1]
        curr = all_results[i]

        changes = []
        for bm_name in ['BM_simple', 'BM_long_string', 'BM_many_numbers']:
            if bm_name in prev['benchmarks'] and bm_name in curr['benchmarks']:
                prev_val = prev['benchmarks'][bm_name]
                curr_val = curr['benchmarks'][bm_name]

                # Check if change is significant (beyond noise)
                prev_std = prev['stddevs'].get(bm_name, 0)
                curr_std = curr['stddevs'].get(bm_name, 0)

                pct_change = ((curr_val - prev_val) / prev_val) * 100

                if abs(pct_change) > 5:
                    direction = "regression" if pct_change > 0 else "improvement"
                    changes.append((bm_name, pct_change, direction, curr_val, curr_std))

        if changes:
            print(f"Commit #{curr['index']}: {curr['short']}")
            for bm_name, pct, direction, val, std in changes:
                print(f"  {bm_name}: {pct:+.2f}% ({direction}) - {val:.2f} ± {std:.2f} ns")
            print()

    # Calculate overall statistics
    print("=" * 120)
    print("STATISTICAL SUMMARY")
    print("=" * 120)
    print()

    for bm_name in ['BM_simple', 'BM_long_string', 'BM_many_numbers']:
        values = [r['benchmarks'][bm_name] for r in all_results if bm_name in r['benchmarks']]
        if values:
            import statistics
            mean = statistics.mean(values)
            median = statistics.median(values)
            stdev = statistics.stdev(values) if len(values) > 1 else 0
            min_val = min(values)
            max_val = max(values)

            print(f"{bm_name}:")
            print(f"  Mean:   {mean:>10.2f} ns")
            print(f"  Median: {median:>10.2f} ns")
            print(f"  StdDev: {stdev:>10.2f} ns ({(stdev/mean*100):.2f}% of mean)")
            print(f"  Range:  {min_val:>10.2f} - {max_val:>10.2f} ns (delta: {max_val-min_val:.2f} ns)")
            print()

    print("=" * 120)
    print(f"Total commits analyzed: {len(all_results)} out of 48")
    print("=" * 120)

if __name__ == '__main__':
    main()
