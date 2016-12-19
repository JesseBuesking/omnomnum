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

%include {
  /*void copy_core(YYSTYPE *from, YYSTYPE *to) {*/
    /*to->begin = from->begin;*/
    /*to->end = from->end;*/
    /*to->suffix = from->suffix;*/
    /*to->dbl = from->dbl;*/
  /*}*/

  /*void add(YYSTYPE *first, double second, YYSTYPE *to, enum suffixValues suffix) {*/
    /*to->begin = first->begin;*/
    /*to->end = first->end;*/
    /*to->suffix = suffix;*/
    /*to->dbl = first->end + second;*/
  /*}*/

  /*void add(YYSTYPE *first, YYSTYPE *second, YYSTYPE *to, enum suffixValues suffix) {*/
    /*to->begin = mini(first->begin, second->begin);*/
    /*to->end = maxi(first->end, second->end);*/
    /*to->suffix = suffix;*/
    /*to->dbl = first->end + second->end;*/
  /*}*/

  /*void mul(YYSTYPE *first, double second, YYSTYPE *to, enum suffixValues suffix) {*/
    /*to->begin = first->begin;*/
    /*to->end = first->end;*/
    /*to->suffix = suffix;*/
    /*to->dbl = first->end * second;*/
  /*}*/

  /*void mul(YYSTYPE *first, YYSTYPE *second, YYSTYPE *to, enum suffixValues suffix) {*/
    /*to->begin = mini(first->begin, second->begin);*/
    /*to->end = maxi(first->end, second->end);*/
    /*to->suffix = suffix;*/
    /*to->dbl = first->end + second->end;*/
  /*}*/
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

final_number(A) ::= less_than_quadrillion(B) AND_A QUARTER(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + 0.25; A.is_dbl = true; }
final_number(A) ::= less_than_quadrillion(B) QUARTERS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl / 4.0; A.is_dbl = true; }
final_number(A) ::= less_than_quadrillion(B) AND_A HALF(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + 0.5; A.is_dbl = true; }
final_number(A) ::= less_than_quadrillion(B) HALVES(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl / 2.0; A.is_dbl = true; }
final_number(A) ::= less_than_quadrillion(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }
final_number(A) ::= first_to_999999999999999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }
/*A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; }*/
/*final_number(A) ::= fraction(B). {*/
    /*A.frac_num = B.dbl;*/
    /*A.frac_denom = B.dbl;*/
    /*A.is_frac = B.is_frac;*/
/*}*/

// should have this, but if it's not being used in a larger number, we should
// keep it as is: it might be 007. if we dont keep it, we'll reduce it to 7,
// when really we should keep it as is
final_number(A) ::= NUMBER(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = true; }
final_number(A) ::= ZERO(B). { A.begin = B.begin; A.end = B.end; A.dbl = 0.0; }

/*final_number(A) ::= less_than_quadrillion(B) AND fraction(C). {*/
    /*A.dbl = B.dbl / C.dbl;*/
/*}*/

/*final_number(A) ::= less_than_quadrillion(B) AND_A fraction(C). {*/
    /*A.dbl = B.dbl / C.dbl;*/
/*}*/

/*final_number ::= fraction.*/
/*fraction ::= less_than_hundred first_to_99th.*/

/*fraction(A) ::= less_than_quadrillion(B) first_to_999999999999999th(C). {*/
    /*A.frac_num = B.dbl;*/
    /*A.frac_denom = C.dbl;*/
    /*A.is_frac = 1;*/
/*}*/

/* --------------------------------------
sub quadrillion regular
-------------------------------------- */

first_to_999999999999999th(A) ::= less_than_thousand(B) TRILLION first_to_999999999999th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 1000000000000.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; A.suffix = C.suffix; }
first_to_999999999999999th(A) ::= less_than_thousand(B) TRILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.is_dbl = B.is_dbl; A.suffix = TH; }
first_to_999999999999999th(A) ::= first_to_999999999999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }

final_number(A) ::= TRILLIONTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000000.0; A.suffix = TH; }
final_number(A) ::= NUMBER(B) TRILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.suffix = TH; A.is_dbl = true; }

/*trillionths(A) ::= less_than_trillion(B) TRILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.suffix = THS; }*/
/*trillionths(A) ::= NUMBER(B) TRILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.suffix = THS; }*/
/*trillionths(A) ::= TRILLIONTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub quadrillion regular
-------------------------------------- */

less_than_quadrillion(A) ::= less_than_thousand(B) TRILLION less_than_trillion_end_only(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 1000000000000.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; }
less_than_quadrillion(A) ::= less_than_thousand(B) TRILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.is_dbl = B.is_dbl; }
less_than_quadrillion(A) ::= less_than_trillion(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

final_number(A) ::= TRILLION(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000000.0; }
final_number(A) ::= NUMBER(B) TRILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000000.0; A.is_dbl = true; }

/* --------------------------------------
sub trillion ordinal
-------------------------------------- */

first_to_999999999999th(A) ::= less_than_thousand(B) BILLION first_to_999999999th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 1000000000.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; A.suffix = C.suffix; }
first_to_999999999999th(A) ::= less_than_thousand(B) BILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.is_dbl = B.is_dbl; A.suffix = TH; }
first_to_999999999999th(A) ::= first_to_999999999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }

final_number(A) ::= BILLIONTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000.0; A.suffix = TH; }
final_number(A) ::= NUMBER(B) BILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.suffix = TH; A.is_dbl = true; }

/*billionths(A) ::= less_than_billion(B) BILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.suffix = THS; }*/
/*billionths(A) ::= NUMBER(B) BILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.suffix = THS; }*/
/*billionths(A) ::= BILLIONTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub trillion regular
-------------------------------------- */

less_than_trillion_end_only(A) ::= less_than_trillion(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }
less_than_trillion_end_only(A) ::= less_than_billion_end_only(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

less_than_trillion(A) ::= less_than_thousand(B) BILLION less_than_billion_end_only(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 1000000000.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; }
less_than_trillion(A) ::= less_than_thousand(B) BILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.is_dbl = B.is_dbl; }
less_than_trillion(A) ::= less_than_billion(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

final_number(A) ::= BILLION(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000000.0; }
final_number(A) ::= NUMBER(B) BILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000000.0; A.is_dbl = true; }

/* --------------------------------------
sub billion ordinal
-------------------------------------- */

first_to_999999999th(A) ::= less_than_thousand(B) MILLION first_to_999999th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 1000000.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; A.suffix = C.suffix; }
first_to_999999999th(A) ::= less_than_thousand(B) MILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.is_dbl = B.is_dbl; A.suffix = TH; }
first_to_999999999th(A) ::= first_to_999999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }

final_number(A) ::= MILLIONTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000.0; A.suffix = TH; }
final_number(A) ::= NUMBER(B) MILLIONTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.suffix = TH; A.is_dbl = true; }

/*millionths(A) ::= less_than_million(B) MILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.suffix = THS; }*/
/*millionths(A) ::= NUMBER(B) MILLIONTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.suffix = THS; }*/
/*millionths(A) ::= MILLIONTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000.0; A.suffix = THS; }*/

/* --------------------------------------
sub billion regular
-------------------------------------- */

less_than_billion_end_only(A) ::= less_than_billion(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }
less_than_billion_end_only(A) ::= less_than_million_end_only(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

less_than_billion(A) ::= less_than_thousand(B) MILLION less_than_million_end_only(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 1000000.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; }
less_than_billion(A) ::= less_than_thousand(B) MILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.is_dbl = B.is_dbl; }
less_than_billion(A) ::= less_than_million(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

final_number(A) ::= MILLION(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000000.0; }
final_number(A) ::= NUMBER(B) MILLION(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000000.0; A.is_dbl = true; }

/* --------------------------------------
sub million ordinal
-------------------------------------- */

first_to_999999th(A) ::= less_than_thousand(B) THOUSAND first_to_999th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 1000.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; A.suffix = C.suffix; }
first_to_999999th(A) ::= less_than_thousand(B) THOUSANDTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.is_dbl = B.is_dbl; A.suffix = TH; }
first_to_999999th(A) ::= first_to_999th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }

final_number(A) ::= THOUSANDTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000.0; A.suffix = TH; }
final_number(A) ::= NUMBER(B) THOUSANDTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.suffix = TH; A.is_dbl = true; }

/*thousandths(A) ::= less_than_thousand(B) THOUSANDTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.suffix = THS; }*/
/*thousandths(A) ::= NUMBER(B) THOUSANDTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.suffix = THS; }*/
/*thousandths(A) ::= THOUSANDTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000.0; A.suffix = THS; }*/

/* --------------------------------------
sub million regular
-------------------------------------- */

less_than_million_end_only(A) ::= less_than_million(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }
less_than_million_end_only(A) ::= less_than_thousand_end_only(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

less_than_million(A) ::= less_than_thousand(B) THOUSAND less_than_thousand_end_only(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 1000.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; }
less_than_million(A) ::= less_than_thousand(B) THOUSAND(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.is_dbl = B.is_dbl; }
less_than_million(A) ::= less_than_thousand(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

final_number(A) ::= THOUSAND(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1000.0; }
final_number(A) ::= NUMBER(B) THOUSAND(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 1000.0; A.is_dbl = true; }

/* --------------------------------------
sub thousand ordinal
-------------------------------------- */

first_to_999th(A) ::= less_than_hundred(B) HUNDRED AND first_to_99th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 100.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; A.suffix = C.suffix; }
first_to_999th(A) ::= less_than_hundred(B) HUNDRED first_to_99th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 100.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; A.suffix = C.suffix; }
first_to_999th(A) ::= less_than_hundred(B) HUNDREDTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.is_dbl = B.is_dbl; A.suffix = TH; }
first_to_999th(A) ::= AND first_to_99th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }
first_to_999th(A) ::= first_to_99th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }

final_number(A) ::= HUNDREDTH(B). { A.begin = B.begin; A.end = B.end; A.dbl = 100.0; A.suffix = TH; }
final_number(A) ::= NUMBER(B) HUNDREDTH(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.suffix = TH; A.is_dbl = true; }

/*hundredths(A) ::= less_than_hundred(B) HUNDREDTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.suffix = THS; }*/
/*hundredths(A) ::= NUMBER(B) HUNDREDTHS(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.suffix = THS; }*/
/*hundredths(A) ::= HUNDREDTHS(B). { A.begin = B.begin; A.end = B.end; A.dbl = 100.0; A.suffix = THS; }*/

/* --------------------------------------
sub thousand regular
-------------------------------------- */

less_than_thousand_end_only(A) ::= AND less_than_hundred(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }
less_than_thousand_end_only(A) ::= less_than_thousand(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

less_than_thousand(A) ::= less_than_hundred(B) HUNDRED AND less_than_hundred(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 100.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; }
less_than_thousand(A) ::= less_than_hundred(B) HUNDRED less_than_hundred(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = (B.dbl * 100.0) + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; }
less_than_thousand(A) ::= less_than_hundred(B) HUNDRED(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.is_dbl = B.is_dbl; }
less_than_thousand(A) ::= less_than_hundred(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

final_number(A) ::= HUNDRED(B). { A.begin = B.begin; A.end = B.end; A.dbl = 100.0; }
final_number(A) ::= NUMBER(B) HUNDRED(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl * 100.0; A.is_dbl = true; }

/* --------------------------------------
sub hundred ordinal
-------------------------------------- */

first_to_99th(A) ::= tens(B) first_to_9th(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.suffix = C.suffix; A.is_dbl = B.is_dbl || C.is_dbl; }
first_to_99th(A) ::= tenth(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }
first_to_99th(A) ::= tenth_to_19th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }
first_to_99th(A) ::= first_to_9th(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.suffix = B.suffix; A.is_dbl = B.is_dbl; }

/* --------------------------------------
sub hundred regular
-------------------------------------- */

less_than_hundred(A) ::= tens(B) less_than_ten(C). { A.begin = mini(B.begin, C.begin); A.end = maxi(B.end, C.end); A.dbl = B.dbl + C.dbl; A.is_dbl = B.is_dbl || C.is_dbl; }
less_than_hundred(A) ::= tens(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }
less_than_hundred(A) ::= less_than_twenty(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

/* --------------------------------------
sub twenty regular
-------------------------------------- */

less_than_twenty(A) ::= ten_to_19(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }
less_than_twenty(A) ::= less_than_ten(B). { A.begin = B.begin; A.end = B.end; A.dbl = B.dbl; A.is_dbl = B.is_dbl; }

/* --------------------------------------
basic terminals
-------------------------------------- */

less_than_ten(A) ::= ONE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 1.0; }
less_than_ten(A) ::= TWO(B). { A.begin = B.begin; A.end = B.end; A.dbl = 2.0; }
less_than_ten(A) ::= THREE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 3.0; }
less_than_ten(A) ::= FOUR(B). { A.begin = B.begin; A.end = B.end; A.dbl = 4.0; }
less_than_ten(A) ::= FIVE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 5.0; }
less_than_ten(A) ::= SIX(B). { A.begin = B.begin; A.end = B.end; A.dbl = 6.0; }
less_than_ten(A) ::= SEVEN(B). { A.begin = B.begin; A.end = B.end; A.dbl = 7.0; }
less_than_ten(A) ::= EIGHT(B). { A.begin = B.begin; A.end = B.end; A.dbl = 8.0; }
less_than_ten(A) ::= NINE(B). { A.begin = B.begin; A.end = B.end; A.dbl = 9.0; }

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
