#ifndef BITMAP_HPP
#define BITMAP_HPP
#include <stdint.h>
#include <sys/types.h>

#define DEBUG  // #build

#ifdef DEBUG

#include <bitset>
#include <iostream>
using std::cout, std::endl, std::bitset;

#endif  // DEBUG

namespace cheesy {

using byte = unsigned char;

// constexpr inline bool is_aligned(const int x) { return x % 8; }

// static byte bitmaps_state[];
// Page layout
// ---------------------------------------------------------------
// [ bitmap {16 bytes} |  |  |  |  | .... * blocks, 4080 bytes * ]
// ---------------------------------------------------------------

inline constexpr static ulong div_and_ceil(ulong a, ulong b) {
    return a / b + (a % b ? 1 : 0);
}

template <typename t, unsigned int slots_count>
class bitmap {
    unsigned int borrowed_slots_counter = 0;

    constexpr static ulong bitmap_size = div_and_ceil(slots_count, 8);
    byte bits[bitmap_size]{0};

    t base[slots_count];

    struct {
        short position : 13;
        short offset : 3;
    } last_freed_bit{0, 0};
    /* ^^^^^^^ to save information about what was freed last time to speed up
     * allocating*/

public:
#ifndef DEBUG
    bitmap() = default;
#endif
#ifdef DEBUG /* wanted to use "#elifdef DEBUG", but older than C++23 standards \
                dont support this directive ("compatibility!" 😬) */
    bitmap() {
        cout << "Bitmap initialized: " << bitmap_size
             << " blocks; each block = " << sizeof(t) << " bytes." << endl;
    };
#endif

    unsigned int get_borrowed_count() const { return borrowed_slots_counter; }

    [[nodiscard("The allocated slot must be stored and freed after using.")]] t*
    give_slot();

    void free_slot(const t*);

    void reset();

#ifdef DEBUG
    void dump(int index = -1) const;
#endif
};

};  // namespace cheesy

#endif
