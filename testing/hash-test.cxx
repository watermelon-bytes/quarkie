#include <algorithm>
#include <cstdint>
#include <cstring>
#include <hash_table/hash.hxx>
#include <iomanip>
#include <iostream>
#include <vector>

using quarkie::hash;

namespace tests {

static void print_hex(int32_t h) {
    std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
              << (uint32_t) h;
}

static bool check(const char* name, const void* data, uint16_t len,
                  int32_t expected) {
    int32_t got = hash(data, len);
    if (got == expected) {
        return true;
    }

    std::cout << "\n[FAIL] " << name << "\n";
    std::cout << "  data len = " << len << "\n";
    std::cout << "  expected: ";
    print_hex(expected);
    std::cout << "\n";
    std::cout << "  got:      ";
    print_hex(got);
    std::cout << "\n";
    return false;
}

#include "test_data.h"
bool strict_collision_check() {
    std::cout << "\nStrict collision check on large generated set ("
              << large_collision_test_set.size() << " entries)...\n";

    std::vector<std::tuple<int32_t, uint16_t, const char*>> results;
    results.reserve(large_collision_test_set.size());

    std::cout << "\nAll computed hashes:\n";
    std::cout
        << "------------------------------------------------------------\n";
    std::cout << "Hash (hex)     Len   Name\n";
    std::cout
        << "------------------------------------------------------------\n";

    for (const auto& [data, len, name] : large_collision_test_set) {
        int32_t h = quarkie::hash(data, len);

        // Логируем каждый хэш сразу
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                  << static_cast<uint32_t>(h) << "   " << std::dec
                  << std::setw(4) << len << "   "
                  << "\"" << name << "\"\n";

        results.emplace_back(h, len, name);
    }

    std::cout
        << "------------------------------------------------------------\n";
    std::cout << "Total computed: " << results.size() << "\n\n";

    // Сортировка для поиска коллизий
    std::sort(results.begin(), results.end());

    size_t collision_count = 0;
    bool has_collision = false;

    std::cout << "Collisions (if any):\n";

    for (size_t i = 1; i < results.size(); ++i) {
        auto [h1, len1, name1] = results[i - 1];
        auto [h2, len2, name2] = results[i];

        if (h1 == h2) {
            if (! has_collision) {
                std::cout << "-------------------------------------------------"
                             "-----------\n";
                has_collision = true;
            }

            ++collision_count;

            std::cout << "Collision at 0x" << std::hex << std::setw(8)
                      << std::setfill('0') << static_cast<uint32_t>(h1)
                      << ":\n";
            std::cout << "  " << std::dec << len1 << "  \"" << name1 << "\"\n";
            std::cout << "  " << std::dec << len2 << "  \"" << name2 << "\"\n";
        }
    }

    if (! has_collision) {
        std::cout << "(no collisions found)\n";
    } else {
        std::cout << "\nTOTAL COLLISIONS FOUND: " << collision_count
                  << "  ← BAD HASH QUALITY!\n";
    }

    std::cout << "\n";

    return ! has_collision;
}
}  // namespace tests

int main() {
    std::cout << "\nStrict collision check...\n";
    bool ok = tests::strict_collision_check();

    std::cout << "\n------------------------------------\n";
    std::cout << (ok ? "All checks passed ✓" : "Some checks FAILED ✗") << "\n";

    return ok ? 0 : 1;
}
