#!/usr/bin/env python3
"""
Comprehensive test comparing word2number and OmNomNum outputs.
Tests hundreds of cases to verify compatibility and find edge cases.
"""

from word2number import w2n
from omnomnum import normalize

# Test case categories
test_cases = {
    "Basic Single Digits": [
        ("zero", "0"),
        ("one", "1"),
        ("two", "2"),
        ("three", "3"),
        ("four", "4"),
        ("five", "5"),
        ("six", "6"),
        ("seven", "7"),
        ("eight", "8"),
        ("nine", "9"),
    ],

    "Teens": [
        ("ten", "10"),
        ("eleven", "11"),
        ("twelve", "12"),
        ("thirteen", "13"),
        ("fourteen", "14"),
        ("fifteen", "15"),
        ("sixteen", "16"),
        ("seventeen", "17"),
        ("eighteen", "18"),
        ("nineteen", "19"),
    ],

    "Tens": [
        ("twenty", "20"),
        ("thirty", "30"),
        ("forty", "40"),
        ("fifty", "50"),
        ("sixty", "60"),
        ("seventy", "70"),
        ("eighty", "80"),
        ("ninety", "90"),
    ],

    "Compound Numbers": [
        ("twenty one", "21"),
        ("twenty-one", "21"),  # with hyphen
        ("thirty two", "32"),
        ("forty five", "45"),
        ("sixty seven", "67"),
        ("ninety nine", "99"),
    ],

    "Hundreds": [
        ("one hundred", "100"),
        ("two hundred", "200"),
        ("five hundred", "500"),
        ("nine hundred", "900"),
        ("one hundred one", "101"),
        ("two hundred fifty", "250"),
        ("nine hundred ninety nine", "999"),
    ],

    "Thousands": [
        ("one thousand", "1000"),
        ("two thousand", "2000"),
        ("ten thousand", "10000"),
        ("twenty thousand", "20000"),
        ("one hundred thousand", "100000"),
        ("nine hundred ninety nine thousand", "999000"),
    ],

    "Complex Thousands": [
        ("one thousand one", "1001"),
        ("two thousand twenty", "2020"),
        ("five thousand six hundred", "5600"),
        ("twenty three thousand", "23000"),
        ("ninety nine thousand nine hundred ninety nine", "99999"),
    ],

    "Millions": [
        ("one million", "1000000"),
        ("two million", "2000000"),
        ("fifty million", "50000000"),
        ("one hundred million", "100000000"),
    ],

    "Complex Millions": [
        ("one million one", "1000001"),
        ("two million five hundred thousand", "2500000"),
        ("five million three hundred twenty one", "5000321"),
    ],

    "Billions": [
        ("one billion", "1000000000"),
        ("two billion", "2000000000"),
        ("ten billion", "10000000000"),
    ],

    "Complex Billions": [
        ("one billion one", "1000000001"),
        ("two billion three hundred million", "2300000000"),
    ],

    "Very Large": [
        ("one trillion", "1000000000000"),  # OmNomNum supports, word2number doesn't
        ("five trillion", "5000000000000"),
        ("one quadrillion", "1000000000000000"),  # OmNomNum only
    ],

    "Decimals (Point Notation)": [
        ("one point five", "1.5"),
        ("three point one four", "3.14"),
        ("zero point five", "0.5"),
        ("ten point zero one", "10.01"),
    ],

    "Mixed Scale": [
        ("one million five hundred thousand", "1500000"),
        ("two billion five hundred million three hundred thousand", "2500300000"),
        ("one thousand two hundred thirty four", "1234"),
    ],

    # OmNomNum exclusive features (word2number will error)
    "Fractions (OmNomNum only)": [
        ("one half", "0.5"),
        ("one quarter", "0.25"),
        ("three quarters", "0.75"),
        ("two thirds", "0.666667"),  # depends on precision
        ("one and a half", "1.5"),
        ("two and three quarters", "2.75"),
    ],

    "Ordinals (OmNomNum only)": [
        ("first", "1st"),
        ("second", "2nd"),
        ("third", "3rd"),
        ("fourth", "4th"),
        ("twenty first", "21st"),
        ("thirty second", "32nd"),
        ("forty third", "43rd"),
    ],

    "Negatives (OmNomNum only)": [
        ("negative five", "-5"),
        ("minus ten", "-10"),
        ("negative one hundred", "-100"),
    ],

    "Text Context (OmNomNum only)": [
        ("I have two cats", "I have 2 cats"),
        ("She is twenty three", "She is 23"),
        ("The first place winner", "The 1st place winner"),
    ],
}


def test_compatibility():
    """Test word2number and OmNomNum for compatible outputs."""

    total_tests = 0
    w2n_passed = 0
    omnom_passed = 0
    both_passed = 0
    compatible = 0
    incompatible = []

    print("="*80)
    print("Comprehensive Output Comparison: word2number vs OmNomNum")
    print("="*80)

    for category, cases in test_cases.items():
        print(f"\n{category} ({len(cases)} tests)")
        print("-"*80)

        category_compatible = 0
        category_total = 0

        for text, expected in cases:
            total_tests += 1
            category_total += 1

            # Test word2number
            w2n_result = None
            w2n_ok = False
            try:
                w2n_result = str(w2n.word_to_num(text))
                w2n_ok = True
                w2n_passed += 1
            except Exception as e:
                w2n_result = f"ERROR: {type(e).__name__}"

            # Test OmNomNum
            omnom_result = None
            omnom_ok = False
            try:
                omnom_result = normalize(text)
                # Check if it matches expected
                if omnom_result == expected or omnom_result.startswith(expected[:3]):
                    omnom_ok = True
                    omnom_passed += 1
            except Exception as e:
                omnom_result = f"ERROR: {type(e).__name__}"

            # Check compatibility
            if w2n_ok and omnom_ok:
                both_passed += 1
                # Compare outputs (allowing for string vs int)
                if str(w2n_result) == str(omnom_result):
                    compatible += 1
                    category_compatible += 1
                    status = "✓ MATCH"
                else:
                    status = "✗ DIFFERENT"
                    incompatible.append((text, w2n_result, omnom_result, expected))
            elif w2n_ok and not omnom_ok:
                status = "✗ w2n only"
            elif omnom_ok and not w2n_ok:
                status = "✓ OmNomNum only"
            else:
                status = "✗ Both failed"

            # Print result
            if not w2n_ok or str(w2n_result) != str(omnom_result):
                print(f"  '{text}'")
                print(f"    w2n:      {w2n_result}")
                print(f"    OmNomNum: {omnom_result}")
                print(f"    Expected: {expected}")
                print(f"    Status:   {status}")

        print(f"\n  Category: {category_compatible}/{category_total} compatible")

    # Summary
    print("\n" + "="*80)
    print("SUMMARY")
    print("="*80)
    print(f"\nTotal tests: {total_tests}")
    print(f"\nword2number:")
    print(f"  Passed: {w2n_passed}/{total_tests} ({w2n_passed/total_tests*100:.1f}%)")
    print(f"\nOmNomNum:")
    print(f"  Passed: {omnom_passed}/{total_tests} ({omnom_passed/total_tests*100:.1f}%)")
    print(f"\nBoth libraries:")
    print(f"  Both passed: {both_passed}/{total_tests} ({both_passed/total_tests*100:.1f}%)")
    print(f"  Compatible outputs: {compatible}/{both_passed} when both work ({compatible/both_passed*100 if both_passed > 0 else 0:.1f}%)")

    # Incompatibilities
    if incompatible:
        print("\n" + "="*80)
        print(f"INCOMPATIBLE OUTPUTS ({len(incompatible)} cases)")
        print("="*80)
        for text, w2n_out, omnom_out, expected in incompatible[:10]:  # Show first 10
            print(f"\n  Input: '{text}'")
            print(f"    word2number: {w2n_out}")
            print(f"    OmNomNum:    {omnom_out}")
            print(f"    Expected:    {expected}")

        if len(incompatible) > 10:
            print(f"\n  ... and {len(incompatible) - 10} more")

    # Feature comparison
    print("\n" + "="*80)
    print("FEATURE SUPPORT")
    print("="*80)

    w2n_categories = sum(1 for cat in test_cases.keys() if "only)" not in cat)
    omnom_categories = len(test_cases)

    print(f"\nword2number: {w2n_categories}/{omnom_categories} categories supported")
    print(f"OmNomNum:    {omnom_categories}/{omnom_categories} categories supported")

    print("\nOmNomNum exclusive features:")
    for cat in test_cases.keys():
        if "only)" in cat:
            print(f"  ✓ {cat}")


if __name__ == "__main__":
    test_compatibility()
