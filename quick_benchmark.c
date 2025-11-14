/* Quick C benchmark for comparison */

#include "omnomnum.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

double get_time_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(void) {
    const char *text = "two hundred";
    int iterations = 1000;

    printf("Quick Benchmark - Bare C Performance\n");
    printf("============================================================\n");
    printf("Iterations: %d\n\n", iterations);

    initOmNomNum();

    ParserState state;
    initParserState(&state);

    // Benchmark
    double start = get_time_sec();
    for (int i = 0; i < iterations; i++) {
        normalize(text, strlen(text), &state);
        resetParserState(&state);
    }
    double end = get_time_sec();

    double elapsed = end - start;
    double us_per_op = (elapsed / iterations) * 1e6;

    printf("Bare C:            %.2f ms (%.1f µs/op)\n", elapsed * 1000, us_per_op);
    printf("\nResult: '%s'\n", state.result);

    freeParserState(&state);
    freeOmNomNum();

    return 0;
}
