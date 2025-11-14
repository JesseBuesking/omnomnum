#!/usr/bin/env python3
"""
Performance comparison: OmNomNum vs word2number
"""

import time

# Test OmNomNum
from omnomnum import normalize, OmNomNum

test_text = "twenty three"
iterations = 10000

print("Performance Comparison: OmNomNum vs word2number")
print("="*70)
print(f"Test: '{test_text}'")
print(f"Iterations: {iterations:,}\n")

# Benchmark OmNomNum - function API
start = time.perf_counter()
for _ in range(iterations):
    result = normalize(test_text)
end = time.perf_counter()
omnom_func_time = end - start

print(f"OmNomNum (function):  {omnom_func_time*1000:.2f} ms ({omnom_func_time/iterations*1e6:.1f} µs/op)")
print(f"  Result: '{result}'")

# Benchmark OmNomNum - class API
nom = OmNomNum()
start = time.perf_counter()
for _ in range(iterations):
    result = nom.normalize(test_text)
end = time.perf_counter()
omnom_class_time = end - start

print(f"OmNomNum (class):     {omnom_class_time*1000:.2f} ms ({omnom_class_time/iterations*1e6:.1f} µs/op)")
print(f"  Result: '{result}'")

# Try word2number if available
try:
    from word2number import w2n

    start = time.perf_counter()
    for _ in range(iterations):
        result = w2n.word_to_num(test_text)
    end = time.perf_counter()
    w2n_time = end - start

    print(f"word2number:          {w2n_time*1000:.2f} ms ({w2n_time/iterations*1e6:.1f} µs/op)")
    print(f"  Result: '{result}'")

    print(f"\nSpeed Comparison:")
    print(f"  OmNomNum (class) is {w2n_time/omnom_class_time:.1f}x FASTER than word2number")
    print(f"  OmNomNum (func)  is {w2n_time/omnom_func_time:.1f}x FASTER than word2number")

except ImportError:
    print(f"word2number:          NOT INSTALLED")
    print(f"\nInstall with: pip install word2number")
except Exception as e:
    print(f"word2number:          ERROR - {e}")
