#ifndef STRING_UTILS_HXX
#define STRING_UTILS_HXX

namespace quarkie {
class node;
};

namespace string_utils {

struct word {
    const char* c;
    unsigned short size;

    inline bool operator==(const word& that) const {
        return ! __builtin_memcmp(this, &that, this->size);
    }

    inline bool operator!=(const char16_t* str) const {
        return __builtin_memcmp(this, str, this->size);
    }
};

extern char separator; /**< any character that will be forbidden in file names
                          but used to split paths */

word take_word(const char* path);

/*
 * #auxilary
 * @brief Extracts the target file name from path, i.e. call on "/foo/bar/faz"
 * will return struct containing pointer to "faz" and size = 3.
 */
word take_filename(const char* path);

/*
 * #auxilary
 * @brief Extracts the folder name that file pointed to by @path is located
 */
word take_directory(const char* path);
quarkie::node* find_file(const char* str);

}  // namespace string_utils

#endif
