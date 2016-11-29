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
    B.double_value = -B.double_value;
    B.spos = mini(A.spos, B.spos);
    B.epos = maxi(A.epos, B.epos);
    insertYYSTYPE(&state->yystypeList, B);
}

final_number(A) ::= one_to_999999999999999(B) AND_A QUARTER(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + 0.25; }
final_number(A) ::= one_to_999999999999999(B) QUARTERS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value / 4.0; }
final_number(A) ::= one_to_999999999999999(B) AND_A HALF(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + 0.5; }
final_number(A) ::= one_to_999999999999999(B) HALVES(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value / 2.0; }
final_number(A) ::= one_to_999999999999999(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
final_number(A) ::= first_to_999999999999999th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
/*final_number(A) ::= fraction(B). {*/
    /*A.frac_num = B.double_value;*/
    /*A.frac_denom = B.double_value;*/
    /*A.is_frac = B.is_frac;*/
/*}*/

// should have this, but if it's not being used in a larger number, we should
// keep it as is: it might be 007. if we dont keep it, we'll reduce it to 7,
// when really we should keep it as is
final_number(A) ::= NUMBER(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
final_number(A) ::= ZERO(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 0.0; }

/*final_number(A) ::= one_to_999999999999999(B) AND fraction(C). {*/
    /*A.double_value = B.double_value / C.double_value;*/
/*}*/

/*final_number(A) ::= one_to_999999999999999(B) AND_A fraction(C). {*/
    /*A.double_value = B.double_value / C.double_value;*/
/*}*/

/*final_number ::= fraction.*/
/*fraction ::= one_to_99 first_to_99th.*/

/*fraction(A) ::= one_to_999999999999999(B) first_to_999999999999999th(C). {*/
    /*A.frac_num = B.double_value;*/
    /*A.frac_denom = C.double_value;*/
    /*A.is_frac = 1;*/
/*}*/

/* --------------------------------------
sub quadrillion regular
-------------------------------------- */

first_to_999999999999999th(A) ::= trillions(B) first_to_999999999999th(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; A.suffix = C.suffix; }
first_to_999999999999999th(A) ::= trillionths(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
first_to_999999999999999th(A) ::= first_to_999999999999th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }

trillionths(A) ::= one_to_999(B) TRILLIONTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000000.0; A.suffix = TH; }
trillionths(A) ::= NUMBER(B) TRILLIONTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000000.0; A.suffix = TH; }
trillionths(A) ::= TRILLIONTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000000000.0; A.suffix = TH; }

/*trillionths(A) ::= one_to_999999999999(B) TRILLIONTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000000.0; A.suffix = THS; }*/
/*trillionths(A) ::= NUMBER(B) TRILLIONTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000000.0; A.suffix = THS; }*/
/*trillionths(A) ::= TRILLIONTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub quadrillion regular
-------------------------------------- */

one_to_999999999999999(A) ::= trillions(B) one_to_999999999999(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; }
one_to_999999999999999(A) ::= trillions(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
one_to_999999999999999(A) ::= one_to_999999999999(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }

trillions(A) ::= one_to_999(B) TRILLION(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000000.0; }
trillions(A) ::= NUMBER(B) TRILLION(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000000.0; }
trillions(A) ::= TRILLION(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000000000.0; }

/* --------------------------------------
sub trillion ordinal
-------------------------------------- */

first_to_999999999999th(A) ::= billions(B) first_to_999999999th(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; A.suffix = C.suffix; }
first_to_999999999999th(A) ::= billionths(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
first_to_999999999999th(A) ::= first_to_999999999th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }

billionths(A) ::= one_to_999(B) BILLIONTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000.0; A.suffix = TH; }
billionths(A) ::= NUMBER(B) BILLIONTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000.0; A.suffix = TH; }
billionths(A) ::= BILLIONTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000000.0; A.suffix = TH; }

/*billionths(A) ::= one_to_999999999(B) BILLIONTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000.0; A.suffix = THS; }*/
/*billionths(A) ::= NUMBER(B) BILLIONTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000.0; A.suffix = THS; }*/
/*billionths(A) ::= BILLIONTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub trillion regular
-------------------------------------- */

one_to_999999999999(A) ::= billions(B) one_to_999999999(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; }
one_to_999999999999(A) ::= billions(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
one_to_999999999999(A) ::= one_to_999999999(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }

billions(A) ::= one_to_999(B) BILLION(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000.0; }
billions(A) ::= NUMBER(B) BILLION(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000000.0; }
billions(A) ::= BILLION(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000000.0; }

/* --------------------------------------
sub billion ordinal
-------------------------------------- */

first_to_999999999th(A) ::= millions(B) first_to_999999th(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; A.suffix = C.suffix; }
first_to_999999999th(A) ::= millionths(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
first_to_999999999th(A) ::= first_to_999999th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }

millionths(A) ::= one_to_999(B) MILLIONTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000.0; A.suffix = TH; }
millionths(A) ::= NUMBER(B) MILLIONTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000.0; A.suffix = TH; }
millionths(A) ::= MILLIONTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000.0; A.suffix = TH; }

/*millionths(A) ::= one_to_999999(B) MILLIONTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000.0; A.suffix = THS; }*/
/*millionths(A) ::= NUMBER(B) MILLIONTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000.0; A.suffix = THS; }*/
/*millionths(A) ::= MILLIONTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub billion regular
-------------------------------------- */

one_to_999999999(A) ::= millions(B) one_to_999999(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; }
one_to_999999999(A) ::= millions(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
one_to_999999999(A) ::= one_to_999999(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }

millions(A) ::= one_to_999(B) MILLION(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000.0; }
millions(A) ::= NUMBER(B) MILLION(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000000.0; }
millions(A) ::= MILLION(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000000.0; }

/* --------------------------------------
sub million ordinal
-------------------------------------- */

first_to_999999th(A) ::= thousands(B) first_to_999th(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; A.suffix = C.suffix; }
first_to_999999th(A) ::= thousandths(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
first_to_999999th(A) ::= first_to_999th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }

thousandths(A) ::= one_to_999(B) THOUSANDTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000.0; A.suffix = TH; }
thousandths(A) ::= NUMBER(B) THOUSANDTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000.0; A.suffix = TH; }
thousandths(A) ::= THOUSANDTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000.0; A.suffix = TH; }

/*thousandths(A) ::= one_to_999(B) THOUSANDTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000.0; A.suffix = THS; }*/
/*thousandths(A) ::= NUMBER(B) THOUSANDTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000.0; A.suffix = THS; }*/
/*thousandths(A) ::= THOUSANDTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000.0; A.suffix = THS; }*/

/* --------------------------------------
sub million regular
-------------------------------------- */

one_to_999999(A) ::= thousands(B) one_to_999(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; }
one_to_999999(A) ::= thousands(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
one_to_999999(A) ::= one_to_999(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }

thousands(A) ::= one_to_999(B) THOUSAND(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000.0; }
thousands(A) ::= NUMBER(B) THOUSAND(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 1000.0; }
thousands(A) ::= THOUSAND(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1000.0; }

/* --------------------------------------
sub thousand ordinal
-------------------------------------- */

first_to_999th(A) ::= hundreds(B) AND first_to_99th(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; A.suffix = C.suffix; }
first_to_999th(A) ::= hundreds(B) first_to_99th(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; A.suffix = C.suffix; }
first_to_999th(A) ::= hundredths(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
first_to_999th(A) ::= AND first_to_99th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
first_to_999th(A) ::= first_to_99th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }

hundredths(A) ::= one_to_99(B) HUNDREDTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 100.0; A.suffix = TH; }
hundredths(A) ::= NUMBER(B) HUNDREDTH(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 100.0; A.suffix = TH; }
hundredths(A) ::= HUNDREDTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 100.0; A.suffix = TH; }

/*hundredths(A) ::= one_to_99(B) HUNDREDTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 100.0; A.suffix = THS; }*/
/*hundredths(A) ::= NUMBER(B) HUNDREDTHS(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 100.0; A.suffix = THS; }*/
/*hundredths(A) ::= HUNDREDTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 100.0; A.suffix = THS; }*/

first_to_99th(A) ::= tens(B) first_to_9th(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; A.suffix = C.suffix; }
first_to_99th(A) ::= tenth(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
first_to_99th(A) ::= tenth_to_19th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }
first_to_99th(A) ::= first_to_9th(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; A.suffix = B.suffix; }

/* --------------------------------------
sub thousand regular
-------------------------------------- */

one_to_999(A) ::= hundreds(B) AND one_to_99(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; }
one_to_999(A) ::= hundreds(B) one_to_99(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; }
one_to_999(A) ::= hundreds(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
one_to_999(A) ::= AND one_to_99(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
one_to_999(A) ::= one_to_99(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }

hundreds(A) ::= one_to_99(B) HUNDRED(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 100.0; }
hundreds(A) ::= NUMBER(B) HUNDRED(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value * 100.0; }
hundreds(A) ::= HUNDRED(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 100.0; }

one_to_99(A) ::= tens(B) one_to_9(C). { A.spos = mini(B.spos, C.spos); A.epos = maxi(B.epos, C.epos); A.double_value = B.double_value + C.double_value; }
one_to_99(A) ::= tens(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
one_to_99(A) ::= ten_to_19(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }
one_to_99(A) ::= one_to_9(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = B.double_value; }

/* --------------------------------------
basic terminals
-------------------------------------- */

one_to_9(A) ::= ONE(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1.0; }
one_to_9(A) ::= TWO(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 2.0; }
one_to_9(A) ::= THREE(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 3.0; }
one_to_9(A) ::= FOUR(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 4.0; }
one_to_9(A) ::= FIVE(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 5.0; }
one_to_9(A) ::= SIX(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 6.0; }
one_to_9(A) ::= SEVEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 7.0; }
one_to_9(A) ::= EIGHT(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 8.0; }
one_to_9(A) ::= NINE(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 9.0; }

ten_to_19(A) ::= TEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 10.0; }
ten_to_19(A) ::= ELEVEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 11.0; }
ten_to_19(A) ::= TWELVE(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 12.0; }
ten_to_19(A) ::= THIRTEEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 13.0; }
ten_to_19(A) ::= FOURTEEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 14.0; }
ten_to_19(A) ::= FIFTEEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 15.0; }
ten_to_19(A) ::= SIXTEEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 16.0; }
ten_to_19(A) ::= SEVENTEEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 17.0; }
ten_to_19(A) ::= EIGHTEEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 18.0; }
ten_to_19(A) ::= NINETEEN(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 19.0; }

tens(A) ::= TWENTY(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 20.0; }
tens(A) ::= THIRTY(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 30.0; }
tens(A) ::= FORTY(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 40.0; }
tens(A) ::= FIFTY(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 50.0; }
tens(A) ::= SIXTY(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 60.0; }
tens(A) ::= SEVENTY(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 70.0; }
tens(A) ::= EIGHTY(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 80.0; }
tens(A) ::= NINETY(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 90.0; }

first_to_9th(A) ::= FIRST(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1.0; A.suffix = ST; }
first_to_9th(A) ::= SECOND(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 2.0; A.suffix = ND; }
first_to_9th(A) ::= THIRD(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 3.0; A.suffix = RD; }
first_to_9th(A) ::= FOURTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 4.0; A.suffix = TH; }
first_to_9th(A) ::= FIFTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 5.0; A.suffix = TH; }
first_to_9th(A) ::= SIXTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 6.0; A.suffix = TH; }
first_to_9th(A) ::= SEVENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 7.0; A.suffix = TH; }
first_to_9th(A) ::= EIGHTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 8.0; A.suffix = TH; }
first_to_9th(A) ::= NINTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 9.0; A.suffix = TH; }

tenth_to_19th(A) ::= TENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 10.0; A.suffix = TH; }
tenth_to_19th(A) ::= ELEVENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 11.0; A.suffix = TH; }
tenth_to_19th(A) ::= TWELFTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 12.0; A.suffix = TH; }
tenth_to_19th(A) ::= THIRTEENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 13.0; A.suffix = TH; }
tenth_to_19th(A) ::= FOURTEENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 14.0; A.suffix = TH; }
tenth_to_19th(A) ::= FIFTEENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 15.0; A.suffix = TH; }
tenth_to_19th(A) ::= SIXTEENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 16.0; A.suffix = TH; }
tenth_to_19th(A) ::= SEVENTEENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 17.0; A.suffix = TH; }
tenth_to_19th(A) ::= EIGHTEENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 18.0; A.suffix = TH; }
tenth_to_19th(A) ::= NINETEENTH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 19.0; A.suffix = TH; }

tenth(A) ::= TWENTIETH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 20.0; A.suffix = TH; }
tenth(A) ::= THIRTIETH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 30.0; A.suffix = TH; }
tenth(A) ::= FOURTIETH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 40.0; A.suffix = TH; }
tenth(A) ::= FIFTIETH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 50.0; A.suffix = TH; }
tenth(A) ::= SIXTIETH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 60.0; A.suffix = TH; }
tenth(A) ::= SEVENTIETH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 70.0; A.suffix = TH; }
tenth(A) ::= EIGHTIETH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 80.0; A.suffix = TH; }
tenth(A) ::= NINETIETH(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 90.0; A.suffix = TH; }

/*first_to_9ths(A) ::= FIRSTS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 1.0; A.suffix = STS; }*/
/*first_to_9ths(A) ::= SECONDS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 2.0; A.suffix = NDS; }*/
/*first_to_9ths(A) ::= THIRDS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 3.0; A.suffix = RDS; }*/
/*first_to_9ths(A) ::= FOURTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 4.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= FIFTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 5.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= SIXTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 6.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= SEVENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 7.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= EIGHTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 8.0; A.suffix = THS; }*/
/*first_to_9ths(A) ::= NINTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 9.0; A.suffix = THS; }*/

/*tenth_to_19ths(A) ::= TENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 10.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= ELEVENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 11.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= TWELFTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 12.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= THIRTEENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 13.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= FOURTEENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 14.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= FIFTEENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 15.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= SIXTEENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 16.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= SEVENTEENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 17.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= EIGHTEENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 18.0; A.suffix = THS; }*/
/*tenth_to_19ths(A) ::= NINETEENTHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 19.0; A.suffix = THS; }*/

/*tenths(A) ::= TWENTIETHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 20.0; A.suffix = THS; }*/
/*tenths(A) ::= THIRTIETHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 30.0; A.suffix = THS; }*/
/*tenths(A) ::= FOURTIETHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 40.0; A.suffix = THS; }*/
/*tenths(A) ::= FIFTIETHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 50.0; A.suffix = THS; }*/
/*tenths(A) ::= SIXTIETHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 60.0; A.suffix = THS; }*/
/*tenths(A) ::= SEVENTIETHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 70.0; A.suffix = THS; }*/
/*tenths(A) ::= EIGHTIETHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 80.0; A.suffix = THS; }*/
/*tenths(A) ::= NINETIETHS(B). { A.spos = B.spos; A.epos = B.epos; A.double_value = 90.0; A.suffix = THS; }*/
