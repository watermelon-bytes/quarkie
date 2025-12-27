#ifndef SUPERBLOCK_CPP
#define SUPERBLOCK_CPP
#include <common_api.hxx>
#include <superblock.hxx>

/*
 * @param given_space How many sectors on disk belongs to the filesystem at its
 * initialisation moment
 * @param external_api The low-level interface that host (e.g. kernel) should
 * provide.
 * NOTE: Filesystem's API doesn't check for the interface correctness.
 * It just calls what's given.
 */
quarkie::superblock::superblock(const sector_no given_space,
                                const low_level_interface* external_api)
    : total_blocks(given_space), external_interface(external_api) {
    // strcpy()?
    for (int i = 0; valid_signature[i] != '\0'; i++) {
        signature[i] = valid_signature[i];
    }
    signature[valid_signature_length] = 0;

    root.init(&root, 52);
#ifdef DEBUG
    cout << "Superblock initialized successfully." << endl;
#endif
}

#endif
