# Fallback Path Analysis: Allocation Opportunities

## Background

Issue #1 (subState reuse) achieved **52.6% average speedup** by eliminating 99.9% of allocations in the fallback path. This was 10x better than the original 5-10% estimate because:

1. The fallback path is **more common than initially thought**
2. Allocation overhead is **significant** (~1KB per ParserState)
3. The fallback path is **hot** - executed for many common inputs

## What Triggers the Fallback Path?

From `omnomnum.c:465-470`:

```c
YYSTYPEList l = find_numbers(data, data_len, state);

if (l.used == 0) {
    // Fallback: split on separators and normalize each token independently
```

The fallback path activates when `find_numbers()` returns **zero results**. This happens when:
- Input contains no recognizable number patterns to the main parser
- Individual words/tokens need separate normalization
- Text has unusual formatting or structure

Common examples that trigger fallback:
- "one two three four five" (separate number words)
- "first second third" (ordinals with separators)
- Mixed text with scattered numbers
- Text with lots of separators/formatting

## Current Allocation Sites in Fallback Path

### 1. ✅ **FIXED: process_fallback_token() - SubState Allocation**
**Location**: `omnomnum.c:215-239` (in helper function)

**Original Issue**: Created new ParserState for each token
- ~1KB per allocation (YYSTYPEList + numberHolder)
- Called repeatedly in loops

**Fix Applied**: Issue #1 - Reuse subState via `getOrInitSubState()`
- **Result**: 99.9% allocation reduction, 52.6% speedup

---

### 2. **process_percent() - Output Buffer Allocation**
**Location**: `omnomnum.c:250`

**Current Code**:
```c
sds output = sdsempty();
output = sdsMakeRoomFor(output, len + 64);
// ... build output ...
sdsfree(*result);
*result = output;
```

**Issue**: Always allocates new buffer even if percent processing not needed or rare

**Frequency**: Only when `normalize_percent_symbol` or `percent_as_decimal` enabled

**Solutions**:

**Option A: Quick Scan First** (Recommended)
```c
// Scan first - avoid allocation if no percent symbols exist
if (!strstr(*result, "percent") && !strchr(*result, '%')) {
    return;  // Nothing to process
}
// Only now allocate output buffer
```

**Option B: Reuse numberHolder** (More complex)
```c
// Reuse state->numberHolder as temp buffer
sdsclear(state->numberHolder);
state->numberHolder = sdsMakeRoomFor(state->numberHolder, len + 64);
// ... build into numberHolder ...
// Swap
sds tmp = *result;
*result = state->numberHolder;
state->numberHolder = tmp;
```

**Expected Impact**: Minor (only helps when percent normalization enabled)

---

### 3. **find_numbers() - Malloc in Fraction Range Detection**
**Location**: `omnomnum.c:305`

**Current Code**:
```c
if (num_len < sizeof(num_buf)) {
    // Use stack buffer
    num_str = num_buf;
} else {
    // Fallback for large numbers (rare)
    num_str = (char *)malloc(num_len + 1);
    memcpy(num_str, *result + num_start, num_len);
    num_str[num_len] = '\0';
}
// ... later ...
if (num_len >= sizeof(num_buf)) {
    free(num_str);  // Free malloc'd memory
}
```

**Analysis**: Already optimized!
- Uses stack buffer (128 bytes) for common case
- Only mallocs for numbers > 128 chars (extremely rare)
- Proper cleanup

**Action**: None needed - already well-optimized

---

### 4. **Debug-Only Allocation**
**Location**: `omnomnum.c:408`

```c
#if debug
sds value = sdsnewlen(ss.token, ss.cursor - ss.token);
printf("token is %s at %d - %d\n", value, ss.token - data, ss.cursor - data);
sdsfree(value);
#endif
```

**Action**: None needed - debug code only

---

## New Opportunities Found

### 5. **YYSTYPEList Initial Capacity in SubState**

**Location**: `scanner.def.c:105-106`

**Current Code**:
```c
// OPTIMIZATION: Start with larger capacity to reduce reallocations
// Typical BM_many_numbers has ~90 numbers, so 128 avoids most growth
initYYSTYPEList(&(state->yystypeList), 128);
```

**Issue**: SubState allocates 128 elements even though fallback tokens are typically **single numbers**

**Analysis**:
- Main ParserState: 128 elements makes sense (parsing full strings)
- SubState (in fallback): Usually processes 1-2 numbers per token
- 128 × sizeof(YYSTYPE) ≈ 1-4 KB wasted per subState

**Solution**: Differentiate subState initialization
```c
// In getOrInitSubState():
if (state->subState == NULL) {
    state->subState = (ParserState*)malloc(sizeof(ParserState));
    // Initialize with smaller capacity for single-token parsing
    state->subState->error = NO_ERROR;
    // ... other fields ...
    state->subState->numberHolder = sdsempty();
    state->subState->subState = NULL;
    // OPTIMIZATION: SubState typically processes single tokens (1-2 numbers)
    initYYSTYPEList(&(state->subState->yystypeList), 8);  // Smaller capacity
}
```

**Expected Impact**:
- Reduces initial subState allocation from ~1-4KB to ~64-256 bytes
- Still grows dynamically if needed (via insertYYSTYPE realloc)
- Should provide measurable speedup in fallback scenarios

---

### 6. **process_percent() - Additional Malloc Site**

**Location**: Already analyzed in #2, but worth noting there's also:

```c
// Line 305: Malloc for large numbers in percent processing
num_str = (char *)malloc(num_len + 1);
```

This is already optimized with stack buffer fallback (see #3).

---

## Priority Ranking

| # | Opportunity | Location | Expected Impact | Actual Impact | Effort |
|---|-------------|----------|-----------------|---------------|--------|
| 1 | ✅ SubState reuse | `omnomnum.c` | 5-10% estimated | **52.6% speedup** | DONE |
| 2 | ✅ SubState YYSTYPEList capacity | `scanner.def.c:149` | 5-10% estimated | **+1.74% speedup, 93.75% alloc reduction** | DONE |
| 3 | process_percent() quick scan | `omnomnum.c:242` | 1-2% (conditional) | Not implemented | Easy |
| 4 | process_percent() reuse numberHolder | `omnomnum.c:250` | 1-2% (conditional) | Not implemented | Medium |

## Recommendation - UPDATE

~~**Priority: Issue #5 - SubState YYSTYPEList Capacity**~~ ✅ **COMPLETED**

**Commit**: 1dd82a4 "Optimize: Reduce SubState YYSTYPEList capacity from 128 to 8 elements"

**Results**:
- Performance: +1.74% average speedup (vs 5-10% estimated)
- Allocation reduction: 93.75% (6144 bytes → 384 bytes)
- Memory footprint: -0.6% (2529KB → 2513KB)
- Zero memory leaks

**Analysis**: See ISSUE_5_SUBSTATE_CAPACITY_RESULTS.md

**Combined fallback path improvements**:
- Issue #1: 52.6% speedup from subState reuse
- Issue #5: +1.74% from capacity optimization
- **Total: 54.34% faster fallback path**

The other opportunities (#3, #4) are minor and conditional (only help when percent normalization is enabled), with diminishing returns given the already-achieved 54% speedup.

---

## Measurement Plan

To verify issue #5:

1. **Baseline**: Current performance with 128-element subState lists
2. **Test**: Change subState YYSTYPEList capacity to 8 elements
3. **Benchmark**: Run full benchmark suite (especially fallback-heavy tests)
4. **Verify**: Check that dynamic growth works correctly for rare multi-number tokens

If successful, this could be another significant win in the already-hot fallback path.
