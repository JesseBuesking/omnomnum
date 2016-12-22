#if defined (__cplusplus)
extern "C" {
#endif
#include "../util.h"
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
    char remove;
};

std::ostream& operator << (std::ostream &o, const TestCase &test_case) {
    o << "<TestCase(input=\"" << test_case.input << "\", expect=\"" << test_case.expect << "\")>";
    return o;
}

void operator >> (const YAML::Node& node, TestCase& test_case) {
    test_case.input = node[0].as<std::string>();
    test_case.expect = node[1].as<std::string>();
    test_case.remove = node[2].as<char>();
}

class Util : public testing::TestWithParam<TestCase> {};

TEST_P(Util, remove_char_inplace) {
    const char* input = GetParam().input.c_str();
    char* modify = (char*)malloc(strlen(input) + 1);
    strcpy(modify, input);
    const char* expect = GetParam().expect.c_str();
    char remove = GetParam().remove;

    size_t modify_len = remove_char_inplace(modify, strlen(modify), remove);

    ASSERT_TRUE(strcmp(expect, modify) == 0) << "expected \"" << expect << "\" given \"" << input << "\", actual \"" << modify << "\"\n";
    ASSERT_TRUE(strlen(modify) == modify_len) << "expected \"" << strlen(input) << "\" given \"" << modify << "\", actual \"" << modify_len << "\"\n";
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
    Util,
    testing::ValuesIn(ReadTestCasesFromDisk("remove_char_inplace.yml")));

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
