# Word-to-Number Lookup Implementation Comparison

## Benchmark Results Summary

All benchmarks run with:
- 10 repetitions
- 1s minimum time per iteration
- -O3 optimization
- Aggregates only reported

| Implementation | BM_simple (ns) | BM_long_string (ns) | BM_many_numbers (ns) | Notes |
|----------------|----------------|---------------------|----------------------|-------|
| **Manual (linear strncmp)** | 181 | **1098** ✓ | 31659 | Original implementation |
| gperf Array-based | 182 (+0.6%) | 1213 (+10.5%) ❌ | 31717 (+0.2%) | Slower on BM_long_string |
| **Simple Hash (winner)** | 182 (+0.6%) | **1097** ✓ | **31300** ✓ | Best overall |

## Implementation Details

### 1. Manual (Linear strncmp chains)
```c
static int map_card_small(const char* s, size_t n, double* out) {
    if (n==3 && !strncmp(s,"one",3)) { *out=1; return 1; }
    if (n==3 && !strncmp(s,"two",3)) { *out=2; return 1; }
    if (n==5 && !strncmp(s,"three",5)) { *out=3; return 1; }
    // ... 6 more comparisons
    return 0;
}
```

**Pros:**
- Simple and straightforward
- Excellent performance on BM_long_string
- No extra indirection

**Cons:**
- Linear search requires up to 9 comparisons
- Repetitive code
- Hard to maintain

### 2. gperf Array-based Perfect Hash
```c
static inline unsigned int card_small_hash(const char *str, size_t len) {
    static const unsigned char asso[256] = { /* 256 elements */ };
    return len + asso[(unsigned char)str[2]];
}

static int map_card_small(const char* s, size_t n, double* out) {
    static const struct { const char *name; double value; } wordlist[] = {
        {""}, {""}, {""}, {"six", 6.0}, {"five", 5.0}, /* ... */
    };
    if (n >= 3 && n <= 5) {
        unsigned int key = card_small_hash(s, n);
        if (key <= 15) {
            const char *w = wordlist[key].name;
            if (*s == *w && !strncmp(s+1, w+1, n-1)) {
                *out = wordlist[key].value;
                return 1;
            }
        }
    }
    return 0;
}
```

**Pros:**
- O(1) hash lookup
- Generated automatically by gperf

**Cons:**
- **10.5% slower on BM_long_string**
- 256-byte asso array (cache pressure)
- Sparse wordlist array with empty slots
- Extra indirection through hash function
- More complex code

**Why it's slower:**
- Array lookup `asso[(unsigned char)str[2]]` requires memory access
- Struct array with empty slots wastes cache lines
- Additional bounds checking (`key <= 15`)
- Two-step process: hash + array lookup

### 3. Simple Hash with Switch (WINNER)
```c
static int map_card_small(const char* s, size_t n, double* out) {
    unsigned int h = (n << 8) | s[0];
    switch (h) {
        case (3<<8)|'o': if (!strncmp(s,"one",3)) { *out=1; return 1; } break;
        case (3<<8)|'t': if (!strncmp(s,"two",3)) { *out=2; return 1; } break;
        case (5<<8)|'t': if (!strncmp(s,"three",5)) { *out=3; return 1; } break;
        case (4<<8)|'f':
            if (!strncmp(s,"four",4)) { *out=4; return 1; }
            if (!strncmp(s,"five",4)) { *out=5; return 1; }
            break;
        // ... 5 more cases
    }
    return 0;
}
```

**Pros:**
- **Matches manual performance** (BM_long_string: 1097 vs 1098 ns)
- **1.1% faster** on BM_many_numbers
- Clean, maintainable code structure
- Compiler optimizes switch to jump table
- Single integer computation (shift + OR)
- No array lookup overhead
- Excellent branch prediction

**Cons:**
- Still requires strncmp for final verification
- Slightly more code than linear (but much more maintainable)

**Why it wins:**
- Hash computation is pure ALU operation (no memory access)
- Switch compiles to efficient jump table
- Only one case collision ('f' for "four"/"five")
- Better code organization without performance penalty

## Conclusion

**Winner: Simple Hash with Switch**

The simple hash implementation provides the best balance of:
1. **Performance**: Matches manual on BM_long_string, beats it on BM_many_numbers
2. **Maintainability**: Clear structure, easy to understand
3. **Simplicity**: No external tools (gperf) required
4. **Efficiency**: Pure computation, no memory indirection

**Key Insight:** Sometimes a simple, well-designed hash is better than a "perfect" hash that requires array lookups and extra indirection.

## Recommendation

Keep the **Simple Hash (length<<8|first_char) with switch** implementation in scanner.re.
Remove the gperf .gperf files as they're no longer needed.
