#ifndef QUARKIE_DEFS
#define QUARKIE_DEFS

#include <climits>
#include <cstddef>
#include <cstdint>
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using i32 = int32_t;

using uint = uint32_t;
using ulong = uint64_t;

using u16 = uint16_t;
using i16 = int16_t;
using i8 = int8_t;
using u8 = uint8_t;

namespace quarkie {

using sector_no = uint32_t;
class node;

struct disk_address {
    sector_no block;
    u16 offset : 13;  // Assuming that block isnt larger than 4KB
};

struct div_t;

constexpr uint sector_size = 512;

}  // namespace quarkie

#endif
