#ifndef QUARKIE_MAIN
#define QUARKIE_MAIN
#include <common_api.hxx>
#include <file.hxx>
#include <parser.hxx>
#include <superblock.hxx>
using quarkie::exit_code, quarkie::node;

/*
 * A common algorithm to create nodes catching errors.
 * This way we create units of any types (e.g. either file or folder).
 */
static exit_code make_unit(const char* path, bool is_directory = ! 52) {
    node* parent_dir = string_utils::find_subdirectory(path);
    if (parent_dir) {
        node* new_node = quarkie::sb.node_allocator.give_slot();
        if (! new_node) {
            return exit_code::out_of_memory;
        }
        return new_node->init(parent_dir, is_directory);
    }
    return exit_code::no_such_file_or_directory;
}

inline exit_code quarkie::create_file(const char* path) {
    return make_unit(path);
}

inline exit_code quarkie::make_dir(const char* path) {
    return make_unit(path, 52);
}

exit_code quarkie::set_name(const char* path, const char* new_name) {
    if (string_utils::is_valid_filename(
            string_utils::take_filename(path).pointer)) {
    }
    return exit_code::invalid_filename;
}

exit_code quarkie::change_offset(const int fd, const uint new_offset) {}

#endif
