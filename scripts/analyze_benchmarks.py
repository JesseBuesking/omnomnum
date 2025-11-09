#!/usr/bin/env python3
"""
Parse and analyze benchmark results from all commits.
"""

import os
import re
from pathlib import Path
from collections import defaultdict

def parse_benchmark_file(filepath):
    """Parse a single benchmark result file."""
    with open(filepath, 'r') as f:
        content = f.read()

    # Check if build failed
    if 'BUILD FAILED' in content:
        return None

    data = {}

    # Extract metadata
    commit_match = re.search(r'Commit: (.+)', content)
    short_match = re.search(r'Short: (.+)', content)
    index_match = re.search(r'Index: (.+)', content)
    message_match = re.search(r'Message: (.+)', content)
    date_match = re.search(r'Date: (.+)', content)

    if commit_match:
        data['commit'] = commit_match.group(1)
    if short_match:
        data['short'] = short_match.group(1)
    if index_match:
        data['index'] = int(index_match.group(1))
    if message_match:
        data['message'] = message_match.group(1)
    if date_match:
        data['date'] = date_match.group(1)

    # Extract benchmark results (us/iter)
    benchmarks = {}
    for line in content.split('\n'):
        bm_match = re.search(r'(BM_\w+): .+ \(([0-9.]+) us/iter\)', line)
        if bm_match:
            benchmarks[bm_match.group(1)] = float(bm_match.group(2))

    data['benchmarks'] = benchmarks
    return data

def main():
    results_dir = Path('/home/user/omnomnum/benchmark_results')

    # Parse all result files
    all_results = []
    for filepath in sorted(results_dir.glob('*.txt')):
        result = parse_benchmark_file(filepath)
        if result and result.get('benchmarks'):
            all_results.append(result)

    if not all_results:
        print("No benchmark results found!")
        return

    # Sort by index
    all_results.sort(key=lambda x: x['index'])

    print("=" * 100)
    print("PERFORMANCE ANALYSIS ACROSS ALL COMMITS")
    print("=" * 100)
    print()

    # Get first and last commit for comparison
    first = all_results[0]
    last = all_results[-1]

    print(f"First successful benchmark:")
    print(f"  Commit #{first['index']}: {first['short']}")
    print(f"  Message: {first['message']}")
    print(f"  Date: {first['date']}")
    print()

    print(f"Latest benchmark:")
    print(f"  Commit #{last['index']}: {last['short']}")
    print(f"  Message: {last['message']}")
    print(f"  Date: {last['date']}")
    print()

    # Print detailed results table
    print("=" * 100)
    print("DETAILED BENCHMARK RESULTS (us/iter)")
    print("=" * 100)
    print()
    print(f"{'#':<4} {'Commit':<10} {'BM_simple':<12} {'BM_long_string':<16} {'BM_many_numbers':<16} {'Message':<40}")
    print("-" * 100)

    for result in all_results:
        bm = result['benchmarks']
        simple = bm.get('BM_simple', 0)
        long_str = bm.get('BM_long_string', 0)
        many = bm.get('BM_many_numbers', 0)
        msg = result['message'][:38] + '..' if len(result['message']) > 40 else result['message']

        print(f"{result['index']:<4} {result['short']:<10} {simple:>10.3f}   {long_str:>13.3f}   {many:>13.3f}   {msg:<40}")

    print()
    print("=" * 100)
    print("PERFORMANCE COMPARISON: First vs Latest")
    print("=" * 100)
    print()

    for bm_name in ['BM_simple', 'BM_long_string', 'BM_many_numbers']:
        first_val = first['benchmarks'].get(bm_name, 0)
        last_val = last['benchmarks'].get(bm_name, 0)

        if first_val > 0 and last_val > 0:
            diff = last_val - first_val
            pct_change = (diff / first_val) * 100
            direction = "slower" if diff > 0 else "faster"

            print(f"{bm_name}:")
            print(f"  First:  {first_val:.3f} us/iter")
            print(f"  Latest: {last_val:.3f} us/iter")
            print(f"  Change: {diff:+.3f} us/iter ({pct_change:+.2f}%) - {abs(pct_change):.2f}% {direction}")
            print()

    # Find best and worst commits for each benchmark
    print("=" * 100)
    print("BEST & WORST COMMITS BY BENCHMARK")
    print("=" * 100)
    print()

    for bm_name in ['BM_simple', 'BM_long_string', 'BM_many_numbers']:
        # Filter results that have this benchmark
        with_bm = [r for r in all_results if bm_name in r['benchmarks']]
        if not with_bm:
            continue

        # Find best (lowest time) and worst (highest time)
        best = min(with_bm, key=lambda x: x['benchmarks'][bm_name])
        worst = max(with_bm, key=lambda x: x['benchmarks'][bm_name])

        print(f"{bm_name}:")
        print(f"  Best:  {best['benchmarks'][bm_name]:.3f} us/iter - #{best['index']:02d} {best['short']} - {best['message']}")
        print(f"  Worst: {worst['benchmarks'][bm_name]:.3f} us/iter - #{worst['index']:02d} {worst['short']} - {worst['message']}")
        print()

    # Find commits with notable changes
    print("=" * 100)
    print("NOTABLE PERFORMANCE CHANGES (>5% improvement or regression)")
    print("=" * 100)
    print()

    for i in range(1, len(all_results)):
        prev = all_results[i-1]
        curr = all_results[i]

        changes = []
        for bm_name in ['BM_simple', 'BM_long_string', 'BM_many_numbers']:
            if bm_name in prev['benchmarks'] and bm_name in curr['benchmarks']:
                prev_val = prev['benchmarks'][bm_name]
                curr_val = curr['benchmarks'][bm_name]
                pct_change = ((curr_val - prev_val) / prev_val) * 100

                if abs(pct_change) > 5:
                    direction = "regression" if pct_change > 0 else "improvement"
                    changes.append((bm_name, pct_change, direction))

        if changes:
            print(f"Commit #{curr['index']}: {curr['short']}")
            print(f"  Message: {curr['message']}")
            for bm_name, pct, direction in changes:
                print(f"    {bm_name}: {pct:+.2f}% ({direction})")
            print()

    print("=" * 100)
    print(f"Total commits analyzed: {len(all_results)} out of 47")
    print("=" * 100)

if __name__ == '__main__':
    main()
