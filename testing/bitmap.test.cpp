#include "../include/bitmap.hpp"

#include "../src/bitmap.cpp"
struct a {
    long x;
};

cheesy::bitmap<a, 100> map;

void test_fill_byte() {
    map.dump(0);
    for (int i = 0; i < 8; ++i) {
        auto x = map.give_slot();
        map.dump(0);
    }
}

int main() {
    cout << "Size of map: " << sizeof(map) << endl;
    map.dump();
    auto x = map.give_slot();
    x->x = 5;
    x->x = 8;
    cout << "x = " << x->x << endl;
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
    cout << "Resetting map." << endl;
    map.reset();
    map.dump();
}
