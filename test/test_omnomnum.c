#if defined (__cplusplus)
extern "C" {
#endif
#include "../omnomnum.h"
#include "../dtoa.h"
#if defined (__cplusplus)
}
#endif

#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using ::testing::InitGoogleTest;

struct TestCase {
    std::string input;
    std::string expect;
    int precision;
};

std::ostream& operator << (std::ostream &o, const TestCase &test_case) {
    o << "<TestCase(input=\"" << test_case.input << "\", expect=\"" << test_case.expect << "\")>";
    return o;
}

void operator >> (const YAML::Node& node, TestCase& test_case) {
    test_case.input = node[0].as<std::string>();
    test_case.expect = node[1].as<std::string>();
    test_case.precision = node[2].as<int>();
}

class OmNomNum : public testing::TestWithParam<TestCase> {};

TEST_P(OmNomNum, normalize) {
    ParserState state;
    initParserState(&state);
    initOmNomNum();

    const char* input = GetParam().input.c_str();
    const char* expect = GetParam().expect.c_str();

    normalize(input, strlen(input), &state);

    ASSERT_TRUE(strcmp(expect, state.result) == 0) << "expected \"" << expect << "\" given \"" << input << "\", actual \"" << state.result << "\"\n";

    sdsfree(state.result);
    freeOmNomNum();
    freeParserState(&state);
}

std::vector<TestCase> ReadTestCasesFromDisk(std::string filename) {
    YAML::Node doc = YAML::LoadFile(filename);

    std::vector<TestCase> input;
    for (unsigned i=0; i < doc.size(); i++) {
        TestCase test_case;
        doc[i] >> test_case;
        input.push_back(test_case);
    }

    return input;
}

INSTANTIATE_TEST_CASE_P(
    Tests,
    OmNomNum,
    testing::ValuesIn(ReadTestCasesFromDisk("cases.yml")));

TEST(MorphStringTest, HandlesTrailingZeros) {
    sds s = sdsnew("12.000");
    morphNumericString(&s, 3);
    ASSERT_TRUE(strcmp("12", s) == 0) << "12 != " << s << "\n";
    sdsfree(s);
}

TEST(OmNomNum, ParseSecond) {
    ParserState state;
    initParserState(&state);
    state.parse_second = true;
    initOmNomNum();

    const char* input = "second place";
    const char* expect = "2nd place";

    normalize(input, strlen(input), &state);

    ASSERT_TRUE(strcmp(expect, state.result) == 0) << "expected \"" << expect << "\" given \"" << input << "\", actual \"" << state.result << "\"\n";

    sdsfree(state.result);
    freeOmNomNum();
    freeParserState(&state);
}

TEST(OmNomNum, ParseSeconds) {
    ParserState state;
    initParserState(&state);
    state.parse_second = true;
    initOmNomNum();

    const char* input = "I'd like seconds";
    const char* expect = "I'd like 2nds";

    normalize(input, strlen(input), &state);

    ASSERT_TRUE(strcmp(expect, state.result) == 0) << "expected \"" << expect << "\" given \"" << input << "\", actual \"" << state.result << "\"\n";

    sdsfree(state.result);
    freeOmNomNum();
    freeParserState(&state);
}

void printlen(char *value, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (value[i] != 0) {
            printf("%c", value[i]);
        } else {
            printf("\\0");
        }
    }
    printf("\n");
}

TEST(NullByteTest, HandlesTrailingZeros) {
    ParserState state;
    initParserState(&state);
    initOmNomNum();

    sds input = sdsnewlen("one \0 two", 9);
    sds expect = sdsnewlen("1 \0 2", 5);

    normalize(input, sdslen(input), &state);

    ASSERT_TRUE(sdscmp(expect, state.result) == 0);

    sdsfree(state.result);
    sdsfree(input);
    sdsfree(expect);
    freeOmNomNum();
    freeParserState(&state);
}
