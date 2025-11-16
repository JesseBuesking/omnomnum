#!/usr/bin/env python3
"""Profile individual C functions to see where time is spent"""

import time
from omnomnum._omnomnum import ffi, lib

test_text = "twenty three"
test_bytes = test_text.encode("utf-8")
iterations = 10000

# Initialize state once
lib.initOmNomNum()
state = ffi.new("ParserState *")
lib.initParserState(state)

print("="*80)
print("Profiling Individual C Functions")
print("="*80)

# Test 1: Just find_numbers (scanner + parser)
start = time.perf_counter()
for _ in range(iterations):
    lib.resetParserState(state)
    l = lib.find_numbers(test_bytes, len(test_bytes), state)
end = time.perf_counter()
find_numbers_us = (end - start) / iterations * 1_000_000
print(f"\nfind_numbers():           {find_numbers_us:.3f} µs/op")

# Test 2: Full normalize (find_numbers + rendering)
start = time.perf_counter()
for _ in range(iterations):
    lib.resetParserState(state)
    lib.normalize(test_bytes, len(test_bytes), state)
end = time.perf_counter()
normalize_us = (end - start) / iterations * 1_000_000
print(f"normalize():              {normalize_us:.3f} µs/op")

# Test 3: Just resetParserState
start = time.perf_counter()
for _ in range(iterations):
    lib.resetParserState(state)
end = time.perf_counter()
reset_us = (end - start) / iterations * 1_000_000
print(f"resetParserState():       {reset_us:.3f} µs/op")

# Test 4: Overhead of getting result string back
start = time.perf_counter()
for _ in range(iterations):
    lib.resetParserState(state)
    lib.normalize(test_bytes, len(test_bytes), state)
    result_sds = state.result
    if result_sds != ffi.NULL:
        result = ffi.string(result_sds).decode("utf-8")
end = time.perf_counter()
full_us = (end - start) / iterations * 1_000_000
print(f"normalize() + get result: {full_us:.3f} µs/op")

# Analysis
print("\n" + "="*80)
print("BREAKDOWN")
print("="*80)

rendering_us = normalize_us - find_numbers_us
result_extract_us = full_us - normalize_us

print(f"\nTime spent in:")
print(f"  find_numbers (scanner+parser): {find_numbers_us:.3f} µs ({find_numbers_us/normalize_us*100:.1f}%)")
print(f"  Rendering/concatenation:       {rendering_us:.3f} µs ({rendering_us/normalize_us*100:.1f}%)")
print(f"  Result extraction (ffi):       {result_extract_us:.3f} µs ({result_extract_us/full_us*100:.1f}%)")
print(f"  resetParserState:              {reset_us:.3f} µs ({reset_us/full_us*100:.1f}%)")

print(f"\nTotal C time: {normalize_us:.3f} µs")
print(f"Total with CFFI: {full_us:.3f} µs")

# Cleanup
lib.freeParserState(state)
