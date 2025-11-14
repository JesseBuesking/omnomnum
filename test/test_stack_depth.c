/* Test to measure actual parser stack depth usage */

#include "../omnomnum.h"
#include <stdio.h>
#include <string.h>

// Must match YYSTACKDEPTH in parser.c
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 32
#endif

typedef struct {
    const char* name;
    const char* input;
} TestCase;

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Test cases covering various parsing scenarios
    TestCase test_cases[] = {
        // Simple cases
        {"simple", "two hundred"},
        {"single", "five"},
        {"ordinal", "twenty first"},

        // Many numbers (stress test)
        {"many_numbers", "one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten"},

        // Complex decimals
        {"decimals_simple", "one point five two point three three point seven"},
        {"decimals_complex", "zero point zero one point one two three four five negative one point two three four five six point seven eight nine"},

        // Fractions
        {"fractions_simple", "one half three quarters five eighths"},
        {"fractions_complex", "one and a half two and three quarters five and seven eighths"},
        {"fractions_numeric", "1/2 3/4 5/8 7/16 11/32"},
        {"mixed_fractions", "1 1/2 2 3/4 5 7/8"},

        // Large numbers
        {"large_numbers", "one hundred twenty three thousand four hundred fifty six point seven eight nine"},
        {"very_large", "nine hundred ninety nine trillion nine hundred ninety nine billion nine hundred ninety nine million nine hundred ninety nine thousand nine hundred ninety nine"},

        // Percentages
        {"percentages", "fifty percent thirty five percent ninety nine point nine percent"},

        // Negative numbers
        {"negatives", "negative five negative ten point five negative twenty three"},

        // Mixed complexity
        {"mixed", "first second third 1st 2nd 3rd one two three 1.5 2.75 50% negative 10"},

        // Long string with scattered numbers
        {"long_string", "hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world two hundred hello world hello world hello world hello world hello world"},

        // Zeros (edge case)
        {"zeros", "zero zero zero zero zero zero zero zero zero zero"},

        // Ordinals
        {"ordinals_small", "first third fifth seventh tenth"},
        {"ordinals_large", "twenty first thirty second forty third ninety ninth hundredth"},

        // Cardinals with magnitudes
        {"magnitudes", "hundred thousand million billion trillion"}
    };

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    printf("Testing parser stack depth across %d test cases...\n\n", num_tests);
    printf("YYSTACKDEPTH = %d (current allocation)\n\n", YYSTACKDEPTH);

    initOmNomNum();

    ParserState state;
    initParserState(&state);

    int overall_max_depth = 0;
    const char* deepest_test = NULL;

    for (int i = 0; i < num_tests; i++) {
        // Run normalization
        normalize(test_cases[i].input, strlen(test_cases[i].input), &state);

        // Get stack depth
#ifdef YYTRACKMAXSTACKDEPTH
        int depth = ParseStackPeak(state.pParser);

        printf("%-20s  depth: %3d  \"%s\"\n",
               test_cases[i].name,
               depth,
               test_cases[i].input);

        if (depth > overall_max_depth) {
            overall_max_depth = depth;
            deepest_test = test_cases[i].name;
        }
#else
        printf("%-20s  (depth tracking disabled)\n", test_cases[i].name);
#endif

        // Reset for next test
        resetParserState(&state);
    }

    freeParserState(&state);
    freeOmNomNum();

#ifdef YYTRACKMAXSTACKDEPTH
    printf("\n" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "\n");
    printf("RESULTS:\n");
    printf("=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "=" "\n");
    printf("Maximum stack depth:     %d / %d slots\n", overall_max_depth, YYSTACKDEPTH);
    printf("Deepest test case:       %s\n", deepest_test);
    printf("Stack utilization:       %.1f%%\n", (overall_max_depth * 100.0) / YYSTACKDEPTH);
    printf("Unused stack slots:      %d\n", YYSTACKDEPTH - overall_max_depth);
    printf("\n");

    if (overall_max_depth < 25) {
        printf("✓ Recommendation: YYSTACKDEPTH could be safely reduced to 32-50\n");
        printf("  Potential savings: ~800-1,100 bytes per parser\n");
    } else if (overall_max_depth < 50) {
        printf("✓ Recommendation: YYSTACKDEPTH could be reduced to 64-75\n");
        printf("  Potential savings: ~400-600 bytes per parser\n");
    } else if (overall_max_depth < 75) {
        printf("⚠ Recommendation: Keep YYSTACKDEPTH at 100 for safety margin\n");
        printf("  Savings would be minimal (~400 bytes) with higher risk\n");
    } else {
        printf("⚠ WARNING: Stack depth approaching YYSTACKDEPTH limit!\n");
        printf("  Consider increasing YYSTACKDEPTH for safety\n");
    }
#else
    printf("\nYYTRACKMAXSTACKDEPTH not enabled. Rebuild with -DYYTRACKMAXSTACKDEPTH\n");
#endif

    return 0;
}
