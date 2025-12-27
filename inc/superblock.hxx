#ifndef SUPERBLOCK_HXX
#define SUPERBLOCK_HXX
#include <uchar.h>

#include <bitmap/bitmap.hxx>
#include <common_api.hxx>
#include <cstdint>
#include <file.hxx>
#include <parser.hxx>

namespace quarkie {

// Used to describe currently open files
struct file_entry {
    node* fdescriptor_ptr;
    uint8_t mode;
    uint cursor; /* Current r/w offset position */
};

enum class modes : uint8_t { read = 1 << 0, write = 1 << 1 };

constexpr uint nodes_limit = 1000;
static const char valid_signature[] = "Spare the sympathy";
/* ^^ Can't declare as `const char*` because then `int
strlen(char)`` is needed and thus
`valid_signature_length` cannot be calculated at
compilation time */
static constexpr uint valid_signature_length =
    sizeof(valid_signature) / sizeof(char);

class superblock {
    char8_t signature[valid_signature_length + 1];
    /**< A kind of FS identificator */
    // static int8_t sector_size;
    uint16_t block_size;
    sector_no total_blocks;

    uint global_node_counter {1};
    // ^^ Also used to assign identificators to nodes

    range free_space_start;

    // #auxilary
    range find_continuous_space(const uint sectors) const;

    // #auxilary
    constexpr inline uint blocks_needed_for(size_t bytes_count) const {
        return div_and_ceil(bytes_count, block_size);
    }

public:
    /* NOTE: This allocator should be used to manage nodes dynamically and must
     * be the only way how nodes are created. */
    quarkie::bitmap<node, nodes_limit> node_allocator;
    node root;
    const low_level_interface*
        external_interface; /**< The whole interface used by filesystem. Pointer
                               is used for conviniency. */

    explicit superblock(const sector_no given_space,
                        const low_level_interface*);

    node* get_node_by_path(const char*) const;

    friend node* string_utils::find_file(const char* path);
};

extern superblock sb;

}  // namespace quarkie

#endif
