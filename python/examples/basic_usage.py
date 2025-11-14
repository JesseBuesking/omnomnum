#!/usr/bin/env python3
"""
Basic usage examples for OmNomNum Python wrapper.
"""

from omnomnum import normalize, find_numbers, OmNomNum


def basic_examples():
    """Simple normalization examples."""
    print("=== Basic Normalization ===\n")

    examples = [
        "I have two hundred dollars",
        "She is twenty three years old",
        "one and a half cups of sugar",
        "The first place winner",
        "negative five degrees",
        "three point one four one five nine",
    ]

    for text in examples:
        result = normalize(text)
        print(f"Input:  {text}")
        print(f"Output: {result}\n")


def fraction_examples():
    """Fraction parsing examples."""
    print("\n=== Fraction Examples ===\n")

    examples = [
        "one half",
        "three quarters",
        "one and three quarters",
        "two and a half",
        "a quarter cup",
    ]

    for text in examples:
        result = normalize(text)
        print(f"{text:30} -> {result}")


def find_numbers_example():
    """Extract numbers from text."""
    print("\n=== Finding Numbers ===\n")

    text = "I bought two apples, three oranges, and one banana for five dollars"
    print(f"Text: {text}\n")

    numbers = find_numbers(text)
    print(f"Found {len(numbers)} numbers:\n")

    for num in numbers:
        word = text[num.begin : num.end]
        print(f"  '{word}' -> {num.value} (position {num.begin}:{num.end})")


def custom_options():
    """Using custom options."""
    print("\n=== Custom Options ===\n")

    # Disable fraction parsing
    nom = OmNomNum(parse_fractions=False)
    text = "one half"
    print(f"With parse_fractions=False:")
    print(f"  '{text}' -> '{nom.normalize(text)}'")

    # Enable fraction reduction
    nom = OmNomNum(reduce_fractions=True)
    text = "two quarters"  # Should reduce to 1/2
    print(f"\nWith reduce_fractions=True:")
    print(f"  '{text}' -> '{nom.normalize(text)}'")

    # Percent as decimal
    text = "fifty percent"
    print(f"\nPercent normalization:")
    print(f"  Default: '{normalize(text)}'")
    print(
        f"  As symbol: '{normalize(text, normalize_percent_symbol=True)}'"
    )
    print(f"  As decimal: '{normalize(text, percent_as_decimal=True)}'")

    # Custom precision
    text = "one third"
    print(f"\nPrecision control:")
    for prec in [2, 4, 6]:
        result = normalize(text, precision=prec)
        print(f"  precision={prec}: '{result}'")


def mixed_content():
    """Normalize text with mixed numbers and words."""
    print("\n=== Mixed Content ===\n")

    texts = [
        "The recipe calls for two and a half cups of flour and three eggs",
        "She finished in second place with a time of one hundred twenty three seconds",
        "I need twenty three dollars and fifty cents",
        "The temperature dropped to negative fifteen degrees",
    ]

    for text in texts:
        result = normalize(text)
        print(f"Input:  {text}")
        print(f"Output: {result}\n")


if __name__ == "__main__":
    basic_examples()
    fraction_examples()
    find_numbers_example()
    custom_options()
    mixed_content()
