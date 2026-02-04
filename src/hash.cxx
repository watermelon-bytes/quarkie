#ifndef QUARKIE_HASH_CXX
#define QUARKIE_HASH_CXX
#include <hash_table/hash.h>

namespace quarkie {

u32 hash(const void* data, const u16 size) {
    if (! data) return 0;

    u32 res = 5381;
    auto arr = reinterpret_cast<const u8*>(data);

    for (u16 i = 0; i < size; ++i) {
        u32 k = 0x5c3a36fa;
        for (uint shift = 0; shift < 4; ++shift) {
            k ^= (i & (0b11 << (shift * 4))) << (shift * 16);
        }
        res += k;
        res *= arr[i] * (i + 52);
    }

    return 0x1afd58cdull * res;
}

u64 hash_string(const char* str) {
    return hash(static_cast<const void*>(str), __builtin_strlen(str));
}

}  // namespace quarkie

#endif  //
