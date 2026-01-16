#ifndef QUARKIE_HASH_CXX
#define QUARKIE_HASH_CXX

#include <hash_table/hash.hxx>

namespace quarkie {

u64 hash(const void* data, const u16 size) {
    if (! data || ! *(char*) data) return 0;

    u64 res = 5381;
    auto arr = reinterpret_cast<const u8*>(data);

    for (u16 i = 0; i < size; ++i) {
        res += i ^ 0x5ce3a36faa;
        res *= arr[i] * (i + 52);
    }

    return 0xff51afd7ed558ccdull * res + size;
}

u64 hash_string(const char* str) {
    return hash(static_cast<const void*>(str), __builtin_strlen(str));
}

}  // namespace quarkie

#endif  //
