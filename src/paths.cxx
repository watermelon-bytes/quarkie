#ifndef PARSER_CPP
#define PARSER_CPP
#include <parser.hxx>
#include <superblock.hxx>

#include "file.hxx"

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
    // `take_filename` checks for pointer validness
    auto* end = path = take_filename(path).pointer - 1;
    if (! end || ! path) {
        return no_word;
    }
    while (*path != separator) path--;

    return word {path, (ushort) (end - path)};
}

extern quarkie::superblock sb;

quarkie::node* find_subdirectory(const char* path) {
    word target = take_word(path);
    const word final_target = take_directory(path);
    quarkie::node* current = quarkie::sb.root.data.children.first_subdir;
    while (current) {
        if (target != reinterpret_cast<const char*>(&current->name)) {
            current = current->next_node;
        } else if (target == final_target) {
            return current;
        } else {
            // Deeper into the tree
            path += target.size;
            target = take_word(path);
            current = current->data.children.first_subdir;
        }
    }

    return nullptr;
}

// TODO:
quarkie::node* find_file(const char* str) {
    word target_filename = take_filename(str);
    quarkie::node* dir = find_subdirectory(str);

    // Linear search through the list is O(n)...
    quarkie::node* curr_node = dir->data.children.eldest_child;
    for (; curr_node != nullptr; curr_node = curr_node->next_node) {
        if (target_filename != (const char*) &curr_node->name) {
            continue;
        }
        return curr_node;
    }
    return nullptr;
}

bool is_valid_filename(const char* req) {
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

};  // namespace string_utils

#endif  // !PARSER_H
