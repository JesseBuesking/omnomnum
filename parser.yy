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

%token_prefix TOKEN_

%token_type { YYSTYPE }

%extra_argument { ParserState *state }

%include {
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.def.h"
#include "sds.h"
#include "dtoa.h"
}

%syntax_error {
    state->error = SYNTAX_ERROR;
#if print_errors
    fprintf(stderr, "Syntax error\n");
#endif
}

%parse_failure {
    state->error = PARSE_FAILURE;
#if print_errors
    fprintf(stderr,"Giving up.  Parser is hopelessly lost...\n");
#endif
}

%start_symbol program

program ::= numbers.

numbers ::= numbers number.
numbers ::= number.

number ::= final_number(A). {
    insertYYSTYPE(&state->yystypeList, A);
}
number ::= NEGATIVE(A) final_number(B). {
    B.dbl = -B.dbl;
    B.begin = A.begin;
    B.end = B.end;
    insertYYSTYPE(&state->yystypeList, B);
}

final_number(A) ::= less_than_quadrillion(B) AND_A QUARTER(C). { COPY_YYSTYPE_FRAC_SET_MULT(A, B, C, 1.0, 4.0); }
final_number(A) ::= less_than_quadrillion(B) AND_A HALF(C). { COPY_YYSTYPE_FRAC_SET_MULT(A, B, C, 1.0, 2.0); }

final_number(A) ::= less_than_quadrillion(B) QUARTERS(C). { COPY_YYSTYPE_FRAC_SET(A, B, C, B.dbl, 4.0); }
final_number(A) ::= ONE(B) QUARTER(C). { COPY_YYSTYPE_FRAC_SET(A, B, C, 1.0, 4.0); }
final_number(A) ::= A(B) QUARTER(C). { COPY_YYSTYPE_FRAC_SET(A, B, C, 1.0, 4.0); }

final_number(A) ::= less_than_quadrillion(B) HALVES(C). { COPY_YYSTYPE_FRAC_SET(A, B, C, B.dbl, 2.0); }
final_number(A) ::= ONE(B) HALF(C). { COPY_YYSTYPE_FRAC_SET(A, B, C, 1.0, 2.0);  }
final_number(A) ::= A(B) HALF(C). { COPY_YYSTYPE_FRAC_SET(A, B, C, 1.0, 2.0); }

final_number(A) ::= less_than_quadrillion(B). { COPY_YYSTYPE_BE_DBL(A, B); }
final_number(A) ::= less_than_quadrillionth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
final_number(A) ::= less_than_quadrillionths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= less_than_quadrillion(B) AND fraction(C). { COPY_YYSTYPE_FRAC_SET_MULT(A, B, C, C.frac_num, C.frac_denom); }
// covered by above since fractiono includes A/AN
//final_number(A) ::= less_than_quadrillion(B) AND_A fraction(C). { COPY_YYSTYPE_FRAC_SET_MULT(A, B, C, C.frac_num, C.frac_denom); }
final_number(A) ::= fraction(B). { COPY_YYSTYPE_BE(A, B); A.frac_num = B.frac_num; A.frac_denom = B.frac_denom; A.is_frac = B.is_frac; }

// TODO: both of these needs AND in order for multiple numbers to work
//fraction(A) ::= less_than_quadrillion(B) less_than_quadrillionths(C). {
//    COPY_YYSTYPE_BE2(A, B, C);
//    COPY_YYSTYPE_FRAC(A, B, C);
//}
//fraction(A) ::= less_than_quadrillion(B) less_than_quadrillionth(C). {
//    COPY_YYSTYPE_BE2(A, B, C);
//    COPY_YYSTYPE_FRAC(A, B, C);
//}

fraction(A) ::= A(B) less_than_quadrillionth(C). { COPY_YYSTYPE_FRAC_SET(A, B, C, 1.0, C.dbl); }
fraction(A) ::= AN(B) less_than_quadrillionth(C). { COPY_YYSTYPE_FRAC_SET(A, B, C, 1.0, C.dbl); }

// should have this, but if it's not being used in a larger number, we should
// keep it as is: it might be 007. if we dont keep it, we'll reduce it to 7,
// when really we should keep it as is
final_number(A) ::= DECIMAL(B). { COPY_YYSTYPE_BE(A, B); A.dbl = B.dbl; A.is_dbl = true; }
final_number(A) ::= ZERO(B). { COPY_YYSTYPE_BE_VALUE(A, B, 0.0); }

/* --------------------------------------
sub quadrillion ordinal
-------------------------------------- */

less_than_quadrillionth(A) ::= less_than_thousand(B) TRILLION less_than_trillionth_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, TRILLION_F); }
less_than_quadrillionth(A) ::= less_than_thousand(B) TRILLIONTH(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, TRILLION_F, TH); }
less_than_quadrillionth(A) ::= less_than_trillionth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= TRILLIONTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, TRILLION_F, TH); }
final_number(A) ::= DECIMAL(B) TRILLIONTH(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, TRILLION_F, TH); }

// ----------------------------

less_than_quadrillionths(A) ::= less_than_thousand(B) TRILLION less_than_trillionths_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, TRILLION_F); }
less_than_quadrillionths(A) ::= less_than_thousand(B) TRILLIONTHS(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, TRILLION_F, THS); }
less_than_quadrillionths(A) ::= less_than_trillionths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= TRILLIONTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, TRILLION_F, THS); }
final_number(A) ::= DECIMAL(B) TRILLIONTHS(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, TRILLION_F, THS); }

/* --------------------------------------
sub quadrillion regular
-------------------------------------- */

less_than_quadrillion(A) ::= less_than_thousand(B) TRILLION less_than_trillion_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD(A, B, C, TRILLION_F); }
less_than_quadrillion(A) ::= less_than_thousand(B) TRILLION(C). { COPY_YYSTYPE_BE_MUL(A, B, C, TRILLION_F); }
less_than_quadrillion(A) ::= less_than_trillion(B). { COPY_YYSTYPE_BE_DBL(A, B); }

final_number(A) ::= TRILLION(B). { COPY_YYSTYPE_BE_VALUE(A, B, TRILLION_F); }
final_number(A) ::= DECIMAL(B) TRILLION(C). { COPY_YYSTYPE_DBL_NUM(A, B, C, TRILLION_F); }
final_number(A) ::= WHOLE_NUMBER(B) TRILLION(C). { COPY_YYSTYPE_BE_MUL(A, B, C, TRILLION_F); }

/* --------------------------------------
sub trillion ordinal
-------------------------------------- */

less_than_trillionth_end_only(A) ::= less_than_trillionth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_trillionth_end_only(A) ::= less_than_billionth_end_only(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_trillionth(A) ::= less_than_thousand(B) BILLION less_than_billionth_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, BILLION_F); }
less_than_trillionth(A) ::= less_than_thousand(B) BILLIONTH(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, BILLION_F, TH); }
less_than_trillionth(A) ::= less_than_billionth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= BILLIONTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, BILLION_F, TH); }
final_number(A) ::= DECIMAL(B) BILLIONTH(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, BILLION_F, TH); }

// ----------------------------

less_than_trillionths_end_only(A) ::= less_than_trillionths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_trillionths_end_only(A) ::= less_than_billionths_end_only(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_trillionths(A) ::= less_than_thousand(B) BILLION less_than_billionths_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, BILLION_F); }
less_than_trillionths(A) ::= less_than_thousand(B) BILLIONTHS(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, BILLION_F, THS); }
less_than_trillionths(A) ::= less_than_billionths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= BILLIONTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, BILLION_F, THS); }
final_number(A) ::= DECIMAL(B) BILLIONTHS(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, BILLION_F, THS); }

/* --------------------------------------
sub trillion regular
-------------------------------------- */

less_than_trillion_end_only(A) ::= less_than_trillion(B). { COPY_YYSTYPE_BE_DBL(A, B); }
less_than_trillion_end_only(A) ::= less_than_billion_end_only(B). { COPY_YYSTYPE_BE_DBL(A, B); }

less_than_trillion(A) ::= less_than_thousand(B) BILLION less_than_billion_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD(A, B, C, BILLION_F); }
less_than_trillion(A) ::= less_than_thousand(B) BILLION(C). { COPY_YYSTYPE_BE_MUL(A, B, C, BILLION_F); }
less_than_trillion(A) ::= less_than_billion(B). { COPY_YYSTYPE_BE_DBL(A, B); }

final_number(A) ::= BILLION(B). { COPY_YYSTYPE_BE_VALUE(A, B, BILLION_F); }
final_number(A) ::= DECIMAL(B) BILLION(C). { COPY_YYSTYPE_DBL_NUM(A, B, C, BILLION_F); }
final_number(A) ::= WHOLE_NUMBER(B) BILLION(C). { COPY_YYSTYPE_BE_MUL(A, B, C, BILLION_F); }

/* --------------------------------------
sub billion ordinal
-------------------------------------- */

less_than_billionth_end_only(A) ::= less_than_billionth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_billionth_end_only(A) ::= less_than_millionth_end_only(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_billionth(A) ::= less_than_thousand(B) MILLION less_than_millionth_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, MILLION_F); }
less_than_billionth(A) ::= less_than_thousand(B) MILLIONTH(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, MILLION_F, TH); }
less_than_billionth(A) ::= less_than_millionth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= MILLIONTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, MILLION_F, TH); }
final_number(A) ::= DECIMAL(B) MILLIONTH(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, MILLION_F, TH); }

// ----------------------------

less_than_billionths_end_only(A) ::= less_than_billionths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_billionths_end_only(A) ::= less_than_millionths_end_only(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_billionths(A) ::= less_than_thousand(B) MILLION less_than_millionths_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, MILLION_F); }
less_than_billionths(A) ::= less_than_thousand(B) MILLIONTHS(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, MILLION_F, THS); }
less_than_billionths(A) ::= less_than_millionths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= MILLIONTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, MILLION_F, THS); }
final_number(A) ::= DECIMAL(B) MILLIONTHS(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, MILLION_F, THS); }

/* --------------------------------------
sub billion regular
-------------------------------------- */

less_than_billion_end_only(A) ::= less_than_billion(B). { COPY_YYSTYPE_BE_DBL(A, B); }
less_than_billion_end_only(A) ::= less_than_million_end_only(B). { COPY_YYSTYPE_BE_DBL(A, B); }

less_than_billion(A) ::= less_than_thousand(B) MILLION less_than_million_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD(A, B, C, MILLION_F); }
less_than_billion(A) ::= less_than_thousand(B) MILLION(C). { COPY_YYSTYPE_BE_MUL(A, B, C, MILLION_F); }
less_than_billion(A) ::= less_than_million(B). { COPY_YYSTYPE_BE_DBL(A, B); }

final_number(A) ::= MILLION(B). { COPY_YYSTYPE_BE_VALUE(A, B, MILLION_F); }
final_number(A) ::= DECIMAL(B) MILLION(C). { COPY_YYSTYPE_DBL_NUM(A, B, C, MILLION_F); }
final_number(A) ::= WHOLE_NUMBER(B) MILLION(C). { COPY_YYSTYPE_BE_MUL(A, B, C, MILLION_F); }

/* --------------------------------------
sub million ordinal
-------------------------------------- */

less_than_millionth_end_only(A) ::= less_than_millionth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_millionth_end_only(A) ::= less_than_thousandth_end_only(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_millionth(A) ::= less_than_thousand(B) THOUSAND less_than_thousandth_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, THOUSAND_F); }
less_than_millionth(A) ::= less_than_thousand(B) THOUSANDTH(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, THOUSAND_F, TH); }
less_than_millionth(A) ::= less_than_thousandth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= THOUSANDTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, THOUSAND_F, TH); }
final_number(A) ::= DECIMAL(B) THOUSANDTH(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, THOUSAND_F, TH); }

// ----------------------------

less_than_millionths_end_only(A) ::= less_than_millionths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_millionths_end_only(A) ::= less_than_thousandths_end_only(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_millionths(A) ::= less_than_thousand(B) THOUSAND less_than_thousandths_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, THOUSAND_F); }
less_than_millionths(A) ::= less_than_thousand(B) THOUSANDTHS(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, THOUSAND_F, THS); }
less_than_millionths(A) ::= less_than_thousandths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= THOUSANDTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, THOUSAND_F, THS); }
final_number(A) ::= DECIMAL(B) THOUSANDTHS(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, THOUSAND_F, THS); }

/* --------------------------------------
sub million regular
-------------------------------------- */

less_than_million_end_only(A) ::= less_than_million(B). { COPY_YYSTYPE_BE_DBL(A, B); }
less_than_million_end_only(A) ::= less_than_thousand_end_only(B). { COPY_YYSTYPE_BE_DBL(A, B); }

less_than_million(A) ::= less_than_thousand(B) THOUSAND less_than_thousand_end_only(C). { COPY_YYSTYPE_BE_MUL_ADD(A, B, C, THOUSAND_F); }
less_than_million(A) ::= less_than_thousand(B) THOUSAND(C). { COPY_YYSTYPE_BE_MUL(A, B, C, THOUSAND_F); }
less_than_million(A) ::= less_than_thousand(B). { COPY_YYSTYPE_BE_DBL(A, B); }

final_number(A) ::= THOUSAND(B). { COPY_YYSTYPE_BE_VALUE(A, B, THOUSAND_F); }
final_number(A) ::= DECIMAL(B) THOUSAND(C). { COPY_YYSTYPE_DBL_NUM(A, B, C, THOUSAND_F); }
final_number(A) ::= WHOLE_NUMBER(B) THOUSAND(C). { COPY_YYSTYPE_BE_MUL(A, B, C, THOUSAND_F); }

/* --------------------------------------
sub thousand ordinal
-------------------------------------- */

less_than_thousandth_end_only(A) ::= AND less_than_hundredth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_thousandth_end_only(A) ::= less_than_thousandth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_thousandth(A) ::= less_than_hundred(B) HUNDRED AND less_than_hundredth(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, HUNDRED_F); }
less_than_thousandth(A) ::= less_than_hundred(B) HUNDRED less_than_hundredth(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, HUNDRED_F); }
less_than_thousandth(A) ::= less_than_hundred(B) HUNDREDTH(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, HUNDRED_F, TH); }
less_than_thousandth(A) ::= less_than_hundredth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= HUNDREDTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, HUNDRED_F, TH); }
final_number(A) ::= DECIMAL(B) HUNDREDTH(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, HUNDRED_F, TH); }

// ----------------------------

less_than_thousandths_end_only(A) ::= AND less_than_hundredths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_thousandths_end_only(A) ::= less_than_thousandths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_thousandths(A) ::= less_than_hundred(B) HUNDRED AND less_than_hundredths(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, HUNDRED_F); }
less_than_thousandths(A) ::= less_than_hundred(B) HUNDRED less_than_hundredths(C). { COPY_YYSTYPE_BE_MUL_ADD_SUFF(A, B, C, HUNDRED_F); }
less_than_thousandths(A) ::= less_than_hundred(B) HUNDREDTHS(C). { COPY_YYSTYPE_BE_MUL_SUFF(A, B, C, HUNDRED_F, THS); }
less_than_thousandths(A) ::= less_than_hundredths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

final_number(A) ::= HUNDREDTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, HUNDRED_F, THS); }
// TODO I think this is covered above in the fraction section
final_number(A) ::= DECIMAL(B) HUNDREDTHS(C). { COPY_YYSTYPE_DBL_NUM_SUFF(A, B, C, HUNDRED_F, THS); }

/* --------------------------------------
sub thousand regular
-------------------------------------- */

less_than_thousand_end_only(A) ::= AND less_than_hundred(B). { COPY_YYSTYPE_BE_DBL(A, B); }
less_than_thousand_end_only(A) ::= less_than_thousand(B). { COPY_YYSTYPE_BE_DBL(A, B); }

less_than_thousand(A) ::= less_than_hundred(B) HUNDRED AND less_than_hundred(C). { COPY_YYSTYPE_BE_MUL_ADD(A, B, C, HUNDRED_F); }
less_than_thousand(A) ::= less_than_hundred(B) HUNDRED less_than_hundred(C). { COPY_YYSTYPE_BE_MUL_ADD(A, B, C, HUNDRED_F); }
less_than_thousand(A) ::= less_than_hundred(B) HUNDRED(C). { COPY_YYSTYPE_BE_MUL(A, B, C, HUNDRED_F); }
less_than_thousand(A) ::= less_than_hundred(B). { COPY_YYSTYPE_BE_DBL(A, B); }

final_number(A) ::= HUNDRED(B). { COPY_YYSTYPE_BE_VALUE(A, B, HUNDRED_F); }
final_number(A) ::= DECIMAL(B) HUNDRED(C). { COPY_YYSTYPE_DBL_NUM(A, B, C, HUNDRED_F); }
final_number(A) ::= WHOLE_NUMBER(B) HUNDRED(C). { COPY_YYSTYPE_BE_MUL(A, B, C, HUNDRED_F); }

/* --------------------------------------
sub hundred ordinal
-------------------------------------- */

less_than_hundredth(A) ::= tens(B) less_than_tenth(C). { COPY_YYSTYPE_BE_ADD_SUFF(A, B, C); }
less_than_hundredth(A) ::= tenth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_hundredth(A) ::= less_than_twentieth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_hundredths(A) ::= tens(B) less_than_tenths(C). { COPY_YYSTYPE_BE_ADD_SUFF(A, B, C); }
less_than_hundredths(A) ::= tenths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_hundredths(A) ::= less_than_twentieths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

/* --------------------------------------
sub hundred regular
-------------------------------------- */

less_than_hundred(A) ::= tens(B) less_than_ten(C). { COPY_YYSTYPE_BE_ADD(A, B, C); }
less_than_hundred(A) ::= tens(B). { COPY_YYSTYPE_BE_DBL(A, B); }
less_than_hundred(A) ::= less_than_twenty(B). { COPY_YYSTYPE_BE_DBL(A, B); }

/* --------------------------------------
sub twenty ordinal
-------------------------------------- */

less_than_twentieth(A) ::= tenth_to_19th(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_twentieth(A) ::= less_than_tenth(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

less_than_twentieths(A) ::= tenths_to_19ths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }
less_than_twentieths(A) ::= less_than_tenths(B). { COPY_YYSTYPE_BE_DBL_SUFF(A, B); }

/* --------------------------------------
sub twenty regular
-------------------------------------- */

less_than_twenty(A) ::= ten_to_19(B). { COPY_YYSTYPE_BE_DBL(A, B); }
less_than_twenty(A) ::= less_than_ten(B). { COPY_YYSTYPE_BE_DBL(A, B); }

/* --------------------------------------
basic terminals
-------------------------------------- */

less_than_ten(A) ::= ONE(B). { COPY_YYSTYPE_BE_VALUE(A, B, 1.0); }
less_than_ten(A) ::= TWO(B). { COPY_YYSTYPE_BE_VALUE(A, B, 2.0); }
less_than_ten(A) ::= THREE(B). { COPY_YYSTYPE_BE_VALUE(A, B, 3.0); }
less_than_ten(A) ::= FOUR(B). { COPY_YYSTYPE_BE_VALUE(A, B, 4.0); }
less_than_ten(A) ::= FIVE(B). { COPY_YYSTYPE_BE_VALUE(A, B, 5.0); }
less_than_ten(A) ::= SIX(B). { COPY_YYSTYPE_BE_VALUE(A, B, 6.0); }
less_than_ten(A) ::= SEVEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 7.0); }
less_than_ten(A) ::= EIGHT(B). { COPY_YYSTYPE_BE_VALUE(A, B, 8.0); }
less_than_ten(A) ::= NINE(B). { COPY_YYSTYPE_BE_VALUE(A, B, 9.0); }

ten_to_19(A) ::= TEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 10.0); }
ten_to_19(A) ::= ELEVEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 11.0); }
ten_to_19(A) ::= TWELVE(B). { COPY_YYSTYPE_BE_VALUE(A, B, 12.0); }
ten_to_19(A) ::= THIRTEEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 13.0); }
ten_to_19(A) ::= FOURTEEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 14.0); }
ten_to_19(A) ::= FIFTEEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 15.0); }
ten_to_19(A) ::= SIXTEEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 16.0); }
ten_to_19(A) ::= SEVENTEEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 17.0); }
ten_to_19(A) ::= EIGHTEEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 18.0); }
ten_to_19(A) ::= NINETEEN(B). { COPY_YYSTYPE_BE_VALUE(A, B, 19.0); }

tens(A) ::= TWENTY(B). { COPY_YYSTYPE_BE_VALUE(A, B, 20.0); }
tens(A) ::= THIRTY(B). { COPY_YYSTYPE_BE_VALUE(A, B, 30.0); }
tens(A) ::= FORTY(B). { COPY_YYSTYPE_BE_VALUE(A, B, 40.0); }
tens(A) ::= FIFTY(B). { COPY_YYSTYPE_BE_VALUE(A, B, 50.0); }
tens(A) ::= SIXTY(B). { COPY_YYSTYPE_BE_VALUE(A, B, 60.0); }
tens(A) ::= SEVENTY(B). { COPY_YYSTYPE_BE_VALUE(A, B, 70.0); }
tens(A) ::= EIGHTY(B). { COPY_YYSTYPE_BE_VALUE(A, B, 80.0); }
tens(A) ::= NINETY(B). { COPY_YYSTYPE_BE_VALUE(A, B, 90.0); }

less_than_tenth(A) ::= FIRST(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 1.0, ST); }
less_than_tenth(A) ::= SECOND(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 2.0, ND); }
less_than_tenth(A) ::= THIRD(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 3.0, RD); }
less_than_tenth(A) ::= FOURTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 4.0, TH); }
less_than_tenth(A) ::= FIFTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 5.0, TH); }
less_than_tenth(A) ::= SIXTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 6.0, TH); }
less_than_tenth(A) ::= SEVENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 7.0, TH); }
less_than_tenth(A) ::= EIGHTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 8.0, TH); }
less_than_tenth(A) ::= NINTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 9.0, TH); }

tenth_to_19th(A) ::= TENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 10.0, TH); }
tenth_to_19th(A) ::= ELEVENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 11.0, TH); }
tenth_to_19th(A) ::= TWELFTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 12.0, TH); }
tenth_to_19th(A) ::= THIRTEENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 13.0, TH); }
tenth_to_19th(A) ::= FOURTEENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 14.0, TH); }
tenth_to_19th(A) ::= FIFTEENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 15.0, TH); }
tenth_to_19th(A) ::= SIXTEENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 16.0, TH); }
tenth_to_19th(A) ::= SEVENTEENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 17.0, TH); }
tenth_to_19th(A) ::= EIGHTEENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 18.0, TH); }
tenth_to_19th(A) ::= NINETEENTH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 19.0, TH); }

tenth(A) ::= TWENTIETH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 20.0, TH); }
tenth(A) ::= THIRTIETH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 30.0, TH); }
tenth(A) ::= FOURTIETH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 40.0, TH); }
tenth(A) ::= FIFTIETH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 50.0, TH); }
tenth(A) ::= SIXTIETH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 60.0, TH); }
tenth(A) ::= SEVENTIETH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 70.0, TH); }
tenth(A) ::= EIGHTIETH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 80.0, TH); }
tenth(A) ::= NINETIETH(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 90.0, TH); }

less_than_tenths(A) ::= FIRSTS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 1.0, STS); }
//less_than_tenths(A) ::= SECONDS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 2.0, NDS); }
less_than_tenths(A) ::= THIRDS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 3.0, RDS); }
less_than_tenths(A) ::= FOURTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 4.0, THS); }
less_than_tenths(A) ::= FIFTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 5.0, THS); }
less_than_tenths(A) ::= SIXTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 6.0, THS); }
less_than_tenths(A) ::= SEVENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 7.0, THS); }
less_than_tenths(A) ::= EIGHTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 8.0, THS); }
less_than_tenths(A) ::= NINTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 9.0, THS); }

tenths_to_19ths(A) ::= TENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 10.0, THS); }
tenths_to_19ths(A) ::= ELEVENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 11.0, THS); }
tenths_to_19ths(A) ::= TWELFTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 12.0, THS); }
tenths_to_19ths(A) ::= THIRTEENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 13.0, THS); }
tenths_to_19ths(A) ::= FOURTEENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 14.0, THS); }
tenths_to_19ths(A) ::= FIFTEENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 15.0, THS); }
tenths_to_19ths(A) ::= SIXTEENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 16.0, THS); }
tenths_to_19ths(A) ::= SEVENTEENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 17.0, THS); }
tenths_to_19ths(A) ::= EIGHTEENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 18.0, THS); }
tenths_to_19ths(A) ::= NINETEENTHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 19.0, THS); }

tenths(A) ::= TWENTIETHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 20.0, THS); }
tenths(A) ::= THIRTIETHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 30.0, THS); }
tenths(A) ::= FOURTIETHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 40.0, THS); }
tenths(A) ::= FIFTIETHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 50.0, THS); }
tenths(A) ::= SIXTIETHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 60.0, THS); }
tenths(A) ::= SEVENTIETHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 70.0, THS); }
tenths(A) ::= EIGHTIETHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 80.0, THS); }
tenths(A) ::= NINETIETHS(B). { COPY_YYSTYPE_BE_VALUE_SUFF(A, B, 90.0, THS); }
