#ifndef SPACE_TRACKER_CPP
#define SPACE_TRACKER_CPP
#include <free_space_tracker.hxx>

namespace quarkie {

template <unsigned short int sector_size>
struct [[gnu::packed]] meta_sector {
    const long signature = 0xDEADCAFE;
    bool is_filled;

    constexpr static size_t size = sector_size / sizeof(range) - 1;
    range descriptors[size];
};

}  // namespace quarkie

#endif  // SPACE_TRACKER_CPP
