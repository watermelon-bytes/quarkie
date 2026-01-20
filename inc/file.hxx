#ifndef FILE_H
#define FILE_H
#include <common_api.hxx>
namespace quarkie {

struct disk_address {
    sector_no block;
    u16 offset : 13;  // Assuming that block isnt larger than 4KB
};

template <uint sector_size>
struct [[gnu::packed]] directory_content {
    sector_no next;
    constexpr static auto max_name_len = 32;
    constexpr static uint capacity =
        (sector_size - sizeof(sector_no)) / max_name_len;
    char names[max_name_len][capacity];
};

struct directory {
    sector_no next;
    constexpr static auto max_name_len = 32;
    constexpr static uint capacity = 10;

    struct {
        char name[max_name_len];
        disk_address addr;
    } items[capacity];
};

struct file {
    u8 big : 1;
    union {
        struct {
            u8 fragmented : 1;
            range descriptors[28];
            sector_no more_descriptors;
        } meta;
        char small_file[235];
    };
    exit_code cleanup_file_space();  // #auxilary for: remove_child
};

struct node {
    constexpr static char valid_signature[] = "QUARKIE_NODE";
    constexpr static auto valid_signature_length =
        sizeof(valid_signature) / sizeof(char);
    char signature[valid_signature_length];
    uint size_bytes;
    i32 identificator; /* speed up searching*/

    u8 is_directory : 1;
    /**< Node type: either directory (when set to 1) or file (clear). Checking
     * this bit is necessary before reading bits of the union below. */

    u8 readonly : 1;
    /**< "Write" operations permissions global status (doesn't depend on how the
     * file was open) Doesn't mean anything for a directory.
     */

    union {
        directory dir;
        file f;
    };

    /*NOTE: These public methods all return `exit_code` because their result
    should be used directly returning from common FS API functions (like
   `create_file`, see signatures in `common_api.hxx`) */

    // By accepting a pointer to node*, we assume that the data is already
    // loaded into RAM
    exit_code add_child(node*);  // approximately: this->eldest_child = node;
    exit_code change_parent(node*);

    exit_code remove_child(const i32, bool);
    exit_code remove_child_by_ptr(node*);

    void set_readonly(bool);  // #auxilary for: make_readonly

    exit_code init(node* parent, bool directory = false);

    /*friend string_utils::word string_utils::take_word(const char*);
    * friend exit_code quarkie::set_name(const char* path, const char*
    new_name); */
};

}  // namespace quarkie

#endif
