#!/usr/bin/env python3
"""
Benchmark: Python C API vs CFFI

Compares the performance of the native C API wrapper vs CFFI wrapper.
"""

import time

# Import both versions
from omnomnum import OmNomNum as OmNomNum_CFFI, normalize as normalize_cffi
import omnomnum_capi
OmNomNum_CAPI = omnomnum_capi.OmNomNum
normalize_capi = omnomnum_capi.normalize

# Import text2num for comparison
from text_to_num import text2num

def benchmark(func, *args, iterations=10000):
    """Benchmark a function"""
    start = time.perf_counter()
    for _ in range(iterations):
        func(*args)
    end = time.perf_counter()

    elapsed_ms = (end - start) * 1000
    per_op_us = (end - start) / iterations * 1_000_000
    ops_per_sec = iterations / (end - start)

    return elapsed_ms, per_op_us, ops_per_sec

test_text = "twenty three"
iterations = 10000

print("="*80)
print("Performance Benchmark: C API vs CFFI")
print("="*80)
print(f"\nTest: Converting '{test_text}'")
print(f"Iterations: {iterations:,}\n")

# Benchmark text2num (baseline)
print("1. text2num (Rust baseline)")
elapsed_ms, per_op_us, ops_per_sec = benchmark(text2num, test_text, "en", iterations=iterations)
text2num_time = per_op_us
print(f"   Time: {elapsed_ms:.2f} ms ({per_op_us:.3f} µs/op) = {ops_per_sec:,.0f} ops/sec")

# Benchmark CFFI (class API)
print("\n2. CFFI (class API - reused instance)")
nom_cffi = OmNomNum_CFFI()
elapsed_ms, per_op_us, ops_per_sec = benchmark(nom_cffi.normalize, test_text, iterations=iterations)
cffi_class_time = per_op_us
print(f"   Time: {elapsed_ms:.2f} ms ({per_op_us:.3f} µs/op) = {ops_per_sec:,.0f} ops/sec")

# Benchmark CFFI (function API)
print("\n3. CFFI (function API - new instance each call)")
elapsed_ms, per_op_us, ops_per_sec = benchmark(normalize_cffi, test_text, iterations=iterations)
cffi_func_time = per_op_us
print(f"   Time: {elapsed_ms:.2f} ms ({per_op_us:.3f} µs/op) = {ops_per_sec:,.0f} ops/sec")

# Benchmark C API (class)
print("\n4. C API (class API - reused instance)")
nom_capi = OmNomNum_CAPI()
elapsed_ms, per_op_us, ops_per_sec = benchmark(nom_capi.normalize, test_text, iterations=iterations)
capi_class_time = per_op_us
print(f"   Time: {elapsed_ms:.2f} ms ({per_op_us:.3f} µs/op) = {ops_per_sec:,.0f} ops/sec")

# Benchmark C API (function)
print("\n5. C API (function API - new instance each call)")
elapsed_ms, per_op_us, ops_per_sec = benchmark(normalize_capi, test_text, iterations=iterations)
capi_func_time = per_op_us
print(f"   Time: {elapsed_ms:.2f} ms ({per_op_us:.3f} µs/op) = {ops_per_sec:,.0f} ops/sec")

# Analysis
print("\n" + "="*80)
print("COMPARISON")
print("="*80)

print("\nvs text2num (Rust baseline):")
print(f"  CFFI class:   {cffi_class_time/text2num_time:.2f}x slower")
print(f"  CFFI function: {cffi_func_time/text2num_time:.2f}x slower")
print(f"  C API class:   {capi_class_time/text2num_time:.2f}x slower")
print(f"  C API function: {capi_func_time/text2num_time:.2f}x slower")

print("\nC API vs CFFI improvement:")
cffi_improvement_class = cffi_class_time / capi_class_time
cffi_improvement_func = cffi_func_time / capi_func_time
print(f"  Class API:    {cffi_improvement_class:.2f}x faster")
print(f"  Function API: {cffi_improvement_func:.2f}x faster")

print("\nOverhead reduction:")
cffi_overhead = cffi_class_time - text2num_time
capi_overhead = capi_class_time - text2num_time
overhead_reduction = cffi_overhead - capi_overhead
print(f"  CFFI overhead:     {cffi_overhead:.3f} µs")
print(f"  C API overhead:    {capi_overhead:.3f} µs")
print(f"  Improvement:       {overhead_reduction:.3f} µs ({overhead_reduction/cffi_overhead*100:.1f}% reduction)")

print("\n" + "="*80)
print("BREAKDOWN")
print("="*80)

# Estimate breakdown
print("\nEstimated components (class API):")
print(f"  C code (normalize): ~0.367 µs (same for both)")
print(f"  CFFI overhead:      ~{cffi_class_time - 0.367:.3f} µs")
print(f"  C API overhead:     ~{capi_class_time - 0.367:.3f} µs")
print(f"  Overhead savings:   ~{(cffi_class_time - 0.367) - (capi_class_time - 0.367):.3f} µs")

print("\n" + "="*80)
print("RECOMMENDATION")
print("="*80)

if cffi_improvement_class >= 1.3:
    print(f"\n✅ C API is {cffi_improvement_class:.1f}x FASTER - Worth switching!")
    print("   Estimated effort: Already done (this implementation)")
    print(f"   Performance gain: {overhead_reduction:.3f} µs per call")
elif cffi_improvement_class >= 1.1:
    print(f"\n⚠️  C API is {cffi_improvement_class:.1f}x faster - Marginal improvement")
    print("   May not be worth the maintenance cost")
else:
    print(f"\n❌ C API is only {cffi_improvement_class:.2f}x faster - Not worth it")
    print("   Overhead is dominated by C code, not bindings")

print("\nNext steps for more speed:")
print("  1. Fast path for simple numbers (2-3x gain)")
print("  2. Caching (10x gain for repeated inputs)")
print("  3. Combined: Up to 10-20x faster than current")
