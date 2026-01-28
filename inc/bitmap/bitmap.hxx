#ifndef BITMAP_HPP
#define BITMAP_HPP
#include <sys/types.h>

#include <bitmap/basic_bitmap.hxx>
#include <climits>
#include <quarkie_defs.hxx>

#define DEBUG

#ifdef DEBUG

#include <bitset>
#include <cassert>
#include <iostream>
using std::cout, std::endl, std::bitset;

#endif  // DEBUG

using byte_t = unsigned char;
namespace quarkie {

template <typename t, uint slots_count>
struct pool {
    static_assert(
        slots_count % (sizeof(u64) / CHAR_BIT) == 0,
        "The number of slots not exceeding 8 is not supported (yet).");
    constexpr pool<t, slots_count>();

    [[nodiscard("The allocated slot must be stored and freed after using.")]]
    t* give_slot();
    void free_slot(const t*);

    quarkie::basic_bitmap<slots_count> allocator_;
    // That trick with anonymous unions to prevent initialisation code (why isnt
    // there a compiler attribute at least to do this????)
    union {
        t base[slots_count];
    };

    uint borrowed_slots_counter_ {0};
    int last_freed_bit_index_;
    /* ^^^^^^^ save here information about what was freed last time
     * to speed up allocating*/
};

template <typename t, uint slots_count>
t* pool<t, slots_count>::give_slot() {
    if (borrowed_slots_counter_ == slots_count) {
        return nullptr;
    }
    uint target_bit;
    if (last_freed_bit_index_ != -1) {
        target_bit = last_freed_bit_index_;
        last_freed_bit_index_ = -1;
        goto found_slot;
    }

    for (uint i {0}; i < allocator_.bitmap_size; i += CHAR_BIT) {
        u64 n;
        __builtin_memcpy(&n, &allocator_.map[i], sizeof(n));
        /* Case where all slots are borrowed */
        if (n == 0xFFFF'FFFF'FFFF'FFFF) {
            continue;
        }

        // now (NOT n) cannot equal to 0
        // thereby we exclude possible undefined result that x86's ctz does when
        // input = 0
        uint free_slot_index = __builtin_clz(~n);
        target_bit = (i * CHAR_BIT) + free_slot_index;
        goto found_slot;
    }
    return nullptr;

found_slot:
    allocator_.enable_bit(target_bit);
    borrowed_slots_counter_++;
    return &base[target_bit];
}

template <typename t, uint slots_count>
void pool<t, slots_count>::free_slot(const t* ptr) {
    if ((void*) ptr >= (void*) &base[allocator_.bitmap_size]) {
        return;
    }
    auto block_ptr = (const byte_t* const) ptr;
    auto base_ptr = (byte_t*) &base; /* i love `auto` when rvalue is casted*/
    long distance = block_ptr - base_ptr;

    /* Check alignment */
    if (distance % sizeof(t) || distance < 0) {
#ifdef DEBUG
        cout << "[warning] `" << __PRETTY_FUNCTION__
             << "`: given block address not aligned or is located outside "
                "accessible pool"
             << endl;
#endif
        return;
    }

    long index = ptr - base;
    allocator_.clear_bit(index);
    last_freed_bit_index_ = index;
    borrowed_slots_counter_--;
}

template <typename t, uint slots_count>
constexpr pool<t, slots_count>::pool()
    : allocator_(), last_freed_bit_index_(0), borrowed_slots_counter_(0) {}

}  // namespace quarkie

#endif
