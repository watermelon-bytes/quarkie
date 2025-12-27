#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <stdint.h>
#include <sys/types.h>

#include <climits>
#include <cstdint>

#ifdef DEBUG
    #include <bitset>
    #include <cassert>
    #include <iostream>
using std::cout, std::endl, std::bitset;

#endif  // DEBUG

namespace quarkie {

using byte = unsigned char;

inline constexpr static ulong div_and_ceil(ulong a, ulong b) {
    return a / b + (a % b ? 1 : 0);
}

template <typename t, uint slots_count>
class bitmap {
    uint borrowed_slots_counter{0};

    constexpr static ulong bitmap_size = div_and_ceil(slots_count, 8);
    byte bits[bitmap_size]{0};

    t base[slots_count];

    struct {
        short position : 13;
        short offset : 3;
    } last_freed_bit{0, 0};
    /* ^^^^^^^ save here information about what was freed last time
     * to speed up allocating*/

public:
    bitmap();

    [[nodiscard("The allocated slot must be stored and freed after using.")]] t*
    give_slot();

    void free_slot(const t*);

    void reset();

#ifdef DEBUG
    uint get_borrowed_count() const { return borrowed_slots_counter; }
    void dump(int index = -1) const;
#endif
};

};  // namespace quarkie

// ======================================================================
template <typename t, uint slots_count>
quarkie::bitmap<t, slots_count>::bitmap() {
    reset();

    constexpr auto extra_slots = slots_count % 8;
    if constexpr (extra_slots > 0) {
        bits[bitmap_size - 1] |= 0xFF >> extra_slots;
    }
#ifdef DEBUG
    cout << "Bitmap initialized: " << slots_count << " blocks (" << bitmap_size
         << " * 8 bits); each block = " << sizeof(t) << " bytes." << endl;
    dump();
#endif
}

template <typename t, uint blocks_count>
t* quarkie::bitmap<t, blocks_count>::give_slot() {
#ifdef DEBUG  // #unnecessary
    cout << "give_slot() called." << endl;
#endif

    if (borrowed_slots_counter == blocks_count) {
#ifdef DEBUG
        cout << "error: bitmap is fulfilled (borrowed_slots_counter = "
             << borrowed_slots_counter << " and blocks_count = " << blocks_count
             << "), cannot allocate slot" << endl;
#endif
        return nullptr;
    }

    int index = last_freed_bit.position, offset = last_freed_bit.offset;

    if (index != -1) {
#ifdef DEBUG
        if (bits[last_freed_bit.position] & (1 << last_freed_bit.offset)) {
            cout << __FILE__ << ": error: inside `" << __FUNCTION__
                 << "`: `last_freed_bit` looks at a borrowed bit" << endl
                 << "last_freed_bit.position = " << last_freed_bit.position
                 << endl
                 << "last_freed_bit.offset = " << last_freed_bit.offset << endl;
            dump(last_freed_bit.offset);

            exit(-1);
        }
#endif  // DEBUG

        last_freed_bit.position = -1; /* not a free slot anymore */
        goto gotten;
    }

    for (index = 0; index < bitmap_size; ++index) {
        /* skip if no zeroes: */
        if (bits[index] == UINT8_MAX) {
            continue;
        }

        for (offset = 0; offset < 8; offset++) {
            if ((bits[index] | (1 << offset)) != bits[index]) {
                goto gotten;
            }
        }
    }
#ifdef DEBUG
    cout << "[panik!!!] borrowed_slots_counter = " << borrowed_slots_counter
         << " and slots limit = " << blocks_count
         << ", but no free slot found: \n";
    dump();
    cout << endl;
#endif
    return nullptr;

gotten:
    bits[index] |= (1 << offset);
    borrowed_slots_counter++;
    return static_cast<t*>(&base[index * 8 + offset]);
}

template <typename t, uint blocks_count>
void quarkie::bitmap<t, blocks_count>::free_slot(const t* ptr) {
    auto block_ptr = (const byte* const)ptr;
    auto base_ptr = (byte* const)base; /* i love `auto` when rvalue is casted*/

    /* Check alignment */
    std::ptrdiff_t distance = block_ptr - base_ptr;
    if (distance % sizeof(t) || distance > sizeof(t) * blocks_count ||
        distance < 0) {
#ifdef DEBUG
        cout << "[warning] aboting `free_slot`: given block address not "
                "aligned or is located outside accessible poll"
             << endl;
#endif
        return;
    }
    const int index = (block_ptr - base_ptr) / sizeof(t) / CHAR_BIT,
              bit_no = ((block_ptr - base_ptr) / sizeof(t)) % CHAR_BIT;
    /* CHAR_BIT = 8 on most modern systems. It shows the number of bits that in
     * a byte */

    /* #ifdef DEBUG
        cout << "free_slot: called on bit " << bit_no << " of byte " << index
             << endl;
    #endif */
    bits[index] &= ~(1 << bit_no);
    borrowed_slots_counter--;

    last_freed_bit.offset = bit_no;
    last_freed_bit.position = index;
}

#ifdef DEBUG
// #debug #auxilary
template <typename t, uint blocks_count>
void quarkie::bitmap<t, blocks_count>::dump(int index) const {
    if (index < 0) {
        for (const byte& slot_set : bits) {
            cout << bitset<CHAR_BIT>(slot_set) << endl;
        }
        return;
    }
    cout << "============" << endl;
    cout << "bitmap: byte " << index << ": [" << bitset<CHAR_BIT>(bits[index])
         << "]" << endl;
    cout << "============" << endl;
}
#endif  // DEBUG

template <typename t, uint blocks_count>
void quarkie::bitmap<t, blocks_count>::reset() {
    for (byte& b : bits) {
        b = 0;
    }
    borrowed_slots_counter = 0;
    last_freed_bit.position = last_freed_bit.offset = 0;
}

#endif
