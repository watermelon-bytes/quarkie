#ifndef QUARKIE_BASIC_BITMAP_HXX
#define QUARKIE_BASIC_BITMAP_HXX
#include <quarkie_defs.hxx>
namespace quarkie {

inline constexpr static ulong div_and_ceil(ulong a, ulong b) {
    return a / b + (a % b ? 1 : 0);
}

struct div_t {
    uint quot, rem;
};

inline constexpr static div_t _div(uint x, uint y) {
    return {.quot = x / y, .rem = x % y};
}

template <uint slots_count>
struct basic_bitmap {
    constexpr static ulong bitmap_size = div_and_ceil(slots_count, CHAR_BIT);
    u8 map[bitmap_size];

    void set_bit(const uint index, const bool value);
    [[gnu::always_inline]] inline void enable_bit(const uint);
    [[gnu::always_inline]] inline void clear_bit(const uint);
    [[gnu::always_inline]] inline bool test_bit(const uint);
    constexpr basic_bitmap<slots_count>();
};

template <uint slots_count>
void basic_bitmap<slots_count>::set_bit(const uint index, const bool value) {
    [[unlikely]] if (index >= bitmap_size) { return; }
    // not sure that attribute "unlikely" is needed here

    const auto [byte, bit_offset] = _div(index, CHAR_BIT);

    u8 mask = ~(u8(1) << bit_offset);
    map[byte] &= mask; /* unset the target bit*/

    map[byte] |= (value << bit_offset);
    /* could've done this via simple if-statement though */
}

template <uint slots_count>
inline void basic_bitmap<slots_count>::clear_bit(const uint bit) {
    set_bit(bit, 0);
}

template <uint slots_count>
inline void basic_bitmap<slots_count>::enable_bit(const uint bit) {
    set_bit(bit, 1);
}

template <uint slots_count>
inline bool basic_bitmap<slots_count>::test_bit(const uint bit) {
    [[unlikely]] if (bit >= bitmap_size) { return 0; }
    return map[bit / CHAR_BIT] & (1 << bit % CHAR_BIT);
}

template <uint slots_count>
constexpr basic_bitmap<slots_count>::basic_bitmap() {
    __builtin_memset(&map, 0, sizeof(map));
    uint tail = slots_count % CHAR_BIT;
    if (tail) {
        map[bitmap_size - 1] |= CHAR_MAX >> tail;
    }
}

};  // namespace quarkie
#endif  // !BASIC_BITMAP_HXX
