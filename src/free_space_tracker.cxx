#ifndef SPACE_TRACKER_CPP
#define SPACE_TRACKER_CPP
#include <climits>
#include <common_api.hxx>
#include <free_space_tracker.hxx>

namespace quarkie {

struct meta_sector {
    long is_free : 1;
    const long signature : sizeof(long) * CHAR_BIT - 1 = 0xace0;

    constexpr static uint capacity =
        sector_size / sizeof(range) - sizeof(long) - sizeof(uint);
    range descriptors[capacity];
    uint size = 0;  // Current count of records.
    meta_sector() = default;
};

}  // namespace quarkie

#endif  // SPACE_TRACKER_CPP
