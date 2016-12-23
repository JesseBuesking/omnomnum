// for size_t
#include <stdlib.h>

// Given a char array and it's length (allowing for strings with nulls), modify
// the char array in-place removing the character supplied.
size_t remove_char_inplace(char *str, size_t str_len, const unsigned char remove) {
    size_t orig_index = 0, new_index = 0;
    for(;orig_index < str_len;) {
        if (str[orig_index] == remove) {
            orig_index += 1;
        } else {
            str[new_index] = str[orig_index];
            orig_index += 1;
            new_index += 1;
        }
    }

    str[new_index] = '\0';
    return new_index;
}

// Given a char array, it's length (allowing for strings with nulls), a
// character to be removed and the character to replace it with, modify the
// char array in-place replacing the character.
void replace_char_inplace(char *str, size_t str_len, const unsigned char remove, const unsigned char replacement) {
    size_t index = 0;
    for(;index < str_len; index++) {
        if (str[index] == remove) {
            str[index] = replacement;
        }
    }
}

// Given a char array, it's length (allowing for strings with nulls), a two-byte
// character to be removed and the character to replace it with, modify the
// char array in-place replacing the character. Useful for two-byte UTF-8
// characters.
size_t replace_two_byte_char_inplace(unsigned char *str, size_t str_len, const unsigned char* remove, const unsigned char replacement) {
    size_t orig_index = 0, new_index = 0;
    for(;orig_index < str_len - 1;) {
        if (str[orig_index] == remove[0] && str[orig_index + 1] == remove[1]) {
            str[new_index] = replacement;
            orig_index += 2;
            new_index += 1;
        } else {
            str[new_index] = str[orig_index];
            orig_index += 1;
            new_index += 1;
        }
    }

    str[new_index] = '\0';
    return new_index;
}
