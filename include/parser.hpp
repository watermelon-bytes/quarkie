#ifndef PARSER_HPP
#define PARSER_HPP
// #include "file.hpp"

// #auxilary
namespace string_utils {

struct word {
    const char* c;
    unsigned short size;
};

extern char separator;

word take_word(const char* path);

word take_filename(const char* path);
word take_directory(const char* path);

}  // namespace string_utils

#endif
