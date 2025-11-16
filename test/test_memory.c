/* Memory leak test for ResultBuffer and OmNomNum
 * Tests multiple scenarios with different buffer sizes and patterns
 */

#include "../omnomnum.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void test_short_text() {
    ParserState state;
    initParserState(&state);

    const char *inputs[] = {
        "one two three",
        "fifty percent",
        "twelve and a half",
        "twenty-first century",
        "one hundred"
    };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 100; j++) {
            normalize(inputs[i], strlen(inputs[i]), &state);
            resetParserState(&state);
        }
    }

    freeParserState(&state);
}

void test_long_text() {
    ParserState state;
    initParserState(&state);

    // Test text that will overflow the 512-byte inline buffer
    const char *long_input =
        "one hundred twenty three million four hundred fifty six thousand seven hundred eighty nine "
        "and ninety nine hundredths plus two hundred thirty four thousand five hundred sixty seven "
        "point eight nine and three quarters and fifty percent and twenty-first place and one half "
        "and one third and two thirds and three eighths and five eighths and seven eighths and "
        "nine tenths and eleven twelfths and thirteen fourteenths and fifteen sixteenths";

    for (int i = 0; i < 50; i++) {
        normalize(long_input, strlen(long_input), &state);
        resetParserState(&state);
    }

    freeParserState(&state);
}

void test_mixed_sizes() {
    ParserState state;
    initParserState(&state);

    // Alternate between short and long inputs to test buffer reuse
    const char *short_text = "one two three";
    const char *long_text =
        "the first second third fourth fifth sixth seventh eighth ninth tenth eleventh twelfth "
        "thirteenth fourteenth fifteenth sixteenth seventeenth eighteenth nineteenth twentieth "
        "twenty-first twenty-second twenty-third twenty-fourth twenty-fifth numbers";

    for (int i = 0; i < 100; i++) {
        if (i % 2 == 0) {
            normalize(short_text, strlen(short_text), &state);
        } else {
            normalize(long_text, strlen(long_text), &state);
        }
        resetParserState(&state);
    }

    freeParserState(&state);
}

void test_many_numbers() {
    ParserState state;
    initParserState(&state);

    // Test with lots of numbers (exercises YYSTYPEList allocations)
    const char *many_nums = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 "
                            "21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 "
                            "41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60";

    for (int i = 0; i < 50; i++) {
        normalize(many_nums, strlen(many_nums), &state);
        resetParserState(&state);
    }

    freeParserState(&state);
}

void test_percent_normalization() {
    ParserState state;
    initParserState(&state);
    state.normalize_percent_symbol = true;

    const char *percent_texts[] = {
        "50 percent",
        "one hundred percent",
        "twenty five percent increase",
        "99.9 percent sure",
        "fifty-fifty percent chance"
    };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 50; j++) {
            normalize(percent_texts[i], strlen(percent_texts[i]), &state);
            resetParserState(&state);
            state.normalize_percent_symbol = true; // restore after reset
        }
    }

    freeParserState(&state);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    printf("Testing memory leaks in ResultBuffer implementation...\n");
    printf("PID: %d\n", getpid());
    printf("Running tests...\n");

    initOmNomNum();

    printf("  - Short text test...\n");
    test_short_text();

    printf("  - Long text test...\n");
    test_long_text();

    printf("  - Mixed sizes test...\n");
    test_mixed_sizes();

    printf("  - Many numbers test...\n");
    test_many_numbers();

    printf("  - Percent normalization test...\n");
    test_percent_normalization();

    freeOmNomNum();

    printf("Tests complete. Check for leaks with:\n");
    printf("  leaks %d\n", getpid());
    printf("Sleeping for 1 second to allow leak detection...\n");
    sleep(1);

    printf("Done.\n");
    return 0;
}
