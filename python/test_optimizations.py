#!/usr/bin/env python3.10
"""Quick test of C API optimizations"""

import time
import omnomnum_capi

nom = omnomnum_capi.OmNomNum()

# Benchmark: Normal case
iterations = 5000
start = time.perf_counter()
for _ in range(iterations):
    nom.normalize('twenty three')
end = time.perf_counter()
per_op_us = (end - start) / iterations * 1_000_000
print(f'Normal case: {per_op_us:.3f} µs/op')

print('✅ Test complete!')
