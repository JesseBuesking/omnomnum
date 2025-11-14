#!/usr/bin/env python3
"""Profile where OmNomNum spends time"""

import time
from omnomnum._omnomnum import ffi, lib
from omnomnum import OmNomNum, normalize
from text_to_num import text2num

test_text = "twenty three"
iterations = 10000

# 1. Pure Python operations
print("="*80)
print("Profiling Overhead Sources")
print("="*80)

# Test 1: Just Python string encode/decode
start = time.perf_counter()
for _ in range(iterations):
    text_bytes = test_text.encode("utf-8")
    text_back = text_bytes.decode("utf-8")
end = time.perf_counter()
encode_decode_us = (end - start) / iterations * 1_000_000
print(f"\nPython encode/decode:     {encode_decode_us:.3f} µs/op")

# Test 2: CFFI overhead - create and use ffi.new
start = time.perf_counter()
for _ in range(iterations):
    state = ffi.new("ParserState *")
end = time.perf_counter()
cffi_new_us = (end - start) / iterations * 1_000_000
print(f"CFFI ffi.new():           {cffi_new_us:.3f} µs/op")

# Test 3: Full OmNomNum instance creation
start = time.perf_counter()
for _ in range(iterations):
    nom = OmNomNum()
end = time.perf_counter()
instance_create_us = (end - start) / iterations * 1_000_000
print(f"OmNomNum() creation:      {instance_create_us:.3f} µs/op")

# Test 4: Reused instance (class API)
nom = OmNomNum()
start = time.perf_counter()
for _ in range(iterations):
    result = nom.normalize(test_text)
end = time.perf_counter()
class_api_us = (end - start) / iterations * 1_000_000
print(f"OmNomNum.normalize():     {class_api_us:.3f} µs/op")

# Test 5: Function API (new instance each time)
start = time.perf_counter()
for _ in range(iterations):
    result = normalize(test_text)
end = time.perf_counter()
func_api_us = (end - start) / iterations * 1_000_000
print(f"normalize() function:     {func_api_us:.3f} µs/op")

# Test 6: text2num for comparison
start = time.perf_counter()
for _ in range(iterations):
    result = text2num(test_text, "en")
end = time.perf_counter()
text2num_us = (end - start) / iterations * 1_000_000
print(f"text2num():               {text2num_us:.3f} µs/op")

# Analysis
print("\n" + "="*80)
print("OVERHEAD ANALYSIS")
print("="*80)

print(f"\nOmNomNum class API breakdown:")
print(f"  Encode/decode overhead:     {encode_decode_us:.3f} µs ({encode_decode_us/class_api_us*100:.1f}%)")
print(f"  Core processing:            {class_api_us - encode_decode_us:.3f} µs ({(class_api_us - encode_decode_us)/class_api_us*100:.1f}%)")
print(f"  Total:                      {class_api_us:.3f} µs")

print(f"\nOmNomNum function API breakdown:")
print(f"  Instance creation:          {instance_create_us:.3f} µs ({instance_create_us/func_api_us*100:.1f}%)")
print(f"  Core processing:            {class_api_us:.3f} µs ({class_api_us/func_api_us*100:.1f}%)")
print(f"  Total:                      {func_api_us:.3f} µs")

print(f"\ntext2num is {class_api_us/text2num_us:.1f}x faster than OmNomNum (class API)")
print(f"text2num is {func_api_us/text2num_us:.1f}x faster than OmNomNum (function API)")

print(f"\nWhere OmNomNum could improve:")
overhead = class_api_us - text2num_us
print(f"  Current overhead vs text2num: {overhead:.3f} µs")
print(f"  If we eliminated encode/decode: {overhead - encode_decode_us:.3f} µs remaining")
print(f"  This suggests the C code itself is slower, not just Python overhead")
