# Performance Optimization Plan

## Profiling Analysis Summary

**Regression**: BM_many_numbers shows 21.73% slowdown (74346 ns → 90503 ns)

### Callgrind CPU Profiling Results

Total instructions executed: 1,547,626,041

**Top Hotspots:**
1. **Memory Allocations (35%+)**
   - `_int_malloc`: 7.15%
   - `_int_free`: 7.52%
   - `malloc`: 5.00%
   - `realloc`: 4.38%
   - `_int_realloc`: 3.82%
   - `free`: 3.27%
   - **Total**: 35%+ of all CPU time

2. **Parser Operations (23%)**
   - `yy_reduce`: 13.89%
   - `Parse`: 9.24%

3. **String Operations (10%+)**
   - `sdsMakeRoomFor`: 4.23%
   - `sdscatlen`: 2.83%
   - `sdscatsds`: 1.74%
   - `sdsempty`: 0.64%

4. **Scanner**:
   - `omnomnum_scanner_start`: 9.37%
   - `find_numbers`: 5.29%

### Massif Memory Profiling Results

- **Peak Memory**: ~20 KB (small footprint)
- **Main Allocations**:
  - `ParseAlloc`: 55.40% of peak (11,280 B)
  - `ensureYYSTYPECapacity`: 19.10% (3,888 B)
  - `sdsMakeRoomFor`: 4.67% (950 B)

**Key Finding**: The problem is not memory footprint, but **allocation frequency**. We're constantly allocating and freeing small chunks, which dominates CPU time.

## Root Cause Analysis

### 1. Frequent `sds` String Allocations

**Location**: `omnomnum.c:432-434`
```c
state->result = sdsempty();              // Allocation #1
state->result = sdsMakeRoomFor(...);     // Allocation #2 (realloc)
```

**Impact**: Every call to `normalize()` allocates a new sds string. With 5000 iterations and ~90 numbers per iteration, this is **5000+ allocations**.

### 2. Temporary String Allocations in `process_percent`

**Location**: `omnomnum.c:228-229, 273`
```c
sds output = sdsempty();                    // Every percent check
output = sdsMakeRoomFor(output, len + 64);  // Immediate realloc
sds num_str = sdsnewlen(...);               // Per number
```

**Impact**: Creates temporary strings for processing that could use stack buffers.

### 3. Parser Allocation Per Call

**Location**: Visible in massif - `ParseAlloc` consumes 55% of peak memory

**Impact**: Parser structures are allocated/freed repeatedly instead of being reused.

### 4. Dynamic Array Growth

**Location**: `ensureYYSTYPECapacity` - 19.10% of memory allocations

**Impact**: YYSTYPE list grows dynamically, triggering reallocations.

## Optimization Strategies

### Priority 1: Reuse `sds` Buffers (High Impact)

**Approach**: Pre-allocate result buffer in `ParserState` and reuse across calls

**Before**:
```c
void normalize(..., ParserState *state) {
    state->result = sdsempty();  // New allocation every time
    state->result = sdsMakeRoomFor(state->result, data_len + 32);
    ...
}
```

**After**:
```c
struct ParserState {
    sds result;
    sds temp_buffer;  // Reusable temp buffer
    ...
};

void normalize(..., ParserState *state) {
    sdsclear(state->result);  // Reset existing buffer
    state->result = sdsMakeRoomFor(state->result, data_len + 32);
    ...
}
```

**Expected Impact**: Eliminate 5000+ allocations → **10-15% speedup**

### Priority 2: Use Stack Buffers for Small Temps (Medium Impact)

**Approach**: Replace small temporary `sds` allocations with stack buffers

**Before**:
```c
sds num_str = sdsnewlen(*result + num_start, num_end - num_start);  // Heap allocation
...
sdsfree(num_str);
```

**After**:
```c
char num_buf[64];  // Stack allocation
size_t num_len = num_end - num_start;
if (num_len < sizeof(num_buf)) {
    memcpy(num_buf, *result + num_start, num_len);
    num_buf[num_len] = '\0';
    // Use num_buf directly
}
```

**Expected Impact**: Reduce allocation overhead for number parsing → **5-8% speedup**

### Priority 3: Parser Object Pooling (Medium Impact)

**Approach**: Reuse parser structure across calls instead of alloc/free

**Before**:
```c
pParser = ParseAlloc();  // Every call
...
ParseFree(pParser);
```

**After**:
```c
struct ParserState {
    void *pParser;  // Persistent parser
    ...
};

void initParserState(ParserState *state) {
    state->pParser = ParseAlloc();  // Once
}

void resetParserState(ParserState *state) {
    // Reset parser state without deallocation
}
```

**Expected Impact**: Eliminate parser alloc/free overhead → **3-5% speedup**

### Priority 4: Pre-allocate YYSTYPE Array (Low Impact)

**Approach**: Pre-allocate with reasonable initial capacity

**Before**:
```c
YYSTYPEList list;
list.capacity = 8;  // Small initial size
```

**After**:
```c
YYSTYPEList list;
list.capacity = 128;  // Typical case: 90 numbers → avoid most reallocations
```

**Expected Impact**: Reduce dynamic array growth → **2-3% speedup**

## Implementation Plan

### Phase 1: Quick Wins (Estimated 15-20% improvement)
1. ✅ Profile with valgrind callgrind/massif
2. ⬜ Implement sds buffer reuse in `ParserState`
3. ⬜ Replace small temp `sds` with stack buffers
4. ⬜ Test and benchmark

### Phase 2: Structural Improvements (Estimated 5-8% additional)
5. ⬜ Implement parser object pooling
6. ⬜ Increase initial YYSTYPE capacity
7. ⬜ Test and benchmark

### Phase 3: Validation
8. ⬜ Run full Google Benchmark suite
9. ⬜ Verify no regressions in BM_simple or BM_long_string
10. ⬜ Target: BM_many_numbers < 80,000 ns (within 10% of original)

## Risk Assessment

**Low Risk**:
- Sds buffer reuse (well-defined lifecycle)
- Stack buffer for small temps (bounded size)
- YYSTYPE pre-allocation (just sizing)

**Medium Risk**:
- Parser object pooling (need to ensure proper reset)

## Success Criteria

- ✅ BM_many_numbers improves by 15-25%
- ✅ BM_simple/BM_long_string remain stable (±2%)
- ✅ All existing tests pass
- ✅ No memory leaks (valgrind massif verification)

## References

- Callgrind output: `callgrind.out.current`
- Massif output: `massif.out.current`
- Test program: `profile_many_numbers.c`
- Baseline: Commit #3 (c90bb9cc) - 74346 ns
- Current: Commit #48 (29bc590c) - 90503 ns

---

**Next Steps**: Begin Phase 1 implementation with sds buffer reuse
