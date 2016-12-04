#if defined (__cplusplus)
extern "C" {
#endif
#include "../omnomnum.h"
#include "../dtoa.h"
#if defined (__cplusplus)
}
#endif

#include <benchmark/benchmark.h>

const int REPETITIONS = 10;

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
BENCHMARK(BM_simple)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_long_string)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_many_numbers)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

int main(int argc, char** argv)
{
    initOmNomNum();

   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();

    freeOmNomNum();
}
