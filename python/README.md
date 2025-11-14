# OmNomNum Python Wrapper

Python bindings for [OmNomNum](https://github.com/JesseBuesking/omnomnum), a fast C library for normalizing numbers in text.

## Installation

### From source

```bash
cd python
pip install .
```

### For development

```bash
cd python
pip install -e ".[dev]"
```

## Quick Start

```python
from omnomnum import normalize, find_numbers

# Basic usage
text = "I have two hundred fifty dollars"
result = normalize(text)
print(result)  # "I have 250 dollars"

# With fractions
text = "one and a half cups"
result = normalize(text)
print(result)  # "1.5 cups"

# Extract numbers
text = "There are two cats and three dogs"
numbers = find_numbers(text)
for num in numbers:
    print(f"Found {num.value} at position {num.begin}:{num.end}")
# Found 2.0 at position 10:13
# Found 3.0 at position 23:28
```

## Features

- **Fast**: Written in C with minimal Python overhead
- **Comprehensive**: Handles integers, decimals, fractions, ordinals, and more
- **Flexible**: Configurable parsing options
- **Type-safe**: Fully typed Python API

## Usage

### Class-based API

```python
from omnomnum import OmNomNum

# Create a normalizer with custom options
nom = OmNomNum(
    precision=2,              # Decimal places
    parse_fractions=True,     # Parse fractions
    reduce_fractions=False,   # Don't reduce fractions
)

# Normalize text
result = nom.normalize("one and three quarters")
print(result)  # "1.75"

# Find numbers
numbers = nom.find_numbers("two cats and three dogs")
for num in numbers:
    print(num.value)  # 2.0, 3.0
```

### Function-based API

```python
from omnomnum import normalize, find_numbers

# Simple normalization
normalize("twenty three")  # "23"

# With options
normalize("50 percent", percent_as_decimal=True)  # "0.5"
normalize("one half", parse_fractions=False)  # "one half"

# Find numbers
numbers = find_numbers("I bought two apples and three oranges")
values = [n.value for n in numbers]  # [2.0, 3.0]
```

## Configuration Options

- **precision** (int): Number of decimal places (default: 6)
- **parse_second** (bool): Parse "second" as ordinal "2nd" (default: False)
- **parse_fractions** (bool): Enable fraction parsing (default: True)
- **reduce_fractions** (bool): Reduce fractions to lowest terms (default: False)
- **normalize_percent_symbol** (bool): Convert "percent" to "%" (default: False)
- **percent_as_decimal** (bool): Convert "n percent" to n/100 (default: False)

## Examples

### Numbers

```python
normalize("one")                    # "1"
normalize("twenty three")           # "23"
normalize("two hundred")            # "200"
normalize("one thousand")           # "1000"
normalize("negative five")          # "-5"
```

### Fractions

```python
normalize("one half")               # "0.5"
normalize("three quarters")         # "0.75"
normalize("one and a half")         # "1.5"
normalize("two and three quarters") # "2.75"
```

### Ordinals

```python
normalize("first")                  # "1st"
normalize("second")                 # "2nd"
normalize("twenty third")           # "23rd"
```

### Decimals

```python
normalize("one point five")         # "1.5"
normalize("three point one four")   # "3.14"
```

### Percentages

```python
normalize("fifty percent", normalize_percent_symbol=True)  # "50%"
normalize("50 percent", percent_as_decimal=True)           # "0.5"
```

### Disable Fraction Parsing

```python
nom = OmNomNum(parse_fractions=False)
nom.normalize("one half")           # "one half" (unchanged)
```

## API Reference

### OmNomNum Class

```python
class OmNomNum:
    def __init__(
        self,
        precision: int = 6,
        parse_second: bool = False,
        parse_fractions: bool = True,
        reduce_fractions: bool = False,
        normalize_percent_symbol: bool = False,
        percent_as_decimal: bool = False,
    )

    def normalize(self, text: str) -> str:
        """Normalize numbers in text."""

    def find_numbers(self, text: str) -> List[Number]:
        """Extract all numbers from text."""
```

### Number Class

```python
class Number:
    """Represents a parsed number."""

    value: Optional[float]              # Numeric value (if is_double)
    is_double: bool                     # Is a regular number
    is_fraction: bool                   # Is a fraction
    fraction_numerator: Optional[float] # Fraction numerator (if is_fraction)
    fraction_denominator: Optional[float] # Fraction denominator (if is_fraction)
    begin: int                          # Start position in text
    end: int                            # End position in text
```

### Convenience Functions

```python
def normalize(text: str, **options) -> str:
    """Normalize numbers in text (one-off usage)."""

def find_numbers(text: str, **options) -> List[Number]:
    """Find all numbers in text (one-off usage)."""
```

## Development

### Running Tests

```bash
pytest tests/
```

### Type Checking

```bash
mypy omnomnum/
```

### Code Formatting

```bash
black omnomnum/ tests/
```

## Requirements

- Python 3.7+
- cffi >= 1.15.0
- C compiler (for building from source)

## License

BSD 3-Clause License - see LICENSE file for details.

## Contributing

Contributions welcome! Please open an issue or submit a pull request on GitHub.

## Comparison with Other Libraries

See [COMPARISON.md](COMPARISON.md) for a detailed comparison with word2number:
- **1.6x faster** than word2number (with reused instance)
- **More features**: fractions, decimals, ordinals, text normalization
- **Drop-in replacement** for basic number conversion

## Credits

- C library: Jesse Buesking
- Python wrapper: Jesse Buesking
