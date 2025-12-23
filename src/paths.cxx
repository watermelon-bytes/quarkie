#ifndef PARSER_CPP
#define PARSER_CPP
#include <parser.hxx>

using string_utils::word;

namespace string_utils {

word take_word(const char* str) {
    word output;
    while (*str == separator || *str == ' ') str++;

    if (*str == '\0') return {nullptr, 0};
    output.c = str;

    while (*str != '/' && *str != '\0') str++;

    output.size = str - output.c;
    return output;
}

using quarkie::node;
/*
qarkie::node* find_file(const char* path) {
    word target = string_utils::take_word(path);

    for (node* curr_node = quarkie::sb.root->eldest_child; curr_node != nullptr;
         curr_node = curr_node->next_node) {
        /* As long as target.size <= strlen(path), memcmp should be safe * /
        if (__builtin_memcmp(curr_node->name, target.c, target.size) == 0) {
            target = string_utils::take_word(path + target.size);
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
