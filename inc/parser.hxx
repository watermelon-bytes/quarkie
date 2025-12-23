#ifndef PARSER_HPP
#define PARSER_HPP
#include "file.hxx"

// #auxilary
namespace string_utils {

struct word {
    const char* c;
    unsigned short size;
};

extern char separator; /**< any character that will be forbidden in file names
                          but used to split paths */

// static
word take_word(const char* path);

/*
 * #auxilary
 * @brief Extracts the target file name from path, i.e. call on "/foo/bar/faz"
 * will return struct containing pointer to "faz" and size = 3.
 */
word take_filename(const char* path);

/*
 * #auxilary
 * @brief Extracts the folder name that file pointed to by @path is located
 */
word take_directory(const char* path);

quarkie::node* find_file(const char* path);

}  // namespace string_utils

#endif
