#ifndef BITMAP_CPP
#define BITMAP_CPP

#include "../include/bitmap.hpp"

#include <sys/types.h>

#ifdef DEBUG
#include <cassert>
#endif  // DEBUG

#include <climits>

template <typename t, unsigned int blocks_count>
t* cheesy::bitmap<t, blocks_count>::give_slot() {
#ifdef DEBUG  // #unnecessary
    cout << "give_slot() called." << endl;
#endif

    if (borrowed_slots_counter == blocks_count) {
    fulfilled:
#ifdef DEBUG
        cout << "error: bitmap is fulfilled, cannot allocate slot" << endl;
        exit(-1);
#endif
        return nullptr;
    }

    if (last_freed_bit.position != -1) {
#ifdef DEBUG
        if (bits[last_freed_bit.position] & (1 << last_freed_bit.offset)) {
            cout << __FILE_NAME__ << ": error: inside `" << __FUNCTION__
                 << "`: `last_freed_bit` looks at a borrowed bit" << endl
                 << "last_freed_bit.position = " << last_freed_bit.position
                 << endl
                 << "last_freed_bit.offset = " << last_freed_bit.offset << endl;
            dump(last_freed_bit.offset);

            exit(-1);
        }
#endif  // DEBUG

        const int index = last_freed_bit.position;
        last_freed_bit.position = -1;  // not a free slot anymore

        bits[index] |= (1 << last_freed_bit.offset);

        return static_cast<t*>(&base[index + last_freed_bit.offset]);
        /* don't know whether it's possible to change `last_freed_bit.position`
         * right when returning */
    }

    for (unsigned i = 0; i < blocks_count; ++i) {
        // fast check
        if (~(bits[i]) == 0) {
            continue;
        }

        for (unsigned off = 0; off < 8; off++) {
            if ((bits[i] | (1 << off)) != bits[i]) {
#ifdef DEBUG
                cout << "give_slot(): returning: "
                     << (void*)(base + sizeof(t) * (i * 8 + off)) << endl;
#endif

                bits[i] |= (1 << off);
                borrowed_slots_counter++;
                return static_cast<t*>(&base[i * 8 + off]);
            }
        }
    }
    goto fulfilled;
}

template <typename t, unsigned int blocks_count>
void cheesy::bitmap<t, blocks_count>::free_slot(const t* ptr) {
    auto block = (const byte* const)ptr;
    auto base_ptr = (byte* const)base;

    /* Check alignment */
    if ((block - base_ptr) % sizeof(block) != 0) {
#ifdef DEBUG
        cout << "[warning] aboting `free_slot`: given block not aligned by base"
             << endl;
#endif
        return;
    }
    const int index = (block - base_ptr) / sizeof(t) / CHAR_BIT,
              bit_no = ((block - base_ptr) / sizeof(t)) % CHAR_BIT;
    /* CHAR_BIT = 8 on most modern systems. It shows the number of bits that in
     * a byte */

#ifdef DEBUG
    cout << "free_slot: called on bit " << bit_no << " of byte " << index
         << endl;
#endif
    bits[index] &= ~(1 << bit_no);
    borrowed_slots_counter--;

    last_freed_bit.offset = bit_no;
    last_freed_bit.position = index;
}

#ifdef DEBUG
template <typename t, unsigned int blocks_count>
void cheesy::bitmap<t, blocks_count>::dump(int index) const {
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

template <typename t, unsigned int blocks_count>
void cheesy::bitmap<t, blocks_count>::reset() {
    for (byte& b : bits) {
        b = 0;
    }
    borrowed_slots_counter = 0;
    last_freed_bit.position = last_freed_bit.offset = 0;
}

#endif  // BITMAP_CPP
