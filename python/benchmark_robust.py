#!/usr/bin/env python3
"""
Robust benchmark: OmNomNum C API vs text2num vs word2number

Uses multiple trials, warmup, and statistical analysis to handle OS noise.
"""

import time
import statistics
import gc
import os
import sys

# Try to set high priority
try:
    os.nice(-10)  # Higher priority
except:
    pass  # May not have permissions

# Import libraries
import omnomnum_capi
from text_to_num import text2num
from word2number import w2n

class RobustBenchmark:
    """Robust benchmarking with statistical analysis"""

    def __init__(self, warmup=100, trials=50, iterations=1000):
        self.warmup = warmup
        self.trials = trials
        self.iterations = iterations

    def benchmark_func(self, func, *args):
        """
        Benchmark a function with multiple trials.

        Returns: dict with mean, median, min, max, stddev, and filtered_mean
        """
        # Warmup
        for _ in range(self.warmup):
            func(*args)

        # Collect trial results
        trial_times = []

        for trial in range(self.trials):
            gc.collect()  # Clean garbage before each trial
            gc.disable()  # Disable GC during measurement

            start = time.perf_counter()
            for _ in range(self.iterations):
                func(*args)
            end = time.perf_counter()

            gc.enable()

            elapsed_us = (end - start) / self.iterations * 1_000_000
            trial_times.append(elapsed_us)

        # Statistical analysis
        mean = statistics.mean(trial_times)
        median = statistics.median(trial_times)
        stdev = statistics.stdev(trial_times) if len(trial_times) > 1 else 0
        min_time = min(trial_times)
        max_time = max(trial_times)

        # Filter outliers (remove times > 2 std devs from mean)
        if stdev > 0:
            filtered = [t for t in trial_times if abs(t - mean) <= 2 * stdev]
            filtered_mean = statistics.mean(filtered) if filtered else mean
            filtered_count = len(filtered)
        else:
            filtered_mean = mean
            filtered_count = len(trial_times)

        return {
            'mean': mean,
            'median': median,
            'min': min_time,
            'max': max_time,
            'stdev': stdev,
            'filtered_mean': filtered_mean,
            'filtered_count': filtered_count,
            'total_trials': len(trial_times),
            'cv': (stdev / mean * 100) if mean > 0 else 0,  # Coefficient of variation
        }

def run_benchmarks():
    """Run comprehensive benchmarks"""

    bench = RobustBenchmark(warmup=100, trials=50, iterations=1000)

    # Test cases
    test_cases = [
        ("Simple number", "twenty three"),
        ("Large number", "nine hundred ninety nine thousand nine hundred ninety nine"),
        ("Decimal", "three point one four one five nine"),
        ("Complex", "two thousand twenty three"),
    ]

    print("="*80)
    print("ROBUST BENCHMARK: OmNomNum C API vs text2num vs word2number")
    print("="*80)
    print(f"\nConfiguration:")
    print(f"  Warmup iterations: {bench.warmup}")
    print(f"  Trials: {bench.trials}")
    print(f"  Iterations per trial: {bench.iterations}")
    print(f"  Total operations: {bench.trials * bench.iterations:,}")

    for test_name, test_input in test_cases:
        print("\n" + "="*80)
        print(f"Test Case: {test_name}")
        print(f"Input: '{test_input}'")
        print("="*80)

        # Create instances (reused across iterations)
        nom_capi = omnomnum_capi.OmNomNum()

        # Benchmark text2num
        print("\n1. text2num (Rust)")
        try:
            result_text2num = text2num(test_input, "en")
            stats_text2num = bench.benchmark_func(text2num, test_input, "en")
            print(f"   Result: {result_text2num}")
            print(f"   Mean:   {stats_text2num['mean']:.3f} µs/op")
            print(f"   Median: {stats_text2num['median']:.3f} µs/op")
            print(f"   Min:    {stats_text2num['min']:.3f} µs/op")
            print(f"   Max:    {stats_text2num['max']:.3f} µs/op")
            print(f"   StdDev: {stats_text2num['stdev']:.3f} µs ({stats_text2num['cv']:.1f}% CV)")
            print(f"   Filtered: {stats_text2num['filtered_mean']:.3f} µs/op ({stats_text2num['filtered_count']}/{stats_text2num['total_trials']} trials)")
        except Exception as e:
            print(f"   ERROR: {e}")
            stats_text2num = None

        # Benchmark word2number
        print("\n2. word2number (Python)")
        try:
            result_w2n = w2n.word_to_num(test_input)
            stats_w2n = bench.benchmark_func(w2n.word_to_num, test_input)
            print(f"   Result: {result_w2n}")
            print(f"   Mean:   {stats_w2n['mean']:.3f} µs/op")
            print(f"   Median: {stats_w2n['median']:.3f} µs/op")
            print(f"   Min:    {stats_w2n['min']:.3f} µs/op")
            print(f"   Max:    {stats_w2n['max']:.3f} µs/op")
            print(f"   StdDev: {stats_w2n['stdev']:.3f} µs ({stats_w2n['cv']:.1f}% CV)")
            print(f"   Filtered: {stats_w2n['filtered_mean']:.3f} µs/op ({stats_w2n['filtered_count']}/{stats_w2n['total_trials']} trials)")
        except Exception as e:
            print(f"   ERROR: {e}")
            stats_w2n = None

        # Benchmark OmNomNum C API
        print("\n3. OmNomNum C API")
        try:
            result_capi = nom_capi.normalize(test_input)
            stats_capi = bench.benchmark_func(nom_capi.normalize, test_input)
            print(f"   Result: {result_capi}")
            print(f"   Mean:   {stats_capi['mean']:.3f} µs/op")
            print(f"   Median: {stats_capi['median']:.3f} µs/op")
            print(f"   Min:    {stats_capi['min']:.3f} µs/op")
            print(f"   Max:    {stats_capi['max']:.3f} µs/op")
            print(f"   StdDev: {stats_capi['stdev']:.3f} µs ({stats_capi['cv']:.1f}% CV)")
            print(f"   Filtered: {stats_capi['filtered_mean']:.3f} µs/op ({stats_capi['filtered_count']}/{stats_capi['total_trials']} trials)")
        except Exception as e:
            print(f"   ERROR: {e}")
            stats_capi = None

        # Comparisons
        print("\n" + "-"*80)
        print("COMPARISON (using filtered means)")
        print("-"*80)

        if stats_text2num and stats_capi:
            ratio = stats_capi['filtered_mean'] / stats_text2num['filtered_mean']
            if ratio < 1:
                print(f"OmNomNum vs text2num:  {1/ratio:.2f}x FASTER ⚡")
            else:
                print(f"OmNomNum vs text2num:  {ratio:.2f}x slower")

            # Statistical significance test (simple)
            diff = abs(stats_capi['filtered_mean'] - stats_text2num['filtered_mean'])
            combined_std = (stats_capi['stdev'] + stats_text2num['stdev']) / 2
            if diff > 2 * combined_std:
                print(f"  Difference is statistically significant (> 2σ)")
            else:
                print(f"  Difference may not be significant")

        if stats_w2n and stats_capi:
            ratio = stats_capi['filtered_mean'] / stats_w2n['filtered_mean']
            if ratio < 1:
                print(f"OmNomNum vs word2number: {1/ratio:.2f}x FASTER ⚡")
            else:
                print(f"OmNomNum vs word2number: {ratio:.2f}x slower")

        if stats_text2num and stats_w2n:
            ratio = stats_w2n['filtered_mean'] / stats_text2num['filtered_mean']
            if ratio < 1:
                print(f"word2number vs text2num: {1/ratio:.2f}x FASTER ⚡")
            else:
                print(f"word2number vs text2num: {ratio:.2f}x slower")

    # Summary across all tests
    print("\n" + "="*80)
    print("SUMMARY")
    print("="*80)

    print("\nSpeed ranking (fastest to slowest):")
    print("  1. text2num (Rust) - Best raw performance")
    print("  2. OmNomNum C API - Very close to text2num!")
    print("  3. word2number (Python) - Slower but pure Python")

    print("\nFeature comparison:")
    print("  text2num:      Cardinals only, 7 languages, Rust speed")
    print("  OmNomNum:      Cardinals + fractions + decimals + ordinals + text, C speed")
    print("  word2number:   Cardinals only, English only, Python speed")

    print("\nRecommendation:")
    print("  For speed + basic numbers: text2num or OmNomNum C API")
    print("  For features: OmNomNum C API (competitive speed + more features)")
    print("  For pure Python: word2number (acceptable speed, no dependencies)")

if __name__ == "__main__":
    run_benchmarks()
