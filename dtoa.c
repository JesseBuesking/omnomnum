/* OmNomNum 0.0.2 -- Gobbles up numbers in strings.
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

#include <stdio.h>
#include <string.h>
#include "sds.h"
#include "grisu2/grisu2.h"

/*
 * From http://stackoverflow.com/a/277810/435460, but converted to use sds.
 */

char DTOA_BUFFER[256];

void morphNumericString(sds *s, int n) {
    char *p = strchr(*s, '.');                    // Find decimal point, if any.
    if (p != NULL) {
        int dotPosition = (p - *s);
        int endRange = dotPosition + n;
        int lastPosition = (int) sdslen(*s) - 1;

        endRange = endRange < lastPosition ? endRange : lastPosition;

        while ((*s)[endRange] == '0') {     // Remove trailing zeros.
            endRange -= 1;
        }

        if ((*s)[endRange] == '.') {        // If all decimals were zeros, remove "."
            endRange -= 1;
        }

        sdsrange(*s, 0, endRange);          // Trim the string to the range.
    }
}

void dtoa(sds *s, double d, int precision) {
    memset(DTOA_BUFFER, 0, 256);
    fill_double(d, DTOA_BUFFER);
    *s = sdscat(*s, DTOA_BUFFER);
    morphNumericString(s, precision);
}
