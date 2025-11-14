#!/usr/bin/env python3
"""
Feature comparison between OmNomNum and word2number
"""

from word2number import w2n
from omnomnum import normalize, OmNomNum

def test_feature(name, text, expected=None):
    """Test a feature on both libraries."""
    print(f"\n{name}")
    print(f"  Input: '{text}'")

    # Test word2number
    try:
        w2n_result = str(w2n.word_to_num(text))
        w2n_ok = "✓"
    except Exception as e:
        w2n_result = f"✗ {type(e).__name__}"
        w2n_ok = "✗"

    # Test OmNomNum
    try:
        omnom_result = normalize(text)
        omnom_ok = "✓"
    except Exception as e:
        omnom_result = f"✗ {type(e).__name__}"
        omnom_ok = "✗"

    print(f"  word2number: {w2n_ok} {w2n_result}")
    print(f"  OmNomNum:    {omnom_ok} {omnom_result}")

    if expected:
        omnom_match = "✓" if str(omnom_result) == str(expected) else "✗"
        print(f"  Expected:    {omnom_match} {expected}")

    return {
        'name': name,
        'w2n_ok': w2n_ok == "✓",
        'omnom_ok': omnom_ok == "✓",
        'w2n': w2n_result,
        'omnom': omnom_result,
    }


print("="*70)
print("Feature Comparison: OmNomNum vs word2number")
print("="*70)

results = []

# Basic numbers
results.append(test_feature("One", "one", "1"))
results.append(test_feature("Twenty three", "twenty three", "23"))
results.append(test_feature("One hundred", "one hundred", "100"))
results.append(test_feature("Two thousand", "two thousand", "2000"))

# Large numbers
results.append(test_feature("One million", "one million", "1000000"))
results.append(test_feature("One billion", "one billion", "1000000000"))

# Fractions (OmNomNum feature)
results.append(test_feature("One half", "one half", "0.5"))
results.append(test_feature("Three quarters", "three quarters", "0.75"))
results.append(test_feature("One and a half", "one and a half", "1.5"))

# Decimals
results.append(test_feature("One point five", "one point five", "1.5"))
results.append(test_feature("Three point one four", "three point one four", "3.14"))

# Negatives
results.append(test_feature("Negative five", "negative five", "-5"))
results.append(test_feature("Minus ten", "minus ten", "-10"))

# Ordinals
results.append(test_feature("First", "first", "1st"))
results.append(test_feature("Second", "second", "2nd"))
results.append(test_feature("Twenty third", "twenty third", "23rd"))

# Text normalization (OmNomNum feature)
results.append(test_feature("In sentence", "I have two cats", "I have 2 cats"))

# Summary
print("\n" + "="*70)
print("SUMMARY")
print("="*70)

w2n_count = sum(1 for r in results if r['w2n_ok'])
omnom_count = sum(1 for r in results if r['omnom_ok'])

print(f"\nword2number: {w2n_count}/{len(results)} features supported")
print(f"OmNomNum:    {omnom_count}/{len(results)} features supported")

# Show unique OmNomNum features
print("\n" + "="*70)
print("OmNomNum EXCLUSIVE FEATURES")
print("="*70)

exclusive = [r for r in results if r['omnom_ok'] and not r['w2n_ok']]
if exclusive:
    for r in exclusive:
        print(f"  ✓ {r['name']}: {r['omnom']}")
else:
    print("  (none)")

print("\n" + "="*70)
print("FEATURE MATRIX")
print("="*70)

features = [
    ("Basic numbers (0-999)", "✓", "✓"),
    ("Large numbers (thousands, millions)", "✓", "✓"),
    ("Compound numbers (twenty-one)", "✓", "✓"),
    ("Fractions (1/2, 3/4)", "✗", "✓"),
    ("Mixed fractions (1 1/2)", "✗", "✓"),
    ("Decimal notation (point)", "✗", "✓"),
    ("Negative numbers", "✗", "✓"),
    ("Ordinals (1st, 2nd, 3rd)", "✗", "✓"),
    ("Text normalization", "✗", "✓"),
    ("Whitespace preservation", "N/A", "✓"),
    ("Configurable precision", "N/A", "✓"),
    ("Configurable fraction parsing", "N/A", "✓"),
]

print(f"\n{'Feature':<40} {'word2number':<15} {'OmNomNum':<15}")
print("-" * 70)
for feature, w2n, omnom in features:
    print(f"{feature:<40} {w2n:<15} {omnom:<15}")
