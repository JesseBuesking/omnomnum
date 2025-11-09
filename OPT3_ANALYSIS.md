# OPT3 (Stack Buffers) Performance Analysis

## Results Summary

| Benchmark | OPT1+OPT2 | OPT1+OPT2+OPT3 | Change |
|-----------|-----------|----------------|--------|
| BM_simple | 181 ns | 177 ns | **+2.2% faster** |
| BM_long_string | 1140 ns | 1371 ns | **-20.3% slower** |
| BM_many_numbers | 31654 ns | 30907 ns | **+2.4% faster** |

## Valgrind Callgrind Results

- Instruction reduction: 38M instructions (-3.1%)
- realloc reduction: 930K instructions
- sdsfree reduction: 288K instructions

**Profiling shows stack buffers work as intended - fewer allocations!**

## The Problem: BM_long_string Regression

**Why -20% slower on long_string?**

OPT3 adds overhead to `process_percent()`:
1. Additional stack frame (256-byte buffer)
2. Branch checks (`if (num_len < sizeof(num_buf))`)
3. memcpy overhead for small strings

If `process_percent()` is called but doesn't find many percent patterns, the overhead
costs more than the savings.

## Analysis: process_percent Usage

Need to check what BM_long_string actually contains and whether it triggers
percent processing significantly.

## Recommendation

**REJECT OPT3** - The 20% regression on long_string outweighs the 2% gains elsewhere.

The stack buffer optimization works (fewer allocations proven by profiling),
but the conditional overhead and stack frame cost dominate in cases where
percent processing doesn't find many matches.

## Alternative: Profile-Guided Optimization

Could add a fast-path check before entering process_percent to avoid overhead
when no percent symbols are present.
