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

    double frac_num;
    double frac_denom;
    bool is_frac;

    unsigned int begin;
    unsigned int end;
    enum suffixValues suffix;
} YYSTYPE;

#ifndef COPY_YYSTYPE_BE
#define COPY_YYSTYPE_BE(A, B) \
    A.begin = B.begin; \
    A.end = B.end;
#endif

#ifndef COPY_YYSTYPE_BE_VALUE
#define COPY_YYSTYPE_BE_VALUE(A, B, VALUE) \
    COPY_YYSTYPE_BE(A, B); \
    A.dbl = VALUE;
#endif

#ifndef COPY_YYSTYPE_BE_VALUE_SUFF
#define COPY_YYSTYPE_BE_VALUE_SUFF(A, B, VALUE, SUFFIX) \
    COPY_YYSTYPE_BE_VALUE(A, B, VALUE); \
    A.suffix = SUFFIX;
#endif

#ifndef COPY_YYSTYPE_DBL
#define COPY_YYSTYPE_DBL(A, B) \
    A.dbl = B.dbl; \
    A.is_dbl = B.is_dbl;
#endif

#ifndef COPY_YYSTYPE_FRAC
#define COPY_YYSTYPE_FRAC(A, B) \
    A.frac_num = B.frac_num; \
    A.frac_denom = B.frac_denom; \
    A.is_frac = B.is_frac;
#endif

#ifndef COPY_YYSTYPE_SUFF
#define COPY_YYSTYPE_SUFF(A, B) \
    A.suffix = B.suffix;
#endif

#ifndef COPY_YYSTYPE_BE_DBL
#define COPY_YYSTYPE_BE_DBL(A, B) \
    COPY_YYSTYPE_DBL(A, B); \
    COPY_YYSTYPE_BE(A, B);
#endif

#ifndef COPY_YYSTYPE_BE_DBL_SUFF
#define COPY_YYSTYPE_BE_DBL_SUFF(A, B) \
    COPY_YYSTYPE_BE_DBL(A, B); \
    COPY_YYSTYPE_SUFF(A, B);
#endif

#ifndef COPY_YYSTYPE_BE2
#define COPY_YYSTYPE_BE2(A, B, C) \
    A.begin = B.begin; \
    A.end = C.end;
#endif

#ifndef COPY_YYSTYPE_BE_MUL
#define COPY_YYSTYPE_BE_MUL(A, B, C, VALUE) \
    COPY_YYSTYPE_BE2(A, B, C); \
    A.dbl = B.dbl * VALUE; \
    A.is_dbl = B.is_dbl;
#endif

#ifndef COPY_YYSTYPE_BE_MUL_SUFF
#define COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, VALUE, SUFFIX) \
    COPY_YYSTYPE_BE_MUL(A, B, C, VALUE); \
    A.suffix = SUFFIX;
#endif

#ifndef COPY_YYSTYPE_BE_ADD
#define COPY_YYSTYPE_BE_ADD(A, B, C) \
    COPY_YYSTYPE_BE2(A, B, C); \
    A.dbl = B.dbl + C.dbl; \
    A.is_dbl = B.is_dbl || C.is_dbl;
#endif

#ifndef COPY_YYSTYPE_BE_ADD_SUFF
#define COPY_YYSTYPE_BE_ADD_SUFF(A, B, C) \
    COPY_YYSTYPE_BE_ADD(A, B, C); \
    A.suffix = C.suffix;
#endif

#ifndef COPY_YYSTYPE_BE_MUL_ADD
#define COPY_YYSTYPE_BE_MUL_ADD(A, B, C, VALUE) \
    COPY_YYSTYPE_BE2(A, B, C); \
    A.dbl = (B.dbl * VALUE) + C.dbl; \
    A.is_dbl = B.is_dbl || C.is_dbl;
#endif

#ifndef COPY_YYSTYPE_BE_MUL_ADD_SUFF
#define COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, VALUE) \
    COPY_YYSTYPE_BE_MUL_ADD(A, B, C, VALUE); \
    A.suffix = C.suffix;
#endif

#ifndef COPY_YYSTYPE_DBL_NUM
#define COPY_YYSTYPE_DBL_NUM(A, B, C, VALUE) \
    COPY_YYSTYPE_BE2(A, B, C); \
    A.dbl = B.dbl * VALUE; \
    A.is_dbl = true;
#endif

#ifndef COPY_YYSTYPE_DBL_NUM_SUFF
#define COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, VALUE, SUFFIX) \
    COPY_YYSTYPE_DBL_NUM(A, B, C, VALUE); \
    A.suffix = SUFFIX;
#endif

#define HUNDRED_F  100.0
#define THOUSAND_F 1000.0
#define MILLION_F  1000000.0
#define BILLION_F  1000000000.0
#define TRILLION_F 1000000000000.0

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
