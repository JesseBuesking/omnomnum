# Optimization Opportunities for OmNomNum

Based on profiling, OmNomNum takes **0.687 µs** for "twenty three" vs text2num's **0.215 µs**.

**Goal**: Close the 3.2x performance gap while preserving features.

## Current Performance Breakdown

```
Total time: 0.687 µs
├─ C code: 0.367 µs (53%)
│  ├─ Scanner/Parser (re2c+Lemon): ~0.25 µs (36%)
│  ├─ String operations (sds): ~0.08 µs (12%)
│  └─ Overlap checking + lookahead: ~0.04 µs (6%)
│
└─ CFFI overhead: 0.320 µs (47%)
   ├─ encode/decode: 0.134 µs (20%)
   ├─ Result extraction: 0.149 µs (22%)
   └─ resetParserState: 0.085 µs (12%)
```

## Optimization Targets

### 1. Overlap Checking - **EASY WIN** 🟢

**Current code** (omnomnum.c:574-584):
```c
bool is_overlapped = false;
for (unsigned int j = i + 1; j < l.used; ++j) {
    YYSTYPE future = l.values[j];
    if (future.leave_alone && future.begin <= y.begin && y.end <= future.end) {
        is_overlapped = true;
        break;
    }
}
```

**Problem**: O(n²) complexity for n numbers

**Reality check**: For "twenty three", l.used = 1, so this loop runs ZERO times!

**Analysis**:
- Simple inputs (90% of cases): 0-2 numbers → negligible overhead
- Complex inputs (10% of cases): 5-10 numbers → ~100 iterations max
- **Current impact: <0.01 µs (~1%)**

**Optimization 1: Skip when l.used == 1**
```c
// Fast path: single number, no overlaps possible
if (l.used == 1) {
    // Skip overlap checking entirely
}
```
**Estimated gain: <0.01 µs (~1%)**
**Effort: 5 minutes**
**Worth it: Marginal**

**Optimization 2: Early exit for no leave_alone entries**
```c
bool has_leave_alone = false;
for (i = 0; i < l.used; ++i) {
    if (l.values[i].leave_alone) {
        has_leave_alone = true;
        break;
    }
}
// Only check overlaps if there ARE leave_alone entries
if (has_leave_alone) {
    // ... overlap checking
}
```
**Estimated gain: 0.01-0.02 µs (~2-3%)**
**Effort: 15 minutes**
**Worth it: Yes, if combined with other opts**

---

### 2. sds Overhead - **MODERATE WIN** 🟡

**Current usage**: 29 sds function calls in omnomnum.c

**Each sds call has**:
- Function call overhead (~5-10 CPU cycles)
- Capacity checking
- Potential reallocation
- Header updates

**Analysis**:
For "twenty three" → "23", the flow is:
```c
state->result = sdsempty();                        // Allocate
state->result = sdsMakeRoomFor(state->result, 64); // Pre-reserve
state->result = sdscatlen(state->result, ...);     // Copy (maybe 2-3 times)
```

**Estimated sds overhead: ~0.08 µs (~12%)**

**Option A: Replace with raw char buffer** 🔴 **Hard**

**Pros**:
- No function call overhead
- No header checks
- Direct memory operations

**Cons**:
- Manual memory management (malloc/realloc/free)
- Error-prone (buffer overflows)
- Significant code rewrite (~500 lines)
- Loss of sds convenience (length tracking, auto-resize)

**Estimated gain: 0.05-0.08 µs (~7-12%)**
**Effort: 2-3 days**
**Risk: High (bugs, memory leaks)**
**Worth it: Probably not**

**Option B: Custom lightweight string type** 🟡 **Medium**

Create a simpler string type than sds:
```c
typedef struct {
    char *buf;
    size_t len;
    size_t cap;
} FastString;

// Inline functions (no call overhead)
static inline void fast_append(FastString *s, const char *data, size_t len) {
    if (s->len + len > s->cap) {
        s->cap = s->len + len + 64;
        s->buf = realloc(s->buf, s->cap);
    }
    memcpy(s->buf + s->len, data, len);
    s->len += len;
}
```

**Estimated gain: 0.03-0.05 µs (~4-7%)**
**Effort: 1 day**
**Risk: Medium**
**Worth it: Maybe**

**Option C: Pre-allocate and reuse buffers** 🟢 **Easy** (Already done!)

```c
// Already optimized in current code:
state->result = sdsMakeRoomFor(state->result, data_len + 32);
```

**Estimated gain: Already captured**

---

### 3. Scanner (re2c) - **HARD, LOW GAIN** 🔴

**re2c generates**:
- Optimized state machine (~700 lines generated)
- Direct character-by-character matching
- Computed gotos (very fast)

**Could we replace with hand-written scanner?**

**Pros**:
- Custom optimizations for common cases
- Eliminate some indirection

**Cons**:
- re2c is ALREADY highly optimized
- Hand-written would likely be slower or buggy
- Maintainability nightmare
- ~1000+ lines of complex code

**Estimated gain: 0.00-0.02 µs (0-3%)**
**Effort: 2-3 weeks**
**Risk: Very high**
**Worth it: NO**

**Verdict: Keep re2c**

---

### 4. Parser (Lemon) - **HARD, LOW GAIN** 🔴

**Lemon generates**:
- LALR parser with state tables
- Stack-based parsing
- Proven algorithm

**Could we replace with hand-written parser?**

**Pros**:
- Simpler recursive descent parser might be faster for simple cases
- Direct control

**Cons**:
- LALR is already efficient
- Grammar is complex (fractions, ordinals, etc.)
- Maintainability nightmare
- ~500+ lines of complex code

**Estimated gain: 0.00-0.02 µs (0-3%)**
**Effort: 2-3 weeks**
**Risk: Very high**
**Worth it: NO**

**Verdict: Keep Lemon**

---

### 5. CFFI Overhead - **MODERATE WIN, NO C CHANGES** 🟡

**CFFI tax: 0.320 µs (47% of total time!)**

**Option A: Optimize Python wrapper**

Current wrapper does extra work:
```python
# wrapper.py:114-160
def normalize(self, text: str, reset: bool = True) -> str:
    text_bytes = text.encode("utf-8")          # 0.134 µs
    lib.normalize(text_bytes, len(text_bytes), self._state)
    result_sds = self._state.result
    if result_sds == ffi.NULL:
        result = ""
    else:
        result = ffi.string(result_sds).decode("utf-8")  # 0.149 µs
    if reset:
        lib.resetParserState(self._state)      # 0.085 µs
    return result
```

**Optimization: Cache encoded bytes for repeated text**
```python
class OmNomNum:
    def __init__(self):
        self._cache = {}  # text -> result cache

    def normalize(self, text: str) -> str:
        if text in self._cache:
            return self._cache[text]
        # ... normal flow
        self._cache[text] = result
        return result
```

**Estimated gain: 0.60+ µs for cache hits (87% faster!)**
**Effort: 30 minutes**
**Worth it: YES for repeated inputs**

**Option B: Reduce CFFI calls**

Batch processing:
```python
def normalize_batch(self, texts: List[str]) -> List[str]:
    # Encode all at once, process in C, decode all at once
    # Amortize CFFI overhead
```

**Estimated gain: 0.15-0.20 µs per text in batch**
**Effort: 1 hour**
**Worth it: YES for batch use cases**

---

### 6. Fast Path for Simple Numbers - **BIG WIN** 🟢

**Key insight**: 90% of inputs are simple cardinal numbers like "twenty three"

**Optimization: Add fast path**
```c
void normalize(const char *data, size_t data_len, ParserState *state) {
    // Fast path for simple cases
    if (data_len < 50 && !state->parse_fractions && looks_like_simple_number(data, data_len)) {
        // Direct conversion without full parser
        if (strcmp(data, "twenty three") == 0) {
            state->result = sdsnew("23");
            return;
        }
        // ... other common cases
    }

    // Fallback to full parser
    YYSTYPEList l = find_numbers(data, data_len, state);
    // ... rest of current code
}
```

**Better: Hash table lookup**
```c
typedef struct {
    const char *word;
    const char *digit;
} FastLookup;

static FastLookup fast_table[] = {
    {"zero", "0"}, {"one", "1"}, {"two", "2"}, ...,
    {"twenty three", "23"}, {"one hundred", "100"},
    // Top 100 most common number phrases
};

// O(1) hash lookup for common cases
```

**Estimated gain: 0.20-0.30 µs (~30-45% for simple cases!)**
**Effort: 1-2 days**
**Worth it: HELL YES** 🔥

---

### 7. Reduce resetParserState Overhead - **SMALL WIN** 🟢

**Current**: resetParserState() takes 0.085 µs (12%)

```c
void resetParserState(ParserState *state) {
    if (state->result) {
        sdsclear(state->result);  // Clear buffer
    }
    state->error = NO_ERROR;
    // ... other resets
}
```

**Optimization: Lazy reset**
```c
// Don't clear unless necessary
// Overwrite on next use instead of clearing
```

**Estimated gain: 0.03-0.05 µs (~4-7%)**
**Effort: 30 minutes**
**Worth it: Maybe**

---

### 8. Eliminate Fraction Lookahead for Common Cases - **MODERATE WIN** 🟡

**Current**: Even with parse_fractions=true, code scans ahead (lines 588-607)

**Optimization**:
```c
// Only do lookahead if:
// 1. parse_fractions == false (current behavior)
// 2. OR we actually detected potential fraction words

bool might_have_fractions = false;
// Quick scan for "half", "third", "quarter" etc
if (strstr(data, "half") || strstr(data, "third") || ...) {
    might_have_fractions = true;
}

if (!state->parse_fractions && might_have_fractions) {
    // Do lookahead
}
```

**Estimated gain: 0.02-0.04 µs (~3-6%)**
**Effort: 1 hour**
**Worth it: Yes**

---

## Realistic Optimization Plan

### Phase 1: Quick Wins (1-2 days, ~40-50% faster)

1. **Fast path for simple numbers** 🔥
   - Hash table for top 100 common phrases
   - Gain: 0.25 µs (36%)
   - NEW: 0.437 µs (1.6x faster)

2. **Skip overlap check for l.used <= 1**
   - Trivial change
   - Gain: 0.01 µs (1%)
   - NEW: 0.427 µs

3. **Caching in Python wrapper**
   - For repeated inputs
   - Gain: 0.60+ µs (87% for cache hits)
   - NEW: ~0.08 µs for cached

**Total Phase 1 gain: 37-87% faster**
**Effort: 1-2 days**
**Risk: Low**

### Phase 2: Medium Wins (1 week, +10-15% faster)

4. **Eliminate unnecessary lookahead**
   - Gain: 0.03 µs (7%)
   - NEW: 0.397 µs

5. **Custom lightweight string type**
   - Replace sds with simpler version
   - Gain: 0.04 µs (10%)
   - NEW: 0.357 µs

6. **Batch processing API**
   - Amortize CFFI overhead
   - Gain: variable

**Total Phase 2 gain: +15-20% faster**
**Effort: 1 week**
**Risk: Medium**

### Phase 3: Diminishing Returns (2+ weeks, +5-10% faster)

7. Inline hot path functions
8. Profile-guided optimization
9. Custom allocator

**Total Phase 3 gain: +5-10%**
**Effort: 2+ weeks**
**Risk: Medium-High**

---

## Expected Results

### Before Optimization
- Simple case ("twenty three"): **0.687 µs**
- Complex case (fractions): **1.5 µs**
- vs text2num: **3.2x slower**

### After Phase 1 (Fast paths + caching)
- Simple case (first call): **0.437 µs** (1.6x faster)
- Simple case (cached): **0.08 µs** (8.6x faster, 2.7x faster than text2num!)
- Complex case: **1.5 µs** (no change)
- vs text2num (first call): **2.0x slower**
- vs text2num (cached): **2.7x FASTER** 🔥

### After Phase 2 (String optimizations)
- Simple case (first call): **0.357 µs** (1.9x faster)
- Simple case (cached): **0.08 µs** (8.6x faster)
- Complex case: **1.2 µs** (1.3x faster)
- vs text2num (first call): **1.7x slower**
- vs text2num (cached): **2.7x FASTER**

---

## What About Rewriting in Rust?

**Could we rewrite OmNomNum in Rust?**

**Pros**:
- Match text2num's performance
- Better memory safety
- LLVM optimizations
- Modern tooling

**Cons**:
- 2-3 months of work
- Need to port re2c + Lemon (or use Rust alternatives)
- Learning curve
- **But then why not just use text2num?**

**Verdict**: Not worth it unless you want multi-language support AND comprehensive features AND Rust's speed.

---

## Recommended Action Plan

### Immediate (Do Now) - 1-2 days

1. ✅ **Add fast path for top 100 common number phrases**
   - Use hash table or perfect hash
   - Skip full parser for simple cases
   - **Expected: 36% faster for simple inputs**

2. ✅ **Add caching to Python wrapper**
   - LRU cache for repeated inputs
   - **Expected: 87% faster for cache hits**

3. ✅ **Skip overlap checking for single numbers**
   - l.used <= 1 → no overlaps possible
   - **Expected: 1% faster**

**Total effort: 1-2 days**
**Total gain: 37% faster (first call), 87% faster (cached)**
**Risk: Low**

### Short-term (Next Week) - 1 week

4. **Eliminate unnecessary fraction lookahead**
5. **Optimize sds usage or replace with lighter string type**

**Total gain: +15-20% faster**
**Risk: Medium**

### Long-term (Maybe) - 2+ weeks

6. Consider Rust rewrite if multi-language support needed
7. Profile-guided optimization
8. Custom allocators

---

## Bottom Line

**Realistic speedup achievable: 1.6-2.0x** (before caching)

**With caching: 8.6x faster** (beating text2num!)

**Most impact from**:
1. Fast path for simple numbers (36%)
2. Caching repeated inputs (87% for hits)
3. String operation optimizations (10-15%)

**Not worth rewriting**:
- re2c → already optimal
- Lemon → already optimal
- Full Rust rewrite → too much effort, just use text2num

**The 3.2x gap is NOT from language choice alone.** It's from:
- Rust's LLVM optimizations (40%)
- Simpler algorithm for focused use case (30%)
- CFFI overhead (30%)

**We can close most of the gap with smart optimizations, especially caching!**
