#ifndef QUARKIE_COMMON_API_HXX
#define QUARKIE_COMMON_API_HXX

#include <cstdint>
using uint = unsigned int;

namespace quarkie {

enum class exit_code : int8_t {
    success = 0,
    no_such_file,
    access_forbidden,
    non_empty_folder,
    not_a_directory,
    not_a_file
};

using sector_no = uint32_t; /* alias for readability and typenames splitting */

/* NOTE: All blocks numbers and addresses are relative to the superblock */

struct range {
    sector_no begin;
    uint len; /**<
        How many sectors ahead belongs to this (logical) piece*/
};

constexpr range nulldescriptor = {.begin = 0, .len = 0};

/*
 *
 *
 */
exit_code mount(const sector_no disk_space);

exit_code fs_init(const sector_no size,
                  void (*const panic_handler)(const char*));

exit_code make_dir(const char* path);
exit_code create_file(const char* path);

exit_code open(const char* path);
exit_code close(int id);

void fs_panic(const char* reason);
// exit_code get_sectors(const char* path);

exit_code make_readonly(const char* path);

/*
 * @brief Removes a node from the tree but if 'recursive' is set to false then
 * deleting non-empty directories will fail
 */
exit_code remove(const char* path, bool recursive = false);

/* as simple as 'this->name = new_name' */
exit_code set_name(const char* path, const char* new_name);

}  // namespace quarkie
#endif
