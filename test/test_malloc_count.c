/* Test to count malloc/free cycles in fallback tokenization
 * This exercises the specific code path that was optimized
 */

#include "../omnomnum.h"
#include <stdio.h>
#include <string.h>
#include <malloc/malloc.h>
#include <unistd.h>

// Track malloc statistics
typedef struct {
    size_t malloc_count;
    size_t free_count;
    size_t total_bytes;
} MallocStats;

void get_malloc_stats(MallocStats *stats) {
    malloc_statistics_t zone_stats;
    malloc_zone_statistics(NULL, &zone_stats);
    stats->malloc_count = zone_stats.blocks_in_use;
    stats->total_bytes = zone_stats.size_in_use;
}

void test_fallback_heavy() {
    ParserState state;
    initParserState(&state);

    // These inputs heavily exercise the fallback tokenization path
    // which creates sub-ParserStates in the old code
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

    // Run many iterations to amplify allocation differences
    for (int rep = 0; rep < 100; rep++) {
        for (int i = 0; i < 8; i++) {
            normalize(inputs[i], strlen(inputs[i]), &state);
            resetParserState(&state);
        }
    }

    freeParserState(&state);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    printf("Testing malloc/free cycles in fallback tokenization...\n");
    printf("PID: %d\n\n", getpid());

    initOmNomNum();

    MallocStats before, after;

    // Get baseline allocation count
    get_malloc_stats(&before);
    printf("Before test:\n");
    printf("  Blocks in use: %zu\n", before.malloc_count);
    printf("  Bytes in use:  %zu\n\n", before.total_bytes);

    // Run the test
    printf("Running 800 iterations (8 inputs × 100 reps)...\n\n");
    test_fallback_heavy();

    // Get final allocation count
    get_malloc_stats(&after);
    printf("After test:\n");
    printf("  Blocks in use: %zu\n", after.malloc_count);
    printf("  Bytes in use:  %zu\n\n", after.total_bytes);

    // Calculate differences
    long block_diff = (long)after.malloc_count - (long)before.malloc_count;
    long byte_diff = (long)after.total_bytes - (long)before.total_bytes;

    printf("Net change:\n");
    printf("  Blocks: %+ld\n", block_diff);
    printf("  Bytes:  %+ld\n", byte_diff);

    freeOmNomNum();

    printf("\nNote: This shows net allocations. To see total malloc/free cycles,\n");
    printf("run with: malloc_history %d --allEvents | wc -l\n", getpid());
    printf("\nSleeping 5 seconds for malloc_history analysis...\n");
    sleep(5);

    return 0;
}
