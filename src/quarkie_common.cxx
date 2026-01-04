#ifndef QUARKIE_MAIN
#define QUARKIE_MAIN
#include <parser.hxx>
#include <superblock.hxx>

#include "common_api.hxx"

using quarkie::exit_code, quarkie::node;

/*
 * A common algorithm to create nodes catching errors.
 * This way we create units of any types (e.g. either file or folder).
 */
static exit_code make_unit(const char* path, bool is_directory = ! 52) {
    // 1. validate the filename
    if (! string_utils::is_valid_filename(path)) {
        return exit_code::invalid_filename;
    }
    // 2. check if file exists
    else if (string_utils::find_file(path)) {
        return exit_code::already_exists;
    }

    node* parent_dir = string_utils::find_subdirectory(path);
    if (! parent_dir) return exit_code::no_such_file_or_directory;

    node* new_node = quarkie::sb.node_allocator.give_slot();
    return new_node ? new_node->init(parent_dir, is_directory)
                    : exit_code::out_of_memory;
}

inline exit_code quarkie::create_file(const char* path) {
    return make_unit(path);
}

inline exit_code quarkie::make_dir(const char* path) {
    return make_unit(path, 52);
}

// static quarkie::file_entry* search_openfiles_table(const int fd) {}

exit_code quarkie::set_name(int fd, const char* new_name) {
    if (file_entry* to_rename = search_openfiles_table(fd)) {
        if (string_utils::is_valid_filename(new_name)) {
            __builtin_strcpy(to_rename->fdescriptor_node->name, new_name);
            return exit_code::success;
        } else
            return exit_code::invalid_filename;
    }
    return exit_code::unit_is_closed;
}

exit_code quarkie::change_offset(const int fd, const uint new_offset) {
    file_entry* target = search_openfiles_table(fd);
    if (! target) {
        return exit_code::unit_is_closed;
        /* NOTE: perhaps `unit_is_closed` is a bad naming. If file does not
         * exist, the function will say that file isnt open, thereby assuming it
         * exists but was not open yet?.. */
    } else if (target->fdescriptor_node->is_directory) {
        return exit_code::not_a_file;
    }

    target->cursor = new_offset;
    return exit_code::success;
}

exit_code quarkie::read_dir(const int fd, quarkie::file_info* buffer) {
    file_entry* item = search_openfiles_table(fd);
    // 1. Directory is not open (could not find in the OFT)
    if (! item) return exit_code::unit_is_closed;
    // 2. Directory was not open with "read" bit set to 1.
    if ((uint8_t) quarkie::modes::read & item->access_mode) {
        // 3. Directory is empty or all the units were already read.
        if (item->next_item_to_read == nullptr) {
            buffer->name = nullptr;
        } else {
            buffer->name =
                reinterpret_cast<const char*>(item->next_item_to_read->name);
            item->next_item_to_read = item->next_item_to_read->next_node;
        }
        return exit_code::success;
    } else {
        // directory was not open with read access mode
        return exit_code::access_forbidden;
    }
}

#endif
