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

void BM_fast_double(benchmark::State& state) {
    double value = 10.234;
    while (state.KeepRunning()) {
        sds buffer = sdsempty();
        fill_double(value, buffer);
        morphNumericString(&buffer, 3);
    }
}
/*BENCHMARK(BM_fast_double)->ReportAggregatesOnly(true);*/

void BM_normal_int(benchmark::State& state) {
    long int value = 100200300400500600;
    while (state.KeepRunning()) {
        char buffer[256] = { '\0' };
        sprintf(buffer, "%ld", value);
    }
}
/*BENCHMARK(BM_normal_int)->ReportAggregatesOnly(true);*/

void BM_fast_int(benchmark::State& state) {
    long long int value = 100200300400500600;
    while (state.KeepRunning()) {
        char buffer2[256] = { '\0' };
        i64toa_branchlut(value, buffer2);
    }
}
/*BENCHMARK(BM_fast_int)->ReportAggregatesOnly(true);*/

void BM_fast_dbl_to_int(benchmark::State& state) {
    double value = 123456789.123456;
    while (state.KeepRunning()) {
        char buffer2[256] = { '\0' };
        i64toa_branchlut((uint64_t)value, buffer2);
    }
}
/*BENCHMARK(BM_fast_dbl_to_int)->ReportAggregatesOnly(true);*/

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
BENCHMARK(BM_simple)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_long_string)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_many_numbers)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_decimals_simple)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_decimals_complex)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_ordinals_common)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_ordinals_large)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_negatives)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_large_numbers)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_large_numbers_composed)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_mixed_fractions_word)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_mixed_fractions_numeric)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_numeric_fractions)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_percents)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_zeros)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_edge_cases_boundary)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_decimal_multipliers)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_compound_numbers)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_all_digits)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_teens)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_tens)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

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
BENCHMARK(BM_comprehensive_mix)->UseRealTime()->Threads(1)->ReportAggregatesOnly(true);

int main(int argc, char** argv)
{
    initOmNomNum();

   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();

    freeOmNomNum();
}
