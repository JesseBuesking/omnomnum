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

#include "omnomnum.h"
#include "scanner.def.h"
#include "dtoa.h"
#include "itoa.h"

#define TOKEN_SEPARATOR 10000
#define TOKEN_CHARACTERS 10001
#define TOKEN_ALL_OTHERS 10002

typedef void (*Handler)(sds *s);    /* A pointer to a handler function */
void func0 (sds *s) {}
void func1 (sds *s) { *s = sdscat(*s, "st"); }
void func2 (sds *s) { *s = sdscat(*s, "sts"); }
void func3 (sds *s) { *s = sdscat(*s, "nd"); }
void func4 (sds *s) { *s = sdscat(*s, "nds"); }
void func5 (sds *s) { *s = sdscat(*s, "rd"); }
void func6 (sds *s) { *s = sdscat(*s, "rds"); }
void func7 (sds *s) { *s = sdscat(*s, "th"); }
void func8 (sds *s) { *s = sdscat(*s, "ths"); }

Handler jump_table[9] = {func0, func1, func2, func3, func4, func5, func6, func7, func8};

void yystypeToString(sds *s, YYSTYPE A, int precision) {
    if (A.is_frac) {
        dtoa(s, A.frac_num, precision);
        *s = sdscat(*s, "/");
        sds tmp = sdsempty();
        dtoa(&tmp, A.frac_denom, precision);
        *s = sdscatsds(*s, tmp);
        sdsfree(tmp);
    } else if (A.is_dbl) {
        dtoa(s, A.dbl, precision);
    } else {
        itoa(s, (uint64_t)A.dbl);
    }

    jump_table[A.suffix](s);
}

void *pParser;
sds numberHolder;

void initOmNomNum(void) {
    /*if (pParser == NULL) {*/
    pParser = ParseAlloc(malloc);
    /*}*/
    numberHolder = sdsempty();
}

void freeOmNomNum(void) {
    if (pParser != NULL) {
        ParseFree(pParser, free);
    }
    sdsfree(numberHolder);
}

YYSTYPEList find_numbers(const char *data, size_t data_len, ParserState *state) {
    YYSTYPE yylval;
    scanstate ss;
    scanstate_init(&ss, data, data_len);

    int scanner_value = -1;
    state->is_parsing = false;

#if debug
    ParseTrace(stderr, (char*)"[Parser] >> ");
#endif
    state->last_token = -1;

    for(;;) {
        RESET_YYSTYPE(yylval);
        scanner_value = omnomnum_scanner_start(state, pParser, &yylval, &ss);

#if debug
        sds value = sdsnewlen(ss.token, ss.cursor - ss.token);
        printf("token is %s at %d - %d\n", value, ss.token - data, ss.cursor - data);
        sdsfree(value);
#endif

        // failsafe to make sure we stay within the bounds
        if (ss.cursor > ss.limit) {
            // at the end of the string? end the loop.
            break;
        }

        if (scanner_value <= 0) {
            if (scanner_value < 0) {
                printf("Scanner returned an error: %d\n", scanner_value);
            }

            // equal to 0 (end of input)
            break;
        }

        if (scanner_value != TOKEN_SEPARATOR && state->last_token != TOKEN_CHARACTERS) {
            // update position information
            yylval.begin = ss.token - data;
            yylval.end = ss.cursor - data;

            // parse stuff
            Parse(pParser, scanner_value, yylval, state);
            state->is_parsing = true;
        }

        state->last_token = scanner_value;
    }

    if (state->is_parsing) {
        Parse(pParser, 0, yylval, state);
        ParseReset(pParser);
    }

    YYSTYPEList l = state->yystypeList;

#if debug
    printf("numbers: %zu\n", l.used);
#endif

    // I don't think this is necessary. The original thought was that if ever
    // RTL languages were supported, we'd need to reorder the items added.
    // However, I *think* the parser will always parse LTR, which means this
    // would do nothing. Since, for now, LTR english is the target language
    // supported, I'm disabling this.
    /*sortYYSTYPElist(&l);*/

    return l;
}

void normalize(const char *data, size_t data_len, ParserState *state) {
    YYSTYPEList l = find_numbers(data, data_len, state);

    if (l.used == 0) {
        state->result = sdsnew(data);
    } else {
        state->result = sdsempty();

        unsigned int lastpos = 0;
        unsigned int i = 0;

        for (i = 0; i < l.used; ++i) {
            YYSTYPE y = l.values[i];

#if debug
            printf("begin: %d, end: %d, value: %lf, suffix: %d\n", y.begin, y.end, y.dbl, y.suffix);
#endif

            if (lastpos < y.begin) {
                // Copy the part of the string leading up to the number to
                // the final string.
                state->result = sdscatlen(
                        state->result,
                        data + lastpos,
                        y.begin - lastpos
                        );
                sdsclear(numberHolder);
            }
            lastpos = y.end;

            yystypeToString(&numberHolder, y, state->precision);
            state->result = sdscatsds(state->result, numberHolder);

            sdsclear(numberHolder);

            // in case the last string was exceptionally large, free up memory
            // TODO test this out
            if (sdslen(numberHolder) > 1) {
                sdsRemoveFreeSpace(numberHolder);
            }
        }

        // Copy what's left of the string to the final string.
        state->result = sdscatlen(
                state->result,
                data + l.values[l.used-1].end,
                data_len - l.values[l.used-1].end
                );
    }
}
