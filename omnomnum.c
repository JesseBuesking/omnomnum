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
    readmem_init(&ss, data, data_len);

    int tok;
    unsigned int start_pos = 0;
    unsigned int token_length = 0;
    int parsing = 0;

#if debug
    ParseTrace(stderr, (char*)"[Parser] >> ");
#endif
    int last_tok = -1;

    do
    {
        tok = omnomnum_scanner_start(&ss);

#if debug
        printf("line %2d, token %d\n", ss.line, tok);
#endif

        if (tok == 0) {
            break;
        }

        if (tok < 0) {
            printf("Scanner returned an error: %d\n", tok);
            break;
        }

        if (tok == TOKEN_SECOND && (state->options & PARSE_SECOND) == 0) {
            // dont' parse "second" by default. easiest way to do this is to
            // treat it as a character token
            tok = TOKEN_CHARACTERS;
        }

        token_length = scan_token_length(&ss);

        // update position information
        yylval.begin = start_pos;
        yylval.end = yylval.begin + token_length;
        start_pos += token_length;

        if (parsing == 0 && (tok == TOKEN_CHARACTERS || tok == TOKEN_SEPARATOR)) {
            // not parsing and we've found a character. gobble it and continue
            last_tok = tok;
            continue;
        }

        if (parsing == 1 && tok == TOKEN_CHARACTERS) {
            if (last_tok != TOKEN_SEPARATOR) {
                // number followed by character... e.g. "oneself"
            } else {
                // finish whatever we had and reset
                Parse(pParser, 0, yylval, state);
            }

            ParseReset(pParser);
            last_tok = tok;
            parsing = 0;
            continue;
        }

#if debug
        sds value = sdsnewlen(ss.token, token_length);
        printf("token is %s at %d - %d\n", value, yylval.begin, yylval.end);
        sdsfree(value);
#endif

        if (tok == TOKEN_NUMBER) {
            // turn string version of number into double
            sds value = sdsnewlen(ss.token, token_length);
            sscanf(value, "%lf", &yylval.dbl);
            sdsfree(value);
        }

        if (tok != TOKEN_SEPARATOR && (last_tok == -1 || last_tok != TOKEN_CHARACTERS)) {
            // parse stuff
            Parse(pParser, tok, yylval, state);
            parsing = 1;
        }

        last_tok = tok;
    } while (tok);

    if (parsing == 1) {
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
