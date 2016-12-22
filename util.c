// for size_t
#include <stdlib.h>

// Given a char array and it's length (allowing for strings with nulls), modify
// the char array in-place removing the character supplied.
size_t remove_char_inplace(char *str, size_t str_len, const char remove) {
    size_t orig_index = 0, new_index = 0;
    for(;orig_index < str_len;)
    {
        if (str[orig_index] == remove) {
            orig_index += 1;
        } else {
            str[new_index] = str[orig_index];
            new_index += 1;
            orig_index += 1;
        }
    }

    str[new_index] = '\0';
    return new_index;
}
