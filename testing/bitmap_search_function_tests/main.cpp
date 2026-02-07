#define DEBUG 1
#include <hash_table/hash.h>

#include <cassert>
#include <inode_struct/directory_node.hxx>

int main() {
    quarkie::low_level_interface my_interface;
    my_interface.read_blocks;
    my_interface.write_blocks;

    quarkie::directory_node_t my_directory;
    my_directory.items.allocator_.map[1] = 0xff;

    u32 my_hash = quarkie::hash_string("eiogjeoigjeiopgjweoipgjiowegjwoepj");
    quarkie::disk_address addr = {52, 42};
    my_directory.items.base[14] = quarkie::directory_item {my_hash, addr};

    auto res = my_directory.lookup(my_hash);
    assert(res.got_error == 0);
    assert(__builtin_memcmp(&res.value, &addr, sizeof(addr)));
}
