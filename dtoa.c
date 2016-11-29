/* OmNomNum 0.0.1 -- Gobbles up numbers in strings.
 *
 * Copyright (c) 2016, Jesse Buesking <jessebuesking at gmail dot com>
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

/*
 * From http://stackoverflow.com/a/277810/435460, but converted to use sds.
 */

void nDecimals(sds *s, double d, int n) {
    int sz; double d2;

    // Allow for negative.

    d2 = (d >= 0) ? d : -d;
    sz = (d >= 0) ? 0 : 1;

    // Add one for each whole digit (0.xx special case).

    if (d2 < 1) {
        sz++;
    }
    while (d2 >= 1) {
        d2 /= 10.0;
        sz++;
    }

    // Adjust for decimal point and fractionals.

    sz += 1 + n;

    // Create format string then use it.
    /*sprintf(s, "%*.*f", sz, n, d);*/
    *s = sdscatprintf(*s, "%*.*f", sz, n, d);
}

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
    nDecimals(s, d, precision);
    morphNumericString(s, precision);
}
