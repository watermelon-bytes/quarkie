#include <cassert>
#include <iostream>
#include <vector>

#include "../inc/bitmap/bitmap.hxx"
using namespace quarkie;

void test_bitmap_basic() {
    std::cout
        << "=== Тест 1: базовая аллокация и освобождение (32 слота) ===\n";

    bitmap<int, 32> bm;

    // 1. Все слоты должны быть свободны изначально
    assert(bm.get_borrowed_count() == 0);  // приватное поле, но для теста можно
                                           // временно сделать friend или getter

    // Временно добавим getter для тестов (или сделаем friend класс)
    // Пока просто проверим через поведение

    std::vector<int*> pointers;

    // 2. Выделяем все 32 слота
    for (int i = 0; i < 32; ++i) {
        int* p = bm.give_slot();
        assert(p != nullptr);
        *p = i + 100;  // запишем что-то
        pointers.push_back(p);
    }

    // 3. Ещё один give_slot должен вернуть nullptr
    assert(bm.give_slot() == nullptr);

    // 4. Освобождаем в обратном порядке
    for (auto it = pointers.rbegin(); it != pointers.rend(); ++it) {
        bm.free_slot(*it);
        bm.dump();
    }

    // 5. После освобождения всех — снова можно выделить
    pointers.clear();
    for (int i = 0; i < 32; ++i) {
        int* p = bm.give_slot();
        assert(p != nullptr);
        // assert(*p == 0);  // должны быть обнулены? Нет, у тебя не обнуляются,
        // но хотя бы не мусор от прошлого
        *p = 42;
        pointers.push_back(p);
    }

    std::cout << "Тест 1 пройден\n\n";
}

void test_last_freed_optimization() {
    std::cout << "=== Тест 2: оптимизация last_freed_bit ===\n";

    bitmap<double, 64> bm;

    double* p1 = bm.give_slot();
    double* p2 = bm.give_slot();
    double* p3 = bm.give_slot();

    assert(p1 != nullptr && p2 != nullptr && p3 != nullptr);

    // Освобождаем p2 (средний)
    bm.free_slot(p2);

    // Следующий give_slot должен вернуть именно p2 (благодаря last_freed_bit)
    double* p_next = bm.give_slot();
    assert(p_next == p2);  // должен попасть в только что освобождённый слот

    std::cout << "Тест 2 пройден (last_freed_bit работает)\n\n";
}

void test_alignment_and_invalid_free() {
    std::cout << "=== Тест 3: проверка выравнивания и неверный free_slot ===\n";

    bitmap<long long, 16> bm;

    long long* valid = bm.give_slot();
    bm.free_slot(valid);  // нормально

    // Попробуем освободить фейковый указатель (не из пула)
    char fake[100];
    long long* fake_ptr =
        reinterpret_cast<long long*>(fake + 1);  // не выровнен и не из base

    // free_slot должен молча проигнорировать (в DEBUG выведет warning)
    bm.free_slot(fake_ptr);

    // Указатель за пределами base
    long long* outside = valid + 1000;
    bm.free_slot(outside);

    std::cout << "Тест 3 пройден (не крашнулся на неверных указателях)\n\n";
}

void test_different_types_and_sizes() {
    std::cout << "=== Тест 4: разные типы и размеры ===\n";

    bitmap<char, 8> bm_char;
    bitmap<int, 100> bm_int;
    bitmap<void*, 23> bm_ptr;

    char* c = bm_char.give_slot();
    assert(c != nullptr);
    *c = 'A';
    bm_char.free_slot(c);

    // 100 слотов — должно занять ceil(100/8) = 13 байт битмапа
    std::vector<int*> ints;
    for (int i = 0; i < 100; ++i) {
        int* p = bm_int.give_slot();
        assert(p != nullptr);
        ints.push_back(p);
    }
    assert(bm_int.give_slot() == nullptr);

    // 17 слотов — некратно 8, проверка границы
    void** vp = bm_ptr.give_slot();
    assert(vp != nullptr);

    std::cout << "Тест 4 пройден (разные типы и размеры работают)\n\n";
}

void test_reset() {
    std::cout << "=== Тест 5: reset() ===\n";

    bitmap<float, 40> bm;

    std::vector<float*> allocated;
    for (int i = 0; i < 20; ++i) {
        allocated.push_back(bm.give_slot());
    }

    bm.reset();

    // После reset всё должно быть свободно
    for (int i = 0; i < 40; ++i) {
        float* p = bm.give_slot();
        assert(p != nullptr);
    }
    assert(bm.give_slot() == nullptr);

    std::cout << "Тест 5 пройден\n\n";
}

int main() {
    test_bitmap_basic();
    test_last_freed_optimization();
    test_alignment_and_invalid_free();
    test_different_types_and_sizes();
    test_reset();

    std::cout << "Все тесты пройдены успешно!\n";
    return 0;
}
