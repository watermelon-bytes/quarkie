#ifndef PARSER_CPP
#define PARSER_CPP
#include <sys/types.h>

#include <parser.hxx>
#include <superblock.hxx>

using string_utils::word;

namespace string_utils {

constexpr auto no_word = word {nullptr, 0};

word take_word(const char* str) {
    word output;
    while (*str == separator || *str == ' ') str++;

    if (! *str) return no_word;
    output.c = str;

    while (*str != '/' && *str != '\0') str++;

    output.size = str - output.c;
    return output;
}

word take_filename(const char* str) {
    if (! str || ! *str) return no_word;
    str += __builtin_strlen(str) - 1;

    while (*str == separator || *str == ' ') str--;
    const auto end = str;

    while (*(str - 1) != separator) --str;

    return {str, (ushort) (end - str + 1)};
}

word take_directory(const char* path) {
    auto* const end = path = take_filename(path).c - 1;
    while (*path != separator) path--;

    return word {path, (ushort) (end - path)};
}

extern quarkie::superblock sb;

quarkie::node* find_subdirectory(const char* path) {
    const word target = take_directory(path);
    quarkie::node* current = quarkie::sb.root.data.children.first_subdir;
    while (target != (const char16_t*) &(current->name)) {
        current = current->next_node;
    }

    if (! current) {
    }
}

quarkie::node* find_file(const char* str) {
    word target = string_utils::take_word(str),
         final_target = string_utils::take_filename(str);

    for (quarkie::node* curr_node = quarkie::sb.root.data.children.eldest_child;
         curr_node != nullptr; curr_node = curr_node->next_node) {
        /* Check if current node matches with current directory we need to find.
         * As long as target.size <= strlen(str), memcmp should be safe */
        if (__builtin_memcmp(curr_node->name, target.c, target.size) == 0) {
            target = string_utils::take_word(str + target.size);
            if (target == final_target && curr_node->is_directory) {
                return nullptr;
            }
        }
    }

    return nullptr;
}

};  // namespace string_utils

#endif  // !PARSER_H
