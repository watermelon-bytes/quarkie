#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <hash_table/hash.h>

#include <file.hxx>

namespace quarkie {

class filenames_hash_table {
    struct {
        u64 hash_val;
        disk_address addr;
    };
    bool lookup(const char*);
    bool insert(const char*);
};

}  // namespace quarkie

#endif  // !DEBUG
