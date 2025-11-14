#!/usr/bin/env python3
"""Compatibility test: text2num vs OmNomNum"""

from text_to_num import text2num
from omnomnum import normalize

# Quick compatibility test on shared features
tests = [
    ("one", "1"),
    ("twenty three", "23"),
    ("one hundred", "100"),
    ("one thousand", "1000"),
    ("one million", "1000000"),
    ("one million one", "1000001"),
]

compatible = 0
total = len(tests)

print("Basic Compatibility (Shared Features)")
print("="*60)
for text, expected in tests:
    t2n = str(text2num(text, "en"))
    omnom = normalize(text)
    match = (t2n == omnom == expected)
    if match:
        compatible += 1
    print(f"{text:25} t2n:{t2n:10} omnom:{omnom:10} {'✓' if match else '✗'}")

print(f"\nCompatibility: {compatible}/{total} ({compatible/total*100:.0f}%)")

# Test OmNomNum exclusive features
print("\n" + "="*60)
print("OmNomNum Exclusive Features (text2num fails)")
print("="*60)

exclusive = [
    ("one point five", "1.5", "Decimals"),
    ("one half", "0.5", "Fractions"),
    ("first", "1st", "Ordinals"),
    ("negative five", "-5", "Negatives"),
    ("I have two cats", "I have 2 cats", "Text normalization"),
]

for text, expected, feature in exclusive:
    try:
        text2num(text, "en")
        t2n_works = True
    except:
        t2n_works = False

    omnom = normalize(text)
    omnom_works = (omnom == expected)

    status = "✓" if omnom_works else "✗"
    print(f"{feature:20} text2num:{'✗' if not t2n_works else '✓'}  OmNomNum:{status}  ({text})")
