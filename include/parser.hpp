#ifndef PARSER_HPP
#define PARSER_HPP
// #include "file.hpp"

namespace string_utils {

struct word {
    char* c;
    unsigned short size;
};

word take_word(const char* path, const char separator);

}  // namespace string_utils

#endif  // !PARSER_HPP
