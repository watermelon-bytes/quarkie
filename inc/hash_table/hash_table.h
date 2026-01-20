#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <hash_table/hash.h>

namespace quarkie {

class filenames_hash_table {
    struct {
        u64 key;
        disk_address value;
    };
    bool lookup(const char*);
    bool insert(const char*);
};

}  // namespace quarkie

#endif  // !DEBUG
