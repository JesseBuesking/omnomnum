# Code Quality Improvements: Reduce Allocations & Memory

## Issues Found

### 1. **Sub-ParserState Allocations in Fallback Path** (MAJOR)
**Location**: omnomnum.c lines 511, 530

**Current Code**:
```c
ParserState sub; initParserState(&sub);
// ... use sub ...
freeParserState(&sub);
```

**Problem**:
- Each `initParserState()` allocates:
  - `numberHolder` (sdsempty) - heap allocation
  - `YYSTYPEList` (128 * sizeof(YYSTYPE)) - ~1KB allocation
- These are created/freed repeatedly in fallback tokenization
- Exact same code duplicated in two places (lines 511-528 and 530-547)

**Solutions**:

**Option A: Add sub-ParserState to main ParserState (best)**
```c
typedef struct {
    // ... existing fields ...
    ParserState *subState;  // Reusable sub-state for recursion
} ParserState;

void initParserState(ParserState *state) {
    // ... existing init ...
    state->subState = malloc(sizeof(ParserState));
    initParserState(state->subState);  // Initialize once
}

// In normalize(), reuse instead of creating:
resetParserState(state->subState);
state->subState->parse_second = state->parse_second;
// ... copy flags ...
YYSTYPEList sl = find_numbers(data + tok_start, tok_len, state->subState);
```

**Option B: Create lightweight init (good)**
```c
// Add a minimal init that doesn't allocate for simple use cases
void initParserStateLite(ParserState *state) {
    state->error = NO_ERROR;
    state->result = NULL;
    state->pParser = NULL;
    state->numberHolder = NULL;  // Don't allocate
    // Minimal YYSTYPE list
    initYYSTYPEList(&(state->yystypeList), 8);
}
```

**Option C: Eliminate recursion (best but complex)**
- Refactor to avoid recursive normalize() calls
- Process tokens directly without sub-ParserState

**Impact**: Could save 1KB+ per recursive call

---

### 2. **Code Duplication in Fallback Tokenization**
**Location**: omnomnum.c lines 511-528 vs 530-547

**Problem**: Identical code blocks differ only by surrounding context

**Solution**: Extract to helper function
```c
static void process_fallback_token(ParserState *state, const char *data,
                                     unsigned int tok_start, unsigned int tok_len) {
    ParserState sub; initParserState(&sub);
    sub.parse_second = state->parse_second;
    sub.precision = state->precision;
    sub.reduce_fractions = state->reduce_fractions;
    sub.parse_fractions = state->parse_fractions;
    sub.normalize_percent_symbol = state->normalize_percent_symbol;
    sub.percent_as_decimal = state->percent_as_decimal;

    YYSTYPEList sl = find_numbers(data + tok_start, tok_len, &sub);
    if (sl.used > 0) {
        sdsclear(state->numberHolder);
        yystypeToStringWithReduction(&state->numberHolder, sl.values[0],
                                      sub.precision, sub.reduce_fractions);
        state->result = sdscatsds(state->result, state->numberHolder);
    } else {
        state->result = sdscatlen(state->result, data + tok_start, tok_len);
    }
    freeParserState(&sub);
}
```

**Impact**: Reduces code size, improves maintainability

---

### 3. **Allocation in process_percent()**
**Location**: omnomnum.c line 224

**Current Code**:
```c
sds output = sdsempty();
output = sdsMakeRoomFor(output, len + 64);
// ... build output ...
sdsfree(*result);
*result = output;
```

**Problem**: Allocates new buffer every time, even though percent processing is optional

**Solution A: Use numberHolder**
```c
static void process_percent(sds *result, ParserState *state) {
    if (!state->normalize_percent_symbol && !state->percent_as_decimal) {
        return;
    }

    size_t len = sdslen(*result);
    if (len == 0) return;

    // Reuse numberHolder instead of allocating new
    sdsclear(state->numberHolder);
    state->numberHolder = sdsMakeRoomFor(state->numberHolder, len + 64);

    // Build into numberHolder instead of output
    // ... processing ...

    // Swap result and numberHolder
    sds tmp = *result;
    *result = state->numberHolder;
    state->numberHolder = tmp;
}
```

**Solution B: In-place modification (better but complex)**
- Modify result in-place where possible
- Only allocate if size increases significantly

**Impact**: Eliminates malloc/free when percent normalization is used

---

### 4. **Redundant NULL Checks**
**Location**: scanner.def.c lines 111, 120, 125-126

**Current Code**:
```c
if (state->result) { sdsclear(state->result); }
if (state->numberHolder) sdsclear(state->numberHolder);
if (state->numberHolder) { sdsfree(state->numberHolder); ...}
if (state->pParser) { ParseFree(state->pParser, free); ...}
```

**Problem**:
- `state->result` can be NULL (line 98 sets it to NULL)
- But `state->numberHolder` is ALWAYS allocated in initParserState (line 102)
- `state->pParser` starts NULL but once allocated stays allocated

**Solution**: Remove unnecessary checks
```c
// resetParserState:
if (state->result) sdsclear(state->result);
sdsclear(state->numberHolder);  // Always allocated

// freeParserState:
sdsfree(state->numberHolder);  // Always allocated
state->numberHolder = NULL;
if (state->pParser) {  // Needed - might be NULL
    ParseFree(state->pParser, free);
    state->pParser = NULL;
}
```

**Impact**: Minor - removes 1-2 branches

---

### 5. **YYSTYPEList Initial Capacity**
**Location**: scanner.def.c line 105

**Current**: 128 elements allocated upfront

**Analysis**:
- Comment says "Typical BM_many_numbers has ~90 numbers"
- 128 * sizeof(YYSTYPE) = 128 * 32 = 4KB (assuming 32-byte struct)
- Most inputs have far fewer numbers

**Solution**: Dynamic initial size based on input length
```c
void initYYSTYPEList(YYSTYPEList *l, size_t initialSize);

// In initParserState:
size_t estimate = expected_input_len / 20;  // Heuristic: 1 number per 20 chars
if (estimate < 8) estimate = 8;
if (estimate > 128) estimate = 128;
initYYSTYPEList(&(state->yystypeList), estimate);
```

But this requires knowing input length at init time. Current approach is fine.

---

### 6. **process_percent Buffer Allocation Pattern**
**Location**: omnomnum.c line 224

**Issue**: Always allocates even if no percent symbols exist

**Better approach**: Scan first, allocate only if needed
```c
static void process_percent(sds *result, ParserState *state) {
    if (!state->normalize_percent_symbol && !state->percent_as_decimal) {
        return;
    }

    size_t len = sdslen(*result);
    if (len == 0) return;

    // Quick scan: does result contain "percent" or "%"?
    if (!strstr(*result, "percent") && !strchr(*result, '%')) {
        return;  // No percent symbols, nothing to do
    }

    // Only now allocate
    sds output = sdsempty();
    // ... rest of processing ...
}
```

**Impact**: Avoids allocation when no percent symbols present

---

## Priority List

1. ~~**HIGH**: Fix sub-ParserState allocations (Option A: add to main ParserState)~~ ✅ **COMPLETED**
   - **Commit**: a7c77b6 "Optimize: Eliminate repeated ParserState allocations via subState reuse"
   - **Results**: 52.6% avg speedup, 99.9% allocation reduction, 1,799 malloc/free cycles eliminated
   - **Analysis**: See SUBSTATE_OPTIMIZATION_RESULTS.md and MALLOC_CYCLE_ANALYSIS.md
2. **MEDIUM**: Optimize process_percent allocation (use numberHolder or quick scan)
3. ~~**MEDIUM**: Extract duplicate fallback tokenization code~~ ✅ **COMPLETED**
   - **Commit**: b0b3a6f "Refactor: Extract duplicate fallback tokenization code"
   - **Results**: -7 net lines, code quality improvement, no performance impact (-0.55% within noise)
4. ~~**LOW**: Remove redundant NULL checks~~ ✅ **COMPLETED**
   - **Commit**: 49c5fcd "Refactor: Remove redundant NULL checks for numberHolder"
   - **Results**: Removed 2 branches, improved code clarity, no meaningful performance impact
5. **LOW**: Consider in-place percent processing

**Summary for #3 and #4**: See ISSUES_3_AND_4_SUMMARY.md

## Estimated Impact

- ~~Sub-ParserState fix~~ ✅ **ACTUAL: 52.6% avg speedup, 99.9% allocation reduction** (originally estimated 5-10%)
- process_percent optimizations: **~100 bytes saved when percent normalization used** (estimated)
- Code duplication fix: **~30 lines reduced** (estimated)
- NULL check removal: **~2 branches removed** (estimated)

~~Total: Modest improvement (~5-10% in fallback scenarios)~~
**ACTUAL (#1 completed): Exceptional 52.6% average performance improvement**

**Additional Optimization - Issue #5** (from FALLBACK_PATH_ANALYSIS.md):
- ✅ **COMPLETED**: SubState YYSTYPEList capacity reduction (128→8)
  - **Commit**: 1dd82a4 "Optimize: Reduce SubState YYSTYPEList capacity from 128 to 8 elements"
  - **Results**: +1.74% avg speedup, 93.75% allocation reduction, -0.6% memory footprint
  - **Analysis**: See ISSUE_5_SUBSTATE_CAPACITY_RESULTS.md
  - **Combined with #1**: 54.34% total fallback path speedup

Remaining items: Minor improvements, primarily code quality focused
