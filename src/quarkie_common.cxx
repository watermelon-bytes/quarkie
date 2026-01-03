#ifndef QUARKIE_MAIN
#define QUARKIE_MAIN
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
        return ! new_node ? exit_code::out_of_memory
                          : new_node->init(parent_dir, is_directory);
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
        __builtin_strcpy(string_utils::find_file(path)->name, new_name);
        return exit_code::success;
    }

    return exit_code::invalid_filename;
}

static quarkie::file_entry* search_openfiles_table(const int fd) {}

exit_code quarkie::change_offset(const int fd, const uint new_offset) {
    file_entry* target = search_openfiles_table(fd);
    if (! target) {
        return exit_code::unit_is_closed;
        /* NOTE: perhaps `unit_is_closed` is a bad naming. If file does not
         * exist, the function will say that file isnt open, thereby assuming it
         * exists but was not open?.. */
    } else if (target->fdescriptor_node->is_directory) {
        return exit_code::not_a_file;
    }

    target->cursor = new_offset;
    return exit_code::success;
}

exit_code quarkie::read_dir(const int fd, quarkie::file_info* buffer) {
    auto* item = search_openfiles_table(fd);
    if (! item) return exit_code::unit_is_closed;

    if ((uint8_t) quarkie::modes::read & item->access_mode) {
        buffer->name = reinterpret_cast<const char*>(&item->next_item_to_read);
        buffer->size = 0;
        return exit_code::success;
    } else {
        return exit_code::access_forbidden;
    }
}

#endif
