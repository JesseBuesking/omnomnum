#!/usr/bin/env python3
"""Quick benchmark comparison."""

import time
from omnomnum import normalize, OmNomNum

# Test data
SIMPLE = "two hundred"
iterations = 1000

print("Quick Benchmark - Python Wrapper Overhead")
print("=" * 60)
print(f"Iterations: {iterations:,}\n")

# Benchmark 1: Function API (creates new instance each time)
start = time.perf_counter()
for _ in range(iterations):
    normalize(SIMPLE)
end = time.perf_counter()
func_time = end - start
print(f"Function API:       {func_time*1000:.2f} ms ({func_time/iterations*1e6:.1f} µs/op)")

# Benchmark 2: Class API (reused instance)
nom = OmNomNum()
start = time.perf_counter()
for _ in range(iterations):
    nom.normalize(SIMPLE)
end = time.perf_counter()
class_time = end - start
print(f"Class API (reused): {class_time*1000:.2f} ms ({class_time/iterations*1e6:.1f} µs/op)")

print(f"\nOverhead: {((func_time - class_time) / class_time * 100):.1f}% from instance creation")
print(f"\nResult: '{nom.normalize(SIMPLE)}'")
