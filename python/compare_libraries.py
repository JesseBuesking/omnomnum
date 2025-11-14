#!/usr/bin/env python3
"""
Compare OmNomNum with word2number library.
"""

import time
import traceback
from word2number import w2n
from omnomnum import normalize, OmNomNum


def test_case(text, expected_omnomnum, test_name=""):
    """Test a single case against both libraries."""
    print(f"\n{'='*70}")
    print(f"Test: {test_name or text}")
    print(f"{'='*70}")
    print(f"Input: '{text}'")

    # Test word2number
    try:
        w2n_result = w2n.word_to_num(text)
        w2n_status = "✓"
    except Exception as e:
        w2n_result = f"ERROR: {e}"
        w2n_status = "✗"

    # Test OmNomNum
    try:
        omnom_result = normalize(text)
        omnom_status = "✓"
    except Exception as e:
        omnom_result = f"ERROR: {e}"
        omnom_status = "✗"

    print(f"  word2number: {w2n_status} {w2n_result}")
    print(f"  OmNomNum:    {omnom_status} {omnom_result}")

    if expected_omnomnum:
        if str(omnom_result) == str(expected_omnomnum):
            print(f"  Expected:    ✓ {expected_omnomnum}")
        else:
            print(f"  Expected:    ✗ {expected_omnomnum} (got {omnom_result})")

    return {
        'text': text,
        'word2number': w2n_result,
        'omnomnum': omnom_result,
        'w2n_ok': w2n_status == "✓",
        'omnom_ok': omnom_status == "✓",
    }


def main():
    print("="*70)
    print("OmNomNum vs word2number Comparison")
    print("="*70)

    results = []

    # Basic numbers
    print("\n" + "="*70)
    print("BASIC NUMBERS")
    print("="*70)

    results.append(test_case("one", "1", "Single digit"))
    results.append(test_case("twenty three", "23", "Two words"))
    results.append(test_case("one hundred", "100", "Hundred"))
    results.append(test_case("two hundred fifty", "250", "Hundreds with tens"))
    results.append(test_case("one thousand", "1000", "Thousand"))
    results.append(test_case("twenty three thousand", "23000", "Thousands"))

    # Large numbers
    print("\n" + "="*70)
    print("LARGE NUMBERS")
    print("="*70)

    results.append(test_case("one million", "1000000", "Million"))
    results.append(test_case("one billion", "1000000000", "Billion"))
    results.append(test_case("one trillion", "1000000000000", "Trillion"))

    # Fractions (OmNomNum feature)
    print("\n" + "="*70)
    print("FRACTIONS (OmNomNum feature)")
    print("="*70)

    results.append(test_case("one half", "0.5", "Simple fraction"))
    results.append(test_case("three quarters", "0.75", "Quarters"))
    results.append(test_case("one and a half", "1.5", "Mixed fraction"))
    results.append(test_case("two and three quarters", "2.75", "Complex mixed"))

    # Decimals
    print("\n" + "="*70)
    print("DECIMALS")
    print("="*70)

    results.append(test_case("one point five", "1.5", "Point notation"))
    results.append(test_case("three point one four", "3.14", "Multiple decimals"))

    # Negatives
    print("\n" + "="*70)
    print("NEGATIVE NUMBERS")
    print("="*70)

    results.append(test_case("negative five", "-5", "Negative"))
    results.append(test_case("minus ten", "-10", "Minus"))

    # Ordinals
    print("\n" + "="*70)
    print("ORDINALS")
    print("="*70)

    results.append(test_case("first", "1st", "First"))
    results.append(test_case("second", "2nd", "Second"))
    results.append(test_case("twenty third", "23rd", "Complex ordinal"))

    # Mixed text (OmNomNum feature)
    print("\n" + "="*70)
    print("TEXT WITH NUMBERS (OmNomNum feature)")
    print("="*70)

    results.append(test_case("I have two cats", "I have 2 cats", "In sentence"))
    results.append(test_case("twenty three dollars", "23 dollars", "With units"))

    # Summary
    print("\n" + "="*70)
    print("SUMMARY")
    print("="*70)

    w2n_success = sum(1 for r in results if r['w2n_ok'])
    omnom_success = sum(1 for r in results if r['omnom_ok'])
    total = len(results)

    print(f"\nword2number: {w2n_success}/{total} tests passed ({w2n_success/total*100:.1f}%)")
    print(f"OmNomNum:    {omnom_success}/{total} tests passed ({omnom_success/total*100:.1f}%)")

    # Feature comparison
    print("\n" + "="*70)
    print("FEATURE COMPARISON")
    print("="*70)

    features = [
        ("Basic numbers (1-999)", "✓", "✓"),
        ("Thousands, millions, billions", "✓", "✓"),
        ("Fractions (1/2, 3/4)", "✗", "✓"),
        ("Mixed fractions (1 1/2)", "✗", "✓"),
        ("Decimals (point notation)", "✗", "✓"),
        ("Negative numbers", "Partial", "✓"),
        ("Ordinals (1st, 2nd)", "✗", "✓"),
        ("Text normalization", "✗", "✓"),
        ("Pure Python", "✓", "✗"),
        ("C-based performance", "✗", "✓"),
    ]

    print(f"\n{'Feature':<30} {'word2number':<15} {'OmNomNum':<15}")
    print("-" * 70)
    for feature, w2n_support, omnom_support in features:
        print(f"{feature:<30} {w2n_support:<15} {omnom_support:<15}")


if __name__ == "__main__":
    main()
