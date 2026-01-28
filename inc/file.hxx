#ifndef FILE_H
#define FILE_H
#include <common_api.hxx>

#include "bitmap/bitmap.hxx"
#include "quarkie_defs.hxx"
namespace quarkie {

struct disk_address {
    sector_no block;
    u16 offset : 13;  // Assuming that block isnt larger than 4KB
};

struct directory_item {
    u32 hashed;
    disk_address pointer;
};

struct directory_node_t {
    // sector_no next;
    constexpr static uint max_name_len = 64, capacity = 18;

    u16 count;
    range full_names_infosector;
    pool<directory_item, capacity> items;

    directory_node_t() : count(0) {}
};

struct directory_content {
    static constexpr auto sector_size = 512;
    static constexpr u16 capacity =
        (sector_size - sizeof(u16)) / directory_node_t::max_name_len;
    static constexpr u16 valid_signature = 0xC8DC;
    u16 signature = valid_signature;
    // sector_no next;
    char names[directory_node_t::max_name_len][capacity];
};

struct file_node_t {
    uint size_in_bytes;
    u8 big : 1;
    union {
        struct {
            u8 fragmented : 1;
            range descriptors[24];
            sector_no more_descriptors;
        } meta;
        char small_file[235];
    };
    exit_code cleanup_file_space();  // #auxilary for: remove_child
};

struct node {
    static constexpr u16 valid_signature = 0xC8AE;
    u16 signature = valid_signature;
    i32 id; /* speed up searching*/

    u8 is_directory : 1;
    /**< Node type: either directory (when set to 1) or file (clear). Checking
     * this bit is necessary before reading bits of the union below. */

    u8 readonly : 1;
    /**< "Write" operations permissions global status (doesn't depend on how the
     * file was open) Doesn't mean anything for a directory.
     */

    union {
        directory_node_t dir;
        file_node_t file;
    };

    /*NOTE: These public methods all return `exit_code` because their result
    should be used directly returning from common FS API functions (like
   `create_file`, see signatures in `common_api.hxx`) */

    // By accepting a pointer to node*, we assume that the data is already
    // loaded into RAM
    exit_code add_child(node*);  // approximately: this->eldest_child = node;
    exit_code change_parent(node*);

    exit_code remove_child(const i32, bool);

    void set_readonly(bool);  // #auxilary for: make_readonly

    exit_code init(node* parent, bool directory = false);

    /*friend string_utils::word string_utils::take_word(const char*);
    * friend exit_code quarkie::set_name(const char* path, const char*
    new_name); */
};
static_assert(sizeof(node) <= 256,
              "Size of `node` must not be greater than 256.");

}  // namespace quarkie

#endif
