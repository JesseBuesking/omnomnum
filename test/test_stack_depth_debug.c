/* Test to debug parser stack depth tracking */

#include "../omnomnum.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    initOmNomNum();

    ParserState state;
    initParserState(&state);

    const char* test = "one hundred twenty three";
    printf("Testing: \"%s\"\n\n", test);

    // Check parser before normalization
    printf("Parser before normalize: %p\n", state.pParser);
#ifdef YYTRACKMAXSTACKDEPTH
    if (state.pParser) {
        printf("Stack depth before: %d\n", ParseStackPeak(state.pParser));
    }
#endif

    // Run normalization
    normalize(test, strlen(test), &state);
    printf("Result: %s\n", state.result);

    // Check parser after normalization
    printf("\nParser after normalize: %p\n", state.pParser);
#ifdef YYTRACKMAXSTACKDEPTH
    if (state.pParser) {
        printf("Stack depth after: %d\n", ParseStackPeak(state.pParser));
    }
#else
    printf("YYTRACKMAXSTACKDEPTH not defined!\n");
#endif

    // Try a more complex expression
    resetParserState(&state);
    const char* test2 = "nine hundred ninety nine trillion nine hundred ninety nine billion";
    printf("\n\nTesting: \"%s\"\n\n", test2);

    normalize(test2, strlen(test2), &state);
    printf("Result: %s\n", state.result);
#ifdef YYTRACKMAXSTACKDEPTH
    if (state.pParser) {
        printf("Stack depth: %d\n", ParseStackPeak(state.pParser));
    }
#endif

    freeParserState(&state);
    freeOmNomNum();

    return 0;
}
