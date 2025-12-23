#ifndef PARSER_CPP
#define PARSER_CPP
#include <sys/types.h>

#include <parser.hxx>

using string_utils::word;

namespace string_utils {

constexpr auto no_word = word{nullptr, 0};

word take_word(const char* str) {
    word output;
    while (*str == separator || *str == ' ') str++;

    if (*str == '\0') return no_word;
    output.c = str;

    while (*str != '/' && *str != '\0') str++;

    output.size = str - output.c;
    return output;
}

word take_word_from_back(const char* str) {
    if (!str || !*str) return no_word;
    str += __builtin_strlen(str) - 1;

    while (*str == separator || *str == ' ') str--;
    const auto end = str;
    while (*(str - 1) != separator) str--;

    return {str, (ushort)(end - str + 1)};
}

word take_filename(const char* str) { return take_word_from_back(str); }

word take_directory(const char* path) {
    auto* const end = path = take_filename(path).c;
    while (*path != separator) path--;

    return word{path, (ushort)(end - path)};
}

/*
qarkie::node* find_file(const char* str) {
    word target = string_utils::take_word(str);

    for (node* curr_node = quarkie::sb.root->eldest_child; curr_node != nullptr;
         curr_node = curr_node->next_node) {
        /* As long as target.size <= strlen(str), memcmp should be safe * /
        if (__builtin_memcmp(curr_node->name, target.c, target.size) == 0) {
            target = string_utils::take_word(str + target.size);
            if (target.size != 0) {
                return curr_node;
            }
        }
    }

    return nullptr;
}
*/
};  // namespace string_utils

#endif  // !PARSER_H
