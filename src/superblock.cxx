#ifndef SUPERBLOCK_CPP
#define SUPERBLOCK_CPP
#include <superblock.hxx>

quarkie::superblock::superblock(
    const sector_no given_space,  // How many sectors on disk belongs to us
    char* (*read_disk_api)(const sector_no, const size_t),
    void (*write_disk_api)(const char*, sector_no, size_t))
// Low-level API should be provided by the host
{
    // like strcpy()
    for (int i = 0; valid_signature[i] != '\0'; i++) {
        signature[i] = valid_signature[i];
    }

    read_blocks = read_disk_api;
    write_blocks = write_disk_api; /* Register low-level storage read-write
                                    API drivers functions. */
    total_blocks = given_space;

#ifdef DEBUG
    cout << "Superblock initialized successfully." << endl;
#endif
}

#endif
