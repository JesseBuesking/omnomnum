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

#include "omnomnum.h"
#include "scanner.def.h"
#include "dtoa.h"

#define TOKEN_SEPARATOR 10000

void yystypeToString(sds *s, YYSTYPE A, int precision) {
    dtoa(s, A.dbl, precision);

    switch (A.suffix) {
        case NO_SUFFIX:
            break;
        case ST:
            *s = sdscat(*s, "st");
            break;
        case STS:
            *s = sdscat(*s, "sts");
            break;
        case ND:
            *s = sdscat(*s, "nd");
            break;
        case NDS:
            *s = sdscat(*s, "nds");
            break;
        case RD:
            *s = sdscat(*s, "rd");
            break;
        case RDS:
            *s = sdscat(*s, "rds");
            break;
        case TH:
            *s = sdscat(*s, "th");
            break;
        case THS:
            *s = sdscat(*s, "ths");
            break;
    }
}

void *pParser;
sds numberHolder;

void initOmNomNum(void) {
    numberHolder = sdsempty();
    pParser = ParseAlloc(malloc);
}

void freeOmNomNum(void) {
    if (pParser!=NULL) {
        ParseFree(pParser, free);
    }
    sdsfree(numberHolder);
}

void normalize(const char *data, size_t data_len, ParserState *state) {
    YYSTYPE yylval;
    scanstate ss;
    scanstate_init(&ss, data, data_len);

    ScannerValue scanner_value;
    state->is_parsing = false;

#if debug
    ParseTrace(stderr, (char*)"[Parser] >> ");
#endif
    state->last_token = -1;

    for(;;) {
        scanner_value = omnomnum_scanner_start(state, pParser, &yylval, &ss);

#if debug
        sds value = sdsnewlen(ss.token, ss.cursor - ss.token);
        printf("token is %s at %d - %d\n", value, ss.token - data, ss.cursor - data);
        sdsfree(value);
#endif

        if (scanner_value.token <= 0) {
            if (scanner_value.token < 0) {
                printf("Scanner returned an error: %d\n", scanner_value.token);
            }
            break;
        }

        if (scanner_value.token != TOKEN_SEPARATOR && state->last_token != TOKEN_CHARACTERS) {
            // update position information
            yylval.begin = ss.token - data;
            yylval.end = ss.cursor - data;

            // parse stuff
            Parse(pParser, scanner_value.token, yylval, state);
            state->is_parsing = true;
        }

        state->last_token = scanner_value.token;
    }

    if (state->is_parsing) {
        Parse(pParser, 0, yylval, state);
        ParseReset(pParser);
    }

    YYSTYPEList l = state->yystypeList;

#if debug
    printf("numbers: %zu\n", l.used);
#endif

    sortYYSTYPElist(&l);

    if (l.used == 0) {
        state->result = sdsnew(data);
    } else {
        state->result = sdsempty();
        sds original = sdsnew(data);

        unsigned int lastpos = 0;
        unsigned int i = 0;

        for (i = 0; i < l.used; ++i) {
            YYSTYPE y = l.values[i];
#if debug
            printf("begin: %d, end: %d, value: %lf, suffix: %d\n", y.begin, y.end, y.dbl, y.suffix);
#endif

            if (lastpos < y.begin) {
                sds tmp = sdsdup(original);
                sdsrange(tmp, lastpos, y.begin - 1);
                state->result = sdscatsds(state->result, tmp);
                sdsfree(tmp);
            }
            lastpos = y.end;

            yystypeToString(&numberHolder, y, 3);
            state->result = sdscatsds(state->result, numberHolder);

            sdsclear(numberHolder);

            // in case the last string was exceptionally large, free up memory
            // TODO test this out
            if (sdslen(numberHolder) > 1) {
                sdsRemoveFreeSpace(numberHolder);
            }
        }

        sds tmp = sdsdup(original);
        sdsrange(tmp, l.values[l.used-1].end, -1);
        state->result = sdscatsds(state->result, tmp);
        sdsfree(tmp);
        sdsfree(original);
    }
}
