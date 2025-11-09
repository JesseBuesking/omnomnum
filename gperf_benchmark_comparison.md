# Complete gperf Benchmark Comparison

## Question
Does using gperf's `-m` flag for truly minimal perfect hash improve performance?

## Results Summary

| Implementation | BM_simple | BM_long_string | BM_many_numbers | Wordlist Size |
|---|---|---|---|---|
| **Manual (winner)** | 181 ns | **1098 ns** ✓ | 31659 ns | N/A |
| gperf default | 182 ns | 1213 ns (+10.5%) | 31717 ns | 16 slots (31% waste) |
| gperf -m 100 (minimal) | 181 ns | **1221 ns (+11%)** ❌ | 31961 ns (+1%) | 12 slots (0% waste) |

## Key Findings

### 1. Minimal Perfect Hash is Still Slower
Even with `-m 100` producing a truly minimal perfect hash (9 keys → indices 3-11 with no gaps), it's **11% slower** than manual on BM_long_string.

### 2. Minimality ≠ Performance
- Default gperf: 4 empty slots (31% waste), 10.5% slower
- Minimal gperf: 0 empty slots (0% waste), **11% slower**

Removing the gaps made it slightly WORSE, not better!

### 3. Why Manual Wins: Zero Memory Indirection

**Manual implementation:**
```c
if (n==3 && !strncmp(s,"one",3)) { *out=1; return 1; }
if (n==3 && !strncmp(s,"two",3)) { *out=2; return 1; }
// ... 7 more branches
```
- Pure register operations
- Zero memory lookups before strncmp
- Compiler can optimize branch prediction

**Minimal gperf implementation:**
```c
key = len + asso[(unsigned char)str[2]];    // Memory load #1 (256-byte array)
w = wordlist[key].name;                      // Memory load #2 (struct array)
if (*s == *w && !strncmp(s+1, w+1, n-1))    // Still need strncmp!
```
- Two memory loads before strncmp
- 256-byte asso array (cache pressure)
- Can't eliminate final strncmp verification
- More complex control flow

### 4. Attempted Optimizations

**Option `-k1` (first char only):**
```
Error: "five" = "four", collision on 'f'
```

**Option `-k'$'` (last char only):**
```
Error: "nine" = "five", collision on 'e'
```

**Option `-k1,'$'` (first + last chars):**
```c
return len + asso[str[len-1]] + asso[str[0]+1];  // TWO array lookups!
```
Even worse performance due to double indirection.

### 5. Why gperf Can't Win

The fundamental issue: **gperf must do hash lookup + verification**.

For small word sets (9-10 words), the overhead of:
- Loading 256-byte asso array
- Computing hash
- Loading wordlist struct
- Still calling strncmp for verification

...is greater than just doing 9 sequential if-comparisons with well-predicted branches.

## Hash Computation Overhead Analysis

**Manual:**
- Average case: ~4.5 comparisons (half the list)
- Best case: 1 comparison
- Worst case: 9 comparisons
- Each comparison: simple integer + string operation

**Minimal gperf:**
- Always: 1 array lookup + 1 struct lookup + 1 strncmp
- Fixed overhead regardless of hit/miss
- Memory access latency dominates

For small N (< 10-15 items), linear search with branch prediction beats hash tables.

## Conclusion

**Manual linear strncmp chains win** for small word sets because:

1. **Zero lookup overhead** - no hash computation required
2. **Better cache behavior** - no 256-byte asso array
3. **Simple control flow** - easier for CPU branch predictor
4. **Readable code** - self-documenting, no magic

The crossover point where hashing becomes beneficial is likely around N=20-30 items, not N=9.

## Recommendation

**Keep the manual implementation.** For these small word sets (9-10 words), the simplicity and performance of linear search is unbeatable.
