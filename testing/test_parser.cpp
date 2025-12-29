#include <cassert>
#define DEBUG
#include <string.h>

#include <iomanip>
#include <iostream>
#include <parser.hxx>

using std::cout, std::endl;
using namespace string_utils;

// Глобальный разделитель — как в твоём коде
char string_utils::separator = '/';

constexpr auto no_word = word {nullptr, 0};

// Красивые цвета в терминале
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

class testcase {
    inline static uint counter = 0;
    uint number;

    const char* description;
    word (*func)(const char*);
    const char* path;

    word expected;
    word gotten;

public:
    testcase(const char* desc, word (*f)(const char*), const char* p,
             const word& exp)
        : description(desc),
          func(f),
          path(p),
          expected(exp),
          number(++counter) {
        run();
    }

    void run() {
        gotten = func(path);

        cout << "TEST #" << std::setw(2) << number << " | " << description
             << "\n"
             << "   Path:     \"" << (path ? path : "(null)") << "\"\n"
             << "   Expected: len=" << expected.size
             << " str=" << (expected.pointer ? expected.pointer : "(null)")
             << "\n"
             << "   Got:      len=" << gotten.size
             << " str=" << (gotten.pointer ? gotten.pointer : "(null)") << "\n";

        bool passed =
            (expected.size == gotten.size) &&
            (expected.pointer == gotten.pointer ||
             (expected.pointer && gotten.pointer &&
              ! memcmp(expected.pointer, gotten.pointer, expected.size)));

        if (passed) {
            cout << GREEN "[PASS]" RESET "\n\n";
        } else {
            cout << RED "[FAIL]" RESET "\n\n";
            std::abort();  // сразу падаем, чтобы видеть где ошибка
        }
    }
};

int main() {
    cout << "=== string_utils tests ===\n\n";

    // ===================================================================
    // take_word
    // ===================================================================
    new testcase("take_word: normal path", take_word, "/foo/bar/faz",
                 word {"foo", 3});
    new testcase("take_word: leading separator", take_word, "///foo/bar",
                 word {"foo", 3});
    new testcase("take_word: spaces skipped", take_word, "   foo   /bar",
                 word {"foo", 3});
    new testcase("take_word: empty string", take_word, "", no_word);
    new testcase("take_word: only separators", take_word, "////", no_word);
    new testcase("take_word: after first word", take_word, "/foo/bar/faz",
                 word {"foo", 3});
    // Чтобы протестировать второе слово — сдвинем указатель вручную
    const char* after_foo = "/foo/bar/faz" + 4;  // указывает на "bar/faz"
    new testcase("take_word: second word", take_word, after_foo,
                 word {"bar", 3});

    // ===================================================================
    // take_filename
    // ===================================================================
    new testcase("take_filename: normal", take_filename, "/foo/bar/faz",
                 word {"faz", 3});
    new testcase("take_filename: root file", take_filename, "/file.txt",
                 word {"file.txt", 8});
    new testcase("take_filename: trailing separator", take_filename,
                 "/foo/bar/", word {"bar", 3});
    // new testcase("take_filename: only root", take_filename, "/", no_word);
    new testcase("take_filename: empty", take_filename, "", no_word);
    // new testcase("take_filename: no separator", take_filename, "justname",
    // word {"justname", 8});

    // ===================================================================
    // take_directory
    // ===================================================================
    new testcase("take_directory: normal", take_directory, "/foo/bar/faz",
                 word {"bar", 3});
    new testcase("take_directory: deep path", take_directory, "/a/b/c/d/e.txt",
                 word {"d", 1});
    new testcase("take_directory: empty", take_directory, "", no_word);

    cout << GREEN "All tests passed successfully!" RESET "\n";
    return 0;
}
