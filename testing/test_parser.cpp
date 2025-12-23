#include <cassert>
#include <ostream>
#define DEBUG
#include <string.h>

#include <iostream>
using std::cout, std::endl;
#include <parser.hxx>

#include "../src/paths.cxx"

char string_utils::separator = '/';

const char* testcase1 = "/root";

void test_take_word_from_back() {
    string_utils::word expected, gotten;
    // -----------------------
    const char* expected_str = "root";
    const uint expected_len = strlen(expected_str);
    cout << "Expected: len = " << expected_len << "; *str = " << expected_str
         << endl;

    gotten = string_utils::take_word_from_back(testcase1);
    cout << "Gotten: len = " << gotten.size << "; *str = " << gotten.c << endl;
    // (expected_len == gotten.size &&
    //      __builtin_memcmp(expected_str, gotten.c, gotten.size) == 0);
}

int main() {
    cout << "----------- TEST 1 ----------" << endl;
    test_take_word_from_back();
    cout << "---------- SUCCESS -----------" << endl;
}
