#ifndef NODE_CPP
#define NODE_CPP
#include <sys/types.h>

#include <climits>
#include <inode_struct/node.hxx>
#include <parser.hxx>
#include <superblock.hxx>

namespace quarkie {

exit_code node::init(disk_address parent_ptr, bool dir_indicator) {
    return exit_code::success;
}

exit_code node::remove_child(const u32 val, bool dir) {}

// node* superblock::get_node_by_path(const char*) {}

exit_code make_readonly(const char* path) {
    node* target = sb.get_node_by_path(path);
    target->set_readonly(true);

    return exit_code::success;
}

error_or<disk_address> directory_node_t::lookup(const u32 hashed_target) {
    u32 chunk;
    bool zeroes_turn = 1;

    for (uint i = 0; i < items_capacity; i += sizeof(chunk) / CHAR_BIT) {
        uint offset = 0;
        __builtin_memcpy(&chunk, &items.allocator_.map[i], sizeof(chunk));
        while (chunk != 0) {
            uint leading_zeroes_count = __builtin_clz(chunk);
            chunk <<= leading_zeroes_count;
            offset += leading_zeroes_count;
            if (zeroes_turn) {
                // now we use leading_zeroes_count as loop counter
                // because theres too many variables, so to avoid using stack
                // and keeping all local variables in registers, we shouldn't
                // add more counters just for beauty
                while (leading_zeroes_count > 0) {
                    uint const index = i + offset - leading_zeroes_count;
                    if (hashed_target == items.base[index].hashed) {
                        return error_or(items.base[index].pointer);
                    }
                    leading_zeroes_count--;
                }
            }
            zeroes_turn = ~zeroes_turn;
            chunk = ~chunk;
        }
    }

    return error_or<disk_address>(exit_code::no_such_file_or_directory);
}

} // namespace quarkie
#endif
