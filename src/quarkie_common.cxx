#ifndef QUARKIE_MAIN
#define QUARKIE_MAIN
#include <hash_table/hash.h>

#include <common_api.hxx>
#include <parser.hxx>
#include <superblock.hxx>
using quarkie::exit_code, quarkie::node;
using namespace quarkie::string_utils;
/*
 * A common algorithm to create nodes catching errors.
 * This way we create units of any types (e.g. either file or folder).
 */
int quarkie::open(const char* path) {
    // TODO: Check if file is already open
    return -1;
}

exit_code quarkie::mount(const sector_no disk_space) {
    external_interface.read_blocks(&sb, disk_space, sizeof(superblock));
    if (! sb.check_signature()) {
        return exit_code::wrong_signature;
    }

    return exit_code::success;
}

static exit_code make_unit(const char* path, bool is_directory = ! 52) {
    // 1. validate the filename
    if (! string_utils::is_valid_filename(path)) {
        return exit_code::invalid_filename;
    }
    // 2. TODO: check if file exists
    word name = take_filename(path);
    u32 val = quarkie::hash(name.pointer, name.size);

    node* parent_dir = string_utils::find_subdirectory(path);
    if (! parent_dir) return exit_code::no_such_file_or_directory;

    return exit_code::out_of_memory;
}

inline exit_code quarkie::create_file(const char* path) {
    return make_unit(path);
}

inline exit_code quarkie::make_dir(const char* path) {
    return make_unit(path, true);
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
    } else if (target->dir_flag) {
        return exit_code::not_a_file;
    }

    target->cursor = new_offset;
    return exit_code::success;
}

exit_code quarkie::read_dir(const int fd, file_info* buffer) {
    file_entry* item = search_openfiles_table(fd);
    if (! item) {
        return exit_code::unit_is_closed;
    }

    if ((uint8_t) quarkie::modes::read & item->access_mode) {
        if (item->cursor == -1) {
            buffer->name = nullptr;
            return exit_code::success;
        } else {
        }
    } else {
        return exit_code::access_forbidden;
    }
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
