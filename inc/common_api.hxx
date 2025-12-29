#ifndef QUARKIE_COMMON_API_HXX
#define QUARKIE_COMMON_API_HXX

#include <cstddef>
#include <cstdint>
using uint = unsigned int;

namespace quarkie {

enum class exit_code : int8_t {
    success = 0,
    no_such_file = 2,
    no_such_file_or_directory = 3,
    access_forbidden = 8,
    non_empty_folder = 11,
    not_a_directory = 9,
    not_a_file = 10,
    out_of_memory = 127,
    invalid_parental_dir = 5,
    invalid_filename = 8,
    open_files_limit_reached = 20
};

enum class modes : uint8_t { read = 1 << 0, write = 1 << 1 };

using sector_no = uint32_t; /* alias for readability and typenames splitting */

/* NOTE: All blocks numbers and addresses are relative to the superblock */

struct range {
    sector_no begin;
    uint len; /**<
        How many sectors ahead belongs to this (logical) piece*/
};

struct low_level_interface {
    char* (*read_blocks)(sector_no source, const size_t);
    void (*write_blocks)(const char* source, sector_no destination, size_t);
    void (*fs_panic)(exit_code error, const char* reason);
};

constexpr range nulldescriptor = {.begin = 0, .len = 0};

#ifdef __cplusplus
extern "C" {
#endif

exit_code mount(const sector_no disk_space);

exit_code fs_init(const sector_no size,
                  void (*const panic_handler)(const char*));

exit_code make_dir(const char* path);
exit_code create_file(const char* path);

/*
 * @brief Locates the file it into the table of open units and returns a
 * descriptor that will be used to perform other operations on this file.
 */
int open(const char* path);

/*
 * @brief Looks for `fd` in the table. If found, deletes the descriptor from the
 * table and returns 0 (`success`). Otherwise, returns `no_such_file`.
 */
exit_code close(int id);

// exit_code get_sectors(const char* path);

exit_code make_readonly(const char* path);

exit_code write_to(const int fd, char* from_where, const uint size);

exit_code read_from(const int fd, char* to_where, const uint size);

exit_code change_offset(const int fd, const uint new_offset);

/*
 * @brief Removes a node from the tree but if 'recursive' is set to `false` then
 * deleting non-empty directories will fail
 */
exit_code remove(const char* path, bool recursive = false);

/* as simple as 'this->name = new_name' */
exit_code set_name(const char* path, const char* new_name);

#ifdef __cplusplus
}  // extern "C"
#endif
}  // namespace quarkie
#endif
