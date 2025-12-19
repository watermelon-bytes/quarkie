#ifndef SPACE_TRACKER_CPP
#define SPACE_TRACKER_CPP
#include "../include/free_space_tracker.hpp"

#include <cstdlib>

#include "../include/file.hpp"

namespace cheesy {

template <int sector_size>
class space_tracker {
public:
    space_tracker();
};

template <unsigned short int sector_size>
struct [[gnu::packed]] meta_sector {
    const long signature = 0xDEADCAFE;
    bool is_filled;

    constexpr static size_t size = sector_size / sizeof(range) - 1;
    range descriptors[size];
};

}  // namespace cheesy

#endif  // SPACE_TRACKER_CPP
