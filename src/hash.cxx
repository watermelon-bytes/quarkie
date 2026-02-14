#ifndef QUARKIE_HASH_CXX
#define QUARKIE_HASH_CXX
#include <hash_table/hash.h>
#include <quarkie_defs.hxx>

namespace quarkie {

u32 hash(const void* data, const u16 size) {
    div_t actual_size = div(size, 4);
    if (! data || actual_size.rem % 4)
        return 0;

    u32 res = 5381;
    auto arr = reinterpret_cast<const u32*>(data);

    for (u16 i = 0; i < actual_size.quot; ++i) {
        u32 k = 0x5c3a36fa;
        k ^= arr[i];
        res += k * i;
    }

    return 0x1afd58cdull * res;
}

u64 hash_string(const char* str) {
    return hash(static_cast<const void*>(str), __builtin_strlen(str));
}

} // namespace quarkie

#endif //
