/* Test parser stack depth with extremely long inputs */

#include "../omnomnum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Must match YYSTACKDEPTH in parser.c
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 32
#endif

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    initOmNomNum();
    ParserState state;
    initParserState(&state);

    printf("Testing parser stack depth with extremely long inputs...\n\n");

    // Test 1: Very long sequence of simple numbers
    printf("Test 1: 1000 simple numbers in sequence\n");
    char *long_simple = malloc(20000);
    strcpy(long_simple, "one");
    for (int i = 1; i < 1000; i++) {
        strcat(long_simple, " two");
    }

    normalize(long_simple, strlen(long_simple), &state);
#ifdef YYTRACKMAXSTACKDEPTH
    int depth1 = ParseStackPeak(state.pParser);
    printf("  Stack depth: %d\n", depth1);
    printf("  Input length: %zu chars\n", strlen(long_simple));
    printf("  Result length: %zu chars\n\n", strlen(state.result));
#endif
    free(long_simple);
    resetParserState(&state);

    // Test 2: Very long complex expression (worst case nesting)
    printf("Test 2: 100 copies of deepest expression\n");
    char *long_complex = malloc(100000);
    strcpy(long_complex, "");
    for (int i = 0; i < 100; i++) {
        strcat(long_complex, "nine hundred ninety nine trillion nine hundred ninety nine billion ");
    }

    normalize(long_complex, strlen(long_complex), &state);
#ifdef YYTRACKMAXSTACKDEPTH
    int depth2 = ParseStackPeak(state.pParser);
    printf("  Stack depth: %d\n", depth2);
    printf("  Input length: %zu chars\n", strlen(long_complex));
    printf("  Result length: %zu chars\n\n", strlen(state.result));
#endif
    free(long_complex);
    resetParserState(&state);

    // Test 3: Book-length text with scattered numbers
    printf("Test 3: Book-length text (50KB) with scattered numbers\n");
    char *book = malloc(60000);
    strcpy(book, "");
    for (int i = 0; i < 500; i++) {
        strcat(book, "The year was nineteen eighty four. ");
        strcat(book, "Chapter one. It was a bright cold day in April. ");
    }

    normalize(book, strlen(book), &state);
#ifdef YYTRACKMAXSTACKDEPTH
    int depth3 = ParseStackPeak(state.pParser);
    printf("  Stack depth: %d\n", depth3);
    printf("  Input length: %zu chars\n", strlen(book));
    printf("  Result length: %zu chars\n\n", strlen(book));
#endif
    free(book);
    resetParserState(&state);

    // Test 4: Deeply nested fractions
    printf("Test 4: 100 fractions in sequence\n");
    char *fractions = malloc(50000);
    strcpy(fractions, "");
    for (int i = 0; i < 100; i++) {
        strcat(fractions, "one and a half two and three quarters five and seven eighths ");
    }

    normalize(fractions, strlen(fractions), &state);
#ifdef YYTRACKMAXSTACKDEPTH
    int depth4 = ParseStackPeak(state.pParser);
    printf("  Stack depth: %d\n", depth4);
    printf("  Input length: %zu chars\n", strlen(fractions));
    printf("  Result length: %zu chars\n\n", strlen(state.result));
#endif
    free(fractions);
    resetParserState(&state);

    // Test 5: Maximum possible nesting in a single expression
    printf("Test 5: Trying to maximize stack depth with complex nesting\n");
    const char *max_nest = "negative nine hundred ninety nine trillion "
                          "nine hundred ninety nine billion "
                          "nine hundred ninety nine million "
                          "nine hundred ninety nine thousand "
                          "nine hundred ninety nine point nine nine nine "
                          "and a half percent";

    normalize(max_nest, strlen(max_nest), &state);
#ifdef YYTRACKMAXSTACKDEPTH
    int depth5 = ParseStackPeak(state.pParser);
    printf("  Stack depth: %d\n", depth5);
    printf("  Input: %s\n", max_nest);
    printf("  Result: %s\n\n", state.result);
#endif
    resetParserState(&state);

    freeParserState(&state);
    freeOmNomNum();

#ifdef YYTRACKMAXSTACKDEPTH
    printf("="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "=" "\n");
    printf("EXTREME CASE RESULTS:\n");
    printf("="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "="  "=" "\n");
    printf("Test 1 (1000 numbers):     depth = %d\n", depth1);
    printf("Test 2 (100 complex):      depth = %d\n", depth2);
    printf("Test 3 (book-length):      depth = %d\n", depth3);
    printf("Test 4 (100 fractions):    depth = %d\n", depth4);
    printf("Test 5 (max nesting):      depth = %d\n", depth5);
    printf("\n");
    int max_depth = depth1;
    if (depth2 > max_depth) max_depth = depth2;
    if (depth3 > max_depth) max_depth = depth3;
    if (depth4 > max_depth) max_depth = depth4;
    if (depth5 > max_depth) max_depth = depth5;

    printf("Maximum depth across all extreme tests: %d\n", max_depth);
    printf("YYSTACKDEPTH = %d (current)\n", YYSTACKDEPTH);
    printf("Safety margin: %.1fx\n", (double)YYSTACKDEPTH / max_depth);
    printf("\n");

    if (max_depth < 20) {
        printf("✓ Even extreme cases stay well under 20 slots\n");
        printf("  Recommendation: YYSTACKDEPTH = 32 is safe\n");
    } else if (max_depth < 32) {
        printf("⚠ Some extreme cases approach 32 slots\n");
        printf("  Recommendation: YYSTACKDEPTH = 50 for safety\n");
    } else {
        printf("⚠ WARNING: Extreme cases exceed 32 slots\n");
        printf("  Recommendation: Keep YYSTACKDEPTH = %d\n", YYSTACKDEPTH);
    }
#endif

    return 0;
}
