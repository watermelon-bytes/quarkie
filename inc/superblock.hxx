#ifndef SUPERBLOCK_HXX
#define SUPERBLOCK_HXX
#include <common_api.hxx>
#include <quarkie_defs.hxx>

namespace quarkie {

struct superblock {
    static constexpr u32 valid_signature = 0xCB7AD;
    u32 signature;
    /**< A kind of FS identificator */
    // static int8_t sector_size;
    u16 block_size;
    sector_no total_blocks;

    uint global_node_counter {1};
    // ^^ Also used to assign identificators to nodes

    // #auxilary
    range find_continuous_space(const uint sectors) const;

    // #auxilary
    constexpr inline uint blocks_needed_for(size_t bytes_count) const {
        return div_and_ceil(bytes_count, block_size);
    }

    /* NOTE: This allocator should be used to a sort of dynamically manage nodes
     * and must be the only way how nodes are created. */
    constexpr static uint nodes_limit = 1000;

    disk_address root;

    const low_level_interface*
        external_interface; /**< The whole interface used by filesystem. Pointer
                               is used for conviniency. */

    explicit superblock(const sector_no given_space,
                        const low_level_interface*);

    node* get_node_by_path(const char*) const;

    bool check_signature() const;
};

// Used to describe currently open files
struct file_entry {
    node* fdescriptor_node;
    uint8_t access_mode;
    union {
        /* For a file. Current r/w offset position */
        uint cursor;

        /* For a directory (i.e. for `quarkie::read_dir`) */
        node* next_item_to_read;
    };
};

constexpr static uint max_open_files = 50;
extern quarkie::pool<file_entry, max_open_files> open_files_table;

extern superblock sb;

}  // namespace quarkie

#endif
