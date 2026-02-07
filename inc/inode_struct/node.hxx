#ifndef FILE_H
#define FILE_H
#include <inode_struct/directory_node.hxx>

namespace quarkie {

struct file_node_t {
    uint size_in_bytes;
    u8 big : 1; /**< This bit is enabled when the `small_file` field must be
                   used to read the file. */
    union {
        struct {
            u8 fragmented : 1;
            range descriptors[24];
            sector_no more_descriptors;
        } meta;
        char small_file[235];
    };
    exit_code cleanup_file_space(); // #auxilary for: remove_child
};

struct alignas(256) node {
    static constexpr u16 valid_signature = 0x7CAE;
    u16 signature = valid_signature;
    i32 id; /* speed up searching*/

    u8 is_directory : 1;
    /**< Node type: either directory (when set to 1) or file (clear). Checking
     * this bit is necessary before reading bits of the union below. */

    u8 readonly : 1;
    /**< "Write" operations permissions global status (doesn't depend on how the
     * file was open). */

    // TODO: comment the followign fields for Doxygen
    union {
        directory_node_t dir;
        file_node_t file;
    };

    /*NOTE: These public methods all return `exit_code` because their result
     * should be used directly returning from common FS API functions (like
     *`create_file`, see signatures in `common_api.hxx`) */

    // By accepting a pointer to node*, we assume that the data is already
    // loaded into RAM
    exit_code add_child(node*); // approximately: this->eldest_child = node;
    exit_code change_parent(node*);

    exit_code remove_child(const u32, bool);

    /* @brief Sets the bit `readonly` to what's passed in `val`. */
    void set_readonly(bool val);

    /*
     *@brief Checks whether the object contains valid_signature. This function
     * should be used to validate raw sectors. Takes a few cycles but ensures
     * the correctness. (Maybe we should extend signature size to 64 bits?)
     */
    bool has_valid_signature() const {
        return valid_signature == this->signature;
    }
    exit_code init(disk_address parent, bool directory = false);
    node() {} // Constructors are EVIL
};
static_assert(sizeof(node) <= 256, "Size of `node` greater than 256.");

} // namespace quarkie

#endif
