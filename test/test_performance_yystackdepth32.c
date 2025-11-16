/* Simple performance test for YYSTACKDEPTH=32 */

#include "../omnomnum.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

double get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

void benchmark(const char *name, const char *input, int iterations) {
    ParserState state;
    initParserState(&state);

    double start = get_time_ms();

    for (int i = 0; i < iterations; i++) {
        normalize(input, strlen(input), &state);
        resetParserState(&state);
    }

    double end = get_time_ms();
    double total_ms = end - start;
    double per_op_us = (total_ms * 1000.0) / iterations;

    printf("%-30s %8d iters  %10.2f ms total  %10.3f µs/op\n",
           name, iterations, total_ms, per_op_us);

    freeParserState(&state);
}

int main() {
    printf("Performance Benchmark - YYSTACKDEPTH=32\n");
    printf("========================================\n\n");

    initOmNomNum();

    // Various test cases
    benchmark("Simple:", "two hundred", 100000);
    benchmark("Many numbers:", "one two three four five six seven eight nine ten", 50000);
    benchmark("Complex:", "nine hundred ninety nine trillion nine hundred ninety nine billion", 10000);
    benchmark("Decimals:", "one point five two point three three point seven", 50000);
    benchmark("Fractions:", "one and a half two and three quarters", 50000);
    benchmark("Long string:",
              "hello world hello world hello world two hundred hello world",
              50000);
    benchmark("Percentages:", "fifty percent thirty percent", 50000);
    benchmark("Negatives:", "negative five negative ten", 50000);

    freeOmNomNum();

    printf("\nAll benchmarks completed successfully.\n");
    return 0;
}
