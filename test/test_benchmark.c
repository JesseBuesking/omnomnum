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

// Fast mode for development: 3 reps, slower thorough mode: 10 reps
#ifdef QUICK_BENCH
const int REPETITIONS = 3;
#else
const int REPETITIONS = 10;
#endif

void BM_fast_double(benchmark::State& state) {
    double value = 10.234;
    while (state.KeepRunning()) {
        sds buffer = sdsempty();
        fill_double(value, buffer);
        morphNumericString(&buffer, 3);
    }
}
/*BENCHMARK(BM_fast_double)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);*/

void BM_normal_int(benchmark::State& state) {
    long int value = 100200300400500600;
    while (state.KeepRunning()) {
        char buffer[256] = { '\0' };
        sprintf(buffer, "%ld", value);
    }
}
/*BENCHMARK(BM_normal_int)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);*/

void BM_fast_int(benchmark::State& state) {
    long long int value = 100200300400500600;
    while (state.KeepRunning()) {
        char buffer2[256] = { '\0' };
        i64toa_branchlut(value, buffer2);
    }
}
/*BENCHMARK(BM_fast_int)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);*/

void BM_fast_dbl_to_int(benchmark::State& state) {
    double value = 123456789.123456;
    while (state.KeepRunning()) {
        char buffer2[256] = { '\0' };
        i64toa_branchlut((uint64_t)value, buffer2);
    }
}
/*BENCHMARK(BM_fast_dbl_to_int)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);*/

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

void BM_decimals_simple(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one point five two point three three point seven four point two five point nine one point five two point three three point seven four point two five point nine one point five two point three three point seven four point two five point nine";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_decimals_simple)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_decimals_complex(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "zero point zero one point one two three four five negative one point two three four five six point seven eight nine zero point zero one point one two three four five negative one point two three four five six point seven eight nine";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_decimals_complex)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_ordinals_common(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "first third fifth seventh tenth eleventh twelfth thirteenth twentieth thirtieth first third fifth seventh tenth eleventh twelfth thirteenth twentieth thirtieth first third fifth seventh tenth eleventh twelfth thirteenth twentieth thirtieth";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_ordinals_common)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_ordinals_large(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "hundredth thousandth millionth billionth trillionth quadrillionth hundredth thousandth millionth billionth trillionth quadrillionth hundredth thousandth millionth billionth trillionth quadrillionth";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_ordinals_large)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_negatives(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "negative one minus two negative three minus four negative five minus six negative seven minus eight negative nine minus ten negative one minus two negative three minus four negative five minus six negative seven minus eight negative nine minus ten";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_negatives)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_large_numbers(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one million two billion three trillion four quadrillion five million six billion seven trillion eight quadrillion one million two billion three trillion four quadrillion five million six billion seven trillion eight quadrillion";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_large_numbers)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_large_numbers_composed(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one hundred twenty three thousand four hundred fifty six seven hundred eighty nine million twelve thousand three hundred forty five six hundred seventy eight billion nine hundred one";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_large_numbers_composed)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_mixed_fractions_word(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one and a half two and three quarters three and two fifths four and one eighth five and seven ninths one and a half two and three quarters three and two fifths four and one eighth five and seven ninths";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_mixed_fractions_word)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_mixed_fractions_numeric(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "1 1/2 2 3/4 3 2/5 4 1/8 5 7/9 1 1/2 2 3/4 3 2/5 4 1/8 5 7/9 1 1/2 2 3/4 3 2/5 4 1/8 5 7/9";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_mixed_fractions_numeric)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_numeric_fractions(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "1/2 1/3 1/4 1/5 1/8 2/3 3/4 5/8 7/8 1/2 1/3 1/4 1/5 1/8 2/3 3/4 5/8 7/8 1/2 1/3 1/4 1/5 1/8 2/3 3/4 5/8 7/8";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_numeric_fractions)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_percents(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "fifty percent twenty five percent one hundred percent seventy five percent thirty three percent fifty percent twenty five percent one hundred percent seventy five percent thirty three percent";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_percents)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_zeros(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero zero";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_zeros)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_edge_cases_boundary(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "ninety nine one hundred one thousand nine hundred ninety nine one million ninety nine one hundred one thousand nine hundred ninety nine one million";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_edge_cases_boundary)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_decimal_multipliers(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one point five million two point three billion zero point five thousand three point seven trillion one point five million two point three billion zero point five thousand three point seven trillion";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_decimal_multipliers)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_compound_numbers(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "twentyone thirtyseven fortyfive fiftysix sixtyseven seventyeight eightyeight ninetynine twentyone thirtyseven fortyfive fiftysix sixtyseven seventyeight eightyeight ninetynine";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_compound_numbers)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_all_digits(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "zero one two three four five six seven eight nine zero one two three four five six seven eight nine zero one two three four five six seven eight nine";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_all_digits)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_teens(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_teens)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_tens(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "twenty thirty forty fifty sixty seventy eighty ninety twenty thirty forty fifty sixty seventy eighty ninety twenty thirty forty fifty sixty seventy eighty ninety";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_tens)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_comprehensive_mix(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one point five million negative three quarters first twenty one and a half percent two billion minus seven point three 99/100 third one thousand";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_comprehensive_mix)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_decimals_simple(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one point five two point three three point seven four point nine five point one six point eight seven point two eight point six nine point four ten point zero";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_decimals_simple)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_decimals_complex(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "three point one four one five nine two six five three five eight nine seven nine three two three eight four six two six four three three eight three two seven nine";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_decimals_complex)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_ordinals_common(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "first second third fourth fifth sixth seventh eighth ninth tenth first second third fourth fifth sixth seventh eighth ninth tenth first second third fourth fifth sixth seventh eighth ninth tenth";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_ordinals_common)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_ordinals_large(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "twenty first thirty second forty third fifty fourth sixty fifth seventy sixth eighty seventh ninety eighth one hundredth one thousandth one millionth twenty first thirty second forty third";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_ordinals_large)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_negatives(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "negative one negative two negative three negative four negative five negative one hundred negative one thousand negative one million negative one billion";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_negatives)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_large_numbers(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one million two million five hundred thousand three billion seven hundred million nine quadrillion eight hundred trillion";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_large_numbers)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_large_numbers_composed(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "nine hundred ninety nine quadrillion nine hundred ninety nine trillion nine hundred ninety nine billion nine hundred ninety nine million nine hundred ninety nine thousand nine hundred ninety nine";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_large_numbers_composed)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_mixed_fractions_word(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one and a half two and three quarters five and two thirds ten and five sixths twenty and seven eighths one and a half two and three quarters";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_mixed_fractions_word)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_mixed_fractions_numeric(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one and 1/2 two and 3/4 five and 2/3 ten and 5/6 twenty and 7/8 one and 1/2 two and 3/4 five and 2/3";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_mixed_fractions_numeric)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_numeric_fractions(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "1/2 3/4 2/3 5/6 7/8 1/4 3/8 5/8 7/16 9/16 11/16 13/16 15/16 1/2 3/4 2/3 5/6 7/8";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_numeric_fractions)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_percents(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "fifty percent twenty five percent seventy five percent ten percent ninety percent one percent ninety nine percent fifty percent twenty five percent";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_percents)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_zeros(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "zero zero point zero zero million zero thousand zero hundred zero zero zero point zero zero zero";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_zeros)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_edge_cases_boundary(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "nineteen twenty ninety nine one hundred nine hundred ninety nine one thousand nine thousand nine hundred ninety nine ten thousand";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_edge_cases_boundary)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_decimal_multipliers(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one point five million two point three billion three point seven thousand four point nine trillion one point five million two point three billion";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_decimal_multipliers)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_compound_numbers(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "twenty one thirty two forty three fifty four sixty five seventy six eighty seven ninety eight twenty one thirty two forty three";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_compound_numbers)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_all_digits(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one two three four five six seven eight nine one two three four five six seven eight nine one two three four five six seven eight nine";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_all_digits)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_teens(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_teens)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_tens(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "ten twenty thirty forty fifty sixty seventy eighty ninety ten twenty thirty forty fifty sixty seventy eighty ninety ten twenty thirty forty";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_tens)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

void BM_comprehensive_mix(benchmark::State& state) {
    ParserState pstate;
    initParserState(&pstate);

    const char* data = "one point five million negative three quarters first twenty one and a half percent two billion minus seven point three 99/100 third one thousand";
    size_t data_len = strlen(data);

    while (state.KeepRunning()) {
        normalize(data, data_len, &pstate);
        resetParserState(&pstate);
    }

    freeParserState(&pstate);
}
BENCHMARK(BM_comprehensive_mix)->UseRealTime()->Threads(1)->Repetitions(REPETITIONS)->ReportAggregatesOnly(true);

int main(int argc, char** argv)
{
    initOmNomNum();

   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();

    freeOmNomNum();
}
