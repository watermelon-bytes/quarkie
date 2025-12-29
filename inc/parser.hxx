#ifndef STRING_UTILS_HXX
#define STRING_UTILS_HXX

// Avoid `#include`s
namespace quarkie {
class node;
};

// Functions defined in this namespace serve as a layer between input request
// and filsystem itself. They cast human-readable paths to structures readable
// by the quarkie filsystem.
namespace string_utils {

struct word {
    const char* pointer;
    unsigned short size;

    inline bool operator==(const word& that) const {
        return ! __builtin_memcmp(this, &that, this->size);
    }

    inline bool operator!=(const char* str) const {
        return __builtin_memcmp(this, str, this->size);
    }
};

extern char separator; /**< This symbol that will be forbidden in file names
                          but used to split paths */

word take_word(const char* path);

/*
 * #auxilary
 * @brief Extracts the target file name from the path, i.e. call on
 * "/foo/bar/faz" will return struct `word` containing pointer to "faz" (and
 * `size` = 3).
 */
word take_filename(const char* path);

/*
 * #auxilary
 * @brief Extracts the folder name that file pointed to by @path is located
 */
word take_directory(const char* path);

/*
 * @brief Returns a pointer to the file described in `str` (`nullptr` is
 * returned if no such file)
 * @param path Absolute path from superblock::root to the target.
 */
quarkie::node* find_file(const char* path);

/*
 * @brief
 *
 */
quarkie::node* find_subdirectory(const char* path);

inline bool is_valid_filename(const char* req);

}  // namespace string_utils

#endif
