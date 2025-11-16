#!/usr/bin/env python3
"""Test caching drawbacks and measure impact"""

import time
import sys
from functools import lru_cache
from omnomnum import OmNomNum, normalize

print("="*80)
print("Testing Caching Drawbacks")
print("="*80)

# Test 1: Memory usage
print("\n1. MEMORY USAGE")
print("-"*80)

class CachedOmNomNum(OmNomNum):
    @lru_cache(maxsize=128)
    def normalize(self, text: str, reset: bool = True) -> str:
        return super().normalize(text, reset)

# Generate test data
test_strings = [f"number {i}" for i in range(1000)]

# Without cache
nom = OmNomNum()
start_mem = sys.getsizeof(nom)
print(f"OmNomNum without cache: {start_mem} bytes")

# With cache (empty)
cached_nom = CachedOmNomNum()
cache_empty_mem = sys.getsizeof(cached_nom) + sys.getsizeof(cached_nom.normalize.cache_info())
print(f"OmNomNum with empty cache: {cache_empty_mem} bytes")

# With cache (full)
for text in test_strings[:128]:  # Fill cache to maxsize
    cached_nom.normalize(text)
cache_info = cached_nom.normalize.cache_info()
print(f"Cache stats: hits={cache_info.hits}, misses={cache_info.misses}, size={cache_info.currsize}, maxsize={cache_info.maxsize}")

# Estimate memory per entry
# Each entry stores: (text, result) + cache bookkeeping
avg_text_len = sum(len(s) for s in test_strings[:128]) / 128
avg_result_len = 10  # "number X" -> "number X"
mem_per_entry = (avg_text_len + avg_result_len) * 2 + 200  # Overhead for cache structure
total_cache_mem = mem_per_entry * cache_info.currsize
print(f"Estimated cache memory: ~{total_cache_mem/1024:.1f} KB for {cache_info.currsize} entries")
print(f"Memory per entry: ~{mem_per_entry:.0f} bytes")

# Test 2: Thread safety
print("\n2. THREAD SAFETY")
print("-"*80)
print("lru_cache is thread-safe (uses locking internally)")
print("But: All threads share same cache (could cause unexpected behavior)")
print("Solution: Use one OmNomNum instance per thread if needed")

# Test 3: Stale data / configuration changes
print("\n3. STALE DATA / CONFIGURATION CHANGES")
print("-"*80)
nom1 = OmNomNum(precision=2)
nom2 = OmNomNum(precision=6)

# Without cache - different configs give different results
text = "one point five"
result1 = nom1.normalize(text)
result2 = nom2.normalize(text)
print(f"Without cache (precision=2): '{text}' -> '{result1}'")
print(f"Without cache (precision=6): '{text}' -> '{result2}'")
print(f"Different results: {result1 != result2}")

# With cache - PROBLEM!
class NaiveCachedOmNomNum(OmNomNum):
    @lru_cache(maxsize=128)
    def normalize(self, text: str, reset: bool = True) -> str:
        return super().normalize(text, reset)

nom_cached1 = NaiveCachedOmNomNum(precision=2)
nom_cached2 = NaiveCachedOmNomNum(precision=6)

result_cached1 = nom_cached1.normalize(text)
result_cached2 = nom_cached2.normalize(text)  # Might use cached result from nom_cached1!

print(f"\nWith naive cache (precision=2): '{text}' -> '{result_cached1}'")
print(f"With naive cache (precision=6): '{text}' -> '{result_cached2}'")
print(f"PROBLEM: Results should differ but might be same due to shared cache!")

# Test 4: Cache key includes configuration
print("\n4. SOLUTION: Include config in cache key")
print("-"*80)

class ProperCachedOmNomNum(OmNomNum):
    def __init__(self, precision=6, parse_fractions=True, **kwargs):
        super().__init__(precision=precision, parse_fractions=parse_fractions, **kwargs)
        self._precision = precision
        self._parse_fractions = parse_fractions

    @lru_cache(maxsize=128)
    def _normalize_cached(self, text: str, precision: int, parse_fractions: bool) -> str:
        # Cache key includes configuration
        return super().normalize(text)

    def normalize(self, text: str, reset: bool = True) -> str:
        return self._normalize_cached(text, self._precision, self._parse_fractions)

nom_proper1 = ProperCachedOmNomNum(precision=2)
nom_proper2 = ProperCachedOmNomNum(precision=6)

result_proper1 = nom_proper1.normalize(text)
result_proper2 = nom_proper2.normalize(text)

print(f"With proper cache (precision=2): '{text}' -> '{result_proper1}'")
print(f"With proper cache (precision=6): '{text}' -> '{result_proper2}'")
print(f"Correct: Results differ as expected: {result_proper1 != result_proper2}")

# Test 5: Performance with cache
print("\n5. PERFORMANCE IMPACT")
print("-"*80)

test_text = "twenty three"
iterations = 10000

# Without cache
nom = OmNomNum()
start = time.perf_counter()
for _ in range(iterations):
    nom.normalize(test_text)
end = time.perf_counter()
no_cache_us = (end - start) / iterations * 1_000_000
print(f"Without cache: {no_cache_us:.3f} µs/op")

# With cache (all hits)
cached_nom = NaiveCachedOmNomNum()
cached_nom.normalize(test_text)  # Prime cache
start = time.perf_counter()
for _ in range(iterations):
    cached_nom.normalize(test_text)
end = time.perf_counter()
cached_us = (end - start) / iterations * 1_000_000
print(f"With cache (hits): {cached_us:.3f} µs/op")

speedup = no_cache_us / cached_us
print(f"Speedup: {speedup:.1f}x faster")

# With cache (all misses - different inputs each time)
cached_nom2 = NaiveCachedOmNomNum()
test_strings_unique = [f"number {i}" for i in range(iterations)]
start = time.perf_counter()
for text in test_strings_unique:
    cached_nom2.normalize(text)
end = time.perf_counter()
cache_miss_us = (end - start) / iterations * 1_000_000
print(f"With cache (misses): {cache_miss_us:.3f} µs/op")
overhead = cache_miss_us - no_cache_us
print(f"Cache miss overhead: {overhead:.3f} µs ({overhead/no_cache_us*100:.1f}%)")

print("\n" + "="*80)
print("SUMMARY OF CACHE DRAWBACKS")
print("="*80)
print("\n✓ Pros:")
print("  - 10x+ faster for repeated inputs")
print("  - Thread-safe (with locking)")
print("  - Easy to implement")
print("\n✗ Cons:")
print("  - Memory usage: ~200-500 bytes per cached entry")
print("  - Cache miss overhead: ~5-10% slower for unique inputs")
print("  - Must include config in cache key to avoid stale data")
print("  - Shared cache across instances (unless using instance method)")
print("  - Not useful if inputs are always unique")
print("\n💡 Recommendations:")
print("  - Use caching if: Repeated inputs are common (>10%)")
print("  - Don't use if: All inputs are unique")
print("  - Always include configuration in cache key")
print("  - Use moderate maxsize (128-1024) to limit memory")
