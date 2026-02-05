#ifndef QUARKIE_DIRECTORY_INODE_HXX
#define QUARKIE_DIRECTORY_INODE_HXX
#include <bitmap/pool.hxx>
#include <common_api.hxx>
#include <quarkie_defs.hxx>
namespace quarkie {

struct directory_item {
    u32 hashed;
    disk_address pointer;
};

/*
 * @brief Stores hashed filenames and
 * Also constants like `max_name_len` show current FS limits.
 */
struct directory_node_t {
    // sector_no next;
    constexpr static uint max_name_len = 64, items_capacity = 18;
    range full_names_infosectors;
    pool<directory_item, items_capacity> items;

    directory_node_t() {}
    /*
     * @brief Looks for hashed_target among the directory items.
     * @return
     */
    error_or<disk_address> lookup(const u32 hashed_target);
};

/*
 * @brief This structure is invented to store full filenames of items (like
 * files) that described directory contains
 */
struct directory_content {
    static constexpr u16 capacity =
        (sector_size - sizeof(u16)) / directory_node_t::max_name_len;
    static constexpr u16 valid_signature = 0xC8DC;
    u16 signature = valid_signature;
    // sector_no next;
    pool<char[directory_node_t::max_name_len], capacity> names;
};

} // namespace quarkie
//
#endif
