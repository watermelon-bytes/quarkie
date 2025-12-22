#ifndef SUPERBLOCK_HXX
#define SUPERBLOCK_HXX
#include "bitmap/bitmap.hxx"
#include "file.hxx"
namespace quarkie {
// constexpr size_t capacity = 1500;
constexpr int nodes_limit = 1000;
static const char* valid_signature = "Spare the sympathy";

class superblock {
protected:
    char8_t signature[52]; /**< A kind of FS identificator */
    // static int8_t sector_size;
    uint16_t block_size;
    sector_no total_blocks;

    // size_t total_size;

    void (*read_blocks)(sector_no source, size_t, char* destination);
    void (*write_blocks)(const char* source, sector_no destination, size_t);

    node root = node(&root, true);
    range free_space_start;

    /* to manage nodes dynamically, use this allocator */
    bitmap<node, nodes_limit> node_allocator;

    // #auxilary
    range find_continuous_space(const unsigned int sectors) const;

    // #auxilary
    inline unsigned int blocks_needed_for(size_t bytes_count) const {
        return div_and_ceil(bytes_count, block_size);
    }

public:
    explicit superblock(const sector_no given_space,
                        void (*read_disk_api)(sector_no, size_t, char*),
                        void (*write_disk_api)(const char*, sector_no, size_t));

    node* get_node_by_path(const char*) const;  // #auxilary
};

extern superblock sb;
}  // namespace quarkie

#endif
