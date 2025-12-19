#ifndef PARSER_CPP
#define PARSER_CPP
#include "../include/parser.hpp"

using string_utils::word;

#ifndef MEMCMP

#endif
namespace string_utils {

word take_word(const char* str) {
    word output;
    while (*str == separator || *str == ' ') {
        if (*str == '\0') {
            return {nullptr, 0};
        }
        str++;
    }

    output.c = str;
    while (*str != '/' && *str != '\0') {
        str++;
    }
    output.size = str - output.c;
    return output;
}

using cheesy::node;
cheesy::node* find_file(const char* path) {
    word target = string_utils::take_word(path);

    for (node* curr_node = cheesy::main.root->eldest_child;
         curr_node != nullptr; curr_node = curr_node->next) {
        if (memcmp(curr_node->name, target, target.size) == 0)

        /* As long as target.size <= strlen(path), memcmp should be safe */
        {
            target = string_utils::take_word(path + target.size);
            if (target.size != 0) {
                return curr_node;
            }
        }
    }

    return nullptr;
}

};  // namespace string_utils

#endif  // !PARSER_H
