#ifndef SPACE_TRACKER_CPP
#define SPACE_TRACKER_CPP
#include <common_api.hxx>
#include <free_space_tracker.hxx>
#include <superblock.hxx>

void free_space_init(const sector_no sectors_to_split) {
    const meta_sector empty;
    for (sector_no curr = 0; curr < sectors_to_split;
         curr += sectors_per_subarea) {
        sb.external_interface->write_blocks(
            reinterpret_cast<const char*>(&empty), curr, 1);
    }
}

}  // namespace quarkie

#endif  // SPACE_TRACKER_CPP
