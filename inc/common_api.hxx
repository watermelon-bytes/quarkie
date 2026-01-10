#ifndef QUARKIE_COMMON_API_HXX
#define QUARKIE_COMMON_API_HXX

#include <quarkie_defs.hxx>

namespace quarkie {

enum class exit_code : int8_t {
    success = 0,
    no_such_file_or_directory = 3,
    access_forbidden = 8,
    already_exists = 7,
    non_empty_folder = 11,
    not_a_directory = 9,
    not_a_file = 10,
    out_of_memory = 127,
    invalid_parental_dir = 5,
    invalid_filename = 8,
    open_files_limit_reached = 29,
    unit_is_closed = 10
};

enum class modes : uint8_t {
    read = 1 << 0,
    write = 1 << 1,
    move = 1 << 2,  // Also allows to delete units
    link = 1 << 3,
};

struct file_info {
    const char* name;
    uint size;
    // time_t last_modified, created; // <- the `time_t` type comes from stdlib
    // (we require freestanding compilation)
};

/* NOTE: All blocks numbers and addresses are relative to the superblock */

struct range {
    sector_no begin;
    uint length; /**<
        How many sectors ahead belongs to this (logical) piece*/
};

struct low_level_interface {
    int (*read_blocks)(char* ready_buffer, sector_no source, const size_t);
    /**<  */

    int (*write_blocks)(const char* source, sector_no destination,
                        const size_t);
    /**< */

    void (*fs_panic)(exit_code error, const char* reason);
    /**< */
};

#ifdef __cplusplus
extern "C" {
#endif

exit_code mount(const sector_no disk_space);

exit_code fs_init(const sector_no size, const low_level_interface*);

inline exit_code make_dir(const char* path);

exit_code read_dir(const int fd, file_info* buf);
// Caller should prepare buffer and pass it ^^^ there through `buf`

inline exit_code create_file(const char* path);

/*
 * @brief Locates the file it into the table of open units and returns a
 * descriptor that must be used to perform other operations such as
 * `read_from()` or `write_to()` on this file. */
int open(const char* path);

/*
 * @brief Looks for `fd` in the global filetable. If found, deletes the
 * descriptor from the table and returns 0 (`success`). Otherwise, returns
 * `no_such_file`.
 */
exit_code close(int id);

// exit_code get_sectors(const char* path);

exit_code make_readonly(const char* path);

exit_code write_to(const int fd, char* from_where, const uint size);

/*
 * @param fd Indicated which file is the target.
 * @param to_where Externally allocated buffer in memory of size `buf_size` */
exit_code read_from(const int fd, char* to_where, const uint buf_size);

/*
 * @brief Sets offset of the file pointed to by `fd` to value given in
 * `new_offset` */
exit_code change_offset(const int fd, const uint new_offset);

/*
 * @brief Removes a node from the tree but if 'recursive' is set to `false` then
 * deleting non-empty directories will fail */
exit_code remove_unit(const char* path, bool recursive = ! 52);

/* as simple as 'this->name = new_name' */
exit_code set_name(const int fd, const char* new_name);

class file_entry;
static quarkie::file_entry* search_openfiles_table(const int fd);

#ifdef __cplusplus
}  // extern "C"
#endif
}  // namespace quarkie
#endif
