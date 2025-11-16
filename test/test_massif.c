/* Simple test to profile memory allocations with valgrind/massif */

#include "../omnomnum.h"
#include <stdio.h>
#include <string.h>

void test_fallback_heavy() {
    ParserState state;
    initParserState(&state);

    // These inputs exercise various code paths including fallback tokenization
    const char *inputs[] = {
        "one two three four five six seven eight nine ten",
        "twenty thirty forty fifty sixty seventy eighty ninety",
        "first second third fourth fifth sixth seventh eighth",
        "hundred thousand million billion trillion",
        "one and a half two and three quarters",
        "fifty percent thirty percent ninety percent",
        "zero one zero two zero three zero four zero five",
        "the first the second the third the fourth the fifth",
        "123.456 789.012 345.678 901.234",
        "one hundred twenty three point four five six",
        "3/4 1/2 5/8 7/16 11/32",
        "twenty-five fifty-seven ninety-nine",
        "1st 2nd 3rd 4th 5th 6th 7th 8th 9th 10th",
        "a dozen a hundred a thousand a million",
        "point five point seven five point nine nine",
        "negative five negative ten negative twenty"
    };

    // Run many iterations to accumulate memory allocations
    for (int rep = 0; rep < 250; rep++) {
        for (int i = 0; i < 16; i++) {
            normalize(inputs[i], strlen(inputs[i]), &state);
            resetParserState(&state);
        }
    }

    freeParserState(&state);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    printf("Starting memory profiling test...\n");
    printf("Running 4000 normalize() calls (16 inputs × 250 reps)...\n");

    initOmNomNum();
    test_fallback_heavy();
    freeOmNomNum();

    printf("Test complete.\n");
    return 0;
}
