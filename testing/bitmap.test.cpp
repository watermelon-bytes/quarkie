#include "../include/bitmap.hpp"

#include "../src/bitmap.cpp"

struct a {
    long x;
};

cheesy::bitmap<a, 10> map;

void test_fill_byte() {
    map.dump(0);
    for (int i = 0; i < 8; ++i) {
        auto x = map.give_slot();
        map.dump(0);
    }
}

int main() {
    for (int i = 0; i < 4; i++) {
        test_fill_byte();
    }
    auto x = map.give_slot();
    map.dump();

    auto y = map.give_slot();
    map.dump();

    auto z = map.give_slot();
    map.dump();

    cout << "freeing 2nd bit..." << endl;
    map.free_slot(y);
    map.dump();

    cout << "allocating again..." << endl;
    y = map.give_slot();
    map.dump();

    cout << "freeing z and y: " << endl;
    map.free_slot(z);
    map.free_slot(y);
    map.dump();
    cout << "calling give_slot()" << endl;
    z = map.give_slot();
    map.dump();
}
