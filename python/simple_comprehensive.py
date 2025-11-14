#!/usr/bin/env python3
"""Simple comprehensive test"""

from word2number import w2n
from omnomnum import normalize

tests = {
    # Basic - both should work
    "Basic": [
        ("one", "1"),
        ("twenty three", "23"),
        ("one hundred", "100"),
        ("two hundred fifty", "250"),
        ("one thousand", "1000"),
        ("twenty three thousand", "23000"),
        ("one million", "1000000"),
        ("one billion", "1000000000"),
    ],

    # Decimals - both should work
    "Decimals": [
        ("one point five", "1.5"),
        ("three point one four", "3.14"),
    ],

    # OmNomNum exclusive
    "Fractions (OmNomNum only)": [
        ("one half", "0.5"),
        ("three quarters", "0.75"),
    ],

    "Ordinals (OmNomNum only)": [
        ("first", "1st"),
        ("second", "2nd"),
    ],

    "Text (OmNomNum only)": [
        ("I have two cats", "I have 2 cats"),
    ],
}

compatible_count = 0
incompatible_count = 0
w2n_only = 0
omnom_only = 0

print("Comprehensive Test Results")
print("="*70)

for category, cases in tests.items():
    print(f"\n{category}:")
    for text, expected in cases:
        # Test word2number
        try:
            w2n_result = str(w2n.word_to_num(text))
            w2n_ok = True
        except:
            w2n_result = "ERROR"
            w2n_ok = False

        # Test OmNomNum
        omnom_result = normalize(text)
        omnom_ok = (omnom_result == expected)

        # Compare
        if w2n_ok and omnom_ok and w2n_result == omnom_result:
            status = "✓ Compatible"
            compatible_count += 1
        elif w2n_ok and omnom_ok and w2n_result != omnom_result:
            status = "✗ Different outputs"
            incompatible_count += 1
        elif not w2n_ok and omnom_ok:
            status = "OmNomNum only"
            omnom_only += 1
        elif w2n_ok and not omnom_ok:
            status = "word2number only"
            w2n_only += 1
        else:
            status = "Both failed"

        print(f"  '{text[:30]:30}' w2n={w2n_result[:10]:10} omnom={omnom_result[:10]:10} {status}")

print("\n" + "="*70)
print("SUMMARY")
print("="*70)
print(f"Compatible:     {compatible_count}")
print(f"Incompatible:   {incompatible_count}")
print(f"OmNomNum only:  {omnom_only}")
print(f"word2number only: {w2n_only}")
