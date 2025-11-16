# Implementation Comparison: OmNomNum vs word2number

## How word2number Works

After examining the source code (`/opt/homebrew/lib/python3.10/site-packages/word2number/w2n.py`), here's how word2number converts text to numbers:

### Architecture: **Dictionary Lookup + Manual Parsing**

```python
# 1. Dictionary mapping words to values
american_number_system = {
    'zero': 0, 'one': 1, 'two': 2, ...,
    'hundred': 100, 'thousand': 1000, 'million': 1000000,
    'billion': 1000000000, 'point': '.'
}

# 2. Process input
def word_to_num(number_sentence):
    # Convert to lowercase, split into words
    split_words = number_sentence.strip().split()

    # Filter to only valid number words
    clean_numbers = [word for word in split_words
                     if word in american_number_system]

    # Find positions of scale words (billion, million, thousand)
    billion_index = clean_numbers.index('billion') if 'billion' in clean_numbers else -1
    million_index = clean_numbers.index('million') if 'million' in clean_numbers else -1
    thousand_index = clean_numbers.index('thousand') if 'thousand' in clean_numbers else -1

    # Process sections between scale words
    # Uses number_formation() to combine numbers in each section
    total = process_billions + process_millions + process_thousands + process_hundreds

    return total
```

### Key Implementation Details

1. **No Parser**: Just string manipulation and list indexing
2. **Filtering Approach**: Removes all non-number words, processes only number words
3. **Section-based Processing**: Finds "billion", "million", "thousand" indices, processes sections between them
4. **Simple Arithmetic**: Multiplies and adds based on position
5. **Error Handling**: Raises `ValueError` for invalid input

### Strengths
- ✓ Simple implementation (~217 lines)
- ✓ Pure Python (no dependencies)
- ✓ Easy to understand
- ✓ Works for basic numbers

### Limitations
- ✗ Can't handle text with numbers ("I have two cats" → error)
- ✗ No fractions
- ✗ No ordinals
- ✗ No negatives
- ✗ Limited decimal support (just "point")
- ✗ Error-prone (raises exceptions)
- ✗ Can't preserve whitespace or context

## How OmNomNum Works

### Architecture: **Formal Grammar + Scanner + Parser**

```
Text → re2c Scanner → Tokens → Lemon Parser → Normalized Output
```

1. **Scanner (re2c)**: Lexical analysis
   - Pattern matching for number words, fractions, decimals, etc.
   - Generates optimized C code for token recognition
   - Handles complex patterns (fractions, ordinals, etc.)

2. **Parser (Lemon)**: Syntactic analysis
   - Formal grammar rules for number composition
   - LALR parser generator (like yacc/bison)
   - Generates efficient C parser code

3. **Processing**:
   - Scans input character by character
   - Recognizes tokens (numbers, fractions, operators)
   - Parser applies grammar rules
   - Outputs normalized text with numbers converted

### Key Implementation Details

1. **Formal Parser**: Grammar-based, handles complex nested structures
2. **Preserves Context**: Keeps non-number text intact
3. **Extensible**: Easy to add new patterns via grammar rules
4. **C Performance**: Compiled C code, highly optimized
5. **Flexible**: Configurable parsing options

### Strengths
- ✓ Handles complex inputs (text with embedded numbers)
- ✓ Supports fractions, ordinals, decimals, negatives
- ✓ Preserves whitespace and context
- ✓ Formal grammar (easier to maintain and extend)
- ✓ Fast (C implementation)
- ✓ Configurable

### Tradeoffs
- ✗ More complex implementation
- ✗ Requires C compiler
- ✗ Larger codebase

## Side-by-Side Comparison

### Example: "twenty three thousand"

**word2number approach:**
```python
# 1. Split: ["twenty", "three", "thousand"]
# 2. Filter (all are valid): ["twenty", "three", "thousand"]
# 3. Find thousand_index: 2
# 4. Process [0:2]: number_formation(["twenty", "three"])
#    = 20 + 3 = 23
# 5. Multiply: 23 * 1000 = 23000
# 6. Return: 23000
```

**OmNomNum approach:**
```
# 1. Scan: TOKEN_TWENTY, TOKEN_THREE, TOKEN_THOUSAND
# 2. Parse via grammar rules:
#    number → compound_number scale_word
#    compound_number → tens_word digit_word
# 3. Semantic actions calculate: (20 + 3) * 1000 = 23000
# 4. Output: "23000"
```

### Example: "I have two cats" (word2number fails)

**word2number:**
```python
# 1. Split: ["I", "have", "two", "cats"]
# 2. Filter: ["two"]  (only valid number word)
# 3. No scale words → return 2
# 4. Caller gets just "2", loses context
```

**OmNomNum:**
```
# 1. Scan: TEXT("I have "), TOKEN_TWO, TEXT(" cats")
# 2. Parser recognizes number, normalizes to "2"
# 3. Output: "I have 2 cats" (context preserved!)
```

## Algorithmic Complexity

### word2number
- **Time**: O(n) where n = number of words
  - Single pass to split
  - Single pass to filter
  - Fixed number of index lookups
  - Linear section processing
- **Space**: O(n) for word lists
- **Scalability**: Good for simple inputs

### OmNomNum
- **Time**: O(n) where n = input length
  - Scanner: O(n) single pass
  - Parser: O(n) LALR parsing
- **Space**: O(n) for token stream and parse tree
- **Scalability**: Excellent, handles complex nested structures

Both are O(n), but OmNomNum handles more complex cases.

## Why OmNomNum Uses a Parser

### Advantages of Parser-Based Approach

1. **Formal Grammar**
   - Clearly defined rules for number composition
   - Easy to understand and maintain
   - Self-documenting

2. **Handles Complexity**
   - Nested structures (fractions within mixed numbers)
   - Context-sensitive parsing
   - Ambiguous inputs

3. **Extensibility**
   - Add new rules without rewriting logic
   - Modular grammar additions
   - Easy to support new number formats

4. **Reliability**
   - Proven parser generator (Lemon/LALR)
   - Well-tested algorithms
   - Predictable behavior

5. **Performance**
   - Generated C code is highly optimized
   - Scanner uses efficient state machines
   - No interpreted overhead

### Why word2number Doesn't Need a Parser

For their simpler use case:
- Only handling isolated number words
- Fixed pattern matching (scale words at fixed positions)
- No context preservation needed
- Simple arithmetic rules

A parser would be overkill for just "convert word list to number".

## Code Size Comparison

### word2number
- **Total**: ~217 lines of Python
- **Core logic**: ~130 lines
- **Dependencies**: None

### OmNomNum
- **Scanner (scanner.re)**: ~700 lines (grammar)
- **Parser (parser.yy)**: ~300 lines (grammar)
- **Generated C**: ~10,000+ lines (scanner.c + parser.c)
- **Core library**: ~500 lines (omnomnum.c)
- **Python wrapper**: ~300 lines
- **Dependencies**: re2c, lemon (build-time)

OmNomNum is significantly larger, but provides much more functionality.

## Performance Analysis

### word2number Bottlenecks
1. **String splitting**: Python string operations
2. **List filtering**: Python list comprehensions
3. **Dictionary lookups**: O(1) but Python dict overhead
4. **Number formation**: Python arithmetic

### OmNomNum Bottlenecks
1. **CFFI overhead**: Python↔C boundary (minimal)
2. **State initialization**: Creating ParserState
3. **String allocation**: sds buffer management
4. **Token generation**: Scanner state machine (very fast)

Despite more complex architecture, OmNomNum is faster due to C implementation.

## Testing Approach Differences

### word2number
- Expects exact number word sequences
- Raises exceptions for invalid input
- Tests must provide clean number strings

### OmNomNum
- Handles arbitrary text
- Preserves context
- More flexible testing (can test with real sentences)

## Conclusion

### word2number: Simple Procedural Approach
- Best for: Converting isolated number words to integers
- Limitations: Can't handle real-world text with embedded numbers

### OmNomNum: Compiler-Based Approach
- Best for: Normalizing numbers in real text
- Features: Comprehensive number handling + context preservation
- Approach: Formal grammar + optimized generated code

The parser-based approach gives OmNomNum:
- More features (fractions, ordinals, text normalization)
- Better extensibility (add features via grammar)
- Higher reliability (proven parser algorithms)
- Better performance (optimized C code)

At the cost of:
- More complex implementation
- Build-time dependencies
- Larger codebase

**For most use cases, OmNomNum's parser-based approach is superior** because it handles real-world text, not just isolated number words.
