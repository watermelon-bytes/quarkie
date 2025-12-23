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

    node root = node(&root, true);

    range free_space_start;

    /* This allocator should be used to manage nodes dynamically and must be the
     * only way nodes are created. */
    bitmap<node, nodes_limit> node_allocator;

    // #auxilary
    range find_continuous_space(const unsigned int sectors) const;

    // #auxilary
    inline unsigned int blocks_needed_for(size_t bytes_count) const {
        return div_and_ceil(bytes_count, block_size);
    }

public:
    char* (*read_blocks)(sector_no source, const size_t);
    void (*write_blocks)(const char* source, sector_no destination, size_t);

    explicit superblock(const sector_no given_space,
                        char* (*read_disk_api)(sector_no, size_t),
                        void (*write_disk_api)(const char*, sector_no, size_t));
};

extern superblock sb;
}  // namespace quarkie

#endif
