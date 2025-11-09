# Remaining Memory Allocation Opportunities

## Current State (After OPT1+OPT2)

### Optimizations Already Applied
1. ✅ **OPT1**: Buffer reuse (state->result, state->numberHolder)
2. ✅ **OPT2**: YYSTYPE capacity 128 (eliminates reallocations)
3. ✅ **Parser pooling**: state->pParser cached and reused (already in codebase)
4. ❌ **OPT3**: Stack buffers - rejected (20% regression on BM_long_string)

### Profiling Baseline

From original profiling (before optimizations):
- **Memory allocations**: 35%+ of CPU time
- **Top hotspots**: malloc (5%), realloc (4.38%), free (3.27%)

After OPT1+OPT2:
- Reduced realloc by 46.8%
- Eliminated most YYSTYPE reallocations
- Removed result buffer churn

## Remaining Allocation Hotspots

### 1. Sub-ParserState Allocations in Fallback Path

**Location**: omnomnum.c lines 493-514, 517-534

```c
// Fallback: split on separators and normalize each token independently
while (pos < data_len) {
    ...
    ParserState sub;            // Stack allocation (OK)
    initParserState(&sub);      // Allocates YYSTYPE, parser, buffers
    YYSTYPEList sl = find_numbers(data + tok_start, tok_len, &sub);
    if (sl.used > 0) {
        sds tmp = sdsempty();   // Heap allocation!
        yystypeToStringWithReduction(&tmp, sl.values[0], sub.precision, sub.reduce_fractions);
        state->result = sdscatsds(state->result, tmp);
        sdsfree(tmp);           // Free immediately
    }
    freeParserState(&sub);      // Frees YYSTYPE, parser, buffers
}
```

**Current overhead per iteration:**
- initParserState(): Allocates YYSTYPE array (128 elements), parser, numberHolder
- sdsempty() + sdsfree(): Temporary string for each token
- freeParserState(): Frees everything

**Frequency**: Only when `l.used == 0` (fallback path, uncommon for typical input)

**Optimization potential**: 
- Pool a sub-ParserState in main ParserState
- Reuse temp buffer instead of sdsempty()/sdsfree()

**Expected impact**: ~2-3% (only helps fallback path, which is rare)

### 2. Temporary SDS Allocations in yystypeToString

**Location**: omnomnum.c lines 507, 527

```c
sds tmp = sdsempty();
yystypeToStringWithReduction(&tmp, sl.values[0], sub.precision, sub.reduce_fractions);
state->result = sdscatsds(state->result, tmp);
sdsfree(tmp);
```

**Why not optimized**: Can't reuse state->result because we're appending to it.

**Optimization potential**: Add a temp_buffer to ParserState for these cases.

**Expected impact**: ~1-2% (only in fallback path)

### 3. Use Custom Allocator (jemalloc/tcmalloc)

**Current**: System malloc (likely ptmalloc2 on Linux)

**Alternative**: Link with jemalloc or tcmalloc

**Benefits:**
- Thread-local caches reduce lock contention
- Better size-class segregation
- Reduced fragmentation
- Often 5-15% faster for allocation-heavy workloads

**How to test:**
```bash
# Install jemalloc
apt-get install libjemalloc-dev

# Link with it
make LDFLAGS="-ljemalloc"

# Or use LD_PRELOAD
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libjemalloc.so.2 ./test/test_benchmark
```

**Expected impact**: 3-10% improvement with zero code changes!

**Trade-offs:**
- External dependency
- Slightly larger binary size
- Different memory usage patterns

### 4. Profile-Guided Optimization (PGO)

**Approach**: Use compiler's PGO to optimize hot paths

```bash
# Step 1: Build with instrumentation
make CFLAGS="-fprofile-generate"
./test/test_benchmark  # Generate profile data

# Step 2: Build with profile optimization
make clean
make CFLAGS="-fprofile-use"
```

**Benefits:**
- Optimizes branch prediction
- Better inlining decisions
- Improved code layout (hot code together)

**Expected impact**: 5-15% improvement

### 5. Eliminate process_percent Overhead

**Current**: process_percent() is called unconditionally at end of normalize()

**Location**: omnomnum.c line 622
```c
process_percent(&state->result, state);
```

**Optimization**: Fast-path check before calling

```c
// Only call if flags are set
if (state->normalize_percent_symbol || state->percent_as_decimal) {
    process_percent(&state->result, state);
}
```

**Why this helps**: Avoids function call overhead when percent processing disabled.

**Expected impact**: 1-2% (small, but zero-cost abstraction principle)

## Recommended Next Steps

### High Impact, Low Effort
1. **Test with jemalloc** (5-10% potential, zero code changes)
2. **Profile-Guided Optimization** (5-15% potential, build system change only)

### Medium Impact, Medium Effort  
3. **Add fast-path check before process_percent()** (1-2%, one-line change)
4. **Pool sub-ParserState for fallback path** (2-3%, moderate complexity)

### Low Priority
5. **Optimize fallback path temp allocations** (1-2%, rare path)

## Analysis: Diminishing Returns

With OPT1+OPT2 applied, we've already:
- Eliminated 46.8% of realloc overhead
- Removed result buffer churn
- Pre-allocated YYSTYPE capacity

**Remaining allocations are mostly unavoidable:**
- Parser data structures (already pooled)
- String content (can't eliminate)
- Fallback path (rare, not worth optimizing heavily)

**Best bang for buck**: External optimizations (jemalloc, PGO) rather than more code changes.

## Recommendation

**Try jemalloc first** - it's the easiest win with potentially the highest impact.

```bash
# Test it right now with LD_PRELOAD
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libjemalloc.so.2 ./test/test_benchmark
```

If that shows improvement, document it as a recommended deployment option.
