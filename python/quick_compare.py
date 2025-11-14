#!/usr/bin/env python3
from word2number import w2n
from omnomnum import normalize

tests = [
    'one',
    'twenty three',
    'one hundred',
    'one thousand',
    'one half',
]

print('word2number vs OmNomNum')
print('='*60)
for text in tests:
    try:
        w2n_result = w2n.word_to_num(text)
    except Exception as e:
        w2n_result = f'ERROR: {type(e).__name__}'

    omnom_result = normalize(text)
    print(f'{text:20} w2n={str(w2n_result):15} omnom={omnom_result}')
