#include "omnomnum.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// BM_many_numbers test case
static const char* many_numbers =
    "one two three four five six seven eight nine ten "
    "one two three four five six seven eight nine ten "
    "one two three four five six seven eight nine ten "
    "one two three four five six seven eight nine ten "
    "one two three four five six seven eight nine ten "
    "one two three four five six seven eight nine ten "
    "one two three four five six seven eight nine ten "
    "one two three four five six seven eight nine ten "
    "one two three four five six seven eight nine ten";

int main() {
    initOmNomNum();

    ParserState st;
    initParserState(&st);

    // Run 5000 iterations like the benchmark
    printf("Running 5000 iterations of BM_many_numbers...\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 5000; i++) {
        normalize(many_numbers, strlen(many_numbers), &st);
        resetParserState(&st);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Total time: %.3f seconds\n", elapsed);
    printf("Average per iteration: %.3f us\n", (elapsed / 5000) * 1e6);

    freeParserState(&st);
    freeOmNomNum();

    return 0;
}
