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

#include "scanner.def.h"
// For ParseFree declaration and ParseReset wrapper
#include "parser.h"
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

void ensureYYSTYPECapacity(YYSTYPEList *l, size_t need) {
    if (l->size < need) {
        l->size = need;
        l->values = (YYSTYPE *)realloc(l->values, l->size * sizeof(YYSTYPE));
    }
}

void initParserState(ParserState *state) {
    state->error = NO_ERROR;
    state->parse_second = false;
    state->parse_fractions = true; // default: keep current behavior
    state->reduce_fractions = false; // default: off (keep current behavior)
    state->normalize_percent_symbol = false; // default: off
    state->percent_as_decimal = false; // default: off
    state->precision = 6;
    state->result = NULL;
    state->is_parsing = false;
    state->last_token = -1;
    state->pParser = NULL;
    state->numberHolder = sdsempty();
    state->subState = NULL; // Lazy-allocate on first use
    // OPTIMIZATION: Start with larger capacity to reduce reallocations
    // Typical BM_many_numbers has ~90 numbers, so 128 avoids most growth
    initYYSTYPEList(&(state->yystypeList), 128);
}

void resetParserState(ParserState *state) {
    state->precision = 6;
    // OPTIMIZATION: Clear result buffer instead of freeing (enables reuse)
    if (state->result) { sdsclear(state->result); }
    state->error = NO_ERROR;
    resetYYSTYPElist(&(state->yystypeList));
    state->parse_second = false;
    state->parse_fractions = true; // keep fractions enabled unless caller disables
    state->reduce_fractions = false; // default: off
    state->normalize_percent_symbol = false; // default: off
    state->percent_as_decimal = false; // default: off
    // Keep the cached parser and scratch buffer; just clear the buffer
    // No NULL check needed - numberHolder is always allocated in initParserState
    sdsclear(state->numberHolder);
}

void freeParserState(ParserState *state) {
    freeYYSTYPElist(&(state->yystypeList));
    // No NULL check needed - numberHolder is always allocated in initParserState
    sdsfree(state->numberHolder);
    state->numberHolder = NULL;
    if (state->pParser) { ParseFree(state->pParser, free); state->pParser = NULL; }
    if (state->subState) {
        freeParserState(state->subState);
        free(state->subState);
        state->subState = NULL;
    }
}

ParserState* getOrInitSubState(ParserState *state) {
    if (state->subState == NULL) {
        // Lazy-allocate and initialize subState on first use
        state->subState = (ParserState*)malloc(sizeof(ParserState));
        initParserState(state->subState);
    } else {
        // Reset existing subState for reuse
        resetParserState(state->subState);
    }
    return state->subState;
}
