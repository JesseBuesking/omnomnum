/* OmNomNum 0.0.1 -- Gobbles up numbers in strings.
 *
 * Copyright (c) 2017, Jesse Buesking <jessebuesking at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of OmNomNum nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
