/* Direct allocation counting test
 * Counts initParserState and freeParserState calls
 */

#include "../omnomnum.h"
#include <stdio.h>
#include <string.h>

// External counters (we'll add these to scanner.def.c temporarily)
extern unsigned long g_init_parser_state_calls;
extern unsigned long g_free_parser_state_calls;

void test_fallback_heavy() {
    ParserState state;
    initParserState(&state);

    const char *inputs[] = {
        "one two three four five six seven eight nine ten",
        "twenty thirty forty fifty sixty seventy eighty ninety",
        "first second third fourth fifth sixth seventh eighth",
        "hundred thousand million billion trillion",
        "one and a half two and three quarters",
        "fifty percent thirty percent ninety percent",
        "zero one zero two zero three zero four zero five",
        "the first the second the third the fourth the fifth"
    };

    for (int rep = 0; rep < 100; rep++) {
        for (int i = 0; i < 8; i++) {
            normalize(inputs[i], strlen(inputs[i]), &state);
            resetParserState(&state);
        }
    }

    freeParserState(&state);
}

int main() {
    printf("Counting ParserState allocations in fallback tokenization...\n\n");

    initOmNomNum();

    // Reset counters
    g_init_parser_state_calls = 0;
    g_free_parser_state_calls = 0;

    printf("Running 800 iterations (8 inputs × 100 reps)...\n");
    test_fallback_heavy();

    printf("\nResults:\n");
    printf("  initParserState() calls: %lu\n", g_init_parser_state_calls);
    printf("  freeParserState() calls: %lu\n", g_free_parser_state_calls);
    printf("  Net difference:          %ld\n",
           (long)g_init_parser_state_calls - (long)g_free_parser_state_calls);

    freeOmNomNum();

    return 0;
}
