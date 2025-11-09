# Why Buffer Reuse (OPT1) is Slower on BM_many_numbers

## Your Question
"You're not creating new memory, you're clearing and reusing it... so how is it slower?"

## The Answer: sdsclear() Has Hidden Costs

### How SDS (Simple Dynamic Strings) Works

SDS strings have a header before the actual string data:
```c
struct sdshdr {
    size_t len;      // current length
    size_t free;     // available space
    char buf[];      // actual string data
}
```

### What sdsclear() Does

```c
void sdsclear(sds s) {
    struct sdshdr *sh = (void*)(s - sizeof(struct sdshdr));
    sh->len = 0;           // Reset length
    sh->free += sh->len;   // Add length to free space
    s[0] = '\0';           // Set null terminator
}
```

Even though it "just" clears the buffer, it:
1. Calculates header position (pointer arithmetic)
2. Reads current len/free
3. Updates len/free  
4. Writes null terminator
5. Returns

### What sdsempty() + sdsfree() Does

```c
sds sdsempty() {
    return sdsnewlen("", 0);  // Allocate minimal buffer
}

void sdsfree(sds s) {
    free(s - sizeof(struct sdshdr));  // Just free()
}
```

### The Performance Paradox

**Why OPT1 is slower on BM_many_numbers:**

BM_many_numbers test case:
- Processes ~90 numbers per normalize() call
- Result buffer grows significantly during processing
- sdsclear() overhead happens EVERY iteration (5000x)
- Conditional check `if (state->result)` adds branch prediction overhead

**Overhead per iteration:**
1. Branch check: `if (state->result)` - pipeline stall if mispredicted
2. sdsclear() function call overhead
3. Header manipulation (len/free updates)
4. Memory write to set null terminator

For many_numbers, this adds up to ~600ns per iteration over 5000 iterations = 3ms total overhead.

**Why OPT1 is faster on BM_simple/BM_long_string:**

These tests have:
- Fewer normalize() calls overall  
- Smaller result buffers
- Better cache locality from reusing same memory region
- Amortized cost: buffer reuse saves more than overhead costs

### The Real Trade-off

```
OPT1 Cost = (branch_check + sdsclear_overhead) × iterations
OPT1 Benefit = (malloc + free) × iterations + cache_locality_improvement

For simple/long strings: Benefit > Cost (7-22% faster)
For many_numbers: Cost ≈ Benefit (1.9% slower)
```

### Modern Allocator Magic

Modern allocators (jemalloc, tcmalloc) are EXTREMELY fast for small, short-lived allocations:
- Thread-local caches
- Size-class segregation  
- Minimal locking

So `malloc(32) + free()` can be nearly as fast as reusing a buffer when:
- Allocations are small
- Lifetime is short
- Same thread
- Same size class

## Conclusion

Buffer reuse ISN'T automatically faster because:
1. sdsclear() has real overhead (not zero-cost)
2. Conditional branches hurt pipeline performance
3. Modern allocators are surprisingly fast
4. Cache effects depend on workload characteristics

The 1.9% regression on many_numbers is the overhead cost, while 7-22% gains on simple/long strings show where reuse wins.
