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

#ifndef SCANNER_DEF_H
#define SCANNER_DEF_H

#include "sds.h"
#include <stdbool.h>

enum suffixValues
{
    NO_SUFFIX,
    ST,
    STS,
    ND,
    NDS,
    RD,
    RDS,
    TH,
    THS
};

enum errors {
    NO_ERROR,
    SYNTAX_ERROR,
    PARSE_FAILURE
};

typedef struct {
    double dbl;
    bool is_dbl;
    enum suffixValues suffix;
    //double frac_num;
    //double frac_denom;
    //unsigned int is_frac;
    unsigned int begin;
    unsigned int end;
} YYSTYPE;

typedef struct {
    YYSTYPE *values;
    size_t used;
    size_t size;
} YYSTYPEList;

typedef struct {
    int precision;
    sds result;
    enum errors error;
    YYSTYPEList yystypeList;
    bool parse_second;
    bool is_parsing;
    int last_token;
} ParserState;

void initYYSTYPEList(YYSTYPEList *l, size_t initialSize);
void insertYYSTYPE(YYSTYPEList *l, YYSTYPE element);
void resetYYSTYPElist(YYSTYPEList *l);
void freeYYSTYPElist(YYSTYPEList *l);
void sortYYSTYPElist(YYSTYPEList *l);

void initParserState(ParserState *state);
void resetParserState(ParserState *state);
void freeParserState(ParserState *state);

#endif // SCANNER_DEF_H
