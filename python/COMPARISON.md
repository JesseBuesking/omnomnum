# OmNomNum vs word2number Comparison

## Executive Summary

**OmNomNum is 1.6x FASTER than word2number** (when using reused instance) and supports **significantly more features** including fractions, decimals, ordinals, and text normalization.

## Performance Comparison

### Benchmark Results

**Test**: Converting "twenty three" to numeric form
**Iterations**: 10,000
**Platform**: macOS ARM64 (Apple Silicon)

| Library | Time (ms) | Time per op (µs) | Speed vs word2number |
|---------|-----------|-----------------|---------------------|
| **word2number** | 11.28 | 1.1 | 1.0x (baseline) |
| **OmNomNum (class)** | 6.88 | 0.7 | **1.6x faster** ⚡ |
| **OmNomNum (function)** | 18.56 | 1.9 | 0.6x slower |

### Performance Analysis

1. **OmNomNum (reused instance) wins**: 1.6x faster than word2number
   - C implementation provides speed advantage
   - ~1.45 million ops/second vs ~887K ops/second

2. **OmNomNum (function API) is slower**: Due to instance creation overhead
   - Creates new `OmNomNum()` each call
   - Still acceptable for non-critical paths (~540K ops/second)

3. **Recommendation**: Use class API for best performance
   ```python
   # Fast: Reuse instance
   nom = OmNomNum()
   for text in texts:
       result = nom.normalize(text)  # 1.6x faster than word2number
   ```

## Feature Comparison

### Supported Features Matrix

| Feature | word2number | OmNomNum | Advantage |
|---------|-------------|----------|-----------|
| **Basic numbers** (0-999) | ✓ | ✓ | Tie |
| **Large numbers** (thousands, millions, billions, trillions) | ✓ | ✓ | Tie |
| **Compound numbers** (twenty-one) | ✓ | ✓ | Tie |
| **Fractions** (1/2, 3/4) | ✗ | ✓ | **OmNomNum** |
| **Mixed fractions** (1 1/2) | ✗ | ✓ | **OmNomNum** |
| **Decimal notation** (point) | ✗ | ✓ | **OmNomNum** |
| **Negative numbers** | ✗ | ✓ | **OmNomNum** |
| **Ordinals** (1st, 2nd, 3rd) | ✗ | ✓ | **OmNomNum** |
| **Text normalization** | ✗ | ✓ | **OmNomNum** |
| **Whitespace preservation** | N/A | ✓ | **OmNomNum** |
| **Configurable precision** | N/A | ✓ | **OmNomNum** |
| **Configurable parsing** | N/A | ✓ | **OmNomNum** |
| **Pure Python** | ✓ | ✗ | word2number |
| **C-based performance** | ✗ | ✓ | **OmNomNum** |

**Winner**: **OmNomNum** - More features + Better performance

### Feature Details

#### What word2number Supports
- ✓ Basic number words (one, two, three...)
- ✓ Compound numbers (twenty-one, thirty-two...)
- ✓ Large scale numbers (thousand, million, billion...)
- ✓ Pure Python implementation (no C dependencies)

#### What OmNomNum Adds
- ✓ **Everything word2number does**, PLUS:
- ✓ **Fractions**: "one half" → "0.5", "three quarters" → "0.75"
- ✓ **Mixed fractions**: "one and a half" → "1.5"
- ✓ **Decimals**: "three point one four" → "3.14"
- ✓ **Negatives**: "negative five" → "-5", "minus ten" → "-10"
- ✓ **Ordinals**: "first" → "1st", "twenty third" → "23rd"
- ✓ **Text normalization**: "I have two cats" → "I have 2 cats"
- ✓ **Configurable**: precision, fraction parsing, percent handling
- ✓ **Faster**: 1.6x performance advantage

## Example Comparisons

### Basic Numbers (Both Support)

```python
# word2number
from word2number import w2n
w2n.word_to_num("twenty three")  # 23

# OmNomNum
from omnomnum import normalize
normalize("twenty three")  # "23"
```

### Fractions (OmNomNum Only)

```python
# word2number
w2n.word_to_num("one half")  # ValueError: No valid number words found

# OmNomNum
normalize("one half")  # "0.5"
normalize("three quarters")  # "0.75"
normalize("one and a half")  # "1.5"
```

### Text Normalization (OmNomNum Only)

```python
# word2number - only handles isolated numbers
w2n.word_to_num("I have two cats")  # ValueError

# OmNomNum - normalizes numbers in context
normalize("I have two cats")  # "I have 2 cats"
normalize("She is twenty three years old")  # "She is 23 years old"
```

### Decimals (OmNomNum Only)

```python
# word2number
w2n.word_to_num("three point one four")  # ValueError

# OmNomNum
normalize("three point one four")  # "3.14"
normalize("one point five")  # "1.5"
```

### Ordinals (OmNomNum Only)

```python
# word2number
w2n.word_to_num("first")  # ValueError

# OmNomNum
normalize("first")  # "1st"
normalize("twenty third")  # "23rd"
```

## Use Case Recommendations

### When to Use word2number
- ✓ Need pure Python (no C dependencies)
- ✓ Only converting basic number words (no fractions, decimals, etc.)
- ✓ Don't need high performance
- ✓ Simpler installation (fewer dependencies)

### When to Use OmNomNum
- ✓ Need fractions, decimals, or ordinals
- ✓ Normalizing numbers in full text (not just isolated numbers)
- ✓ Performance matters (processing many texts)
- ✓ Need configurability (precision, parsing options)
- ✓ Want comprehensive number handling in one library

## Migration from word2number

### Drop-in Replacement (for basic numbers)

```python
# Before (word2number)
from word2number import w2n
result = w2n.word_to_num("twenty three")  # Returns: 23

# After (OmNomNum)
from omnomnum import normalize
result = normalize("twenty three")  # Returns: "23" (string)

# Convert to int if needed
result = int(normalize("twenty three"))  # Returns: 23
```

### Key Differences

| Aspect | word2number | OmNomNum |
|--------|-------------|----------|
| **Return type** | `int` | `str` |
| **Input** | Isolated number words only | Any text with numbers |
| **Error handling** | Raises `ValueError` | Returns text unchanged |
| **Whitespace** | Not preserved | Preserved |

### Migration Example

```python
# word2number code
from word2number import w2n

try:
    number = w2n.word_to_num("twenty three")
    print(f"Got: {number}")
except ValueError:
    print("Not a number")

# Equivalent OmNomNum code
from omnomnum import normalize, find_numbers

text = "twenty three"
result = normalize(text)

if result != text:  # Changed means it was a number
    number = int(result)
    print(f"Got: {number}")
else:
    print("Not a number")

# Or use find_numbers()
numbers = find_numbers(text)
if numbers:
    print(f"Got: {numbers[0].value}")
else:
    print("Not a number")
```

## Performance Tuning

### word2number (Limited Options)
```python
# Only one way to use it
from word2number import w2n
result = w2n.word_to_num(text)  # ~1.1 µs/op
```

### OmNomNum (Optimize for Your Use Case)
```python
from omnomnum import normalize, OmNomNum

# Slow: Creates new instance each time
for text in texts:
    result = normalize(text)  # ~1.9 µs/op

# Fast: Reuse instance (1.6x faster than word2number!)
nom = OmNomNum()
for text in texts:
    result = nom.normalize(text)  # ~0.7 µs/op
```

## Conclusion

### Performance Winner: **OmNomNum** 🏆
- 1.6x faster when using reused instance
- 0.6x slower with function API (but more convenient)

### Feature Winner: **OmNomNum** 🏆
- Supports everything word2number does
- Plus fractions, decimals, negatives, ordinals
- Plus text normalization
- Plus configuration options

### Overall Recommendation: **OmNomNum**

Unless you specifically need pure Python with no C dependencies, **OmNomNum is the better choice**:
- Faster performance
- More features
- Better suited for real-world text processing
- Active development with recent optimizations

word2number is a good library for simple use cases, but OmNomNum provides a more complete solution with better performance.

## Detailed Benchmark Data

### Test Configuration
- **Platform**: macOS ARM64 (Apple Silicon M1/M2)
- **Python**: 3.10
- **Compiler**: clang with -O3 optimization
- **Test**: Converting "twenty three"
- **Iterations**: 10,000 per library
- **Method**: `time.perf_counter()` for high-precision timing

### Raw Results
```
word2number:          11.28 ms (1.1 µs/op) = 887,000 ops/sec
OmNomNum (class):      6.88 ms (0.7 µs/op) = 1,453,000 ops/sec
OmNomNum (function):  18.56 ms (1.9 µs/op) = 539,000 ops/sec
```

### Throughput Comparison
- word2number: ~887K normalizations/second
- OmNomNum: **~1.45M normalizations/second** (63% faster)

Both are fast enough for virtually all use cases, but OmNomNum has the edge when processing large volumes of text.
