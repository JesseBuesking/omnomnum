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
#ifndef SCAN_H
#define SCAN_H

// to pull in the definition for size_t
#include <sys/types.h>
// pull in definition for NULL
#include <string.h>
#include "scanner.def.h"

// for re2c...
#define YYCTYPE     unsigned char
#define YYCURSOR    (ss->cursor)
#define YYLIMIT     (ss->limit)
#define YYMARKER    (ss->marker)

/*
 * Fills the scan buffer with more data. Since we're reading from memory, this
 * does nothing.
 */
#define YYFILL(n) if (ss->cursor >= ss->limit) return 0;
#define YYDEBUG(state, current) printf("state: '%d', current: '0x%02X'\n", state, (unsigned char)current);

// forward declaration
struct scanstate;
/*
 * Contains all state for a single scan session.
 *
 * This structure is used by a scanner to preserve its state.
 *
 * All charptrs are declared const to help ensure that you don't
 * accidentally end up modifying the buffer as it's being scanned.
 */
struct scanstate {
    const char *cursor; /*The current character being looked at by the scanner. This is the same as re2c's YYCURSOR.*/
    const char *limit;  /*The last (uppermost) valid character in the current buffer. This is the same as re2c's YYLIMIT.*/
    const char *marker; /*Used internally by re2c engine to handle backtracking. This is the same as re2c's YYMARKER.*/
    const char *token;  /*The start of the current token.*/
};
typedef struct scanstate scanstate;

void scanstate_init(scanstate *ss, const char *bufptr, size_t bufsiz);

#endif // SCAN_H
