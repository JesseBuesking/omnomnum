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
#include "scanner.def.h"
// pull in definitions for malloc and free
#include <stdlib.h>

size_t RESET_LIST_SIZE = 8;

void initYYSTYPEList(YYSTYPEList *l, size_t initialSize) {
    l->values = (YYSTYPE *)malloc(initialSize * sizeof(YYSTYPE));
    l->used = 0;
    l->size = initialSize;
}

void insertYYSTYPE(YYSTYPEList *l, YYSTYPE element) {
    if (l->used == l->size) {
        if (l->size == 0) {
            l->size = 1;
        } else {
            l->size *= 2;
        }
        l->values = (YYSTYPE *)realloc(l->values, l->size * sizeof(YYSTYPE));
    }
    l->values[l->used] = element;
    l->used += 1;
}

void freeYYSTYPElist(YYSTYPEList *l) {
    free(l->values);
}

void resetYYSTYPElist(YYSTYPEList *l) {
    if (l->size > RESET_LIST_SIZE) {
        l->size = RESET_LIST_SIZE;
        l->values = (YYSTYPE *)realloc(l->values, l->size * sizeof(YYSTYPE));
    }
    l->used = 0;
}

int compare(const void* a, const void* b) {
     YYSTYPE yya = * ( (YYSTYPE*) a );
     YYSTYPE yyb = * ( (YYSTYPE*) b );

     if ( yya.begin == yyb.begin ) return 0;
     else if ( yya.begin < yyb.begin ) return -1;
     else return 1;
}

void sortYYSTYPElist(YYSTYPEList *l) {
    qsort(l->values, l->used, sizeof(YYSTYPE), compare);
}

void initParserState(ParserState *state) {
    state->error = NO_ERROR;
    state->parse_second = false;
    initYYSTYPEList(&(state->yystypeList), 4);
}

void resetParserState(ParserState *state) {
    state->precision = 3;
    sdsfree(state->result);
    state->error = NO_ERROR;
    resetYYSTYPElist(&(state->yystypeList));
    state->parse_second = false;
}

void freeParserState(ParserState *state) {
    freeYYSTYPElist(&(state->yystypeList));
}
