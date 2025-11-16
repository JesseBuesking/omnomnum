/*
 * Bare C benchmark for comparison with Python wrapper.
 * Mirrors the Python benchmark scenarios.
 */

#include "omnomnum.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef struct {
    const char *name;
    double mean_ns;
    double median_ns;
    double stdev_ns;
    double min_ns;
    double max_ns;
    int iterations;
} BenchmarkResult;

double get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

double calculate_mean(double *times, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += times[i];
    }
    return sum / n;
}

double calculate_median(double *times, int n) {
    // Simple median (assumes already sorted or small n)
    if (n % 2 == 0) {
        return (times[n/2 - 1] + times[n/2]) / 2.0;
    } else {
        return times[n/2];
    }
}

double calculate_stdev(double *times, int n, double mean) {
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = times[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / (n - 1));
}

double find_min(double *times, int n) {
    double min = times[0];
    for (int i = 1; i < n; i++) {
        if (times[i] < min) min = times[i];
    }
    return min;
}

double find_max(double *times, int n) {
    double max = times[0];
    for (int i = 1; i < n; i++) {
        if (times[i] > max) max = times[i];
    }
    return max;
}

BenchmarkResult benchmark(
    const char *name,
    void (*func)(ParserState *),
    ParserState *state,
    int iterations,
    int warmup,
    int repetitions
) {
    BenchmarkResult result;
    result.name = name;
    result.iterations = iterations;

    // Warmup
    for (int i = 0; i < warmup; i++) {
        func(state);
        resetParserState(state);
    }

    // Benchmark
    double times[repetitions];
    for (int rep = 0; rep < repetitions; rep++) {
        double start = get_time_ns();
        for (int i = 0; i < iterations; i++) {
            func(state);
            resetParserState(state);
        }
        double end = get_time_ns();
        times[rep] = end - start;
    }

    // Calculate statistics
    result.mean_ns = calculate_mean(times, repetitions);
    result.median_ns = calculate_median(times, repetitions);
    result.stdev_ns = calculate_stdev(times, repetitions, result.mean_ns);
    result.min_ns = find_min(times, repetitions);
    result.max_ns = find_max(times, repetitions);

    return result;
}

void print_result(BenchmarkResult *result) {
    printf("\n%s\n", result->name);
    printf("  Iterations: %d\n", result->iterations);
    printf("  Mean:       %.3f ms\n", result->mean_ns / 1e6);
    printf("  Median:     %.3f ms\n", result->median_ns / 1e6);
    printf("  StdDev:     %.3f ms\n", result->stdev_ns / 1e6);
    printf("  Min:        %.3f ms\n", result->min_ns / 1e6);
    printf("  Max:        %.3f ms\n", result->max_ns / 1e6);
    printf("  Per Op:     %.0f ns\n", result->mean_ns / result->iterations);
    printf("  Ops/sec:    %.0f\n", result->iterations / (result->mean_ns / 1e9));
}

// Benchmark functions
void bench_simple(ParserState *state) {
    const char *text = "two hundred fifty";
    normalize(text, strlen(text), state);
}

void bench_complex(ParserState *state) {
    const char *text = "I have two hundred fifty dollars and three hundred twenty five cents";
    normalize(text, strlen(text), state);
}

void bench_many_numbers(ParserState *state) {
    const char *text = "one two three four five six seven eight nine ten";
    normalize(text, strlen(text), state);
}

void bench_fractions(ParserState *state) {
    const char *text = "one and a half cups of sugar and three quarters teaspoon of salt";
    normalize(text, strlen(text), state);
}

void bench_long_text(ParserState *state) {
    const char *text = "The first place winner received twenty three thousand dollars, "
                       "the second place received twelve thousand five hundred, and "
                       "third place received five thousand";
    normalize(text, strlen(text), state);
}

void bench_find_numbers(ParserState *state) {
    const char *text = "one two three four five six seven eight nine ten";
    find_numbers(text, strlen(text), state);
}

void bench_no_numbers(ParserState *state) {
    const char *text = "hello world this has no numbers";
    normalize(text, strlen(text), state);
}

int main(void) {
    printf("======================================================================\n");
    printf("OmNomNum C Benchmark (for comparison with Python wrapper)\n");
    printf("======================================================================\n");

    initOmNomNum();

    ParserState state;
    initParserState(&state);

    BenchmarkResult result;

    // Benchmark 1: Simple normalization
    result = benchmark(
        "Simple normalization (C)",
        bench_simple,
        &state,
        10000,  // iterations
        1000,   // warmup
        10      // repetitions
    );
    print_result(&result);

    // Benchmark 2: Complex text
    result = benchmark(
        "Complex text with multiple numbers (C)",
        bench_complex,
        &state,
        10000,
        1000,
        10
    );
    print_result(&result);

    // Benchmark 3: Many numbers
    result = benchmark(
        "Many numbers (C)",
        bench_many_numbers,
        &state,
        10000,
        1000,
        10
    );
    print_result(&result);

    // Benchmark 4: Fractions
    result = benchmark(
        "Fractions (C)",
        bench_fractions,
        &state,
        10000,
        1000,
        10
    );
    print_result(&result);

    // Benchmark 5: Long text
    result = benchmark(
        "Long text (C)",
        bench_long_text,
        &state,
        10000,
        1000,
        10
    );
    print_result(&result);

    // Benchmark 6: find_numbers
    result = benchmark(
        "find_numbers (C)",
        bench_find_numbers,
        &state,
        10000,
        1000,
        10
    );
    print_result(&result);

    // Benchmark 7: No-op
    result = benchmark(
        "No-op (text without numbers) (C)",
        bench_no_numbers,
        &state,
        10000,
        1000,
        10
    );
    print_result(&result);

    freeParserState(&state);
    freeOmNomNum();

    printf("\n======================================================================\n");
    printf("Benchmark Complete\n");
    printf("======================================================================\n");

    return 0;
}
