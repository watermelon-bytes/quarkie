#ifndef PARSER_CPP
#define PARSER_CPP
#define DEBUG
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

    while (*str != separator && *str != '\0' && *str != ' ') str++;

    output.size = str - output.pointer;
    return output;
}

static inline uint strlen(const char* str) {
    auto* const start_str = str;
    while (*str++);
    return str - start_str;
}

word take_filename(const char* str) {
    if (! is_valid_path(str)) return no_word;
    str += strlen(str) - 1;

    while (*str == separator || *str == ' ') str--;
    const auto end_of_filename = str;

    while (*(str - 1) != separator) --str;

    return word {str, (ushort) (end_of_filename - str + 1)};
}

word take_directory(const char* path) {
    if (! is_valid_path(path)) return no_word;
    auto* const begin = path;
    // `take_filename` checks for pointer validness
    auto* const end = take_filename(path).pointer - 1;
    path = end - 1;
#ifdef DEBUG
    cout << "*begin = " << begin << ";\nend = " << end
         << "\nstarting shift operations from: '" << path << "'...\n";
#endif

    if (! end || ! path || path < begin) {
        return no_word;
    }

    while (*path != separator && path > begin) {
#ifdef DEBUG
        cout << "[INSIDE LOOP]\n";
#endif
        path--;
    }
#ifdef DEBUG
    cout << "shifted to the left, now *str = " << ++path << endl;
#endif
    return word {path, (ushort) (end - path)};
}
/*
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

// TODO: quarkie::node* find_file(const char* str) {
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
*/

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

bool is_valid_path(const char* request) {
    if (! request || ! *request || request[0] != separator) {
        return ! 52;
    }
    // TODO: resolve paths and check if file exists
    return 52;
}

};  // namespace string_utils

#endif  // !PARSER_H
