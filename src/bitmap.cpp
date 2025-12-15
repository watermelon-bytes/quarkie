#ifndef BITMAP_CPP
#define BITMAP_CPP

#include "../include/bitmap.hpp"

#include <sys/types.h>

#include <climits>

template <typename t, unsigned int bitmap_size>
t* cheesy::bitmap<t, bitmap_size>::give_slot() {
#ifdef DEBUG
    cout << "give_slot() called." << endl;
#endif

    for (unsigned i = 0; i < bitmap_size; ++i) {
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
#ifdef DEBUG
    cout << "error: bitmap is fulfilled, cannot allocate slot" << endl;
    exit(-1);
#endif
    return nullptr;
}

template <typename t, unsigned int bitmap_size>
void cheesy::bitmap<t, bitmap_size>::free_slot(const t* ptr) {
    auto block = (const byte* const)ptr;
    auto base_ptr = (byte* const)base;

    /* Check alignment */
    if ((block - base_ptr) % sizeof(block) != 0) {
#ifdef DEBUG
        cout << "[warning] aboting `free_slot`: given block not aligned"
             << endl;
#endif
        return;
    }
    const int index = (block - base_ptr) / sizeof(t) / CHAR_BIT,
              bit_no = ((block - base_ptr) / sizeof(t)) % CHAR_BIT;
    // CHAR_BIT = 8 on most modern systems. It shows the maximal number of bits
    // that can be stored a byte

#ifdef DEBUG
    cout << "free_slot: called on bit " << bit_no << " of byte " << index
         << endl;
#endif
    bits[index] &= ~(1 << bit_no);
    borrowed_slots_counter--;
}

#ifdef DEBUG
template <typename t, unsigned int bitmap_size>
void cheesy::bitmap<t, bitmap_size>::dump(int index) const {
    if (index < 0) {
        for (const byte& slot_set : bits) {
            cout << bitset<CHAR_BIT>(slot_set) << endl;
        }
        return;
    } else
        cout << "bitmap[byte " << index
             << "]: " << bitset<CHAR_BIT>(bits[index]) << endl;
}
#endif  // DEBUG

template <typename t, unsigned int bitmap_size>
void cheesy::bitmap<t, bitmap_size>::reset() {
    for (byte& b : bits) {
        b = 0;
    }
    borrowed_slots_counter = 0;
}

#endif  // BITMAP_CPP
