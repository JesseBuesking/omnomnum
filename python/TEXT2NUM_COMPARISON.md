# OmNomNum vs text2num Comparison

## Executive Summary

**text2num (Allo-Media) is 2.7x FASTER** than OmNomNum for basic number conversion, but **supports fewer features**. text2num focuses solely on cardinal numbers, while OmNomNum provides comprehensive support for fractions, decimals, ordinals, negatives, and text normalization.

## Performance Comparison

### Benchmark Results

**Test**: Converting "twenty three" to numeric form
**Iterations**: 10,000
**Platform**: macOS ARM64 (Apple Silicon)

| Library | Time (ms) | Time per op (µs) | Speed vs OmNomNum | Ops/second |
|---------|-----------|------------------|-------------------|------------|
| **text2num** | 2.40 | **0.2** | **2.7x faster** ⚡ | 4,172,679 |
| **OmNomNum (class)** | 6.48 | 0.6 | 1.0x (baseline) | 1,543,597 |
| **OmNomNum (function)** | 17.69 | 1.8 | 0.4x slower | 565,331 |

### Performance Analysis

1. **text2num wins on raw speed**: 2.7x faster than OmNomNum
   - Rust implementation with aggressive optimizations
   - Focused on single use case (cardinal numbers only)
   - ~4.2 million ops/second vs ~1.5 million ops/second

2. **Why text2num is faster**:
   - Written in Rust (known for performance)
   - Simpler feature set = less overhead
   - Highly optimized Rust parser

3. **OmNomNum trades speed for features**:
   - C implementation (fast, but not Rust-level)
   - More complex parsing for fractions, decimals, ordinals
   - CFFI overhead between Python and C

## Feature Comparison

### Supported Features Matrix

| Feature | text2num | OmNomNum | Advantage |
|---------|----------|----------|-----------|
| **Basic numbers** (0-999) | ✓ | ✓ | Tie |
| **Large numbers** (thousands, millions, billions) | ✓ | ✓ | Tie |
| **Compound numbers** (twenty-one) | ✓ | ✓ | Tie |
| **Decimals** (point notation) | ✗ | ✓ | **OmNomNum** |
| **Fractions** (1/2, 3/4) | ✗ | ✓ | **OmNomNum** |
| **Mixed fractions** (1 1/2) | ✗ | ✓ | **OmNomNum** |
| **Ordinals** (1st, 2nd, 3rd) | ✗ | ✓ | **OmNomNum** |
| **Negative numbers** | ✗ | ✓ | **OmNomNum** |
| **Text normalization** | ✗ | ✓ | **OmNomNum** |
| **Multi-language support** | ✓ (7 languages) | ✗ (English only) | **text2num** |
| **Rust performance** | ✓ | ✗ | **text2num** |
| **Configurable precision** | N/A | ✓ | **OmNomNum** |

**Feature Breadth Winner**: **OmNomNum** (more number formats)
**Raw Speed Winner**: **text2num** (2.7x faster)
**Language Support Winner**: **text2num** (7 languages vs 1)

## Compatibility Results

### Basic Numbers (Both Libraries) ✓

For basic cardinal numbers, **outputs are 100% compatible**:

| Input | text2num | OmNomNum | Status |
|-------|----------|----------|--------|
| "one" | `1` | `"1"` | ✓ Compatible |
| "twenty three" | `23` | `"23"` | ✓ Compatible |
| "one hundred" | `100` | `"100"` | ✓ Compatible |
| "one thousand" | `1000` | `"1000"` | ✓ Compatible |
| "one million" | `1000000` | `"1000000"` | ✓ Compatible |
| "one million one" | `1000001` | `"1000001"` | ✓ Compatible |

**Note**: text2num returns integers, OmNomNum returns strings. Convert with `int(normalize(text))` if needed.

### Decimals (OmNomNum Only)

text2num **does not support** decimal notation:

| Input | text2num | OmNomNum | Status |
|-------|----------|----------|--------|
| "one point five" | ❌ Error | `"1.5"` | OmNomNum only |
| "three point one four" | ❌ Error | `"3.14"` | OmNomNum only |
| "zero point five" | ❌ Error | `"0.5"` | OmNomNum only |

### Fractions (OmNomNum Only)

text2num **does not support** fractions:

| Input | text2num | OmNomNum | Status |
|-------|----------|----------|--------|
| "one half" | ❌ Error | `"0.5"` | OmNomNum only |
| "one quarter" | ❌ Error | `"0.25"` | OmNomNum only |
| "three quarters" | ❌ Error | `"0.75"` | OmNomNum only |
| "one and a half" | ❌ Error | `"1.5"` | OmNomNum only |

### Ordinals (OmNomNum Only)

text2num **does not support** ordinal numbers:

| Input | text2num | OmNomNum | Status |
|-------|----------|----------|--------|
| "first" | ❌ Error | `"1st"` | OmNomNum only |
| "second" | ❌ Error | `"2nd"` | OmNomNum only |
| "twenty first" | ❌ Error | `"21st"` | OmNomNum only |

### Negatives (OmNomNum Only)

text2num **does not support** negative numbers:

| Input | text2num | OmNomNum | Status |
|-------|----------|----------|--------|
| "negative five" | ❌ Error | `"-5"` | OmNomNum only |
| "minus ten" | ❌ Error | `"-10"` | OmNomNum only |

### Text Normalization (OmNomNum Only)

text2num **requires isolated number words**, cannot handle text:

| Input | text2num | OmNomNum | Status |
|-------|----------|----------|--------|
| "I have two cats" | ❌ Error | `"I have 2 cats"` | OmNomNum only |
| "She is twenty three" | ❌ Error | `"She is 23"` | OmNomNum only |
| "The first place winner" | ❌ Error | `"The 1st place winner"` | OmNomNum only |

## Implementation Comparison

### text2num (Allo-Media)

**Language**: Rust (v3.0+), Python (v2.x)
**Architecture**: Parser-based approach implemented in Rust
**Parser**: Rust's rule-based parser (details not public)
**Strengths**:
- ✓ Extremely fast (Rust performance)
- ✓ Multi-language support (Dutch, English, French, German, Italian, Portuguese, Spanish)
- ✓ Pure parser-based approach
- ✓ Low resource consumption

**Limitations**:
- ✗ Cardinal numbers only (no fractions, decimals, ordinals)
- ✗ Requires isolated number words (no text normalization)
- ✗ No configuration options
- ✗ Raises errors for unsupported inputs

### OmNomNum

**Language**: C
**Architecture**: re2c Scanner + Lemon LALR Parser
**Parser**: Formal grammar with generated C code
**Strengths**:
- ✓ Comprehensive number support (fractions, decimals, ordinals, negatives)
- ✓ Text normalization (preserves context)
- ✓ Configurable (precision, parsing options)
- ✓ Formal grammar (maintainable, extensible)

**Tradeoffs**:
- ✗ Slower than text2num (C vs Rust)
- ✗ English only (no multi-language support)
- ✗ More complex implementation

## Use Case Recommendations

### When to Use text2num

✓ **Speed is critical** (need absolute fastest performance)
✓ **Only need cardinal numbers** (0-999,999,999,999+)
✓ **Multi-language support** (need French, Spanish, German, etc.)
✓ **Processing clean number words** (no text context)
✓ **High throughput** (processing millions of numbers/second)

**Example use cases**:
- High-frequency trading systems
- Real-time voice transcription (numbers only)
- Multi-language ASR post-processing
- Batch processing of clean number data

### When to Use OmNomNum

✓ **Need fractions, decimals, or ordinals**
✓ **Normalizing numbers in full text** (not just isolated numbers)
✓ **Need configurability** (precision, parsing options)
✓ **Want comprehensive number handling** in one library
✓ **English-language processing**

**Example use cases**:
- Natural language processing pipelines
- Document normalization
- Recipe parsing (fractions)
- Address normalization (ordinals)
- Financial document processing

## Key Differences

### Return Types

- **text2num**: Returns Python `int` type
- **OmNomNum**: Returns Python `str` type

### Error Handling

- **text2num**: Raises `ValueError` for unsupported inputs
- **OmNomNum**: Returns input unchanged for unsupported inputs

Example:
```python
# text2num
try:
    result = text2num("I have two cats", "en")
except ValueError:
    print("Not supported")  # Raises error

# OmNomNum
result = normalize("unsupported text")  # Returns "unsupported text" unchanged
```

### Input Requirements

- **text2num**: **Isolated number words only** (strict)
- **OmNomNum**: **Any text** with embedded numbers (flexible)

## Migration Guide

### From text2num to OmNomNum

```python
# Before (text2num)
from text_to_num import text2num
number = text2num("twenty three", "en")  # Returns: 23 (int)

# After (OmNomNum)
from omnomnum import normalize
number = int(normalize("twenty three"))  # Returns: 23 (int)
# Or keep as string:
text = normalize("twenty three")  # Returns: "23" (str)
```

### From OmNomNum to text2num

Only possible if you **only use basic cardinal numbers**:

```python
# Before (OmNomNum)
from omnomnum import normalize
result = normalize("twenty three")  # Returns: "23" (str)

# After (text2num) - ONLY if no fractions, decimals, ordinals, etc.
from text_to_num import text2num
result = text2num("twenty three", "en")  # Returns: 23 (int)
```

**Warning**: text2num will fail on:
- Fractions: "one half"
- Decimals: "one point five"
- Ordinals: "first"
- Negatives: "negative five"
- Text: "I have two cats"

## Performance Tuning

### text2num (Simple)

```python
from text_to_num import text2num

# Only one way to use it
result = text2num(text, "en")  # ~0.2 µs/op
```

### OmNomNum (Optimize for Your Use Case)

```python
from omnomnum import normalize, OmNomNum

# Slow: Creates new instance each time
for text in texts:
    result = normalize(text)  # ~1.8 µs/op

# Faster: Reuse instance
nom = OmNomNum()
for text in texts:
    result = nom.normalize(text)  # ~0.6 µs/op (still 3x slower than text2num)
```

## Compatibility Score

### Overlapping Features: 100% Compatible ✓

For basic cardinal numbers, **outputs are identical** (accounting for int vs string).

### Feature Coverage

| Category | text2num | OmNomNum |
|----------|----------|----------|
| Basic numbers (0-9) | ✓ | ✓ |
| Teens (10-19) | ✓ | ✓ |
| Tens (20, 30, ..., 90) | ✓ | ✓ |
| Hundreds | ✓ | ✓ |
| Thousands | ✓ | ✓ |
| Millions | ✓ | ✓ |
| Billions | ✓ | ✓ |
| **Languages** | **7** | 1 |
| Decimals | ✗ | ✓ |
| Fractions | ✗ | ✓ |
| Ordinals | ✗ | ✓ |
| Negatives | ✗ | ✓ |
| Text normalization | ✗ | ✓ |
| Configurability | ✗ | ✓ |

**Coverage**: text2num supports ~40% of OmNomNum's number format features, but adds multi-language support

## Conclusion

### Performance Winner: **text2num** 🏆
- 2.7x faster than OmNomNum
- Rust implementation provides significant speed advantage
- Best for high-throughput scenarios

### Feature Winner: **OmNomNum** 🏆
- Comprehensive number format support
- Text normalization capabilities
- Configurable parsing options

### Multi-Language Winner: **text2num** 🏆
- Supports 7 languages (Dutch, English, French, German, Italian, Portuguese, Spanish)
- OmNomNum is English-only

### Overall Recommendation: **Depends on Use Case**

**Choose text2num if**:
- Speed is your top priority (2.7x faster)
- You only need cardinal numbers
- You need multi-language support
- You're processing clean, isolated number words

**Choose OmNomNum if**:
- You need fractions, decimals, ordinals, or negatives
- You're normalizing numbers in real text (not isolated)
- You need configuration options
- You prefer comprehensive features over raw speed
- You're only working with English

### Best of Both Worlds?

For maximum performance with OmNomNum features, consider:
1. Using text2num for simple cardinal numbers (fast path)
2. Using OmNomNum for complex cases (fractions, ordinals, text)

```python
from text_to_num import text2num
from omnomnum import normalize

def smart_normalize(text):
    """Use fast text2num when possible, OmNomNum for complex cases"""
    # Try fast path first
    try:
        return str(text2num(text, "en"))
    except ValueError:
        # Fall back to comprehensive OmNomNum
        return normalize(text)
```

## Detailed Benchmark Data

### Test Configuration
- **Platform**: macOS ARM64 (Apple Silicon)
- **Python**: 3.10
- **text2num**: 3.0.0 (Rust implementation)
- **OmNomNum**: Latest (C implementation with CFFI)
- **Test**: Converting "twenty three"
- **Iterations**: 10,000
- **Method**: `time.perf_counter()` for high-precision timing

### Raw Results
```
text2num:             2.40 ms (0.2 µs/op) = 4,172,679 ops/sec
OmNomNum (class):     6.48 ms (0.6 µs/op) = 1,543,597 ops/sec
OmNomNum (function): 17.69 ms (1.8 µs/op) =   565,331 ops/sec
```

### Throughput Comparison
- text2num: **~4.2M normalizations/second** (fastest)
- OmNomNum (class): ~1.5M normalizations/second
- OmNomNum (function): ~565K normalizations/second

Both are fast enough for virtually all use cases, but text2num has a significant edge for high-throughput scenarios.

## Summary Table

| Aspect | text2num | OmNomNum | Winner |
|--------|----------|----------|--------|
| **Speed** | 0.2 µs/op | 0.6 µs/op | text2num (2.7x) |
| **Cardinal numbers** | ✓ | ✓ | Tie |
| **Fractions** | ✗ | ✓ | OmNomNum |
| **Decimals** | ✗ | ✓ | OmNomNum |
| **Ordinals** | ✗ | ✓ | OmNomNum |
| **Negatives** | ✗ | ✓ | OmNomNum |
| **Text normalization** | ✗ | ✓ | OmNomNum |
| **Languages** | 7 | 1 | text2num |
| **Return type** | int | str | Depends |
| **Error handling** | Raises | Returns unchanged | Depends |
| **Implementation** | Rust | C | text2num (speed) |

**Bottom Line**: text2num is faster and supports more languages, but OmNomNum supports more number formats. Choose based on your specific needs.
