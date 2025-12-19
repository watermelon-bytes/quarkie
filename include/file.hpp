#ifndef FILE_H
#define FILE_H

#include <uchar.h>

#include <cstddef>
#include <cstdint>

#include "bitmap/bitmap.hpp"

#define DEBUG
namespace cheesy {

enum class exit_code : int8_t {
    success = 0,

    no_such_file,
    access_forbidden,
    non_empty_folder,
    not_a_directory,
    not_a_file
};

using sector_no = uint32_t; /* alias for readability and typenames splitting */

/* NOTE: All blocks numbers and addresses are relative to the superblock */

struct range {
    sector_no begin;
    unsigned int len; /*How many sectors ahead belongs to this (logical) piece*/
};

constexpr range nulldescriptor = {0, 0};

exit_code fs_init(const sector_no size,
                  void (*const panic_handler)(const char*));

exit_code make_dir(const char* path);
exit_code create_file(const char* path);

void fs_panic(const char* reason);
// exit_code get_sectors(const char* path);

exit_code make_readonly(const char* path);

/* Removes a node from the tree but if 'recursive' is set to false then deleting
 * non-empty directories will fail */
exit_code remove(const char* path, bool recursive = false);

/* as simple as 'this->name = new_name' */
exit_code set_name(const char* path, const char* new_name);

class node {
protected:
    struct {
        uint8_t directory : 1; /* folder or not */
        uint8_t readonly : 1;  /* write access */

        uint8_t more_sectors : 1;
        /* ^^^^^^^^^^
         * If set to 0, then all metadata is contained in `data.descriptors`. If
         * sectors descriptors dont fit in 'data.descriptors' it means that
         * reading from 'data.more_sectors' is needed */

        uint8_t : 0;  // reserved
    } attributes;

    int32_t identificator;  // for faster searching

    constexpr static uint max_name_len = 32;
    /* 32 should be enough for simplicity */
    char8_t name[max_name_len];

    // directory-parent
    node* mother;  // (for "/" it equals to `this`)

    node* next_node;
    /* Next node in the current directory (files inside single
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
       `create_file`, see signatures above) */

    exit_code add_child(node*);  // approximately: this->eldest_child = node;
    exit_code change_parent(node*);

    exit_code remove_child(int32_t);
    exit_code remove_child_by_ptr(node*);

    exit_code cleanup_file_space();  // #auxilary for: remove_child

    void set_readonly(bool);  // #auxilary for: make_readonly
    explicit node(node* parent, bool directory = false);
};

constexpr size_t capacity = 1500;
constexpr int nodes_limit = 1000;

class superblock {
protected:
    // uint8_t signature[52]; // A kind of FS identificator
    // static int8_t sector_size;
    uint16_t block_size;
    sector_no total_blocks;

    // size_t total_size;

    void (*read_blocks)(sector_no source, size_t, char* destination);
    void (*write_blocks)(const char* source, sector_no destination, size_t);

    node root = node(&root, true);

    bitmap<node, nodes_limit> node_allocator;
    /*  TODO: allocate space to manage it via this bitmap */

    struct {
        range data[capacity];
        size_t size;
    } void_descriptor;  // free space

    // #auxilary
    range find_continuous_space(const unsigned int sectors) const;

    // #auxilary
    inline unsigned int blocks_needed_for(size_t bytes_count) const {
        return div_and_ceil(bytes_count, block_size);
    }

public:
    explicit superblock(
        const sector_no given_space,
        void (*read_disk_api)(sector_no, size_t, char*),
        void (*write_disk_api)(
            const char*, sector_no,
            size_t))  // Low-level API should be provided by the host
    {
        void_descriptor.data[void_descriptor.size++] = {
            .begin = blocks_needed_for(sizeof(superblock)),
            .len = given_space - blocks_needed_for(sizeof(superblock))};

        read_blocks = read_disk_api;
        write_blocks = write_disk_api; /* Register low-level storage read-write
                                        API drivers functions. */
        total_blocks = given_space;

        /* TODO: find space on disk for `node`s, borrow it and insert instead of
         * `nullptr`*/
#ifdef DEBUG
        cout << "Superblock initialized successfully." << endl;
#endif
    }

    node* get_node_by_path(const char*) const;  // #auxilary
};

extern superblock main;

}  // namespace cheesy

#endif
