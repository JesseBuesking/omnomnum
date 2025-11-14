# OmNomNum Python Wrapper - Quick Start Guide

## Installation

### Requirements

- Python 3.7 or higher
- C compiler (gcc, clang, or MSVC)
- pip

### Install from source

```bash
cd python
pip install .
```

### Development installation

```bash
cd python
pip install -e ".[dev]"
```

## 5-Minute Tutorial

### 1. Import the library

```python
from omnomnum import normalize, find_numbers
```

### 2. Normalize text with numbers

```python
# Convert written numbers to digits
text = "I have two hundred fifty dollars"
result = normalize(text)
print(result)
# Output: "I have 250 dollars"
```

### 3. Handle fractions

```python
# Fractions are converted to decimals by default
text = "Add one and a half cups of flour"
result = normalize(text)
print(result)
# Output: "Add 1.5 cups of flour"
```

### 4. Extract numbers from text

```python
# Get all numbers and their positions
text = "There are two cats and three dogs"
numbers = find_numbers(text)

for num in numbers:
    word = text[num.begin:num.end]
    print(f"Found '{word}' = {num.value}")
# Output:
# Found 'two' = 2.0
# Found 'three' = 3.0
```

### 5. Use custom options

```python
from omnomnum import OmNomNum

# Create normalizer with custom settings
nom = OmNomNum(
    precision=2,              # 2 decimal places
    parse_fractions=True,     # Parse fractions (default)
    percent_as_decimal=True   # Convert "50%" to "0.5"
)

# Use it multiple times
print(nom.normalize("one third"))          # "0.33"
print(nom.normalize("fifty percent"))      # "0.5"
print(nom.normalize("two and a quarter"))  # "2.25"
```

## Common Use Cases

### Natural Language Processing

```python
from omnomnum import normalize

# Normalize text before processing
text = "The temperature rose from twenty to thirty degrees"
normalized = normalize(text)
# "The temperature rose from 20 to 30 degrees"
```

### Data Extraction

```python
from omnomnum import find_numbers

# Extract structured data
recipe = "Mix two eggs with three cups of flour"
numbers = find_numbers(recipe)

ingredients = []
for num in numbers:
    word = recipe[num.begin:num.end]
    ingredients.append({
        'quantity': num.value,
        'text': word,
        'position': (num.begin, num.end)
    })
```

### Text Normalization Pipeline

```python
from omnomnum import OmNomNum

# Create reusable normalizer
normalizer = OmNomNum(precision=2)

# Process multiple documents
documents = [
    "Revenue increased by twenty five percent",
    "We need three hundred fifty units",
    "The project took one and a half years"
]

normalized_docs = [normalizer.normalize(doc) for doc in documents]
```

### Configuration Examples

```python
from omnomnum import normalize

# Don't parse fractions (keep as text)
normalize("one half", parse_fractions=False)
# Output: "one half"

# Convert percentages to decimals
normalize("fifty percent", percent_as_decimal=True)
# Output: "0.5"

# Normalize percent symbol
normalize("fifty percent", normalize_percent_symbol=True)
# Output: "50%"

# Control decimal precision
normalize("one third", precision=4)
# Output: "0.3333"
```

## Next Steps

- Read the full [README](README.md) for detailed API documentation
- Check out [examples/basic_usage.py](examples/basic_usage.py) for more examples
- Read [CONTRIBUTING.md](CONTRIBUTING.md) if you want to contribute

## Getting Help

- **Issues**: https://github.com/JesseBuesking/omnomnum/issues
- **Discussions**: https://github.com/JesseBuesking/omnomnum/discussions

## Testing Your Installation

Run this to verify everything works:

```python
from omnomnum import normalize

test_cases = [
    ("one", "1"),
    ("twenty three", "23"),
    ("one hundred", "100"),
    ("one half", "0.5"),
]

for input_text, expected in test_cases:
    result = normalize(input_text)
    status = "✓" if result == expected else "✗"
    print(f"{status} '{input_text}' -> '{result}' (expected '{expected}')")
```

If all tests pass with ✓, your installation is working correctly!
