#include "omnomnum.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static double now_sec() {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec/1e9;
}

static void run_case(const char* name, const char* data, int iters) {
    ParserState st; initParserState(&st);
    double t0 = now_sec();
    for (int i=0;i<iters;i++) {
        normalize(data, strlen(data), &st);
        resetParserState(&st);
    }
    double t1 = now_sec();
    freeParserState(&st);
    printf("%s: %.6f sec total (%.3f us/iter)\n", name, (t1-t0), (t1-t0)*1e6/iters);
}

int main() {
    initOmNomNum();
    const char* simple = "two hundred";
    const char* long_s = "hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world two hundred hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world";
    const char* many = "one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten";
    run_case("BM_simple", simple, 50000);
    run_case("BM_long_string", long_s, 5000);
    run_case("BM_many_numbers", many, 5000);
    freeOmNomNum();
    return 0;
}

