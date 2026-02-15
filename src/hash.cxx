#ifndef QUARKIE_HASH_CXX
#define QUARKIE_HASH_CXX
#include <hash_table/hash.h>
#include <quarkie_defs.hxx>

namespace quarkie {

u32 hash(const void* data, const u16 length) {
    if (! data)
        return 0;
    auto [size, excess] = div(length, 4);

    u32 res = 5381;
    auto arr = reinterpret_cast<const u32*>(data);

    const u32 k = 0x5C3A36FA;
    for (u16 i = 0; i < size; ++i) {
        res += (i + 1) * (k ^ arr[i]);
    }

    if (excess) {
        u32 x {0};
        __builtin_memcpy(&x, &arr[size], excess);
        res += (k ^ x) * (size + 1);
    }
    return 0x1AFD58CDull * res;
}

u64 hash_string(const char* str) {
    return hash(static_cast<const void*>(str), __builtin_strlen(str));
}

} // namespace quarkie

#endif //
