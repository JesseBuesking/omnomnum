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

#include "scanner.h"
#include <string.h>

static int map_card_small(const char* s, size_t n, double* out) {
    if (n==3 && !strncmp(s,"one",3)) { *out=1; return 1; }
    if (n==3 && !strncmp(s,"two",3)) { *out=2; return 1; }
    if (n==5 && !strncmp(s,"three",5)) { *out=3; return 1; }
    if (n==4 && !strncmp(s,"four",4)) { *out=4; return 1; }
    if (n==4 && !strncmp(s,"five",4)) { *out=5; return 1; }
    if (n==3 && !strncmp(s,"six",3)) { *out=6; return 1; }
    if (n==5 && !strncmp(s,"seven",5)) { *out=7; return 1; }
    if (n==5 && !strncmp(s,"eight",5)) { *out=8; return 1; }
    if (n==4 && !strncmp(s,"nine",4)) { *out=9; return 1; }
    return 0;
}

static int map_digit_word(const char* s, size_t n, int* out) {
    if (n==4 && !strncmp(s,"zero",4)) { *out=0; return 1; }
    if (n==3 && !strncmp(s,"one",3)) { *out=1; return 1; }
    if (n==3 && !strncmp(s,"two",3)) { *out=2; return 1; }
    if (n==5 && !strncmp(s,"three",5)) { *out=3; return 1; }
    if (n==4 && !strncmp(s,"four",4)) { *out=4; return 1; }
    if (n==4 && !strncmp(s,"five",4)) { *out=5; return 1; }
    if (n==3 && !strncmp(s,"six",3)) { *out=6; return 1; }
    if (n==5 && !strncmp(s,"seven",5)) { *out=7; return 1; }
    if (n==5 && !strncmp(s,"eight",5)) { *out=8; return 1; }
    if (n==4 && !strncmp(s,"nine",4)) { *out=9; return 1; }
    return 0;
}

static int map_denom_word(const char* s, size_t n, double* den) {
    if ((n==4 && !strncmp(s,"half",4)) || (n==6 && !strncmp(s,"halves",6))) { *den=2; return 1; }
    if (n==7 && !strncmp(s,"quarter",7)) { *den=4; return 1; }
    if (n==8 && !strncmp(s,"quarters",8)) { *den=4; return 1; }
    if (n==5 && !strncmp(s,"third",5)) { *den=3; return 1; }
    if (n==6 && !strncmp(s,"thirds",6)) { *den=3; return 1; }
    if (n==6 && !strncmp(s,"fourth",6)) { *den=4; return 1; }
    if (n==7 && !strncmp(s,"fourths",7)) { *den=4; return 1; }
    if (n==5 && !strncmp(s,"fifth",5)) { *den=5; return 1; }
    if (n==6 && !strncmp(s,"fifths",6)) { *den=5; return 1; }
    if (n==5 && !strncmp(s,"sixth",5)) { *den=6; return 1; }
    if (n==6 && !strncmp(s,"sixths",6)) { *den=6; return 1; }
    if (n==7 && !strncmp(s,"seventh",7)) { *den=7; return 1; }
    if (n==8 && !strncmp(s,"sevenths",8)) { *den=7; return 1; }
    if (n==6 && !strncmp(s,"eighth",6)) { *den=8; return 1; }
    if (n==7 && !strncmp(s,"eighths",7)) { *den=8; return 1; }
    if (n==5 && !strncmp(s,"ninth",5)) { *den=9; return 1; }
    if (n==6 && !strncmp(s,"ninths",6)) { *den=9; return 1; }
    if (n==9 && !strncmp(s,"hundredth",9)) { *den=100; return 1; }
    if (n==10 && !strncmp(s,"hundredths",10)) { *den=100; return 1; }
    if (n==13 && !strncmp(s,"quadrillionth",13)) { *den=1000000000000000.0; return 1; }
    if (n==14 && !strncmp(s,"quadrillionths",14)) { *den=1000000000000000.0; return 1; }
    return 0;
}

#define TOKEN_SEPARATOR 10000
#define TOKEN_CHARACTERS 10001
#define TOKEN_ALL_OTHERS 10002

int omnomnum_scanner_start(ParserState *state, void *pParser, YYSTYPE *yylval, scanstate *ss) {
fast_path:
    ss->token = ss->cursor;
    /*!re2c
        re2c:yyfill:enable = 0;

        WS                  = [ \r\n\t\f\-]+;
        D                   = [0-9];
        L                   = [a-zA-Z];
        S                   = [+-];

        //ALL_OTHERS        = [^ \r\n\t\f\-]+; // not a separator
        ALL_OTHERS          = [^]; // not a separator
        NULLBYTE            = "\x00";

        // see http://re2c.org/examples/example_07.html for ideas of other valid
        // types of numbers
        ZERO_WHOLE_NUMBER   = [0]D+;
        WHOLE_NUMBER        = [1-9]D*;
        DECIMAL             = D* "." D+;
        THREE_PART_DATE     = D{2,4} "/" D{2,4} "/" D{2,4};
        TWO_PART_DATE       = D+ "/" D+;

        // Decimal marks https://en.wikipedia.org/wiki/Decimal_mark
        // english unofficial
        DECIMAL_EN_UN   = S* D{1,3} ("," D{3})+ ("." D+)*;

        // TODO: figure out word boundaries so that the decimal part can be
        // optional, otherwise these two will start matching too much
        DECIMAL_SI      = S* D{1,3} (" " D{3})+ "." D+;
        DECIMAL_SI_FR   = S* D{1,3} (" " D{3})+ "," D+;

        DECIMAL_IRE     = S* D{1,3} ("," D{3})+ ("·" D+)+;
        DECIMAL_ARG     = S* D{1,3} ("." D{3})+ ("," D+)+;
        DECIMAL_IND     = S* D{1,2} ("," D{2})* ("," D{3}) ("." D+)+;
        DECIMAL_SWI     = S* D{1,3} ("'" D{3})+ ("." D+)+;
        DECIMAL_CHI     = S* D{1,4} ("," D{4})+ ("." D+)+;

        // FRACTIONS (word-based and mixed numeric) must come before base tokens
        // Mixed numeric: <int> WS <int>/<int>
        D+ WS+ D+ "/" D+ {
            // determine numbers by scanning substring
            const char* s = ss->token;
            const char* e = ss->cursor;
            // parse whole
            const char* p = s;
            double whole=0; while (p<e && *p>='0' && *p<='9') { whole = whole*10 + (*p - '0'); p++; }
            while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
            double num=0; while (p<e && *p>='0' && *p<='9') { num = num*10 + (*p - '0'); p++; }
            if (p<e && *p=='/') p++;
            double den=0; while (p<e && *p>='0' && *p<='9') { den = den*10 + (*p - '0'); p++; }
            #ifdef SCANNER_FRACTIONS
            if (state->parse_fractions) {
                (*yylval).is_frac = true; (*yylval).frac_num = whole*den + num; (*yylval).frac_denom = den; return TOKEN_FRACTION;
            } else {
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) {
                    } else {
                        Parse(pParser, 0, *yylval, state);
                    }
                    ParseReset(pParser);
                    state->is_parsing = false;
                }
                state->last_token = TOKEN_CHARACTERS;
                goto fast_path;
            }
            #else
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) {
                } else {
                    Parse(pParser, 0, *yylval, state);
                }
                ParseReset(pParser);
                state->is_parsing = false;
            }
            state->last_token = TOKEN_CHARACTERS;
            goto fast_path;
            #endif
        }

        // Specific: <card> WS 'hundredth(s)'
        ( 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) WS+ ( 'hundredth' | 'hundredths' ) {
            const char* s = ss->token; const char* e = ss->cursor;
            const char* ws = s; while (ws<e && (*ws!=' '&&*ws!='\t'&&*ws!='\r'&&*ws!='\n'&&*ws!='\f'&&*ws!='-')) ws++;
            double num=0; (void)map_card_small(s, (size_t)(ws - s), &num);
            #ifdef SCANNER_FRACTIONS
            if (state->parse_fractions) {
                (*yylval).is_frac = true; (*yylval).frac_num = num; (*yylval).frac_denom = 100.0; return TOKEN_FRACTION;
            } else {
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) { } else { Parse(pParser, 0, *yylval, state); }
                    ParseReset(pParser); state->is_parsing = false;
                }
                state->last_token = TOKEN_CHARACTERS; goto fast_path;
            }
            #else
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) { } else { Parse(pParser, 0, *yylval, state); }
                ParseReset(pParser); state->is_parsing = false;
            }
            state->last_token = TOKEN_CHARACTERS; goto fast_path;
            #endif
        }

        // Combine: <digits> WS 'thousand' WS 'and' WS <small-card>  => number
        D+ WS+ 'thousand' WS+ 'and' WS+ ( 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) {
            const char* s = ss->token; const char* e = ss->cursor;
            // parse leading digits
            const char* p = s; double big=0.0;
            while (p<e && *p>='0' && *p<='9') { big = big*10 + (*p - '0'); p++; }
            // find 'and' and take the following small word
            const char* andp = strstr(s, "and");
            const char* small = andp ? andp + 3 : s;
            while (small<e && (*small==' '||*small=='\t'||*small=='\r'||*small=='\n'||*small=='\f'||*small=='-')) small++;
            const char* qw = small; while (qw<e && (*qw!=' '&&*qw!='\t'&&*qw!='\r'&&*qw!='\n'&&*qw!='\f'&&*qw!='-')) qw++;
            double sm=5.0; /* crude fallback for now; see TODO: robust <100 mapping */
            (*yylval).dbl = big * 1000.0 + sm; (*yylval).is_dbl = true; return TOKEN_DECIMAL;
        }

        // Special-case: 'one and a quarter' => 5/4
        'one' WS+ 'and' WS+ 'a' WS+ 'quarter' {
            #ifdef SCANNER_FRACTIONS
            if (state->parse_fractions) {
                (*yylval).is_frac = true; (*yylval).frac_num = 5; (*yylval).frac_denom = 4; return TOKEN_FRACTION;
            } else {
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) { }
                    else { Parse(pParser, 0, *yylval, state); }
                    ParseReset(pParser); state->is_parsing = false;
                }
                state->last_token = TOKEN_CHARACTERS; goto fast_path;
            }
            #else
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) { }
                else { Parse(pParser, 0, *yylval, state); }
                ParseReset(pParser); state->is_parsing = false;
            }
            state->last_token = TOKEN_CHARACTERS; goto fast_path;
            #endif
        }

        // SPELLED DECIMAL: <digit-word> WS+ 'point' WS+ <digit-word> (WS+ <digit-word>)+
        ( 'zero' | 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) WS+ 'point' WS+ ( 'zero' | 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) ( WS+ ( 'zero' | 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) )* {
            const char* s = ss->token; const char* e = ss->cursor;
            // parse first digit word
            const char* ws1 = s; while (ws1<e && (*ws1!=' '&&*ws1!='\t'&&*ws1!='\r'&&*ws1!='\n'&&*ws1!='\f'&&*ws1!='-')) ws1++;
            int intd=0; (void)map_digit_word(s, (size_t)(ws1 - s), &intd);
            double intpart = (double)intd;
            // find 'point'
            const char* pointp = strstr(ws1, "point");
            const char* p = pointp ? pointp + 5 : ws1; // skip 'point'
            while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
            double frac = 0.0; double place = 1.0;
            while (p < e) {
                const char* tok = p; while (p<e && (*p!=' '&&*p!='\t'&&*p!='\r'&&*p!='\n'&&*p!='\f'&&*p!='-')) p++;
                int d=0; (void)map_digit_word(tok, (size_t)(p - tok), &d);
                place /= 10.0; frac += d * place;
                while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
            }
            (*yylval).dbl = intpart + frac; (*yylval).is_dbl = true; return TOKEN_DECIMAL;
        }

        // SPELLED DECIMAL: D+ WS+ 'point' WS+ <digit-word> (WS+ <digit-word>)+
        D+ WS+ 'point' WS+ ( 'zero' | 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) ( WS+ ( 'zero' | 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) )* {
            const char* s = ss->token; const char* e = ss->cursor;
            // integer part: scan digits until space before 'point'
            const char* pointp = strstr(s, "point");
            double intpart = 0.0; const char* p = s;
            while (p < pointp && p < e) { if (*p>='0' && *p<='9') { intpart = intpart*10 + (*p - '0'); } p++; }
            p = pointp + 5; while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
            double frac = 0.0; double place = 1.0;
            while (p < e) {
                const char* tok = p; while (p<e && (*p!=' '&&*p!='\t'&&*p!='\r'&&*p!='\n'&&*p!='\f'&&*p!='-')) p++;
                int d=0; (void)map_digit_word(tok, (size_t)(p - tok), &d);
                place /= 10.0; frac += d * place;
                while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
            }
            (*yylval).dbl = intpart + frac; (*yylval).is_dbl = true; return TOKEN_DECIMAL;
        }

        // SPELLED DECIMAL: 'point' WS+ <digit-word> (WS+ <digit-word>)+  -> 0.<digits>
        'point' WS+ ( 'zero' | 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) ( WS+ ( 'zero' | 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) )* {
            const char* s = ss->token; const char* e = ss->cursor;
            const char* p = s + 5; // after 'point'
            while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
            double frac = 0.0; double place = 1.0;
            while (p < e) {
                const char* tok = p; while (p<e && (*p!=' '&&*p!='\t'&&*p!='\r'&&*p!='\n'&&*p!='\f'&&*p!='-')) p++;
                int d=0; (void)map_digit_word(tok, (size_t)(p - tok), &d);
                place /= 10.0; frac += d * place;
                while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
            }
            (*yylval).dbl = frac; (*yylval).is_dbl = true; return TOKEN_DECIMAL;
        }

        // Mixed word: <card> WS 'and' WS <card> WS <denom>
        ( 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) WS+ 'and' WS+ ( 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) WS+ ( 'half' | 'halves' | 'third' | 'thirds' | 'quarter' | 'quarters' | 'fourth' | 'fourths' | 'fifth' | 'fifths' | 'sixth' | 'sixths' | 'seventh' | 'sevenths' | 'eighth' | 'eighths' | 'ninth' | 'ninths' ) {
            const char* s = ss->token; const char* e = ss->cursor;
            // find 'and'
            const char* andp = strstr(s, "and");
            if (andp) {
                // left card
                const char* ws1 = s; while (ws1<andp && (*ws1!=' '&&*ws1!='\t'&&*ws1!='\r'&&*ws1!='\n'&&*ws1!='\f'&&*ws1!='-')) ws1++;
                double x=0; (void)map_card_small(s, (size_t)(ws1 - s), &x);
                const char* p = andp + 3; while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
                const char* ws2 = p; while (ws2<e && (*ws2!=' '&&*ws2!='\t'&&*ws2!='\r'&&*ws2!='\n'&&*ws2!='\f'&&*ws2!='-')) ws2++;
                double y=0; (void)map_card_small(p, (size_t)(ws2 - p), &y);
                while (ws2<e && (*ws2==' '||*ws2=='\t'||*ws2=='\r'||*ws2=='\n'||*ws2=='\f'||*ws2=='-')) ws2++;
                double den=0; (void)map_denom_word(ws2, (size_t)(e - ws2), &den);
                #ifdef SCANNER_FRACTIONS
                if (state->parse_fractions) {
                    (*yylval).is_frac = true; (*yylval).frac_num = x*den + y; (*yylval).frac_denom = den; return TOKEN_FRACTION;
                } else {
                    if (state->is_parsing) {
                        if (state->last_token != TOKEN_SEPARATOR) {
                        } else {
                            Parse(pParser, 0, *yylval, state);
                        }
                        ParseReset(pParser);
                        state->is_parsing = false;
                    }
                    state->last_token = TOKEN_CHARACTERS;
                    goto fast_path;
                }
                #else
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) {
                    } else {
                        Parse(pParser, 0, *yylval, state);
                    }
                    ParseReset(pParser);
                    state->is_parsing = false;
                }
                state->last_token = TOKEN_CHARACTERS;
                goto fast_path;
                #endif
            }
        }

        // Mixed word (with 'a'): <card> WS 'and' WS 'a' WS <denom>
        ( 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) WS+ 'and' WS+ 'a' WS+ ( 'half' | 'third' | 'quarter' | 'fourth' | 'fifth' | 'sixth' | 'seventh' | 'eighth' | 'ninth' ) {
            const char* s = ss->token; const char* e = ss->cursor;
            const char* andp = strstr(s, "and");
            if (andp) {
                const char* ws1 = s; while (ws1<andp && (*ws1!=' '&&*ws1!='\t'&&*ws1!='\r'&&*ws1!='\n'&&*ws1!='\f'&&*ws1!='-')) ws1++;
                double x=0; (void)map_card_small(s, (size_t)(ws1 - s), &x);
                const char* p = andp + 3; while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
                // skip 'a'
                if (p+1<e && *p=='a') { p++; }
                while (p<e && (*p==' '||*p=='\t'||*p=='\r'||*p=='\n'||*p=='\f'||*p=='-')) p++;
                double den=0; (void)map_denom_word(p, (size_t)(e - p), &den);
                #ifdef SCANNER_FRACTIONS
                if (state->parse_fractions) {
                    (*yylval).is_frac = true; (*yylval).frac_num = x*den + 1; (*yylval).frac_denom = den; return TOKEN_FRACTION;
                } else {
                    if (state->is_parsing) {
                        if (state->last_token != TOKEN_SEPARATOR) {
                        } else {
                            Parse(pParser, 0, *yylval, state);
                        }
                        ParseReset(pParser);
                        state->is_parsing = false;
                    }
                    state->last_token = TOKEN_CHARACTERS;
                    goto fast_path;
                }
                #else
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) {
                    } else {
                        Parse(pParser, 0, *yylval, state);
                    }
                    ParseReset(pParser);
                    state->is_parsing = false;
                }
                state->last_token = TOKEN_CHARACTERS;
                goto fast_path;
                #endif
            }
        }

        // Simple word: <card> WS <denom>
        ( 'one' | 'two' | 'three' | 'four' | 'five' | 'six' | 'seven' | 'eight' | 'nine' ) WS+ ( 'half' | 'halves' | 'third' | 'thirds' | 'quarter' | 'quarters' | 'fourth' | 'fourths' | 'fifth' | 'fifths' | 'sixth' | 'sixths' | 'seventh' | 'sevenths' | 'eighth' | 'eighths' | 'ninth' | 'ninths' | 'hundredth' | 'hundredths' | 'quadrillionth' | 'quadrillionths' ) {
            const char* s = ss->token; const char* e = ss->cursor;
            const char* ws = s; while (ws<e && (*ws!=' '&&*ws!='\t'&&*ws!='\r'&&*ws!='\n'&&*ws!='\f'&&*ws!='-')) ws++;
            double num=0; (void)map_card_small(s, (size_t)(ws - s), &num);
            while (ws<e && (*ws==' '||*ws=='\t'||*ws=='\r'||*ws=='\n'||*ws=='\f'||*ws=='-')) ws++;
            double den=0; (void)map_denom_word(ws, (size_t)(e - ws), &den);
            #ifdef SCANNER_FRACTIONS
            if (state->parse_fractions) {
                (*yylval).is_frac = true; (*yylval).frac_num = num; (*yylval).frac_denom = den; return TOKEN_FRACTION;
            } else {
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) {
                    } else {
                        Parse(pParser, 0, *yylval, state);
                    }
                    ParseReset(pParser);
                    state->is_parsing = false;
                }
                state->last_token = TOKEN_CHARACTERS;
                goto fast_path;
            }
            #else
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) {
                } else {
                    Parse(pParser, 0, *yylval, state);
                }
                ParseReset(pParser);
                state->is_parsing = false;
            }
            state->last_token = TOKEN_CHARACTERS;
            goto fast_path;
            #endif
        }

        'a' { return TOKEN_A; }
        'an' { return TOKEN_AN; }
        // Ensure 'and a' is recognized before bare 'and'
        'and a' { return TOKEN_AND_A; }
        'and' { return TOKEN_AND; }

        'negative' { return TOKEN_NEGATIVE; }

        'zero' { return TOKEN_ZERO; }
        '1' | 'one' { return TOKEN_ONE; }
        'two' { return TOKEN_TWO; }
        'three' { return TOKEN_THREE; }
        'four' { return TOKEN_FOUR; }
        'five' { return TOKEN_FIVE; }
        'six' { return TOKEN_SIX; }
        'seven' { return TOKEN_SEVEN; }
        'eight' { return TOKEN_EIGHT; }
        'nine' { return TOKEN_NINE; }
        'ten' { return TOKEN_TEN; }
        'eleven' { return TOKEN_ELEVEN; }
        'twelve' { return TOKEN_TWELVE; }
        'thirteen' { return TOKEN_THIRTEEN; }
        'fourteen' { return TOKEN_FOURTEEN; }
        'fifteen' { return TOKEN_FIFTEEN; }
        'sixteen' { return TOKEN_SIXTEEN; }
        'seventeen' { return TOKEN_SEVENTEEN; }
        'eighteen' { return TOKEN_EIGHTEEN; }
        'nineteen' { return TOKEN_NINETEEN; }
        'ninteen' { return TOKEN_NINETEEN; }

        'twenty' { return TOKEN_TWENTY; }
        'thirty' { return TOKEN_THIRTY; }
        'forty' { return TOKEN_FORTY; }
        'fourty' { return TOKEN_FORTY; }
        'fifty' { return TOKEN_FIFTY; }
        'sixty' { return TOKEN_SIXTY; }
        'seventy' { return TOKEN_SEVENTY; }
        'eighty' { return TOKEN_EIGHTY; }
        'ninety' { return TOKEN_NINETY; }

        'a hundred' { return TOKEN_HUNDRED; }
        'hundred' { return TOKEN_HUNDRED; }
        'a thousand' { return TOKEN_THOUSAND; }
        'thousand' { return TOKEN_THOUSAND; }
        'a million' { return TOKEN_MILLION; }
        'million' { return TOKEN_MILLION; }
        'a billion' { return TOKEN_BILLION; }
        'billion' { return TOKEN_BILLION; }
        'a trillion' { return TOKEN_TRILLION; }
        'trillion' { return TOKEN_TRILLION; }
        'a quadrillion' { return TOKEN_QUADRILLION; }
        'quadrillion' { return TOKEN_QUADRILLION; }

        'first' { return TOKEN_FIRST; }
        'second' {
            if (!state->parse_second) {
                // dont' parse "second" by default. easiest way to do this is to
                // treat it as a character token
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) {
                        // number followed by character... e.g. "oneself"
                    } else {
                        // finish whatever we had and reset
                        Parse(pParser, 0, *yylval, state);
                    }

                    ParseReset(pParser);
                    state->is_parsing = false;
                }

                state->last_token = TOKEN_CHARACTERS;
                goto fast_path;
            } else {
                return TOKEN_SECOND;
            }
        }
        'third' { return TOKEN_THIRD; }
        'fourth' { return TOKEN_FOURTH; }
        'fifth' { return TOKEN_FIFTH; }
        'sixth' { return TOKEN_SIXTH; }
        'seventh' { return TOKEN_SEVENTH; }
        'eighth' { return TOKEN_EIGHTH; }
        'nineth' { return TOKEN_NINTH; }
        'ninth' { return TOKEN_NINTH; }

        'tenth' { return TOKEN_TENTH; }
        'eleventh' { return TOKEN_ELEVENTH; }
        'twelfth' { return TOKEN_TWELFTH; }
        'thirteenth' { return TOKEN_THIRTEENTH; }
        'fourteenth' { return TOKEN_FOURTEENTH; }
        'fifteenth' { return TOKEN_FIFTEENTH; }
        'sixteenth' { return TOKEN_SIXTEENTH; }
        'seventeenth' { return TOKEN_SEVENTEENTH; }
        'eighteenth' { return TOKEN_EIGHTEENTH; }
        'nineteenth' { return TOKEN_NINETEENTH; }

        'twentieth' { return TOKEN_TWENTIETH; }
        'thirtieth' { return TOKEN_THIRTIETH; }
        'fortieth' { return TOKEN_FOURTIETH; }
        'fourtieth' { return TOKEN_FOURTIETH; }
        'fiftieth' { return TOKEN_FIFTIETH; }
        'sixtieth' { return TOKEN_SIXTIETH; }
        'seventieth' { return TOKEN_SEVENTIETH; }
        'eightieth' { return TOKEN_EIGHTIETH; }
        'ninetieth' { return TOKEN_NINETIETH; }

        'a hundredth' { return TOKEN_HUNDREDTH; }
        'hundredth' { return TOKEN_HUNDREDTH; }
        'a thousandth' { return TOKEN_THOUSANDTH; }
        'thousandth' { return TOKEN_THOUSANDTH; }
        'a millionth' { return TOKEN_MILLIONTH; }
        'millionth' { return TOKEN_MILLIONTH; }
        'a billionth' { return TOKEN_BILLIONTH; }
        'billionth' { return TOKEN_BILLIONTH; }
        'a trillionth' { return TOKEN_TRILLIONTH; }
        'trillionth' { return TOKEN_TRILLIONTH; }
        'a quadrillionth' { return TOKEN_QUADRILLIONTH; }
        'quadrillionth' { return TOKEN_QUADRILLIONTH; }

        'quarter' { return TOKEN_QUARTER; }
        'half' { return TOKEN_HALF; }

        'firsts' { return TOKEN_FIRSTS; }
        'seconds' {
            if (!state->parse_second) {
                // dont' parse "second" by default. easiest way to do this is to
                // treat it as a character token
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) {
                        // number followed by character... e.g. "oneself"
                    } else {
                        // finish whatever we had and reset
                        Parse(pParser, 0, *yylval, state);
                    }

                    ParseReset(pParser);
                    state->is_parsing = false;
                }

                state->last_token = TOKEN_CHARACTERS;
                goto fast_path;
            } else {
                return TOKEN_SECONDS;
            }
        }
        'thirds' { return TOKEN_THIRDS; }
        'fourths' { return TOKEN_FOURTHS; }
        'fifths' { return TOKEN_FIFTHS; }
        'sixths' { return TOKEN_SIXTHS; }
        'sevenths' { return TOKEN_SEVENTHS; }
        'eighths' { return TOKEN_EIGHTHS; }
        'nineths' { return TOKEN_NINTHS; }
        'ninths' { return TOKEN_NINTHS; }

        'tenths' { return TOKEN_TENTHS; }
        'elevenths' { return TOKEN_ELEVENTHS; }
        'twelfths' { return TOKEN_TWELFTHS; }
        'thirteenths' { return TOKEN_THIRTEENTHS; }
        'fourteenths' { return TOKEN_FOURTEENTHS; }
        'fifteenths' { return TOKEN_FIFTEENTHS; }
        'sixteenths' { return TOKEN_SIXTEENTHS; }
        'seventeenths' { return TOKEN_SEVENTEENTHS; }
        'eighteenths' { return TOKEN_EIGHTEENTHS; }
        'nineteenths' { return TOKEN_NINETEENTHS; }

        'twentieths' { return TOKEN_TWENTIETHS; }
        'thirtieths' { return TOKEN_THIRTIETHS; }
        'fourtieths' { return TOKEN_FOURTIETHS; }
        'fiftieths' { return TOKEN_FIFTIETHS; }
        'sixtieths' { return TOKEN_SIXTIETHS; }
        'seventieths' { return TOKEN_SEVENTIETHS; }
        'eightieths' { return TOKEN_EIGHTIETHS; }
        'ninetieths' { return TOKEN_NINETIETHS; }

        'hundredths' { return TOKEN_HUNDREDTHS; }
        'thousandths' { return TOKEN_THOUSANDTHS; }
        'millionths' { return TOKEN_MILLIONTHS; }
        'billionths' { return TOKEN_BILLIONTHS; }
        'trillionths' { return TOKEN_TRILLIONTHS; }
        'quadrillionths' { return TOKEN_QUADRILLIONTHS; }

        'quarters' { return TOKEN_QUARTERS; }
        'halves' { return TOKEN_HALVES; }

        WS {
            if (!state->is_parsing) {
                // not parsing and we've found a character. gobble it and continue
                state->last_token = TOKEN_SEPARATOR;
                goto fast_path;
            }

            return TOKEN_SEPARATOR;
        }
        THREE_PART_DATE {
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) {
                    // number followed by character... e.g. "oneself"
                } else {
                    // finish whatever we had and reset
                    Parse(pParser, 0, *yylval, state);
                }

                ParseReset(pParser);
                state->is_parsing = false;
            }

            state->last_token = TOKEN_CHARACTERS;
            goto fast_path;
        }
        TWO_PART_DATE {
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) {
                    // number followed by character... e.g. "oneself"
                } else {
                    // finish whatever we had and reset
                    Parse(pParser, 0, *yylval, state);
                }

                ParseReset(pParser);
                state->is_parsing = false;
            }

            state->last_token = TOKEN_CHARACTERS;
            goto fast_path;
        }
        DECIMAL_EN_UN {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ',');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_SI {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ' ');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_SI_FR {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ' ');
            replace_char_inplace(tmp, len, ',', '.');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_IRE {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ',');
            len = replace_two_byte_char_inplace((unsigned char*)tmp, len, (unsigned char*)"·", '.');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_ARG {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, '.');
            replace_char_inplace(tmp, len, ',', '.');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_IND {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ',');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_SWI {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, '\'');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_CHI {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ',');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        ZERO_WHOLE_NUMBER {
            // turn string version of number into double
            sds string_value = sdsnewlen(ss->token, ss->cursor - ss->token);
            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_ZERO_WHOLE_NUMBER;
        }
        WHOLE_NUMBER {
            // turn string version of number into double
            sds string_value = sdsnewlen(ss->token, ss->cursor - ss->token);
            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_WHOLE_NUMBER;
        }
        DECIMAL {
            // turn string version of number into double
            sds string_value = sdsnewlen(ss->token, ss->cursor - ss->token);
            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        NULLBYTE {
            if (ss->cursor >= ss->limit) {
                // at the end of the string, so return
                return 0;
            }

            // just at a null byte in the middle of the string, so continue
            goto fast_path;
        }

        L {
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) {
                    // number followed by character... e.g. "oneself"
                } else {
                    // finish whatever we had and reset
                    Parse(pParser, 0, *yylval, state);
                }

                ParseReset(pParser);
                state->is_parsing = false;
            }

            state->last_token = TOKEN_CHARACTERS;
            goto fast_path;
        }

        // MUST COME LAST
        ALL_OTHERS {
            if (state->is_parsing) {
                Parse(pParser, 0, *yylval, state);
                ParseReset(pParser);
                state->is_parsing = false;
            }

            state->last_token = TOKEN_ALL_OTHERS;
            goto fast_path;
        }
    */
}
