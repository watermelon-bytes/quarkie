#ifndef QUARKIE_CACHE_HXX
#define QUARKIE_CACHE_HXX

#include <bitmap/basic_bitmap.hxx>
#include <common_api.hxx>
#include <cstdint>
#include <free_space_tracker.hxx>

#include "bitmap/bitmap.hxx"

namespace quarkie {

struct raw_sector {
    uint8_t bytes[free_space::sector_size];
};

constexpr uint cache_size = 12;
/*
 * @class cache
 * Serves as a cache layer between the disk and the RAM. When reading a
 * meta-sector needed (primarily with `nodes`), filesystem first checks if cache
 * contains it. On cache hit the requested information will be immediately
 * returned to the caller. Otherwise cache looks for an unborrowed slot and if
 * doesnt find it, writes out least used cache page back to the disk. Then the
 * necessary block will be loaded to the free slot from the secondary storage.
 */
class cache {
    pool<raw_sector, cache_size> content;
    /**< To check the status of the cache content */
    bool check_cache_hit(const sector_no) const;
    sector_no indeces[cache_size];

public:
    raw_sector* load_sector(const sector_no);
};

}  // namespace quarkie
#endif  // !
