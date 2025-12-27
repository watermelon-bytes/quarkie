#ifndef QUARKIE_COMMON_API_HXX
#define QUARKIE_COMMON_API_HXX

#include <cstddef>
#include <cstdint>
using uint = unsigned int;

namespace quarkie {

enum class exit_code : int8_t {
    success = 0,
    no_such_file = 2,
    access_forbidden = 8,
    non_empty_folder = 11,
    not_a_directory = 9,
    not_a_file = 10,
    out_of_memory = 127,
    invalid_parental_dir = 5
};

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
 * @brief Marks the file as open.
 */
int open(const char* path);

/*
 *
 */
exit_code close(int id);

// exit_code get_sectors(const char* path);

exit_code make_readonly(const char* path);

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
