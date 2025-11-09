# Why gperf Didn't Generate a Minimal Perfect Hash

## The Problem

gperf claims to generate "minimal perfect hash functions" but by default it generated:
- 9 keys → hash range 3-15 (13 values) = **4 empty slots**

This is NOT minimal - a true minimal perfect hash should map 9 keys to exactly 9 consecutive values with NO empty slots.

## The Solution: `-m` Flag

The `-m` (multiple iterations) flag makes gperf truly minimize the hash table:

```bash
# Default gperf
gperf test_minimal.gperf
# Output: MIN_HASH_VALUE=3, MAX_HASH_VALUE=15 (range=13, 4 wasted slots)

# With -m 100 (try 100 different hash functions)
gperf -m 100 test_minimal.gperf
# Output: MIN_HASH_VALUE=3, MAX_HASH_VALUE=11 (range=9, 0 wasted slots!) ✓
```

## What `-m` Does

From `gperf --help`:
```
-m, --multiple-iterations=ITERATIONS
    Perform multiple choices of the -i and -j values,
    and choose the best results. This increases the
    running time by a factor of ITERATIONS but does a
    good job minimizing the generated table size.
```

Without `-m`, gperf uses a fast heuristic that often leaves gaps.
With `-m N`, gperf tries N different hash functions and picks the one with the smallest table.

## Results Comparison

### Default gperf (fast, not minimal):
```c
static const char * wordlist[] = {
    "", "", "",        // Indices 0-2: WASTED
    "six",             // Index 3
    "five",            // Index 4
    "seven",           // Index 5
    "", "",            // Indices 6-7: WASTED
    "two",             // Index 8
    "four",            // Index 9
    "three",           // Index 10
    "", "",            // Indices 11-12: WASTED
    "one",             // Index 13
    "nine",            // Index 14
    "eight"            // Index 15
};
// 16 slots for 9 keys = 44% wasted space!
```

### gperf -m 100 (slower generation, truly minimal):
```c
static const char * wordlist[] = {
    "", "", "",        // Indices 0-2 (offset, can't avoid with this hash formula)
    "six",             // Index 3
    "five",            // Index 4
    "seven",           // Index 5
    "four",            // Index 6
    "three",           // Index 7
    "two",             // Index 8
    "nine",            // Index 9
    "eight",           // Index 10
    "one"              // Index 11
};
// 12 slots for 9 keys, but indices 3-11 are PACKED (0% waste in used range)
```

## Why Not Zero-Based?

The hash formula is `len + asso_values[str[2]]`. Given these specific words, the minimum value this formula can produce is 3, not 0. That's determined by the character distribution in the input words.

A truly "zero-based minimal" hash would require:
1. Computing the hash
2. Subtracting MIN_HASH_VALUE to shift to 0

But this adds an extra subtraction operation, which might hurt performance.

## Performance Impact

**Default gperf:**
- Larger wordlist array (16 vs 12 elements)
- More cache lines (sparse array)
- Empty slots waste memory

**gperf -m 100:**
- Smaller wordlist array (12 elements)
- Denser packing (better cache locality)
- No wasted slots in the active range

## Terminology Clarification

- **Perfect hash**: No collisions (gperf always does this)
- **Minimal perfect hash**: Maps N keys to exactly N values with no gaps
  - Default gperf: **NOT** minimal (has gaps)
  - `gperf -m 100`: **YES** minimal (no gaps in range)

## Conclusion

To get a truly minimal perfect hash from gperf, use the `-m` flag:

```bash
gperf -m 100 input.gperf > output.c
```

The higher the iteration count, the better the minimization (at the cost of longer generation time).

For our 9-key example:
- Default: 13-value range (31% overhead)
- `-m 100`: 9-value range (0% overhead) ✓
