#ifndef SPACE_TRACKER_HPP
#define SPACE_TRACKER_HPP
#include <bitmap/bitmap.hxx>
#include <common_api.hxx>

namespace quarkie {
namespace free_space {
using ulong = unsigned long;
void free_space_init(const sector_no size_of_space);

constexpr int sector_size = 512;

struct meta_sector {
    ulong is_free : 1;
    const ulong signature : sizeof(long) * CHAR_BIT - 1;

    constexpr static uint capacity =
        sector_size / sizeof(range) - sizeof(long) - sizeof(uint);
    bitmap<range, capacity> descriptors;

    static constexpr uint sectors_per_subarea = capacity * 2 - 1;
    // I am too lazy to use a calculator
    // But akschually its a compile time feature 🤌

    uint size = 0;  // Current count of records.
    meta_sector() : is_free(1), signature(0xace0) {}
};

struct global_metadata {
    uint subarea_size_in_blocks;
    uint total_subareas;

    bool check_health();
    explicit global_metadata();
};

static_assert(sizeof(meta_sector) <= sector_size,
              "[error] meta_sector doesn't fit into a disk sector");

}  // namespace free_space
}  // namespace quarkie
#endif
