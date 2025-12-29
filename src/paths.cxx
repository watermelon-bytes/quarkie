#ifndef PARSER_CPP
#define PARSER_CPP
#include <parser.hxx>
#include <superblock.hxx>

using string_utils::word;

namespace string_utils {

constexpr auto no_word = word {nullptr, 0};

word take_word(const char* str) {
    word output;
    while (*str == separator || *str == ' ') str++;

    if (! *str) return no_word;
    output.pointer = str;

    while (*str != separator && *str != '\0') str++;

    output.size = str - output.pointer;
    return output;
}

static inline uint strlen(const char* str) {
    uint counter = 0;
    while (*str++) {
        counter++;
    }
    return counter;
}

word take_filename(const char* str) {
    if (! str || ! *str) return no_word;
    str += strlen(str) - 1;

    while (*str == separator || *str == ' ') str--;
    const auto end_of_filename = str;

    while (*(str - 1) != separator) --str;

    return word {str, (ushort) (end_of_filename - str + 1)};
}

word take_directory(const char* path) {
    auto* end = path = take_filename(path).pointer - 1;
    while (*path != separator) path--;

    return word {path, (ushort) (end - path)};
}

extern quarkie::superblock sb;

quarkie::node* find_subdirectory(const char* path) {
    word target;

    target = take_directory(path);
    quarkie::node* current = quarkie::sb.root.data.children.first_subdir;
    while (target != (const char*) &(current->name)) {
        if (! (current->next_node)) {
            return nullptr;
        }

        current = current->next_node;
    }

    return nullptr;
}

quarkie::node* find_file(const char* str) {
    word target = string_utils::take_word(str),
         final_target = string_utils::take_filename(str);

    for (quarkie::node* curr_node = &sb.root; curr_node != nullptr;
         curr_node = curr_node->next_node) {
        /* Check if current node matches with current directory we need to find.
         * As long as target.size <= strlen(str), memcmp should be safe */
    }
    quarkie::node* curr_directory = &sb.root;
    return nullptr;
}

};  // namespace string_utils

inline bool string_utils::is_valid_filename(const char* req) {
    if (! __builtin_strcmp(req, "..") || ! __builtin_strcmp(req, ".")) {
        return ! 52;
    }

    for (uint i = 0; req[i] != 0; ++i) {
        if (req[i] == separator || i > quarkie::node::max_name_len) {
            return ! 52;
        }
    }
    return 52;
}

#endif  // !PARSER_H
