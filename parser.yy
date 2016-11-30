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

#define maxi(a, b) (a > b ? a : b)
#define mini(a, b) (a < b ? a : b)
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

// ------------------------------------TODO------------------------------------
// look into using rd, and flagging if rds is used (rd is not typically
// used with fractions, but rds is)
// ------------------------------------TODO------------------------------------

program ::= num_nonnum.
program ::= nonnum_num.

num_nonnum ::= numbers nonnum_num.
num_nonnum ::= numbers.

nonnum_num ::= nonnum num_nonnum.
nonnum_num ::= nonnum.

nonnum ::= nonnum CHARACTERS.
nonnum ::= CHARACTERS.

numbers ::= numbers number.
numbers ::= number.

number ::= final_number(A). {
    insertYYSTYPE(&state->yystypeList, A);
}
number ::= NEGATIVE(A) final_number(B). {
    B.dbl = -B.dbl;
    B.begin = mini(A.begin, B.begin);
    B.end = maxi(A.end, B.end);
    insertYYSTYPE(&state->yystypeList, B);
}

final_number(A) ::= one_to_999999999999999(B) AND_A QUARTER(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + 0.25; }
final_number(A) ::= one_to_999999999999999(B) QUARTERS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl / 4.0; }
final_number(A) ::= one_to_999999999999999(B) AND_A HALF(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + 0.5; }
final_number(A) ::= one_to_999999999999999(B) HALVES(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl / 2.0; }
final_number(A) ::= one_to_999999999999999(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
final_number(A) ::= first_to_999999999999999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
/*final_number(A) ::= fraction(B). {*/
    /*A.frac_num = B.dbl;*/
    /*A.frac_denom = B.dbl;*/
    /*A.is_frac = B.is_frac;*/
/*}*/

// should have this, but if it's not being used in a larger number, we should
// keep it as is: it might be 007. if we dont keep it, we'll reduce it to 7,
// when really we should keep it as is
final_number(A) ::= NUMBER(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
final_number(A) ::= ZERO(B). { A.begin = B.begin; A.end = B.end; A.dbl = 0.0; }

/*final_number(A) ::= one_to_999999999999999(B) AND fraction(C). {*/
    /*A.dbl = B.dbl / C.dbl;*/
/*}*/

/*final_number(A) ::= one_to_999999999999999(B) AND_A fraction(C). {*/
    /*A.dbl = B.dbl / C.dbl;*/
/*}*/

/*final_number ::= fraction.*/
/*fraction ::= one_to_99 first_to_99th.*/

/*fraction(A) ::= one_to_999999999999999(B) first_to_999999999999999th(C). {*/
    /*A.frac_num = B.dbl;*/
    /*A.frac_denom = C.dbl;*/
    /*A.is_frac = 1;*/
/*}*/

/* --------------------------------------
sub quadrillion regular
-------------------------------------- */

first_to_999999999999999th(A) ::= trillions(B) first_to_999999999999th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.suffix = C.suffix; }
first_to_999999999999999th(A) ::= trillionths(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
first_to_999999999999999th(A) ::= first_to_999999999999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }

trillionths(A) ::= one_to_999(B) TRILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.suffix = TH; }
trillionths(A) ::= NUMBER(B) TRILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.suffix = TH; }
trillionths(A) ::= TRILLIONTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000000.0; A.suffix = TH; }

/*trillionths(A) ::= one_to_999999999999(B) TRILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.suffix = THS; }*/
/*trillionths(A) ::= NUMBER(B) TRILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.suffix = THS; }*/
/*trillionths(A) ::= TRILLIONTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub quadrillion regular
-------------------------------------- */

one_to_999999999999999(A) ::= trillions(B) one_to_999999999999(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; }
one_to_999999999999999(A) ::= trillions(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
one_to_999999999999999(A) ::= one_to_999999999999(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }

trillions(A) ::= one_to_999(B) TRILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; }
// TODO hoist up, only applies to top level (aka cant have 99.9 trillion nine hundred thousand, only 99.9 trillion)
trillions(A) ::= NUMBER(B) TRILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; }
trillions(A) ::= TRILLION(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000000.0; }

/* --------------------------------------
sub trillion ordinal
-------------------------------------- */

first_to_999999999999th(A) ::= billions(B) first_to_999999999th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.suffix = C.suffix; }
first_to_999999999999th(A) ::= billionths(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
first_to_999999999999th(A) ::= first_to_999999999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }

billionths(A) ::= one_to_999(B) BILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.suffix = TH; }
billionths(A) ::= NUMBER(B) BILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.suffix = TH; }
billionths(A) ::= BILLIONTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000.0; A.suffix = TH; }

/*billionths(A) ::= one_to_999999999(B) BILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.suffix = THS; }*/
/*billionths(A) ::= NUMBER(B) BILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.suffix = THS; }*/
/*billionths(A) ::= BILLIONTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub trillion regular
-------------------------------------- */

one_to_999999999999(A) ::= billions(B) one_to_999999999(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; }
one_to_999999999999(A) ::= billions(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
one_to_999999999999(A) ::= one_to_999999999(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }

billions(A) ::= one_to_999(B) BILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; }
billions(A) ::= NUMBER(B) BILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; }
billions(A) ::= BILLION(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000.0; }

/* --------------------------------------
sub billion ordinal
-------------------------------------- */

first_to_999999999th(A) ::= millions(B) first_to_999999th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.suffix = C.suffix; }
first_to_999999999th(A) ::= millionths(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
first_to_999999999th(A) ::= first_to_999999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }

millionths(A) ::= one_to_999(B) MILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.suffix = TH; }
millionths(A) ::= NUMBER(B) MILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.suffix = TH; }
millionths(A) ::= MILLIONTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000.0; A.suffix = TH; }

/*millionths(A) ::= one_to_999999(B) MILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.suffix = THS; }*/
/*millionths(A) ::= NUMBER(B) MILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.suffix = THS; }*/
/*millionths(A) ::= MILLIONTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub billion regular
-------------------------------------- */

one_to_999999999(A) ::= millions(B) one_to_999999(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; }
one_to_999999999(A) ::= millions(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
one_to_999999999(A) ::= one_to_999999(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }

millions(A) ::= one_to_999(B) MILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; }
millions(A) ::= NUMBER(B) MILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; }
millions(A) ::= MILLION(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000.0; }

/* --------------------------------------
sub million ordinal
-------------------------------------- */

first_to_999999th(A) ::= thousands(B) first_to_999th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.suffix = C.suffix; }
first_to_999999th(A) ::= thousandths(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
first_to_999999th(A) ::= first_to_999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }

thousandths(A) ::= one_to_999(B) THOUSANDTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.suffix = TH; }
thousandths(A) ::= NUMBER(B) THOUSANDTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.suffix = TH; }
thousandths(A) ::= THOUSANDTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000.0; A.suffix = TH; }

/*thousandths(A) ::= one_to_999(B) THOUSANDTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.suffix = THS; }*/
/*thousandths(A) ::= NUMBER(B) THOUSANDTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.suffix = THS; }*/
/*thousandths(A) ::= THOUSANDTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000.0; A.suffix = THS; }*/

/* --------------------------------------
sub million regular
-------------------------------------- */

one_to_999999(A) ::= thousands(B) one_to_999(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; }
one_to_999999(A) ::= thousands(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
one_to_999999(A) ::= one_to_999(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }

thousands(A) ::= one_to_999(B) THOUSAND(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; }
thousands(A) ::= NUMBER(B) THOUSAND(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; }
thousands(A) ::= THOUSAND(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000.0; }

/* --------------------------------------
sub thousand ordinal
-------------------------------------- */

first_to_999th(A) ::= hundreds(B) AND first_to_99th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.suffix = C.suffix; }
first_to_999th(A) ::= hundreds(B) first_to_99th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.suffix = C.suffix; }
first_to_999th(A) ::= hundredths(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
first_to_999th(A) ::= AND first_to_99th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
first_to_999th(A) ::= first_to_99th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }

hundredths(A) ::= one_to_99(B) HUNDREDTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.suffix = TH; }
hundredths(A) ::= NUMBER(B) HUNDREDTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.suffix = TH; }
hundredths(A) ::= HUNDREDTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 100.0; A.suffix = TH; }

/*hundredths(A) ::= one_to_99(B) HUNDREDTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.suffix = THS; }*/
/*hundredths(A) ::= NUMBER(B) HUNDREDTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.suffix = THS; }*/
/*hundredths(A) ::= HUNDREDTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 100.0; A.suffix = THS; }*/

first_to_99th(A) ::= tens(B) first_to_9th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.suffix = C.suffix; }
first_to_99th(A) ::= tenth(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
first_to_99th(A) ::= tenth_to_19th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }
first_to_99th(A) ::= first_to_9th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }

/* --------------------------------------
sub thousand regular
-------------------------------------- */

one_to_999(A) ::= hundreds(B) AND one_to_99(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; }
one_to_999(A) ::= hundreds(B) one_to_99(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; }
one_to_999(A) ::= hundreds(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
one_to_999(A) ::= AND one_to_99(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
one_to_999(A) ::= one_to_99(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }

hundreds(A) ::= one_to_99(B) HUNDRED(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; }
hundreds(A) ::= NUMBER(B) HUNDRED(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; }
hundreds(A) ::= HUNDRED(B). { A.begin = B.begin; A.end = B.end; A.dbl = 100.0; }

one_to_99(A) ::= tens(B) one_to_9(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; }
one_to_99(A) ::= tens(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
one_to_99(A) ::= ten_to_19(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }
one_to_99(A) ::= one_to_9(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; }

/* --------------------------------------
basic terminals
-------------------------------------- */

one_to_9(A) ::= ONE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1.0; }
one_to_9(A) ::= TWO(B). { A.begin = B.begin; A.end = B.end; A.dbl = 2.0; }
one_to_9(A) ::= THREE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 3.0; }
one_to_9(A) ::= FOUR(B). { A.begin = B.begin; A.end = B.end; A.dbl = 4.0; }
one_to_9(A) ::= FIVE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 5.0; }
one_to_9(A) ::= SIX(B). { A.begin = B.begin; A.end = B.end; A.dbl = 6.0; }
one_to_9(A) ::= SEVEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 7.0; }
one_to_9(A) ::= EIGHT(B). { A.begin = B.begin; A.end = B.end; A.dbl = 8.0; }
one_to_9(A) ::= NINE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 9.0; }

ten_to_19(A) ::= TEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 10.0; }
ten_to_19(A) ::= ELEVEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 11.0; }
ten_to_19(A) ::= TWELVE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 12.0; }
ten_to_19(A) ::= THIRTEEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 13.0; }
ten_to_19(A) ::= FOURTEEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 14.0; }
ten_to_19(A) ::= FIFTEEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 15.0; }
ten_to_19(A) ::= SIXTEEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 16.0; }
ten_to_19(A) ::= SEVENTEEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 17.0; }
ten_to_19(A) ::= EIGHTEEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 18.0; }
ten_to_19(A) ::= NINETEEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 19.0; }

tens(A) ::= TWENTY(B). { A.begin = B.begin; A.end = B.end; A.dbl = 20.0; }
tens(A) ::= THIRTY(B). { A.begin = B.begin; A.end = B.end; A.dbl = 30.0; }
tens(A) ::= FORTY(B). { A.begin = B.begin; A.end = B.end; A.dbl = 40.0; }
tens(A) ::= FIFTY(B). { A.begin = B.begin; A.end = B.end; A.dbl = 50.0; }
tens(A) ::= SIXTY(B). { A.begin = B.begin; A.end = B.end; A.dbl = 60.0; }
tens(A) ::= SEVENTY(B). { A.begin = B.begin; A.end = B.end; A.dbl = 70.0; }
tens(A) ::= EIGHTY(B). { A.begin = B.begin; A.end = B.end; A.dbl = 80.0; }
tens(A) ::= NINETY(B). { A.begin = B.begin; A.end = B.end; A.dbl = 90.0; }

first_to_9th(A) ::= FIRST(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1.0; A.suffix = ST; }
first_to_9th(A) ::= SECOND(B). { A.begin = B.begin; A.end = B.end; A.dbl = 2.0; A.suffix = ND; }
first_to_9th(A) ::= THIRD(B). { A.begin = B.begin; A.end = B.end; A.dbl = 3.0; A.suffix = RD; }
first_to_9th(A) ::= FOURTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 4.0; A.suffix = TH; }
first_to_9th(A) ::= FIFTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 5.0; A.suffix = TH; }
first_to_9th(A) ::= SIXTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 6.0; A.suffix = TH; }
first_to_9th(A) ::= SEVENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 7.0; A.suffix = TH; }
first_to_9th(A) ::= EIGHTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 8.0; A.suffix = TH; }
first_to_9th(A) ::= NINTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 9.0; A.suffix = TH; }

tenth_to_19th(A) ::= TENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 10.0; A.suffix = TH; }
tenth_to_19th(A) ::= ELEVENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 11.0; A.suffix = TH; }
tenth_to_19th(A) ::= TWELFTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 12.0; A.suffix = TH; }
tenth_to_19th(A) ::= THIRTEENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 13.0; A.suffix = TH; }
tenth_to_19th(A) ::= FOURTEENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 14.0; A.suffix = TH; }
tenth_to_19th(A) ::= FIFTEENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 15.0; A.suffix = TH; }
tenth_to_19th(A) ::= SIXTEENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 16.0; A.suffix = TH; }
tenth_to_19th(A) ::= SEVENTEENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 17.0; A.suffix = TH; }
tenth_to_19th(A) ::= EIGHTEENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 18.0; A.suffix = TH; }
tenth_to_19th(A) ::= NINETEENTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 19.0; A.suffix = TH; }

tenth(A) ::= TWENTIETH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 20.0; A.suffix = TH; }
tenth(A) ::= THIRTIETH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 30.0; A.suffix = TH; }
tenth(A) ::= FOURTIETH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 40.0; A.suffix = TH; }
tenth(A) ::= FIFTIETH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 50.0; A.suffix = TH; }
tenth(A) ::= SIXTIETH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 60.0; A.suffix = TH; }
tenth(A) ::= SEVENTIETH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 70.0; A.suffix = TH; }
tenth(A) ::= EIGHTIETH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 80.0; A.suffix = TH; }
tenth(A) ::= NINETIETH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 90.0; A.suffix = TH; }

/*first_to_9ths(A) ::= FIRSTS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1.0; A.suffix = STS; }*/
/*first_to_9ths(A) ::= SECONDS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 2.0; A.suffix = NDS; }*/
/*first_to_9ths(A) ::= THIRDS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 3.0; A.suffix = RDS; }*/
/*first_to_9ths(A) ::= FOURTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 4.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= FIFTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 5.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= SIXTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 6.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= SEVENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 7.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= EIGHTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 8.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= NINTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 9.0; A.suffix = THS; }*/

/*tenth_to_19ths(A) ::= TENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 10.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= ELEVENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 11.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= TWELFTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 12.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= THIRTEENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 13.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= FOURTEENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 14.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= FIFTEENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 15.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= SIXTEENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 16.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= SEVENTEENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 17.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= EIGHTEENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 18.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= NINETEENTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 19.0; A.suffix = THS; }*/

/*tenths(A) ::= TWENTIETHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 20.0; A.suffix = THS; }*/
/*tenths(A) ::= THIRTIETHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 30.0; A.suffix = THS; }*/
/*tenths(A) ::= FOURTIETHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 40.0; A.suffix = THS; }*/
/*tenths(A) ::= FIFTIETHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 50.0; A.suffix = THS; }*/
/*tenths(A) ::= SIXTIETHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 60.0; A.suffix = THS; }*/
/*tenths(A) ::= SEVENTIETHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 70.0; A.suffix = THS; }*/
/*tenths(A) ::= EIGHTIETHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 80.0; A.suffix = THS; }*/
/*tenths(A) ::= NINETIETHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 90.0; A.suffix = THS; }*/
