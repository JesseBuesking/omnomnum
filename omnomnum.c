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

#include "omnomnum.h"
#include "scanner.def.h"
#include "dtoa.h"
#include "itoa.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/* time.h was only used for internal profiling (removed) */

#define TOKEN_SEPARATOR 10000
#define TOKEN_CHARACTERS 10001
#define TOKEN_ALL_OTHERS 10002

typedef void (*Handler)(sds *s);    /* A pointer to a handler function */
void func0 (sds *s) { (void)s; /* unused - placeholder for jump table */ }
void func1 (sds *s) { *s = sdscat(*s, "st"); }
void func2 (sds *s) { *s = sdscat(*s, "sts"); }
void func3 (sds *s) { *s = sdscat(*s, "nd"); }
void func4 (sds *s) { *s = sdscat(*s, "nds"); }
void func5 (sds *s) { *s = sdscat(*s, "rd"); }
void func6 (sds *s) { *s = sdscat(*s, "rds"); }
void func7 (sds *s) { *s = sdscat(*s, "th"); }
void func8 (sds *s) { *s = sdscat(*s, "ths"); }

Handler jump_table[9] = {func0, func1, func2, func3, func4, func5, func6, func7, func8};

static int is_space_or_hyphen(char c) {
    return c==' '||c=='\t'||c=='\r'||c=='\n'||c=='\f'||c=='-';
}

static int is_letter(char c) {
    return (c>='a'&&c<='z')||(c>='A'&&c<='Z');
}

static int word_match(const char* data, unsigned int pos, unsigned int len, const char* word, unsigned int* consumed) {
    unsigned int wlen = (unsigned int)strlen(word);
    if (pos + wlen > len) return 0;
    if (strncmp(data + pos, word, wlen) != 0) return 0;
    char next = (pos + wlen < len) ? data[pos + wlen] : '\0';
    if (is_letter(next)) return 0; // ensure word boundary
    *consumed = wlen;
    return 1;
}

/* Reserved for Task 13: robust word-to-number mapping in mixed patterns (e.g., "5 thousand and three") */
static int match_cardinal_small(const char* data, unsigned int pos, unsigned int len, double* value, unsigned int* consumed) {
    struct { const char* w; int v; } map[] = {
        {"one",1},{"two",2},{"three",3},{"four",4},{"five",5},{"six",6},{"seven",7},{"eight",8},{"nine",9},
    };
    for (unsigned i=0;i<sizeof(map)/sizeof(map[0]);++i) {
        unsigned c=0; if (word_match(data,pos,len,map[i].w,&c)) { *value = (double)map[i].v; *consumed=c; return 1; }
    }
    return 0;
}

/* Reserved for future use: additional denominator matching beyond scanner rules */
static int match_denominator_word(const char* data, unsigned int pos, unsigned int len, double* denom, unsigned int* consumed) {
    struct { const char* w; int v; } map[] = {
        {"fourth",4},{"fourths",4},
        {"eighth",8},{"eighths",8},
        {"ninth",9},{"ninths",9},
    };
    for (unsigned i=0;i<sizeof(map)/sizeof(map[0]);++i) {
        unsigned c=0; if (word_match(data,pos,len,map[i].w,&c)) { *denom = (double)map[i].v; *consumed=c; return 1; }
    }
    return 0;
}

/* Reserved for future use: whitespace/hyphen validation */
static int is_ws_or_hyphen_only(const char* data, unsigned int a, unsigned int b) {
    for (unsigned int i=a;i<b;i++) {
        if (!is_space_or_hyphen(data[i])) return 0;
    }
    return 1;
}

/* Compute greatest common divisor using Euclidean algorithm */
static uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/* Check if a word is a denominator word (for fraction handling) */
static int is_denominator_word(const char* s, size_t len) {
    /* Common fraction denominators that should be preserved when parse_fractions=false */
    if (len == 4 && !strncmp(s, "half", 4)) return 1;
    if (len == 6 && !strncmp(s, "halves", 6)) return 1;
    if (len == 5 && !strncmp(s, "third", 5)) return 1;
    if (len == 6 && !strncmp(s, "thirds", 6)) return 1;
    if (len == 7 && !strncmp(s, "quarter", 7)) return 1;
    if (len == 8 && !strncmp(s, "quarters", 8)) return 1;
    if (len == 6 && !strncmp(s, "fourth", 6)) return 1;
    if (len == 7 && !strncmp(s, "fourths", 7)) return 1;
    if (len == 5 && !strncmp(s, "fifth", 5)) return 1;
    if (len == 6 && !strncmp(s, "fifths", 6)) return 1;
    if (len == 5 && !strncmp(s, "sixth", 5)) return 1;
    if (len == 6 && !strncmp(s, "sixths", 6)) return 1;
    if (len == 7 && !strncmp(s, "seventh", 7)) return 1;
    if (len == 8 && !strncmp(s, "sevenths", 8)) return 1;
    if (len == 6 && !strncmp(s, "eighth", 6)) return 1;
    if (len == 7 && !strncmp(s, "eighths", 7)) return 1;
    if (len == 5 && !strncmp(s, "ninth", 5)) return 1;
    if (len == 6 && !strncmp(s, "ninths", 6)) return 1;
    if (len == 5 && !strncmp(s, "tenth", 5)) return 1;
    if (len == 6 && !strncmp(s, "tenths", 6)) return 1;
    /* Add more as needed - these are the most common */
    return 0;
}

void yystypeToString(sds *s, YYSTYPE A, int precision) {
    if (A.is_frac) {
        if (A.frac_num < 0) {
            *s = sdscat(*s, "-");
        }
        dtoa(s, A.frac_num < 0 ? -A.frac_num : A.frac_num, precision);
        *s = sdscat(*s, "/");
        sds tmp = sdsempty();
        dtoa(&tmp, A.frac_denom, precision);
        *s = sdscatsds(*s, tmp);
        sdsfree(tmp);
    } else if (A.is_dbl) {
        dtoa(s, A.dbl, precision);
    } else {
        if (A.dbl < 0) {
            *s = sdscat(*s, "-");
            itoa(s, (uint64_t)(-A.dbl));
        } else {
            itoa(s, (uint64_t)A.dbl);
        }
    }

    jump_table[A.suffix](s);
}

void yystypeToStringWithReduction(sds *s, YYSTYPE A, int precision, bool reduce_fractions) {
    if (A.is_frac) {
        double num = A.frac_num;
        double denom = A.frac_denom;

        if (reduce_fractions) {
            // Check if numerator and denominator are whole numbers
            double num_abs = num < 0 ? -num : num;
            if (num_abs == (uint64_t)num_abs && denom == (uint64_t)denom) {
                uint64_t n = (uint64_t)num_abs;
                uint64_t d = (uint64_t)denom;
                uint64_t divisor = gcd(n, d);
                if (divisor > 1) {
                    num = num < 0 ? -(double)(n / divisor) : (double)(n / divisor);
                    denom = (double)(d / divisor);
                }
            }
        }

        if (num < 0) {
            *s = sdscat(*s, "-");
        }
        dtoa(s, num < 0 ? -num : num, precision);
        *s = sdscat(*s, "/");
        sds tmp = sdsempty();
        dtoa(&tmp, denom, precision);
        *s = sdscatsds(*s, tmp);
        sdsfree(tmp);
    } else if (A.is_dbl) {
        dtoa(s, A.dbl, precision);
    } else {
        if (A.dbl < 0) {
            *s = sdscat(*s, "-");
            itoa(s, (uint64_t)(-A.dbl));
        } else {
            itoa(s, (uint64_t)A.dbl);
        }
    }

    jump_table[A.suffix](s);
}

void initOmNomNum(void) {
    /* No global state to initialize; kept for API compatibility. */
}

void freeOmNomNum(void) {
    /* No global state to free; kept for API compatibility. */
}

/* Post-process result to handle percent normalization */
static void process_percent(sds *result, ParserState *state) {
    if (!state->normalize_percent_symbol && !state->percent_as_decimal) {
        return; // no percent processing needed
    }

    size_t len = sdslen(*result);
    if (len == 0) return;

    sds output = sdsempty();
    output = sdsMakeRoomFor(output, len + 64);

    size_t i = 0;
    while (i < len) {
        // Check if we're at a digit or negative sign (potential number start)
        bool is_num_start = ((*result)[i] >= '0' && (*result)[i] <= '9') || (*result)[i] == '-';

        if (is_num_start) {
            size_t num_start = i;
            // Find end of number (digits, dots, slashes, minus signs)
            while (i < len && (
                ((*result)[i] >= '0' && (*result)[i] <= '9') ||
                (*result)[i] == '.' || (*result)[i] == '/' || (*result)[i] == '-'
            )) {
                i++;
            }
            size_t num_end = i;

            // Check if followed by " percent" or "%"
            bool has_percent_word = false;
            bool has_percent_symbol = false;
            size_t after_percent = i;

            // Skip whitespace
            while (i < len && ((*result)[i] == ' ' || (*result)[i] == '\t')) {
                i++;
            }

            // Check for "percent"
            if (i + 7 <= len && strncmp(*result + i, "percent", 7) == 0) {
                char next = (i + 7 < len) ? (*result)[i + 7] : '\0';
                if (!is_letter(next)) {
                    has_percent_word = true;
                    after_percent = i + 7;
                }
            }
            // Check for "%"
            else if (i < len && (*result)[i] == '%') {
                has_percent_symbol = true;
                after_percent = i + 1;
            }

            if (has_percent_word || has_percent_symbol) {
                // Extract the number
                sds num_str = sdsnewlen(*result + num_start, num_end - num_start);

                // Check for embedded hyphens (ranges like "20-30")
                // A leading hyphen is fine (negative number), but embedded ones indicate a range
                char *embedded_hyphen = strchr(num_str + (num_str[0] == '-' ? 1 : 0), '-');
                bool is_range = (embedded_hyphen != NULL);

                if (is_range) {
                    // This is a range like "20-30 percent", don't convert - keep original
                    output = sdscatlen(output, *result + num_start, after_percent - num_start);
                } else if (state->percent_as_decimal) {
                    // Convert to decimal: n → n/100
                    // Check if it's a fraction (contains '/')
                    char *slash = strchr(num_str, '/');
                    double value;
                    if (slash) {
                        // Parse as fraction: numerator/denominator
                        *slash = '\0';
                        double numerator = strtod(num_str, NULL);
                        double denominator = strtod(slash + 1, NULL);
                        if (denominator != 0) {
                            value = numerator / denominator;
                        } else {
                            value = numerator; // fallback if denominator is 0
                        }
                    } else {
                        // Parse as regular number
                        value = strtod(num_str, NULL);
                    }
                    value /= 100.0;

                    char buf[64];
                    int written = snprintf(buf, sizeof(buf), "%.*g", state->precision + 2, value);
                    output = sdscatlen(output, buf, written);
                } else if (state->normalize_percent_symbol) {
                    // Just normalize the symbol: number + "%"
                    output = sdscatsds(output, num_str);
                    output = sdscat(output, "%");
                } else {
                    // Keep as-is (shouldn't reach here due to early return)
                    output = sdscatlen(output, *result + num_start, after_percent - num_start);
                }

                sdsfree(num_str);
                i = after_percent;
                continue;
            } else {
                // Not followed by percent, copy number as-is and reset position
                output = sdscatlen(output, *result + num_start, num_end - num_start);
                i = num_end;  // Reset to right after number, so whitespace gets processed normally
                continue;
            }
        }

        // Regular character, just copy
        output = sdscatlen(output, *result + i, 1);
        i++;
    }

    sdsfree(*result);
    *result = output;
}

/* Internal profiling removed. */

YYSTYPEList find_numbers(const char *data, size_t data_len, ParserState *state) {
    /* profiling removed */
    YYSTYPE yylval;
    scanstate ss;
    scanstate_init(&ss, data, data_len);

    int scanner_value = -1;
    state->is_parsing = false;
    /* Ensure a parser exists for this request; reuse across calls */
    if (state->pParser == NULL) {
        state->pParser = ParseAlloc(malloc);
    }
    /* Reset parser to a clean state for this run */
    ParseReset(state->pParser);

    /* Pre-reserve an estimated capacity for number results to reduce reallocs */
    size_t estimate = data_len > 0 ? (data_len / 6) : 8; /* heuristic tokens/6 chars */
    if (estimate < 8) estimate = 8;
    if (estimate > 4096) estimate = 4096;
    ensureYYSTYPECapacity(&state->yystypeList, estimate);

#if debug
    ParseTrace(stderr, (char*)"[Parser] >> ");
#endif
    state->last_token = -1;

    for(;;) {
        RESET_YYSTYPE(yylval);
        scanner_value = omnomnum_scanner_start(state, state->pParser, &yylval, &ss);

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
            Parse(state->pParser, scanner_value, yylval, state);
            state->is_parsing = true;
        }

        state->last_token = scanner_value;
    }

    if (state->is_parsing) {
        Parse(state->pParser, 0, yylval, state);
        ParseReset(state->pParser);
    }

    YYSTYPEList l = state->yystypeList;
    /* profiling removed */

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
    /* profiling removed */
    YYSTYPEList l = find_numbers(data, data_len, state);

    // Note: fraction handling is now performed in the scanner; no post-pass merge required.

    if (l.used == 0) {
        // Fallback: split on separators and normalize each token independently
        state->result = sdsempty();
        /* Pre-reserve to reduce reallocations during append */
        state->result = sdsMakeRoomFor(state->result, (size_t)data_len + 32);
        unsigned int pos = 0;
        while (pos < data_len) {
            // skip leading separators
            unsigned int start = pos;
            while (pos < data_len) {
                char c = data[pos];
                if (c==' '||c=='\r'||c=='\n'||c=='\t'||c=='\f'||c=='-') {
                    // copy separator as-is
                    pos++;
                } else {
                    break;
                }
            }
            if (pos > start) {
                state->result = sdscatlen(state->result, data + start, pos - start);
            }
            if (pos >= data_len) break;
            // find end of token (non-separator run)
            unsigned int tok_start = pos;
            while (pos < data_len) {
                char c = data[pos];
                if (c==' '||c=='\r'||c=='\n'||c=='\t'||c=='\f'||c=='-') break;
                pos++;
            }
            unsigned int tok_len = pos - tok_start;
            if (tok_len > 0) {
                // If parse_fractions is disabled and this token is a denominator word,
                // preserve it as-is to avoid converting it to an ordinal (e.g., "thirds" → "3rds")
                if (!state->parse_fractions && is_denominator_word(data + tok_start, tok_len)) {
                    state->result = sdscatlen(state->result, data + tok_start, tok_len);
                } else {
                    ParserState sub; initParserState(&sub);
                    // Copy all runtime flags to respect caller's settings
                    sub.parse_second = state->parse_second;
                    sub.precision = state->precision;
                    sub.reduce_fractions = state->reduce_fractions;
                    sub.parse_fractions = state->parse_fractions;
                    sub.normalize_percent_symbol = state->normalize_percent_symbol;
                    sub.percent_as_decimal = state->percent_as_decimal;
                    YYSTYPEList sl = find_numbers(data + tok_start, tok_len, &sub);
                    if (sl.used > 0) {
                        sds tmp = sdsempty();
                        yystypeToStringWithReduction(&tmp, sl.values[0], sub.precision, sub.reduce_fractions);
                        state->result = sdscatsds(state->result, tmp);
                        sdsfree(tmp);
                    } else {
                        state->result = sdscatlen(state->result, data + tok_start, tok_len);
                    }
                    freeParserState(&sub);
                }
            }
        }
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
            }
            lastpos = y.end;

            // Directly render into the result buffer to avoid an extra copy
            yystypeToStringWithReduction(&state->result, y, state->precision, state->reduce_fractions);
        }

        // Copy what's left of the string to the final string.
        state->result = sdscatlen(
                state->result,
                data + l.values[l.used-1].end,
                data_len - l.values[l.used-1].end
                );
    }

    // Post-process for percent normalization if requested
    process_percent(&state->result, state);

    /* profiling removed */
}
