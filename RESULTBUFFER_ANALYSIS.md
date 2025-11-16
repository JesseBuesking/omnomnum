# ResultBuffer Performance Analysis

## Summary
ResultBuffer implementation resulted in **11% average regression** instead of expected improvements.

## Root Cause: SDS Was Already Optimal

### What SDS Does Right
1. **Immediate return on sdsMakeRoomFor** (sds.c:206):
   ```c
   if (avail >= addlen) return s;  // No realloc needed!
   ```

2. **Buffer reuse in baseline** (omnomnum.c:446-452):
   ```c
   if (state->result) {
       sdsclear(state->result);  // Keeps allocation!
   } else {
       state->result = sdsempty();
   }
   state->result = sdsMakeRoomFor(state->result, (size_t)data_len + 32);
   ```

3. **Inline length checks**: sdslen/sdsavail are inline functions with minimal overhead

4. **Smart pre-allocation**: Doubles size or adds 1MB to reduce future reallocs

### Why ResultBuffer Failed

1. **Two branches per append**:
   ```c
   if (rb->overflow) { ... }           // Branch 1
   if (rb->len + dlen < RB_INLINE_SIZE) { ... }  // Branch 2
   ```
   - Branch mispredictions cost ~10-20 cycles each
   - SDS's fast path has no branches (line 206 returns immediately)

2. **Large stack footprint**: 512 bytes per ResultBuffer
   - Evicts hot data from L1 cache
   - Most strings are <100 bytes

3. **Cache pressure**: Stack buffer competes with actual working data

4. **Lost SDS optimizations**:
   - SDS header is cache-adjacent to data (better locality)
   - SDS uses optimized realloc paths
   - Our memcpy doesn't benefit from these

## Benchmark Results

| Metric | Value |
|--------|-------|
| Improvements | 4 benchmarks |
| Regressions | 33 benchmarks |
| Avg regression | 11.0% |
| Worst regression | BM_ordinals_large: 28.6% |

## Key Insights

1. **SDS buffer reuse already eliminates most malloc/free overhead**
2. **Pre-reservation (sdsMakeRoomFor) already optimizes growth**
3. **Inline functions beat our branching logic**
4. **Cache locality matters more than stack vs heap**

## Conclusion

The baseline code was already highly optimized through:
- Buffer reuse via sdsclear
- Pre-reservation via sdsMakeRoomFor
- SDS's inline fast paths

Adding an inline buffer introduced overhead (branches, cache pressure) without providing benefit, since SDS's existing allocation was already being reused.

## Recommendation

**Revert ResultBuffer changes**. The current SDS-based approach is optimal for our access patterns.

## Alternative Optimizations to Consider

1. **Profile-guided optimization**: Use actual profiling data to find real bottlenecks
2. **Reduce temporary allocations**: Focus on the temp SDS strings in normalize()
3. **Parser optimizations**: May have more impact than string buffer changes
4. **SIMD operations**: For number parsing/formatting
