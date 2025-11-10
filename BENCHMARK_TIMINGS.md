# OmNomNum Benchmark Suite - Timing Results

## Test Configuration
- **Hardware**: 16 CPU cores @ 2600.04 MHz
- **BENCH_PREFIX**: /home/user/omnomnum/tools
- **Build Time**: ~9 seconds (first run only, cached afterwards)

## Benchmark Mode Timings (Actual Measurements)

### Summary Table
| Mode | Settings | Runtime | Total (with build) | Use Case |
|------|----------|---------|-------------------|----------|
| **FAST** | 0.05s min_time, 5 reps | **14 seconds** | ~23 seconds | Quick iteration during development |
| **DEFAULT** | 0.1s min_time, 10 reps | **54 seconds** | ~63 seconds | Standard CI/CD performance checks |
| **ACCURATE** | 0.2s min_time, 10 reps | **108 seconds** | ~117 seconds | Comprehensive regression analysis |

### Detailed Results

#### FAST Mode (14 seconds)
- Configuration: `--benchmark_min_time=0.05s --benchmark_repetitions=5`
- Best for: Rapid development iteration
- Coefficient of Variation: Generally <3%
- Total benchmarks run: 38 (22 original + 16 new)

#### DEFAULT Mode (54 seconds)  
- Configuration: `--benchmark_min_time=0.1s --benchmark_repetitions=10`
- Best for: CI/CD performance monitoring
- Coefficient of Variation: Generally <2%
- Recommended for: Pull request checks

#### ACCURATE Mode (108 seconds)
- Configuration: `--benchmark_min_time=0.2s --benchmark_repetitions=10`
- Best for: Detailed profiling with valgrind
- Coefficient of Variation: Generally <1.5%
- Recommended for: Performance optimization work

## Coverage Summary

The enhanced benchmark suite (38 total benchmarks) covers:

### Core Functionality (22 original benchmarks)
- Simple number parsing
- Long strings with numbers
- Many consecutive numbers
- Decimal handling (simple & complex)
- Ordinal numbers (common & large)
- Negative numbers
- Large numbers & compositions
- Mixed fractions (word & numeric)
- Numeric fractions
- Percentages
- Zero handling
- Edge cases & boundaries
- Decimal multipliers
- Compound numbers
- All single digits
- Teen numbers
- Tens
- Comprehensive mixed scenarios

### New Coverage (16 additional benchmarks)
- Numeric literals (WHOLE_NUMBER, DECIMAL tokens)
- Spelled decimals (word-based like "three point one four")
- International decimal formats (1,234.56 patterns)
- Date preservation (MM/DD/YYYY patterns)
- Article prefixes ("a hundred", "a thousand")
- Common misspellings ("ninteen", "fourty", "nineth")
- Special thousand patterns ("1000 and three")
- Simple fraction words (standalone "half", "quarter")
- Mixed numeric/text content
- Zero-prefixed numbers (0123, 0456)
- Ordinal fractions ("one hundredth", "two thousandths")
- Complex compound numbers ("twenty one", "thirty two")
- Alternating numeric and word types
- Long numeric sequences (123456789)
- Very large cardinals (max value compositions)

## Usage Examples

```bash
# Fast mode (development)
BENCH_PREFIX=/path/to/tools BENCH_MODE=fast bash scripts/benchmark_current.sh output.json

# Default mode (CI/CD)
BENCH_PREFIX=/path/to/tools BENCH_MODE=default bash scripts/benchmark_current.sh output.json

# Accurate mode (profiling)
BENCH_PREFIX=/path/to/tools BENCH_MODE=accurate bash scripts/benchmark_current.sh output.json

# With valgrind (use accurate mode)
BENCH_MODE=accurate bash scripts/benchmark_current.sh output.json
valgrind --tool=callgrind ./test/test_benchmark --benchmark_min_time=0.2s
```

## Profiling Integration

All modes work well with valgrind tools:

### Callgrind (Function-level profiling)
```bash
valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./test/test_benchmark
callgrind_annotate callgrind.out
```

### Massif (Memory profiling)  
```bash
valgrind --tool=massif --massif-out-file=massif.out ./test/test_benchmark
ms_print massif.out
```

### Cachegrind (Cache profiling)
```bash
valgrind --tool=cachegrind --cachegrind-out-file=cachegrind.out ./test/test_benchmark
cg_annotate cachegrind.out
```

## Conclusion

All three benchmark modes complete in reasonable time and provide comprehensive coverage for:
- ✅ Performance regression detection
- ✅ Holistic profiling with valgrind
- ✅ Edge case performance validation
- ✅ Optimization verification across all codepaths
