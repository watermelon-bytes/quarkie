#include <algorithm>
#include <bitmap/pool.hxx>
#include <cassert>
#include <cstdint>
#include <random>
#include <vector>
using quarkie::pool;
#define REQUIRE(expr)                                                     \
    do {                                                                  \
        if (! (expr)) {                                                   \
            std::fprintf(stderr, "FAIL: %s:%d  %s\n", __FILE__, __LINE__, \
                         #expr);                                          \
            std::fflush(stderr);                                          \
            std::abort();                                                 \
        }                                                                 \
    } while (0)

#define REQUIRE_EQ(a, b) REQUIRE((a) == (b))
#define REQUIRE_NE(a, b) REQUIRE((a) != (b))
#define REQUIRE_NULL(p) REQUIRE((p) == nullptr)
#define REQUIRE_NOT_NULL(p) REQUIRE((p) != nullptr)

struct TestObj {
    uint64_t magic = 0x1122334455667788ULL;
    int id = -1;
};

void test_basic_properties() {
    using Pool64 = pool<TestObj, 64>;
    using Pool128 = pool<TestObj, 128>;

    static_assert(sizeof(Pool64) <= 64 * sizeof(TestObj) + 32,
                  "size estimation");
}

void test_allocate_all_and_overflow() {
    pool<TestObj, 128> p;

    std::vector<TestObj*> ptrs;
    ptrs.reserve(128);

    for (int i = 0; i < 128; ++i) {
        auto* obj = p.give_slot();
        REQUIRE_NOT_NULL(obj);
        obj->id = i;
        ptrs.push_back(obj);
    }

    REQUIRE_EQ(p.borrowed_slots_counter_, 128u);

    REQUIRE_NULL(p.give_slot());
}

void test_free_and_reuse() {
    pool<TestObj, 8> p;

    auto* a = p.give_slot();
    auto* b = p.give_slot();
    REQUIRE_NE(a, b);

    p.free_slot(a);
    auto* a2 = p.give_slot();
    *a2 = TestObj();

    REQUIRE_EQ(a2->magic, 0x1122334455667788ULL);

    p.free_slot(b);
    p.free_slot(a2);
}

void test_roundtrip_random_order() {
    pool<TestObj, 256> p;
    std::vector<TestObj*> allocated;

    for (int i = 0; i < 240; ++i) {
        allocated.push_back(p.give_slot());
    }

    std::mt19937 gen {777};
    std::shuffle(allocated.begin(), allocated.end(), gen);

    for (size_t i = 0; i < 120; ++i) {
        p.free_slot(allocated[i]);
    }

    for (size_t i = 0; i < 120; ++i) {
        auto* obj = p.give_slot();
        REQUIRE_NOT_NULL(obj);
    }

    REQUIRE_EQ(p.borrowed_slots_counter_, 240u);
}

int main() {
    std::printf("Running pool tests...\n");

    test_basic_properties();
    std::printf("\tbasic properties ✓\n");

    test_allocate_all_and_overflow();
    std::printf("\tallocate all + overflow ✓\n");

    test_free_and_reuse();
    std::printf("\tfree & reuse same slot ✓\n");

    test_roundtrip_random_order();
    std::printf("\trandom free/alloc roundtrip ✓\n");

    std::printf("\nAll tests passed!\n");
    return 0;
}
