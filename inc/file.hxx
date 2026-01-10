#ifndef FILE_H
#define FILE_H
#include <common_api.hxx>
namespace quarkie {

struct disk_address {
    sector_no block;
    u16 offset : 13;  // Assuming that block isnt larger than 4KB
};

struct node {
    u8 is_directory : 1;
    /**< Node type: either directory (when set to 1) or file (clear) */

    u8 readonly : 1;
    /**< "Write" operations permissions global status (doesn't depend on how the
     * file was open) Doesn't mean anything for a directory.
     */

    u8 fragmented : 1;
    /**<
     * If set to 0, then all metadata is contained in
     * `data.descriptors`. Otherwise when sectors descriptors dont
     * fit in 'data.descriptors' it means that reading from
     * 'data.more_sectors' is needed */

    u8 : 0; /* reserved */

    constexpr static char valid_signature[] = "QUARKIE_NODE";
    constexpr static auto valid_signature_length =
        sizeof(valid_signature) / sizeof(char);
    char signature[valid_signature_length];

    i32 identificator; /* speed up searching*/

    constexpr static uint max_name_len = 32;
    /* 32 should be enough for simplicity */
    char name[max_name_len];

    // directory-parent
    disk_address mother;  // (for "/" it equals to `this`)

    disk_address next_node;
    /**< Next node in the current directory (files inside single
     * directory are implemented as a linked list). May be
     * either file or directory*/

    /*===================================================================*/
    union {
        struct {
            disk_address eldest_child; /**<
            Stores a pointer to the head of the linked list of located here file
            descriptors */

            disk_address first_subdir; /**< For a File: Stores linked list
                                   subdirectories. */
        } children;
        /*================================================================*/
        range descriptors[4]; /* For a File: descriptors of the space where the
                               * bits of the file are stored. */
        sector_no more_descriptors;
        /**< The sector that contains just ranges (extended for a File).  */
    } data;
    /*===================================================================*/

    /*NOTE: These public methods all return `exit_code` because their result
        should be used directly returning from common FS API functions (like
       `create_file`, see signatures in `common_api.hxx`) */

    // By accepting a pointer to node*, we assume that the data is already
    // loaded into RAM
    exit_code add_child(node*);  // approximately: this->eldest_child = node;
    exit_code change_parent(node*);

    exit_code remove_child(const i32, bool);
    exit_code remove_child_by_ptr(node*);

    exit_code cleanup_file_space();  // #auxilary for: remove_child

    void set_readonly(bool);  // #auxilary for: make_readonly

    exit_code init(node* parent, bool directory = false);

    /*friend string_utils::word string_utils::take_word(const char*);
    * friend exit_code quarkie::set_name(const char* path, const char*
    new_name); */
};

}  // namespace quarkie

#endif
