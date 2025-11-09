# Small Keyset Lookup Strategies - Comprehensive Benchmark

## Problem
Lookup 9 small strings (one, two, three, four, five, six, seven, eight, nine) and return their numeric values.

## All Strategies Tested

### 1. Manual Linear Search (strncmp) ✓ CHOSEN
```c
if (n==3 && !strncmp(s,"one",3)) { *out=1; return 1; }
if (n==3 && !strncmp(s,"two",3)) { *out=2; return 1; }
// ... 7 more branches
return 0;
```

**Characteristics:**
- Simple linear if-chain
- Length check + strncmp for each candidate
- Average 4.5 comparisons

### 2. Optimized Decision Tree (switch + char checks)
```c
switch (n) {
case 3:
    if (s[0]=='o' && s[1]=='n' && s[2]=='e') { *out=1; return 1; }
    // ... more length-3 cases
case 4:
    if (s[0]=='f' && s[3]=='r') { *out=4; return 1; } // four
    // ... more length-4 cases
}
```

**Characteristics:**
- Switch on length first (jump table)
- Character-by-character comparison (no function call)
- Discriminate by first/last chars

### 3. Binary Search (sorted array)
```c
static const struct { const char* key; size_t len; double val; } table[] = {
    {"eight", 5, 8}, {"five", 4, 5}, ... // sorted
};
// Binary search with length + memcmp
```

**Characteristics:**
- log2(9) ≈ 4 comparisons worst case
- Sorted array with binary search
- Compare length first, then memcmp

### 4. gperf Default
Generated perfect hash with single array lookup + strncmp.

### 5. gperf Minimal (-m 100)
Truly minimal perfect hash (no wasted slots), 1 lookup + strncmp.

### 6. gperf Preset A (-n -k'1,2,$')
Minimal perfect hash using 3 array lookups (no length in hash).

### 7. gperf Preset B (-k'1,2,$' +len)
Near-minimal hash using 3 array lookups + length.

## Benchmark Results

All tests run on Google Benchmark with 10 repetitions.

| Strategy | BM_simple | BM_long_string | BM_many_numbers | Complexity |
|----------|-----------|----------------|-----------------|------------|
| **Manual Linear ✓** | **629 ns** | **2672 ns** | **86798 ns** | O(n) avg 4.5 |
| Decision Tree | 617 ns (-2%) | 2686 ns (+0.5%) | 87719 ns (+1%) | O(1) worst |
| Binary Search | 626 ns (-0.5%) | 2657 ns (-0.6%) | 85980 ns (-0.9%) | O(log n) |
| gperf default | 632 ns | 2713 ns | 87217 ns | O(1) |
| gperf -m 100 | 631 ns | 2721 ns | 87461 ns | O(1) |
| gperf Preset A | 638 ns (+1.4%) | 2650 ns (-0.8%) | 86333 ns (-0.5%) | O(1) |
| gperf Preset B | 630 ns (+0.2%) | 2659 ns (-0.5%) | 84979 ns (-2.1%) | O(1) |

## Key Finding

**All seven strategies perform identically within ±2% margin of error.**

For N=9 on modern hardware:
- Linear search (4.5 avg comparisons) ≈ Binary search (4 worst) ≈ Hash table (O(1))
- CPU optimizations dominate: out-of-order execution, branch prediction, L1 cache
- Differences are within statistical noise

## Why They're All The Same Speed

### 1. Modern CPU Features
- **Out-of-order execution** parallelizes independent operations
- **Branch predictor** handles predictable patterns (linear search) well
- **Speculative execution** starts work before branches resolve

### 2. Small Data Size
- All lookup tables fit in L1 cache (32KB)
- 256-byte gperf asso_values array: ~4 cycles per access
- 9-element sorted array: entire dataset cache-resident

### 3. Compiler Optimizations
- GCC -O2 with LTO inlines aggressively
- Both manual and hash approaches optimize to similar machine code
- Switch statements become jump tables

### 4. Scale Matters
- For N=9, setup/teardown overhead dominates
- Real performance differences only emerge at N≥50-100
- At small N, simplicity and readability matter more than algorithm

## Decision: Manual Linear Search Wins

While all approaches perform identically, the simple manual implementation is chosen for:

1. **Simplicity** - 9 readable if-statements vs complex generated code
2. **Maintainability** - Easy to modify, no build step
3. **Debuggability** - Clear logic, no magic
4. **Code size** - Smaller binary (no large static arrays)
5. **Zero dependencies** - No gperf or code generation

## When Each Strategy Becomes Best

### Linear Search (N ≤ 20)
Best for: Tiny keysets, maximum simplicity
- Pro: No setup, readable, debuggable
- Con: O(n) comparisons

### Decision Tree (N = 10-50)
Best for: Hand-optimized hot paths, when you control the build
- Pro: O(1) worst case, predictable
- Con: Tedious to write, fragile to changes

### Binary Search (N = 20-100)
Best for: Medium keysets, no codegen desired
- Pro: Simple, O(log n), good cache behavior
- Con: Requires sorted array, more comparisons than hash

### gperf / Perfect Hash (N ≥ 50-100)
Best for: Large keysets (keywords, protocols, enums)
- Pro: True O(1), fastest for large N
- Con: Code generation, larger binary, harder to debug

### Modern MPH libs (N ≥ 1000)
Best for: Massive keysets, extreme space constraints
- Pro: Minimal space, very fast
- Con: Build complexity, overkill for small N

## Practical Recommendations

1. **For N ≤ 20:** Use linear search. Don't overthink it.

2. **For N = 20-100:** Use binary search if you want simplicity, or decision tree if you need guaranteed O(1).

3. **For N ≥ 100:** Consider gperf or other perfect hashing. Benchmark to verify improvement.

4. **If misses are common:** Linear/binary search bail out early. Hash tables check full hash first.

5. **If code generation is OK:** gperf with `-C -l -c -m 50 -k '1,2,$'` works well.

6. **If you want no dependencies:** Binary search is the sweet spot for medium N.

## Lessons Learned

1. **Don't optimize prematurely** - For small N, all approaches are equivalent
2. **Measure, don't assume** - Theory (O(1) vs O(n)) doesn't predict real performance at small scale
3. **Modern CPUs are smart** - Branch prediction makes linear search competitive
4. **Simplicity has value** - When performance is equal, choose the most readable code
5. **Scale matters** - Algorithmic complexity only matters at larger N
6. **Context is king** - Cache behavior, branch prediction, and CPU features dominate at small scale

## References

- gperf manual: https://www.gnu.org/software/gperf/manual/gperf.html
- Google Benchmark: https://github.com/google/benchmark
- Modern CPU architecture: Branch prediction, out-of-order execution, cache hierarchy
