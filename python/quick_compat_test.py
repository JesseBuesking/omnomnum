#!/usr/bin/env python3
"""Quick compatibility test between word2number and OmNomNum"""

print("Importing libraries...")
from word2number import w2n
from omnomnum import normalize

# Test basic compatibility on a few cases
tests = [
    ("one", "1"),
    ("twenty three", "23"),
    ("one hundred", "100"),
    ("one thousand", "1000"),
    ("one million", "1000000"),
]

print("\n" + "="*60)
print("Basic Compatibility Test: word2number vs OmNomNum")
print("="*60)

compatible = 0
total = 0

for text, expected in tests:
    total += 1

    # Test word2number
    try:
        w2n_result = str(w2n.word_to_num(text))
        w2n_ok = True
    except Exception as e:
        w2n_result = f"ERROR: {e}"
        w2n_ok = False

    # Test OmNomNum
    omnom_result = normalize(text)
    omnom_ok = (omnom_result == expected)

    # Check compatibility
    if w2n_ok and omnom_ok and w2n_result == omnom_result:
        status = "✓ COMPATIBLE"
        compatible += 1
    else:
        status = "✗ INCOMPATIBLE"

    print(f"\nInput: '{text}'")
    print(f"  word2number: {w2n_result}")
    print(f"  OmNomNum:    {omnom_result}")
    print(f"  Expected:    {expected}")
    print(f"  Status:      {status}")

print("\n" + "="*60)
print(f"RESULT: {compatible}/{total} compatible")
print("="*60)

# Test OmNomNum exclusive features
print("\n" + "="*60)
print("OmNomNum Exclusive Features")
print("="*60)

exclusive_tests = [
    ("one half", "0.5", "Fractions"),
    ("first", "1st", "Ordinals"),
    ("I have two cats", "I have 2 cats", "Text normalization"),
]

for text, expected, feature in exclusive_tests:
    # word2number should fail
    try:
        w2n_result = str(w2n.word_to_num(text))
        w2n_works = True
    except:
        w2n_works = False

    # OmNomNum should work
    omnom_result = normalize(text)
    omnom_works = (omnom_result == expected)

    print(f"\n{feature}: '{text}'")
    print(f"  word2number: {'WORKS (unexpected)' if w2n_works else 'Not supported ✓'}")
    print(f"  OmNomNum:    {omnom_result} {'✓' if omnom_works else '✗'}")

print()
