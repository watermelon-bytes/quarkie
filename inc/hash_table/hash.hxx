#ifndef QUARKIE_HASH_HXX
#define QUARKIE_HASH_HXX
#include <quarkie_defs.hxx>

namespace quarkie {

u64 hash(const void* data, const u16 size);
u64 hash_string(const char* data);

}  // namespace quarkie

#endif
