#!/usr/bin/env python3
"""Performance benchmark: text2num vs OmNomNum"""

import time
from text_to_num import text2num
from omnomnum import normalize, OmNomNum

def benchmark(func, text, iterations=10000):
    """Benchmark a function"""
    start = time.perf_counter()
    for _ in range(iterations):
        func(text)
    end = time.perf_counter()

    elapsed_ms = (end - start) * 1000
    per_op_us = (end - start) / iterations * 1_000_000
    ops_per_sec = iterations / (end - start)

    return elapsed_ms, per_op_us, ops_per_sec

# Test string
test_text = "twenty three"

# Number of iterations
iterations = 10000

print("="*80)
print("Performance Benchmark: text2num vs OmNomNum")
print("="*80)
print(f"\nTest: Converting '{test_text}'")
print(f"Iterations: {iterations:,}")
print()

# Benchmark text2num
print("Testing text2num...")
elapsed_ms, per_op_us, ops_per_sec = benchmark(
    lambda t: text2num(t, "en"),
    test_text,
    iterations
)
text2num_time = per_op_us
print(f"text2num:           {elapsed_ms:.2f} ms ({per_op_us:.1f} µs/op) = {ops_per_sec:,.0f} ops/sec")

# Benchmark OmNomNum (class - reused instance)
print("\nTesting OmNomNum (class API - reused instance)...")
nom = OmNomNum()
elapsed_ms, per_op_us, ops_per_sec = benchmark(
    nom.normalize,
    test_text,
    iterations
)
omnom_class_time = per_op_us
print(f"OmNomNum (class):   {elapsed_ms:.2f} ms ({per_op_us:.1f} µs/op) = {ops_per_sec:,.0f} ops/sec")

# Benchmark OmNomNum (function - new instance each call)
print("\nTesting OmNomNum (function API - new instance each call)...")
elapsed_ms, per_op_us, ops_per_sec = benchmark(
    normalize,
    test_text,
    iterations
)
omnom_func_time = per_op_us
print(f"OmNomNum (function): {elapsed_ms:.2f} ms ({per_op_us:.1f} µs/op) = {ops_per_sec:,.0f} ops/sec")

# Comparison
print("\n" + "="*80)
print("COMPARISON")
print("="*80)

if omnom_class_time < text2num_time:
    speedup = text2num_time / omnom_class_time
    print(f"\nOmNomNum (class) is {speedup:.1f}x FASTER than text2num")
else:
    speedup = omnom_class_time / text2num_time
    print(f"\ntext2num is {speedup:.1f}x faster than OmNomNum (class)")

if omnom_func_time < text2num_time:
    speedup = text2num_time / omnom_func_time
    print(f"OmNomNum (function) is {speedup:.1f}x faster than text2num")
else:
    speedup = omnom_func_time / text2num_time
    print(f"text2num is {speedup:.1f}x faster than OmNomNum (function)")

print("\n" + "="*80)
print("RECOMMENDATION")
print("="*80)
print("\nFor best performance:")
print("  1. OmNomNum (class API) - Fastest when reusing instance")
print("  2. text2num - Good middle ground")
print("  3. OmNomNum (function API) - Convenient but slower due to instance creation")
print("\nUse class API for performance-critical applications:")
print("  nom = OmNomNum()")
print("  for text in texts:")
print("      result = nom.normalize(text)")
