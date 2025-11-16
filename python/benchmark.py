#!/usr/bin/env python3
"""
Benchmark Python wrapper performance.
"""

import time
import statistics
from typing import List, Callable
from omnomnum import normalize, find_numbers, OmNomNum


class BenchmarkResult:
    def __init__(self, name: str, times: List[float], iterations: int):
        self.name = name
        self.times = times
        self.iterations = iterations
        self.mean = statistics.mean(times)
        self.median = statistics.median(times)
        self.stdev = statistics.stdev(times) if len(times) > 1 else 0
        self.min = min(times)
        self.max = max(times)

    def ns_per_op(self) -> float:
        """Nanoseconds per operation."""
        return (self.mean / self.iterations) * 1_000_000_000

    def ops_per_sec(self) -> float:
        """Operations per second."""
        return self.iterations / self.mean


def benchmark(
    name: str,
    func: Callable,
    iterations: int = 10000,
    warmup: int = 1000,
    repetitions: int = 10,
) -> BenchmarkResult:
    """
    Benchmark a function.

    Args:
        name: Benchmark name
        func: Function to benchmark
        iterations: Number of iterations per run
        warmup: Number of warmup iterations
        repetitions: Number of times to repeat the benchmark
    """
    # Warmup
    for _ in range(warmup):
        func()

    # Run benchmark
    times = []
    for _ in range(repetitions):
        start = time.perf_counter()
        for _ in range(iterations):
            func()
        end = time.perf_counter()
        times.append(end - start)

    return BenchmarkResult(name, times, iterations)


def print_result(result: BenchmarkResult):
    """Print benchmark result."""
    print(f"\n{result.name}")
    print(f"  Iterations: {result.iterations:,}")
    print(f"  Mean:       {result.mean * 1000:.3f} ms")
    print(f"  Median:     {result.median * 1000:.3f} ms")
    print(f"  StdDev:     {result.stdev * 1000:.3f} ms")
    print(f"  Min:        {result.min * 1000:.3f} ms")
    print(f"  Max:        {result.max * 1000:.3f} ms")
    print(f"  Per Op:     {result.ns_per_op():.0f} ns")
    print(f"  Ops/sec:    {result.ops_per_sec():,.0f}")


def main():
    """Run all benchmarks."""
    print("=" * 70)
    print("OmNomNum Python Wrapper Benchmark")
    print("=" * 70)

    # Test data
    simple = "two hundred fifty"
    complex_text = "I have two hundred fifty dollars and three hundred twenty five cents"
    many_numbers = "one two three four five six seven eight nine ten"
    fractions = "one and a half cups of sugar and three quarters teaspoon of salt"
    long_text = "The first place winner received twenty three thousand dollars, " \
                "the second place received twelve thousand five hundred, and " \
                "third place received five thousand"

    # Create reusable instance
    nom = OmNomNum()

    # Benchmark 1: Simple normalization (function API)
    result = benchmark(
        "Simple normalization (function API)",
        lambda: normalize(simple),
        iterations=10000,
    )
    print_result(result)

    # Benchmark 2: Simple normalization (class API, reused instance)
    result = benchmark(
        "Simple normalization (class API, reused)",
        lambda: nom.normalize(simple),
        iterations=10000,
    )
    print_result(result)

    # Benchmark 3: Complex text
    result = benchmark(
        "Complex text with multiple numbers",
        lambda: normalize(complex_text),
        iterations=10000,
    )
    print_result(result)

    # Benchmark 4: Many numbers
    result = benchmark(
        "Many numbers",
        lambda: normalize(many_numbers),
        iterations=10000,
    )
    print_result(result)

    # Benchmark 5: Fractions
    result = benchmark(
        "Fractions",
        lambda: normalize(fractions),
        iterations=10000,
    )
    print_result(result)

    # Benchmark 6: Long text
    result = benchmark(
        "Long text",
        lambda: normalize(long_text),
        iterations=10000,
    )
    print_result(result)

    # Benchmark 7: find_numbers (function API)
    result = benchmark(
        "find_numbers (function API)",
        lambda: find_numbers(many_numbers),
        iterations=10000,
    )
    print_result(result)

    # Benchmark 8: find_numbers (class API)
    result = benchmark(
        "find_numbers (class API, reused)",
        lambda: nom.find_numbers(many_numbers),
        iterations=10000,
    )
    print_result(result)

    # Benchmark 9: Instance creation overhead
    result = benchmark(
        "Instance creation + normalize",
        lambda: OmNomNum().normalize(simple),
        iterations=1000,  # Lower iterations due to overhead
    )
    print_result(result)

    # Benchmark 10: No-op (text without numbers)
    no_numbers = "hello world this has no numbers"
    result = benchmark(
        "No-op (text without numbers)",
        lambda: normalize(no_numbers),
        iterations=10000,
    )
    print_result(result)

    print("\n" + "=" * 70)
    print("Benchmark Summary")
    print("=" * 70)
    print("\nRecommendations:")
    print("  - Use class API (OmNomNum()) for best performance")
    print("  - Reuse instances when processing multiple texts")
    print("  - Function API is convenient but creates new instance each time")
    print("=" * 70)


if __name__ == "__main__":
    main()
