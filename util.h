#ifndef OMNOMNUM_UTIL_H
#define OMNOMNUM_UTIL_H

// for size_t
#include <stdlib.h>

size_t remove_char_inplace(const char *str, size_t str_len, const unsigned char remove);

void replace_char_inplace(char *str, size_t str_len, const unsigned char remove, const unsigned char replacement);

size_t replace_two_byte_char_inplace(unsigned char *str, size_t str_len, const unsigned char* remove, const unsigned char replacement);

#endif
