#ifndef SUPERBLOCK_CPP
#define SUPERBLOCK_CPP

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
    : total_blocks(given_space),
      external_interface(external_api),
      signature(valid_signature) {
#ifdef DEBUG
    cout << "Superblock initialized successfully." << endl;
#endif
}

bool quarkie::superblock::check_signature() const {
    return this->signature == valid_signature;
}

#endif
