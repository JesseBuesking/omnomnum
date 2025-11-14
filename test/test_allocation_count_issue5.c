// Test to count YYSTYPEList allocations in subState
// Compile: cc -I. -o test/test_allocation_count_issue5 test/test_allocation_count_issue5.c omnomnum.c parser.c scanner.c scanner.def.c sds.c -DDEBUG_ALLOCATIONS
// Run: ./test/test_allocation_count_issue5

#include "omnomnum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External counters (defined in scanner.def.c with DEBUG_ALLOCATIONS)
#ifdef DEBUG_ALLOCATIONS
extern unsigned long g_yystype_init_total_bytes;
extern unsigned long g_yystype_realloc_count;
#endif

int main() {
    printf("Testing YYSTYPEList allocations in fallback path...\n\n");

    // Test strings that trigger fallback path (separate number words)
    const char *test_strings[] = {
        "one two three four five",
        "first second third fourth fifth",
        "twenty one hundred two thousand",
        "one million two thousand three hundred forty five",
        "ninety nine percent",
    };
    int num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
    int iterations = 100;

    ParserState state;
    initParserState(&state);

    #ifdef DEBUG_ALLOCATIONS
    g_yystype_init_total_bytes = 0;
    g_yystype_realloc_count = 0;
    #endif

    printf("Running %d test strings x %d iterations = %d normalizations\n",
           num_tests, iterations, num_tests * iterations);
    printf("Each normalization triggers multiple subState uses (one per token)\n\n");

    for (int iter = 0; iter < iterations; iter++) {
        for (int i = 0; i < num_tests; i++) {
            resetParserState(&state);
            normalize(test_strings[i], strlen(test_strings[i]), &state);
            if (state.result) {
                sdsfree(state.result);
                state.result = NULL;
            }
        }
    }

    freeParserState(&state);

    #ifdef DEBUG_ALLOCATIONS
    printf("Results:\n");
    printf("  Total YYSTYPEList allocation bytes: %lu\n", g_yystype_init_total_bytes);
    printf("  YYSTYPEList realloc count: %lu\n", g_yystype_realloc_count);
    printf("\nNote: Optimized version should show significant reduction in initial allocation bytes\n");
    printf("      (128 elements = ~1-4KB per subState init, should reduce to 8 elements = ~64-256 bytes)\n");
    #else
    printf("Build with -DDEBUG_ALLOCATIONS to see allocation counts\n");
    #endif

    return 0;
}
