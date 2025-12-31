#ifndef BASIC_BITMAP_HXX
#define BASIC_BITMAP_HXX
#include <sys/types.h>

class basic_bitmap {
    u_char bits[100];
    [[gnu::always_inline]] inline void set_bit(const uint);
    [[gnu::always_inline]] inline void clear_bit(const uint);
};

#endif  // !BASIC_BITMAP_HXX
