#ifndef FILE_H
#define FILE_H
#include <uchar.h>

#include "common_api.hxx"

namespace quarkie {

class node {
protected:
    struct {
        uint8_t directory
            : 1; /**< Node type: either directory (set to 1) or file (clear) */
        uint8_t readonly : 1; /**< "Write" operations permissions status */

        uint8_t more_sectors
            : 1; /**<
                  * If set to 0, then all metadata is contained in
                  * `data.descriptors`. Otherwise when sectors descriptors dont
                  * fit in 'data.descriptors' it means that reading from
                  * 'data.more_sectors' is needed */

        uint8_t : 0; /* reserved */
    } attributes;

    int32_t identificator;  // for faster searching

    constexpr static uint max_name_len = 32;
    /* 32 should be enough for simplicity */
    char8_t name[max_name_len];

    // directory-parent
    node* mother;  // (for "/" it equals to `this`)

    node* next_node;
    /**< Next node in the current directory (files inside single
     * directory are implemented as a linked list). May be
     * either file or directory*/

    union {
        /* For a Directory: pointer to the head of the linked list of located
         * here files and subdirectories descriptors */
        node* eldest_child;  // to travel through directory we follow the
                             // pointer to next node

        range descriptors[4]; /* For a File: descriptors of the space where the
                               * bits of the file are stored. */
        sector_no more_descriptors;
        /* The sector that contains just ranges (extended for a File).   */
    } data;

    // inline bool is_directory() const { return flags.directory; }
    // inline bool is_readonly() const { return flags.readonly; }
public:
    /*NOTE: These public methods all return `exit_code` because their result
        will be used directly returning from common FS API functions (like
       `create_file`, see signatures in `common_api.hxx`) */

    exit_code add_child(node*);  // approximately: this->eldest_child = node;
    exit_code change_parent(node*);

    exit_code remove_child(int32_t);
    exit_code remove_child_by_ptr(node*);

    exit_code cleanup_file_space();  // #auxilary for: remove_child

    void set_readonly(bool);  // #auxilary for: make_readonly
    explicit node(node* parent, bool directory = false);
};

}  // namespace quarkie

#endif
