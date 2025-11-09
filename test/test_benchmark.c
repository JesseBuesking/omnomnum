#if defined (__cplusplus)
extern "C" {
#endif
#include "../omnomnum.h"
#include "../dtoa.h"
#if defined (__cplusplus)
}
#endif

#include <benchmark/benchmark.h>

#if defined(__cplusplus)
#include <string>
#endif

#ifndef GIT_SHA
#define GIT_SHA "unknown"
#endif
#ifndef GIT_DESC
#define GIT_DESC "unknown"
#endif
#ifndef BUILD_TIME
#define BUILD_TIME "unknown"
#endif
#ifndef TREE_DIRTY_STR
#define TREE_DIRTY_STR "unknown"
#endif

static void add_bench_context() {
#if defined(__cplusplus)
    benchmark::AddCustomContext("git_sha", std::string(GIT_SHA));
    benchmark::AddCustomContext("git_desc", std::string(GIT_DESC));
    benchmark::AddCustomContext("build_time", std::string(BUILD_TIME));
    benchmark::AddCustomContext("tree_dirty", std::string(TREE_DIRTY_STR));
#endif
}

const int REPETITIONS = 10;

void BM_fast_double(benchmark::State& state) {
    double value = 10.234;
    while (state.KeepRunning()) {
        sds buffer = sdsempty();
        fill_double(value, buffer);
        morphNumericString(&buffer, 3);
    }
}
/*BENCHMARK(BM_fast_double)->UseRealTime()->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);*/

void BM_normal_int(benchmark::State& state) {
    long int value = 100200300400500600;
    while (state.KeepRunning()) {
        char buffer[256] = { '\0' };
        sprintf(buffer, "%ld", value);
    }
}
/*BENCHMARK(BM_normal_int)->UseRealTime()->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);*/

void BM_fast_int(benchmark::State& state) {
    long long int value = 100200300400500600;
    while (state.KeepRunning()) {
        char buffer2[256] = { '\0' };
        i64toa_branchlut(value, buffer2);
    }
}
/*BENCHMARK(BM_fast_int)->UseRealTime()->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);*/

void BM_fast_dbl_to_int(benchmark::State& state) {
    double value = 123456789.123456;
    while (state.KeepRunning()) {
        char buffer2[256] = { '\0' };
        i64toa_branchlut((uint64_t)value, buffer2);
    }
}
/*BENCHMARK(BM_fast_dbl_to_int)->UseRealTime()->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);*/

void BM_simple(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "two hundred";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_simple)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_long_string(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world two hundred hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_long_string)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_many_numbers(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten one two three four five six seven eight nine ten";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_many_numbers)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_fractions_common(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one half two thirds three quarters four fifths five sixths one half two thirds three quarters four fifths five sixths one half two thirds three quarters four fifths five sixths one half two thirds three quarters four fifths five sixths";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_fractions_common)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_fractions_uncommon(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one quadrillionth two trillionths three billionths four millionths five thousandths one quadrillionth two trillionths three billionths four millionths five thousandths one quadrillionth two trillionths three billionths four millionths five thousandths";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_fractions_uncommon)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_fractions_mixed(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one half two hundredths three quadrillionths four fifths five thousandths six thirds seven millionths eight quarters nine billionths ten sixths one half two hundredths three quadrillionths four fifths five thousandths six thirds seven millionths eight quarters nine billionths ten sixths";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_fractions_mixed)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

int main(int argc, char** argv)
{
    initOmNomNum();

   ::benchmark::Initialize(&argc, argv);
   add_bench_context();
   ::benchmark::RunSpecifiedBenchmarks();

    freeOmNomNum();
}
