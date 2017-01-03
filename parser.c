/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 37 "parser.yy"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.def.h"
#include "sds.h"
#include "dtoa.h"

#ifndef COPY_YYSTYPE_BE
#define COPY_YYSTYPE_BE(A, B) \
    A.begin = B.begin; \
    A.end = B.end; \
    A.is_frac = B.is_frac; \
    A.is_dbl = B.is_dbl; \
    A.leave_alone = B.leave_alone;
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
#define COPY_YYSTYPE_FRAC(A, B, C) \
    A.frac_num = B.dbl; \
    A.frac_denom = C.dbl; \
    A.is_frac = true;
#endif

#ifndef COPY_YYSTYPE_SUFF
#define COPY_YYSTYPE_SUFF(A, B) \
    A.suffix = B.suffix;
#endif

#ifndef COPY_YYSTYPE_BE_DBL
#define COPY_YYSTYPE_BE_DBL(A, B) \
    COPY_YYSTYPE_BE(A, B); \
    COPY_YYSTYPE_DBL(A, B);
#endif

#ifndef COPY_YYSTYPE_BE_DBL_SUFF
#define COPY_YYSTYPE_BE_DBL_SUFF(A, B) \
    COPY_YYSTYPE_BE_DBL(A, B); \
    COPY_YYSTYPE_SUFF(A, B);
#endif

#ifndef COPY_YYSTYPE_BE2
#define COPY_YYSTYPE_BE2(A, B, C) \
    A.begin = B.begin; \
    A.end = C.end; \
    A.is_frac = B.is_frac | C.is_frac; \
    A.is_dbl = B.is_dbl | C.is_dbl; \
    A.leave_alone = B.leave_alone | C.leave_alone;
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

#ifndef COPY_YYSTYPE_BE_ADD_VALUE
#define COPY_YYSTYPE_BE_ADD_VALUE(A, B, C, VALUE) \
    COPY_YYSTYPE_BE2(A, B, C); \
    A.dbl = VALUE + C.dbl; \
    A.is_dbl = B.is_dbl || C.is_dbl;
#endif

#ifndef COPY_YYSTYPE_BE_ADD_SUFF
#define COPY_YYSTYPE_BE_ADD_SUFF(A, B, C) \
    COPY_YYSTYPE_BE_ADD(A, B, C); \
    A.suffix = C.suffix;
#endif

#ifndef COPY_YYSTYPE_BE_ADD_SUFF_VALUE
#define COPY_YYSTYPE_BE_ADD_SUFF_VALUE(A, B, C, VALUE) \
    COPY_YYSTYPE_BE2(A, B, C); \
    A.dbl = VALUE + C.dbl; \
    A.is_dbl = B.is_dbl || C.is_dbl; \
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

#ifndef COPY_YYSTYPE_FRAC_SET
#define COPY_YYSTYPE_FRAC_SET(A, B, C, NUMERATOR, DENOMINATOR) \
    COPY_YYSTYPE_BE2(A, B, C); \
    A.frac_num = NUMERATOR; \
    A.frac_denom = DENOMINATOR; \
    A.is_frac = true;
#endif

#ifndef COPY_YYSTYPE_FRAC_SET_MULT
#define COPY_YYSTYPE_FRAC_SET_MULT(A, B, C, NUMERATOR, DENOMINATOR) \
    COPY_YYSTYPE_BE2(A, B, C); \
    A.frac_num = (B.dbl * DENOMINATOR) + NUMERATOR; \
    A.frac_denom = DENOMINATOR; \
    A.is_frac = true;
#endif

#define HUNDRED_F  100.0
#define THOUSAND_F 1000.0
#define MILLION_F  1000000.0
#define BILLION_F  1000000000.0
#define TRILLION_F 1000000000000.0
#line 186 "parser.c"
#include "parser.h"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
#if INTERFACE
#define TOKEN_NEGATIVE                        1
#define TOKEN_AND_A                           2
#define TOKEN_QUARTER                         3
#define TOKEN_HALF                            4
#define TOKEN_QUARTERS                        5
#define TOKEN_ONE                             6
#define TOKEN_A                               7
#define TOKEN_HALVES                          8
#define TOKEN_AND                             9
#define TOKEN_AN                             10
#define TOKEN_ZERO_WHOLE_NUMBER              11
#define TOKEN_WHOLE_NUMBER                   12
#define TOKEN_DECIMAL                        13
#define TOKEN_ZERO                           14
#define TOKEN_TRILLION                       15
#define TOKEN_TRILLIONTH                     16
#define TOKEN_TRILLIONTHS                    17
#define TOKEN_BILLION                        18
#define TOKEN_BILLIONTH                      19
#define TOKEN_BILLIONTHS                     20
#define TOKEN_MILLION                        21
#define TOKEN_MILLIONTH                      22
#define TOKEN_MILLIONTHS                     23
#define TOKEN_THOUSAND                       24
#define TOKEN_THOUSANDTH                     25
#define TOKEN_THOUSANDTHS                    26
#define TOKEN_HUNDRED                        27
#define TOKEN_HUNDREDTH                      28
#define TOKEN_HUNDREDTHS                     29
#define TOKEN_TWO                            30
#define TOKEN_THREE                          31
#define TOKEN_FOUR                           32
#define TOKEN_FIVE                           33
#define TOKEN_SIX                            34
#define TOKEN_SEVEN                          35
#define TOKEN_EIGHT                          36
#define TOKEN_NINE                           37
#define TOKEN_TEN                            38
#define TOKEN_ELEVEN                         39
#define TOKEN_TWELVE                         40
#define TOKEN_THIRTEEN                       41
#define TOKEN_FOURTEEN                       42
#define TOKEN_FIFTEEN                        43
#define TOKEN_SIXTEEN                        44
#define TOKEN_SEVENTEEN                      45
#define TOKEN_EIGHTEEN                       46
#define TOKEN_NINETEEN                       47
#define TOKEN_TWENTY                         48
#define TOKEN_THIRTY                         49
#define TOKEN_FORTY                          50
#define TOKEN_FIFTY                          51
#define TOKEN_SIXTY                          52
#define TOKEN_SEVENTY                        53
#define TOKEN_EIGHTY                         54
#define TOKEN_NINETY                         55
#define TOKEN_FIRST                          56
#define TOKEN_SECOND                         57
#define TOKEN_THIRD                          58
#define TOKEN_FOURTH                         59
#define TOKEN_FIFTH                          60
#define TOKEN_SIXTH                          61
#define TOKEN_SEVENTH                        62
#define TOKEN_EIGHTH                         63
#define TOKEN_NINTH                          64
#define TOKEN_TENTH                          65
#define TOKEN_ELEVENTH                       66
#define TOKEN_TWELFTH                        67
#define TOKEN_THIRTEENTH                     68
#define TOKEN_FOURTEENTH                     69
#define TOKEN_FIFTEENTH                      70
#define TOKEN_SIXTEENTH                      71
#define TOKEN_SEVENTEENTH                    72
#define TOKEN_EIGHTEENTH                     73
#define TOKEN_NINETEENTH                     74
#define TOKEN_TWENTIETH                      75
#define TOKEN_THIRTIETH                      76
#define TOKEN_FOURTIETH                      77
#define TOKEN_FIFTIETH                       78
#define TOKEN_SIXTIETH                       79
#define TOKEN_SEVENTIETH                     80
#define TOKEN_EIGHTIETH                      81
#define TOKEN_NINETIETH                      82
#define TOKEN_FIRSTS                         83
#define TOKEN_SECONDS                        84
#define TOKEN_THIRDS                         85
#define TOKEN_FOURTHS                        86
#define TOKEN_FIFTHS                         87
#define TOKEN_SIXTHS                         88
#define TOKEN_SEVENTHS                       89
#define TOKEN_EIGHTHS                        90
#define TOKEN_NINTHS                         91
#define TOKEN_TENTHS                         92
#define TOKEN_ELEVENTHS                      93
#define TOKEN_TWELFTHS                       94
#define TOKEN_THIRTEENTHS                    95
#define TOKEN_FOURTEENTHS                    96
#define TOKEN_FIFTEENTHS                     97
#define TOKEN_SIXTEENTHS                     98
#define TOKEN_SEVENTEENTHS                   99
#define TOKEN_EIGHTEENTHS                    100
#define TOKEN_NINETEENTHS                    101
#define TOKEN_TWENTIETHS                     102
#define TOKEN_THIRTIETHS                     103
#define TOKEN_FOURTIETHS                     104
#define TOKEN_FIFTIETHS                      105
#define TOKEN_SIXTIETHS                      106
#define TOKEN_SEVENTIETHS                    107
#define TOKEN_EIGHTIETHS                     108
#define TOKEN_NINETIETHS                     109
#endif
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 159
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define ParseTOKENTYPE  YYSTYPE 
#endif
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#if INTERFACE
#define ParseARG_SDECL  ParserState *state ;
#define ParseARG_PDECL , ParserState *state 
#define ParseARG_FETCH  ParserState *state  = yypParser->state 
#define ParseARG_STORE yypParser->state  = state 
#endif
#define YYNSTATE             60
#define YYNRULE              242
#define YY_MAX_SHIFT         59
#define YY_MIN_SHIFTREDUCE   293
#define YY_MAX_SHIFTREDUCE   534
#define YY_MIN_REDUCE        535
#define YY_MAX_REDUCE        776
#define YY_ERROR_ACTION      777
#define YY_ACCEPT_ACTION     778
#define YY_NO_ACTION         779
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (2483)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   774,    7,  436,  436,  436,  439,   51,   18,  442,  442,
 /*    10 */    19,  310,   46,   40,  313,    2,  318,  324,    4,  339,
 /*    20 */   347,    8,  364,  372,   11,  389,  397,   56,  415,  424,
 /*    30 */   452,  453,  454,  455,  456,  457,  458,  459,  460,  461,
 /*    40 */   462,  463,  464,  465,  466,  467,  468,  469,  470,  471,
 /*    50 */   472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
 /*    60 */   482,  483,  484,  485,  486,  487,  488,  489,  490,  491,
 /*    70 */   492,  493,  494,  495,  496,  497,  498,  499,  500,  501,
 /*    80 */   502,  503,  504,  505,  506,  507,  508,  509,  510,  511,
 /*    90 */   512,  513,  514,  515,  516,  517,  518,  519,  520,  521,
 /*   100 */   522,  523,  524,  525,  526,  527,  528,  529,  530,  531,
 /*   110 */     7,   12,  387,  395,   20,   51,   18,   19,  537,   19,
 /*   120 */   310,   46,   40,  313,    2,  318,  324,    4,  339,  347,
 /*   130 */     8,  364,  372,   11,  389,  397,   56,  415,  424,  452,
 /*   140 */   453,  454,  455,  456,  457,  458,  459,  460,  461,  462,
 /*   150 */   463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
 /*   160 */   473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
 /*   170 */   483,  484,  485,  486,  487,  488,  489,  490,  491,  492,
 /*   180 */   493,  494,  495,  496,  497,  498,  499,  500,  501,  502,
 /*   190 */   503,  504,  505,  506,  507,  508,  509,  510,  511,  512,
 /*   200 */   513,  514,  515,  516,  517,  518,  519,  520,  521,  522,
 /*   210 */   523,  524,  525,  526,  527,  528,  529,  530,  531,   51,
 /*   220 */    18,  298,  301,   19,  310,   46,   40,  313,    2,  318,
 /*   230 */   324,    4,  339,  347,    8,  364,  372,   11,  389,  397,
 /*   240 */    56,  415,  424,  452,  453,  454,  455,  456,  457,  458,
 /*   250 */   459,  460,  461,  462,  463,  464,  465,  466,  467,  468,
 /*   260 */   469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
 /*   270 */   479,  480,  481,  482,  483,  484,  485,  486,  487,  488,
 /*   280 */   489,  490,  491,  492,  493,  494,  495,  496,  497,  498,
 /*   290 */   499,  500,  501,  502,  503,  504,  505,  506,  507,  508,
 /*   300 */   509,  510,  511,  512,  513,  514,  515,  516,  517,  518,
 /*   310 */   519,  520,  521,  522,  523,  524,  525,  526,  527,  528,
 /*   320 */   529,  530,  531,  451,   14,  316,   24,   16,  337,  306,
 /*   330 */    25,  362,   23,   27,  387,    6,   16,  337,   10,   25,
 /*   340 */   362,   13,   27,  387,   57,   27,  387,  452,  453,  454,
 /*   350 */   455,  456,  457,  458,  459,  460,  461,  462,  463,  464,
 /*   360 */   465,  466,  467,  468,  469,  470,  471,  472,  473,  474,
 /*   370 */   475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
 /*   380 */   485,  486,  487,  488,  489,  490,  491,  492,  493,  494,
 /*   390 */   495,  496,  497,  498,  499,  500,  501,  502,  503,  504,
 /*   400 */   505,  506,  507,  508,  509,  510,  511,  512,  513,  514,
 /*   410 */   515,  516,  517,  518,  519,  520,  521,  522,  523,  524,
 /*   420 */   525,  526,  527,  528,  529,  530,  531,  451,   29,  413,
 /*   430 */    24,  478,  479,  480,  481,  482,  483,  484,  485,  486,
 /*   440 */    25,  362,   10,   27,  387,   13,  295,  296,   57,   32,
 /*   450 */   413,  452,  453,  454,  455,  456,  457,  458,  459,  460,
 /*   460 */   461,  462,  463,  464,  465,  466,  467,  468,  469,  470,
 /*   470 */   471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
 /*   480 */   481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
 /*   490 */   491,  492,  493,  494,  495,  496,  497,  498,  499,  500,
 /*   500 */   501,  502,  503,  504,  505,  506,  507,  508,  509,  510,
 /*   510 */   511,  512,  513,  514,  515,  516,  517,  518,  519,  520,
 /*   520 */   521,  522,  523,  524,  525,  526,  527,  528,  529,  530,
 /*   530 */   531,  451,   31,   34,   24,    1,  316,  322,    5,  337,
 /*   540 */   345,    9,  362,  370,   12,  387,  395,   55,  537,   13,
 /*   550 */   297,  537,   57,  300,   39,  452,  453,  454,  455,  456,
 /*   560 */   457,  458,  459,  460,  461,  462,  463,  464,  465,  466,
 /*   570 */   467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
 /*   580 */   477,  478,  479,  480,  481,  482,  483,  484,  485,  486,
 /*   590 */   487,  488,  489,  490,  491,  492,  493,  494,  495,  496,
 /*   600 */   497,  498,  499,  500,  501,  502,  503,  504,  505,  506,
 /*   610 */   507,  508,  509,  510,  511,  512,  513,  514,  515,  516,
 /*   620 */   517,  518,  519,  520,  521,  522,  523,  524,  525,  526,
 /*   630 */   527,  528,  529,  530,  531,  451,  537,  537,   24,  412,
 /*   640 */   430,  421,   36,  446,  437,  438,  448,  440,  441,  450,
 /*   650 */   444,  445,  447,  449,  537,  537,   57,  537,  537,  452,
 /*   660 */   453,  454,  455,  456,  457,  458,  459,  460,  461,  462,
 /*   670 */   463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
 /*   680 */   473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
 /*   690 */   483,  484,  485,  486,  487,  488,  489,  490,  491,  492,
 /*   700 */   493,  494,  495,  496,  497,  498,  499,  500,  501,  502,
 /*   710 */   503,  504,  505,  506,  507,  508,  509,  510,  511,  512,
 /*   720 */   513,  514,  515,  516,  517,  518,  519,  520,  521,  522,
 /*   730 */   523,  524,  525,  526,  527,  528,  529,  530,  531,  451,
 /*   740 */   537,  537,   22,  411,  429,  420,   36,  446,  437,  438,
 /*   750 */   448,  440,  441,  450,  444,  445,  447,  449,   21,  413,
 /*   760 */   422,  537,  537,  452,  453,  454,  455,  456,  457,  458,
 /*   770 */   459,  460,  461,  462,  463,  464,  465,  466,  467,  468,
 /*   780 */   469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
 /*   790 */   479,  480,  481,  482,  483,  484,  485,  486,  487,  488,
 /*   800 */   489,  490,  491,  492,  493,  494,  495,  496,  497,  498,
 /*   810 */   499,  500,  501,  502,  503,  504,  505,  506,  507,  508,
 /*   820 */   509,  510,  511,  512,  513,  514,  515,  516,  517,  518,
 /*   830 */   519,  520,  521,  522,  523,  524,  525,  526,  527,  528,
 /*   840 */   529,  530,  531,  451,  410,  428,  419,   36,  446,  437,
 /*   850 */   438,  448,  440,  441,  450,  444,  445,  447,  449,    9,
 /*   860 */   362,  370,   12,  387,  395,  537,  537,  452,  453,  454,
 /*   870 */   455,  456,  457,  458,  459,  460,  461,  462,  463,  464,
 /*   880 */   465,  466,  467,  468,  469,  470,  471,  472,  473,  474,
 /*   890 */   475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
 /*   900 */   485,  486,  487,  488,  489,  490,  491,  492,  493,  494,
 /*   910 */   495,  496,  497,  498,  499,  500,  501,  502,  503,  504,
 /*   920 */   505,  506,  507,  508,  509,  510,  511,  512,  513,  514,
 /*   930 */   515,  516,  517,  518,  519,  520,  521,  522,  523,  524,
 /*   940 */   525,  526,  527,  528,  529,  530,  531,  299,  302,  412,
 /*   950 */   451,  537,   38,  446,  437,  438,  537,  332,  537,   15,
 /*   960 */   357,  445,   17,  382,  537,   26,  407,  537,   28,  435,
 /*   970 */   537,   58,  537,  537,  452,  453,  454,  455,  456,  457,
 /*   980 */   458,  459,  460,  461,  462,  463,  464,  465,  466,  467,
 /*   990 */   468,  469,  470,  471,  472,  473,  474,  475,  476,  477,
 /*  1000 */   478,  479,  480,  481,  482,  483,  484,  485,  486,  487,
 /*  1010 */   488,  489,  490,  491,  492,  493,  494,  495,  496,  497,
 /*  1020 */   498,  499,  500,  501,  502,  503,  504,  451,  411,  537,
 /*  1030 */    35,   38,  446,  437,  438,  537,  537,  537,  537,   17,
 /*  1040 */   445,  537,   26,  537,  537,   28,  537,  537,   58,  537,
 /*  1050 */   537,  452,  453,  454,  455,  456,  457,  458,  459,  460,
 /*  1060 */   461,  462,  463,  464,  465,  466,  467,  468,  469,  470,
 /*  1070 */   471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
 /*  1080 */   481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
 /*  1090 */   491,  492,  493,  494,  495,  496,  497,  498,  499,  500,
 /*  1100 */   501,  502,  503,  504,  451,  410,  537,  537,   38,  446,
 /*  1110 */   437,  438,  537,   15,  537,  537,   17,  445,  537,   26,
 /*  1120 */   537,  537,   28,  537,  537,   58,  537,  537,  452,  453,
 /*  1130 */   454,  455,  456,  457,  458,  459,  460,  461,  462,  463,
 /*  1140 */   464,  465,  466,  467,  468,  469,  470,  471,  472,  473,
 /*  1150 */   474,  475,  476,  477,  478,  479,  480,  481,  482,  483,
 /*  1160 */   484,  485,  486,  487,  488,  489,  490,  491,  492,  493,
 /*  1170 */   494,  495,  496,  497,  498,  499,  500,  501,  502,  503,
 /*  1180 */   504,  451,  537,  537,   35,    5,  337,  345,    9,  362,
 /*  1190 */   370,   12,  387,  395,  537,  537,   26,  537,  537,   28,
 /*  1200 */   537,  537,   58,  537,  537,  452,  453,  454,  455,  456,
 /*  1210 */   457,  458,  459,  460,  461,  462,  463,  464,  465,  466,
 /*  1220 */   467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
 /*  1230 */   477,  478,  479,  480,  481,  482,  483,  484,  485,  486,
 /*  1240 */   487,  488,  489,  490,  491,  492,  493,  494,  495,  496,
 /*  1250 */   497,  498,  499,  500,  501,  502,  503,  504,  451,  537,
 /*  1260 */   537,   35,  537,  412,  430,  537,   37,  446,  437,  438,
 /*  1270 */   537,  537,  537,  450,  444,  445,   28,  449,  537,   58,
 /*  1280 */   537,  537,  452,  453,  454,  455,  456,  457,  458,  459,
 /*  1290 */   460,  461,  462,  463,  464,  465,  466,  467,  468,  469,
 /*  1300 */   470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
 /*  1310 */   480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
 /*  1320 */   490,  491,  492,  493,  494,  495,  496,  497,  498,  499,
 /*  1330 */   500,  501,  502,  503,  504,  451,  537,  537,   35,  537,
 /*  1340 */   408,  426,  417,   36,  446,  437,  438,  448,  440,  441,
 /*  1350 */   450,  444,  445,  447,  449,  537,   59,  537,  537,  452,
 /*  1360 */   453,  454,  455,  456,  457,  458,  459,  460,  461,  462,
 /*  1370 */   463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
 /*  1380 */   473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
 /*  1390 */   483,  484,  485,  486,  487,  488,  489,  490,  491,  492,
 /*  1400 */   493,  494,  495,  496,  497,  498,  499,  500,  501,  502,
 /*  1410 */   503,  504,  451,  537,  537,   30,  331,  319,  325,  356,
 /*  1420 */   340,  348,  381,  365,  373,  406,  390,  398,  434,  416,
 /*  1430 */   425,  537,  537,  537,  537,  537,  452,  453,  454,  455,
 /*  1440 */   456,  457,  458,  459,  460,  461,  462,  463,  464,  465,
 /*  1450 */   466,  467,  468,  469,  470,  471,  472,  473,  474,  475,
 /*  1460 */   476,  477,  478,  479,  480,  481,  482,  483,  484,  485,
 /*  1470 */   486,  487,  488,  489,  490,  491,  492,  493,  494,  495,
 /*  1480 */   496,  497,  498,  499,  500,  501,  502,  503,  504,  451,
 /*  1490 */   386,  409,  537,  537,  537,  414,   54,  537,   37,  446,
 /*  1500 */   437,  438,  537,  537,  537,  450,  444,  445,  537,  449,
 /*  1510 */   537,  537,  537,  452,  453,  454,  455,  456,  457,  458,
 /*  1520 */   459,  460,  461,  462,  463,  464,  465,  466,  467,  468,
 /*  1530 */   469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
 /*  1540 */   479,  480,  481,  482,  483,  484,  485,  486,  487,  488,
 /*  1550 */   489,  490,  491,  492,  493,  494,  495,  496,  497,  498,
 /*  1560 */   499,  500,  501,  502,  503,  504,  315,   42,  333,  321,
 /*  1570 */   341,  327,  349,  334,  338,  342,  346,  350,  354,  359,
 /*  1580 */   363,  367,  371,  375,  379,  384,  388,  392,  396,  400,
 /*  1590 */   414,   50,  423,   36,  446,  437,  438,  448,  440,  441,
 /*  1600 */   450,  444,  445,  447,  449,  314,   42,  333,  320,  341,
 /*  1610 */   326,  349,  334,  338,  342,  346,  350,  354,  359,  363,
 /*  1620 */   367,  371,  375,  379,  384,  388,  392,  396,  400,  414,
 /*  1630 */    50,  423,   36,  446,  437,  438,  448,  440,  441,  450,
 /*  1640 */   444,  445,  447,  449,   33,  470,  471,  472,  473,  474,
 /*  1650 */   475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
 /*  1660 */   485,  486,  487,  488,  489,  490,  491,  492,  493,  494,
 /*  1670 */   495,  496,  497,  498,  499,  500,  501,  502,  503,  504,
 /*  1680 */   537,  537,  537,  470,  471,  472,  473,  474,  475,  476,
 /*  1690 */   477,  478,  479,  480,  481,  482,  483,  484,  485,  486,
 /*  1700 */   487,  488,  489,  490,  491,  492,  493,  494,  495,  496,
 /*  1710 */   497,  498,  499,  500,  501,  502,  503,  504,  778,    3,
 /*  1720 */     3,  293,   48,  304,  305,  307,  537,   41,  317,  537,
 /*  1730 */   323,  537,  329,  537,  338,  537,  346,  537,  354,  537,
 /*  1740 */   363,  537,  371,  537,  379,  537,  388,  537,  396,  537,
 /*  1750 */   414,   50,  423,   36,  446,  437,  438,  448,  440,  441,
 /*  1760 */   450,  444,  445,  447,  449,  533,  293,   48,  304,  305,
 /*  1770 */   307,  537,   41,  317,  537,  323,  537,  329,  537,  338,
 /*  1780 */   537,  346,  537,  354,  537,  363,  537,  371,  537,  379,
 /*  1790 */   537,  388,  537,  396,  537,  414,   50,  423,   36,  446,
 /*  1800 */   437,  438,  448,  440,  441,  450,  444,  445,  447,  449,
 /*  1810 */    44,  537,  537,  537,  537,  537,  335,  358,  343,  366,
 /*  1820 */   351,  374,  359,  363,  367,  371,  375,  379,  384,  388,
 /*  1830 */   392,  396,  400,  414,   50,  423,   36,  446,  437,  438,
 /*  1840 */   448,  440,  441,  450,  444,  445,  447,  449,   44,  537,
 /*  1850 */   537,  537,  537,  537,  336,  358,  344,  366,  352,  374,
 /*  1860 */   359,  363,  367,  371,  375,  379,  384,  388,  392,  396,
 /*  1870 */   400,  414,   50,  423,   36,  446,  437,  438,  448,  440,
 /*  1880 */   441,  450,  444,  445,  447,  449,  537,  294,   48,  304,
 /*  1890 */   305,  307,  537,   41,  317,  537,  323,  537,  329,  537,
 /*  1900 */   338,  537,  346,  537,  354,  537,  363,  537,  371,  537,
 /*  1910 */   379,  537,  388,  537,  396,  537,  414,   50,  423,   36,
 /*  1920 */   446,  437,  438,  448,  440,  441,  450,  444,  445,  447,
 /*  1930 */   449,   49,  537,  537,  537,  537,  537,  537,  537,  537,
 /*  1940 */   537,  537,  537,  360,  383,  368,  391,  376,  399,  384,
 /*  1950 */   388,  392,  396,  400,  414,   50,  423,   36,  446,  437,
 /*  1960 */   438,  448,  440,  441,  450,  444,  445,  447,  449,   49,
 /*  1970 */   537,  537,  537,  537,  537,  537,  537,  537,  537,  537,
 /*  1980 */   537,  361,  383,  369,  391,  377,  399,  384,  388,  392,
 /*  1990 */   396,  400,  414,   50,  423,   36,  446,  437,  438,  448,
 /*  2000 */   440,  441,  450,  444,  445,  447,  449,  451,  385,  409,
 /*  2010 */   537,  537,  537,  414,   54,  537,   37,  446,  437,  438,
 /*  2020 */   537,  537,  537,  450,  444,  445,  537,  449,  537,  537,
 /*  2030 */   537,  452,  453,  454,  455,  456,  457,  458,  459,  537,
 /*  2040 */   411,  429,  537,   37,  446,  437,  438,  537,  537,  537,
 /*  2050 */   450,  444,  445,  537,  449,  537,  537,  478,  479,  480,
 /*  2060 */   481,  482,  483,  484,  485,  486,  410,  428,  537,   37,
 /*  2070 */   446,  437,  438,  537,  537,  427,  450,  444,  445,  537,
 /*  2080 */   449,  537,  537,  537,  505,  506,  507,  508,  509,  510,
 /*  2090 */   511,  512,  513,  385,  409,  393,  418,  401,  414,   50,
 /*  2100 */   423,   36,  446,  437,  438,  448,  440,  441,  450,  444,
 /*  2110 */   445,  447,  449,  427,  537,  537,  537,  537,  537,  537,
 /*  2120 */   537,  537,  537,  537,  537,  537,  537,  537,  537,  537,
 /*  2130 */   537,  386,  409,  394,  418,  402,  414,   50,  423,   36,
 /*  2140 */   446,  437,  438,  448,  440,  441,  450,  444,  445,  447,
 /*  2150 */   449,  315,   45,  333,  537,  537,  537,  408,  334,  338,
 /*  2160 */    38,  446,  437,  438,  359,  363,  537,  537,  537,  445,
 /*  2170 */   384,  388,  537,  537,  537,  414,   52,  537,   37,  446,
 /*  2180 */   437,  438,  537,  537,  537,  450,  444,  445,  537,  449,
 /*  2190 */   537,  537,  537,  537,  537,  314,   45,  333,  537,  537,
 /*  2200 */   537,  537,  334,  338,  537,  537,  537,  537,  359,  363,
 /*  2210 */   451,  537,  537,  537,  384,  388,  537,  537,  537,  414,
 /*  2220 */    52,  537,   37,  446,  437,  438,  537,  537,  537,  450,
 /*  2230 */   444,  445,  537,  449,  452,  453,  454,  455,  456,  457,
 /*  2240 */   458,  459,  537,  537,  537,  537,  537,  537,  537,  537,
 /*  2250 */   537,  537,  537,  537,  537,  537,  537,  537,  537,  537,
 /*  2260 */   478,  479,  480,  481,  482,  483,  484,  485,  486,   47,
 /*  2270 */   537,  537,  537,  537,  537,  336,  358,  537,  537,  537,
 /*  2280 */   537,  359,  363,  537,  537,  537,  537,  384,  388,  537,
 /*  2290 */   537,  537,  414,   52,  537,   37,  446,  437,  438,  537,
 /*  2300 */   537,   47,  450,  444,  445,  537,  449,  335,  358,  537,
 /*  2310 */   537,  537,  537,  359,  363,  537,  537,  537,  537,  384,
 /*  2320 */   388,  537,  537,  537,  414,   52,  537,   37,  446,  437,
 /*  2330 */   438,  537,  537,  537,  450,  444,  445,  308,  449,  537,
 /*  2340 */   537,   43,  317,  537,  537,  537,  537,  537,  338,  537,
 /*  2350 */   537,  537,  537,  537,  363,  537,  537,  537,  537,  537,
 /*  2360 */   388,  537,  537,  537,  414,   52,  537,   37,  446,  437,
 /*  2370 */   438,  537,  537,  537,  450,  444,  445,  309,  449,  537,
 /*  2380 */   537,   43,  317,  537,  537,  537,  537,  537,  338,  537,
 /*  2390 */   537,  537,  537,  537,  363,  537,  537,  537,  537,  537,
 /*  2400 */   388,  537,  537,  537,  414,   52,  537,   37,  446,  437,
 /*  2410 */   438,  537,  537,   53,  450,  444,  445,  537,  449,  537,
 /*  2420 */   537,  537,  537,  537,  537,  361,  383,  537,  537,  537,
 /*  2430 */   537,  384,  388,  537,  537,  537,  414,   52,  537,   37,
 /*  2440 */   446,  437,  438,  537,  537,   53,  450,  444,  445,  537,
 /*  2450 */   449,  537,  537,  537,  537,  537,  537,  360,  383,  537,
 /*  2460 */   537,  537,  537,  384,  388,  537,  537,  537,  414,   52,
 /*  2470 */   537,   37,  446,  437,  438,  537,  537,  537,  450,  444,
 /*  2480 */   445,  537,  449,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     0,    1,  147,  147,  147,  150,    6,    7,  153,  153,
 /*    10 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*    20 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    30 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*    40 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*    50 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*    60 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*    70 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*    80 */    80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
 /*    90 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*   100 */   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,
 /*   110 */     1,   24,   25,   26,    7,    6,    7,   10,  158,   10,
 /*   120 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*   130 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*   140 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   150 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*   160 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   170 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*   180 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   190 */    81,   82,   83,   84,   85,   86,   87,   88,   89,   90,
 /*   200 */    91,   92,   93,   94,   95,   96,   97,   98,   99,  100,
 /*   210 */   101,  102,  103,  104,  105,  106,  107,  108,  109,    6,
 /*   220 */     7,    3,    4,   10,   11,   12,   13,   14,   15,   16,
 /*   230 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*   240 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   250 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*   260 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   270 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   280 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*   290 */    77,   78,   79,   80,   81,   82,   83,   84,   85,   86,
 /*   300 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*   310 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   320 */   107,  108,  109,    6,   15,   16,    9,   18,   19,  118,
 /*   330 */    21,   22,    9,   24,   25,   18,   18,   19,   21,   21,
 /*   340 */    22,   24,   24,   25,   27,   24,   25,   30,   31,   32,
 /*   350 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*   360 */    43,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*   370 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   380 */    63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
 /*   390 */    73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
 /*   400 */    83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*   410 */    93,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   420 */   103,  104,  105,  106,  107,  108,  109,    6,   27,   28,
 /*   430 */     9,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*   440 */    21,   22,   21,   24,   25,   24,    3,    4,   27,   27,
 /*   450 */    28,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   460 */    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*   470 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   480 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*   490 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   500 */    79,   80,   81,   82,   83,   84,   85,   86,   87,   88,
 /*   510 */    89,   90,   91,   92,   93,   94,   95,   96,   97,   98,
 /*   520 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*   530 */   109,    6,    9,    9,    9,   15,   16,   17,   18,   19,
 /*   540 */    20,   21,   22,   23,   24,   25,   26,    2,  158,   24,
 /*   550 */     5,  158,   27,    8,    9,   30,   31,   32,   33,   34,
 /*   560 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*   570 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   580 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*   590 */    65,   66,   67,   68,   69,   70,   71,   72,   73,   74,
 /*   600 */    75,   76,   77,   78,   79,   80,   81,   82,   83,   84,
 /*   610 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   620 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   630 */   105,  106,  107,  108,  109,    6,  158,  158,    9,  143,
 /*   640 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*   650 */   154,  155,  156,  157,  158,  158,   27,  158,  158,   30,
 /*   660 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   670 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*   680 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   690 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*   700 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   710 */    81,   82,   83,   84,   85,   86,   87,   88,   89,   90,
 /*   720 */    91,   92,   93,   94,   95,   96,   97,   98,   99,  100,
 /*   730 */   101,  102,  103,  104,  105,  106,  107,  108,  109,    6,
 /*   740 */   158,  158,    9,  143,  144,  145,  146,  147,  148,  149,
 /*   750 */   150,  151,  152,  153,  154,  155,  156,  157,   27,   28,
 /*   760 */    29,  158,  158,   30,   31,   32,   33,   34,   35,   36,
 /*   770 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*   780 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   790 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   800 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*   810 */    77,   78,   79,   80,   81,   82,   83,   84,   85,   86,
 /*   820 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*   830 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   840 */   107,  108,  109,    6,  143,  144,  145,  146,  147,  148,
 /*   850 */   149,  150,  151,  152,  153,  154,  155,  156,  157,   21,
 /*   860 */    22,   23,   24,   25,   26,  158,  158,   30,   31,   32,
 /*   870 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*   880 */    43,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*   890 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   900 */    63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
 /*   910 */    73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
 /*   920 */    83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*   930 */    93,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   940 */   103,  104,  105,  106,  107,  108,  109,    3,    4,  143,
 /*   950 */     6,  158,  146,  147,  148,  149,  158,   15,  158,   15,
 /*   960 */    18,  155,   18,   21,  158,   21,   24,  158,   24,   27,
 /*   970 */   158,   27,  158,  158,   30,   31,   32,   33,   34,   35,
 /*   980 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*   990 */    46,   47,   48,   49,   50,   51,   52,   53,   54,   55,
 /*  1000 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*  1010 */    66,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  1020 */    76,   77,   78,   79,   80,   81,   82,    6,  143,  158,
 /*  1030 */     9,  146,  147,  148,  149,  158,  158,  158,  158,   18,
 /*  1040 */   155,  158,   21,  158,  158,   24,  158,  158,   27,  158,
 /*  1050 */   158,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1060 */    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*  1070 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
 /*  1080 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*  1090 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*  1100 */    79,   80,   81,   82,    6,  143,  158,  158,  146,  147,
 /*  1110 */   148,  149,  158,   15,  158,  158,   18,  155,  158,   21,
 /*  1120 */   158,  158,   24,  158,  158,   27,  158,  158,   30,   31,
 /*  1130 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1140 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  1150 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*  1160 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*  1170 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*  1180 */    82,    6,  158,  158,    9,   18,   19,   20,   21,   22,
 /*  1190 */    23,   24,   25,   26,  158,  158,   21,  158,  158,   24,
 /*  1200 */   158,  158,   27,  158,  158,   30,   31,   32,   33,   34,
 /*  1210 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1220 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  1230 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*  1240 */    65,   66,   67,   68,   69,   70,   71,   72,   73,   74,
 /*  1250 */    75,   76,   77,   78,   79,   80,   81,   82,    6,  158,
 /*  1260 */   158,    9,  158,  143,  144,  158,  146,  147,  148,  149,
 /*  1270 */   158,  158,  158,  153,  154,  155,   24,  157,  158,   27,
 /*  1280 */   158,  158,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1290 */    38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
 /*  1300 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  1310 */    58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1320 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  1330 */    78,   79,   80,   81,   82,    6,  158,  158,    9,  158,
 /*  1340 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1350 */   153,  154,  155,  156,  157,  158,   27,  158,  158,   30,
 /*  1360 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1370 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*  1380 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  1390 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*  1400 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*  1410 */    81,   82,    6,  158,  158,    9,   15,   16,   17,   18,
 /*  1420 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  1430 */    29,  158,  158,  158,  158,  158,   30,   31,   32,   33,
 /*  1440 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1450 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  1460 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*  1470 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*  1480 */    74,   75,   76,   77,   78,   79,   80,   81,   82,    6,
 /*  1490 */   138,  139,  158,  158,  158,  143,  144,  158,  146,  147,
 /*  1500 */   148,  149,  158,  158,  158,  153,  154,  155,  158,  157,
 /*  1510 */   158,  158,  158,   30,   31,   32,   33,   34,   35,   36,
 /*  1520 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*  1530 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  1540 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1550 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*  1560 */    77,   78,   79,   80,   81,   82,  119,  120,  121,  122,
 /*  1570 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1580 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1590 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1600 */   153,  154,  155,  156,  157,  119,  120,  121,  122,  123,
 /*  1610 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  133,
 /*  1620 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1630 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  1640 */   154,  155,  156,  157,    9,   48,   49,   50,   51,   52,
 /*  1650 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  1660 */    63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
 /*  1670 */    73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
 /*  1680 */   158,  158,  158,   48,   49,   50,   51,   52,   53,   54,
 /*  1690 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*  1700 */    65,   66,   67,   68,   69,   70,   71,   72,   73,   74,
 /*  1710 */    75,   76,   77,   78,   79,   80,   81,   82,  111,  112,
 /*  1720 */   113,  114,  115,  116,  117,  118,  158,  120,  121,  158,
 /*  1730 */   123,  158,  125,  158,  127,  158,  129,  158,  131,  158,
 /*  1740 */   133,  158,  135,  158,  137,  158,  139,  158,  141,  158,
 /*  1750 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1760 */   153,  154,  155,  156,  157,  113,  114,  115,  116,  117,
 /*  1770 */   118,  158,  120,  121,  158,  123,  158,  125,  158,  127,
 /*  1780 */   158,  129,  158,  131,  158,  133,  158,  135,  158,  137,
 /*  1790 */   158,  139,  158,  141,  158,  143,  144,  145,  146,  147,
 /*  1800 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1810 */   120,  158,  158,  158,  158,  158,  126,  127,  128,  129,
 /*  1820 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1830 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  1840 */   150,  151,  152,  153,  154,  155,  156,  157,  120,  158,
 /*  1850 */   158,  158,  158,  158,  126,  127,  128,  129,  130,  131,
 /*  1860 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1870 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  1880 */   152,  153,  154,  155,  156,  157,  158,  114,  115,  116,
 /*  1890 */   117,  118,  158,  120,  121,  158,  123,  158,  125,  158,
 /*  1900 */   127,  158,  129,  158,  131,  158,  133,  158,  135,  158,
 /*  1910 */   137,  158,  139,  158,  141,  158,  143,  144,  145,  146,
 /*  1920 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  1930 */   157,  120,  158,  158,  158,  158,  158,  158,  158,  158,
 /*  1940 */   158,  158,  158,  132,  133,  134,  135,  136,  137,  138,
 /*  1950 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  1960 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  120,
 /*  1970 */   158,  158,  158,  158,  158,  158,  158,  158,  158,  158,
 /*  1980 */   158,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1990 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2000 */   151,  152,  153,  154,  155,  156,  157,    6,  138,  139,
 /*  2010 */   158,  158,  158,  143,  144,  158,  146,  147,  148,  149,
 /*  2020 */   158,  158,  158,  153,  154,  155,  158,  157,  158,  158,
 /*  2030 */   158,   30,   31,   32,   33,   34,   35,   36,   37,  158,
 /*  2040 */   143,  144,  158,  146,  147,  148,  149,  158,  158,  158,
 /*  2050 */   153,  154,  155,  158,  157,  158,  158,   56,   57,   58,
 /*  2060 */    59,   60,   61,   62,   63,   64,  143,  144,  158,  146,
 /*  2070 */   147,  148,  149,  158,  158,  120,  153,  154,  155,  158,
 /*  2080 */   157,  158,  158,  158,   83,   84,   85,   86,   87,   88,
 /*  2090 */    89,   90,   91,  138,  139,  140,  141,  142,  143,  144,
 /*  2100 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2110 */   155,  156,  157,  120,  158,  158,  158,  158,  158,  158,
 /*  2120 */   158,  158,  158,  158,  158,  158,  158,  158,  158,  158,
 /*  2130 */   158,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2140 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2150 */   157,  119,  120,  121,  158,  158,  158,  143,  126,  127,
 /*  2160 */   146,  147,  148,  149,  132,  133,  158,  158,  158,  155,
 /*  2170 */   138,  139,  158,  158,  158,  143,  144,  158,  146,  147,
 /*  2180 */   148,  149,  158,  158,  158,  153,  154,  155,  158,  157,
 /*  2190 */   158,  158,  158,  158,  158,  119,  120,  121,  158,  158,
 /*  2200 */   158,  158,  126,  127,  158,  158,  158,  158,  132,  133,
 /*  2210 */     6,  158,  158,  158,  138,  139,  158,  158,  158,  143,
 /*  2220 */   144,  158,  146,  147,  148,  149,  158,  158,  158,  153,
 /*  2230 */   154,  155,  158,  157,   30,   31,   32,   33,   34,   35,
 /*  2240 */    36,   37,  158,  158,  158,  158,  158,  158,  158,  158,
 /*  2250 */   158,  158,  158,  158,  158,  158,  158,  158,  158,  158,
 /*  2260 */    56,   57,   58,   59,   60,   61,   62,   63,   64,  120,
 /*  2270 */   158,  158,  158,  158,  158,  126,  127,  158,  158,  158,
 /*  2280 */   158,  132,  133,  158,  158,  158,  158,  138,  139,  158,
 /*  2290 */   158,  158,  143,  144,  158,  146,  147,  148,  149,  158,
 /*  2300 */   158,  120,  153,  154,  155,  158,  157,  126,  127,  158,
 /*  2310 */   158,  158,  158,  132,  133,  158,  158,  158,  158,  138,
 /*  2320 */   139,  158,  158,  158,  143,  144,  158,  146,  147,  148,
 /*  2330 */   149,  158,  158,  158,  153,  154,  155,  116,  157,  158,
 /*  2340 */   158,  120,  121,  158,  158,  158,  158,  158,  127,  158,
 /*  2350 */   158,  158,  158,  158,  133,  158,  158,  158,  158,  158,
 /*  2360 */   139,  158,  158,  158,  143,  144,  158,  146,  147,  148,
 /*  2370 */   149,  158,  158,  158,  153,  154,  155,  116,  157,  158,
 /*  2380 */   158,  120,  121,  158,  158,  158,  158,  158,  127,  158,
 /*  2390 */   158,  158,  158,  158,  133,  158,  158,  158,  158,  158,
 /*  2400 */   139,  158,  158,  158,  143,  144,  158,  146,  147,  148,
 /*  2410 */   149,  158,  158,  120,  153,  154,  155,  158,  157,  158,
 /*  2420 */   158,  158,  158,  158,  158,  132,  133,  158,  158,  158,
 /*  2430 */   158,  138,  139,  158,  158,  158,  143,  144,  158,  146,
 /*  2440 */   147,  148,  149,  158,  158,  120,  153,  154,  155,  158,
 /*  2450 */   157,  158,  158,  158,  158,  158,  158,  132,  133,  158,
 /*  2460 */   158,  158,  158,  138,  139,  158,  158,  158,  143,  144,
 /*  2470 */   158,  146,  147,  148,  149,  158,  158,  158,  153,  154,
 /*  2480 */   155,  158,  157,
};
#define YY_SHIFT_USE_DFLT (2483)
#define YY_SHIFT_COUNT    (59)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (2204)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */   109,  317,  317,    0,  421,  421,  421,  213,  525,  525,
 /*    10 */   525,  629,  629,  629, 1021, 1021, 1175, 1175,  944, 1098,
 /*    20 */  1098,  733,  837,  837,  837, 1252, 1252, 1329, 1329, 1406,
 /*    30 */  1483, 1483, 1635, 1597, 1597, 1597, 2001, 2204,  375,  107,
 /*    40 */  1401,  520, 1167,  309,  838,  318,  942,  419,  545,   87,
 /*    50 */   731,  218,  401,  321,  422,  443,  323,  323,  523,  524,
};
#define YY_REDUCE_USE_DFLT (-146)
#define YY_REDUCE_COUNT (39)
#define YY_REDUCE_MIN   (-145)
#define YY_REDUCE_MAX   (2325)
static const short yy_reduce_ofst[] = {
 /*     0 */  1607, 1447, 1486, 1652, 1690, 1728, 1690, 1773, 1811, 1849,
 /*    10 */  1811, 1955, 1993, 1955, 2032, 2076, 2149, 2181, 2221, 2261,
 /*    20 */  2221,  496,  600,  701, 1197, 2293, 2325, 1352, 1870, 1120,
 /*    30 */  1897, 1923,  806,  885,  962, 2014, -145, -144, -143,  211,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   777,  570,  572,  777,  597,  595,  777,  777,  622,  620,
 /*    10 */   777,  647,  645,  777,  777,  777,  777,  777,  777,  777,
 /*    20 */   777,  673,  777,  777,  777,  777,  777,  777,  777,  673,
 /*    30 */   777,  777,  777,  777,  777,  777,  685,  685,  777,  777,
 /*    40 */   554,  646,  646,  777,  646,  777,  553,  777,  545,  646,
 /*    50 */   674,  693,  674,  777,  777,  777,  675,  777,  777,  777,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "NEGATIVE",      "AND_A",         "QUARTER",     
  "HALF",          "QUARTERS",      "ONE",           "A",           
  "HALVES",        "AND",           "AN",            "ZERO_WHOLE_NUMBER",
  "WHOLE_NUMBER",  "DECIMAL",       "ZERO",          "TRILLION",    
  "TRILLIONTH",    "TRILLIONTHS",   "BILLION",       "BILLIONTH",   
  "BILLIONTHS",    "MILLION",       "MILLIONTH",     "MILLIONTHS",  
  "THOUSAND",      "THOUSANDTH",    "THOUSANDTHS",   "HUNDRED",     
  "HUNDREDTH",     "HUNDREDTHS",    "TWO",           "THREE",       
  "FOUR",          "FIVE",          "SIX",           "SEVEN",       
  "EIGHT",         "NINE",          "TEN",           "ELEVEN",      
  "TWELVE",        "THIRTEEN",      "FOURTEEN",      "FIFTEEN",     
  "SIXTEEN",       "SEVENTEEN",     "EIGHTEEN",      "NINETEEN",    
  "TWENTY",        "THIRTY",        "FORTY",         "FIFTY",       
  "SIXTY",         "SEVENTY",       "EIGHTY",        "NINETY",      
  "FIRST",         "SECOND",        "THIRD",         "FOURTH",      
  "FIFTH",         "SIXTH",         "SEVENTH",       "EIGHTH",      
  "NINTH",         "TENTH",         "ELEVENTH",      "TWELFTH",     
  "THIRTEENTH",    "FOURTEENTH",    "FIFTEENTH",     "SIXTEENTH",   
  "SEVENTEENTH",   "EIGHTEENTH",    "NINETEENTH",    "TWENTIETH",   
  "THIRTIETH",     "FOURTIETH",     "FIFTIETH",      "SIXTIETH",    
  "SEVENTIETH",    "EIGHTIETH",     "NINETIETH",     "FIRSTS",      
  "SECONDS",       "THIRDS",        "FOURTHS",       "FIFTHS",      
  "SIXTHS",        "SEVENTHS",      "EIGHTHS",       "NINTHS",      
  "TENTHS",        "ELEVENTHS",     "TWELFTHS",      "THIRTEENTHS", 
  "FOURTEENTHS",   "FIFTEENTHS",    "SIXTEENTHS",    "SEVENTEENTHS",
  "EIGHTEENTHS",   "NINETEENTHS",   "TWENTIETHS",    "THIRTIETHS",  
  "FOURTIETHS",    "FIFTIETHS",     "SIXTIETHS",     "SEVENTIETHS", 
  "EIGHTIETHS",    "NINETIETHS",    "error",         "program",     
  "numbers",       "number",        "final_number",  "less_than_quadrillion",
  "less_than_quadrillionth",  "less_than_quadrillionths",  "fraction",      "less_than_trillionth_end_only",
  "less_than_thousand",  "less_than_trillionth",  "less_than_trillionths_end_only",  "less_than_trillionths",
  "less_than_trillion_end_only",  "less_than_trillion",  "less_than_billionth_end_only",  "less_than_billionth",
  "less_than_billionths_end_only",  "less_than_billionths",  "less_than_billion_end_only",  "less_than_billion",
  "less_than_millionth_end_only",  "less_than_millionth",  "less_than_millionths_end_only",  "less_than_millionths",
  "less_than_million_end_only",  "less_than_million",  "less_than_thousandth_end_only",  "less_than_thousandth",
  "less_than_thousandths_end_only",  "less_than_thousandths",  "less_than_thousand_end_only",  "less_than_hundredth",
  "less_than_hundred",  "less_than_hundredths",  "tens",          "less_than_tenth",
  "tenth",         "less_than_twentieth",  "less_than_tenths",  "tenths",      
  "less_than_twentieths",  "less_than_ten",  "less_than_twenty",  "tenth_to_19th",
  "tenths_to_19ths",  "ten_to_19",   
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "number ::= final_number",
 /*   1 */ "number ::= NEGATIVE final_number",
 /*   2 */ "final_number ::= less_than_quadrillion AND_A QUARTER",
 /*   3 */ "final_number ::= less_than_quadrillion AND_A HALF",
 /*   4 */ "final_number ::= less_than_quadrillion QUARTERS",
 /*   5 */ "final_number ::= ONE QUARTER",
 /*   6 */ "final_number ::= A QUARTER",
 /*   7 */ "final_number ::= less_than_quadrillion HALVES",
 /*   8 */ "final_number ::= ONE HALF",
 /*   9 */ "final_number ::= A HALF",
 /*  10 */ "final_number ::= less_than_quadrillion",
 /*  11 */ "final_number ::= less_than_quadrillionth",
 /*  12 */ "final_number ::= less_than_quadrillionths",
 /*  13 */ "final_number ::= less_than_quadrillion AND fraction",
 /*  14 */ "final_number ::= fraction",
 /*  15 */ "fraction ::= A less_than_quadrillionth",
 /*  16 */ "fraction ::= AN less_than_quadrillionth",
 /*  17 */ "final_number ::= ZERO_WHOLE_NUMBER",
 /*  18 */ "final_number ::= WHOLE_NUMBER",
 /*  19 */ "final_number ::= DECIMAL",
 /*  20 */ "final_number ::= ZERO",
 /*  21 */ "less_than_quadrillionth ::= TRILLION less_than_trillionth_end_only",
 /*  22 */ "less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only",
 /*  23 */ "less_than_quadrillionth ::= less_than_thousand TRILLIONTH",
 /*  24 */ "less_than_quadrillionth ::= less_than_trillionth",
 /*  25 */ "final_number ::= TRILLIONTH",
 /*  26 */ "final_number ::= DECIMAL TRILLIONTH",
 /*  27 */ "less_than_quadrillionths ::= TRILLION less_than_trillionths_end_only",
 /*  28 */ "less_than_quadrillionths ::= less_than_thousand TRILLION less_than_trillionths_end_only",
 /*  29 */ "less_than_quadrillionths ::= less_than_thousand TRILLIONTHS",
 /*  30 */ "less_than_quadrillionths ::= less_than_trillionths",
 /*  31 */ "final_number ::= TRILLIONTHS",
 /*  32 */ "final_number ::= DECIMAL TRILLIONTHS",
 /*  33 */ "less_than_quadrillion ::= TRILLION less_than_trillion_end_only",
 /*  34 */ "less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only",
 /*  35 */ "less_than_quadrillion ::= less_than_thousand TRILLION",
 /*  36 */ "less_than_quadrillion ::= less_than_trillion",
 /*  37 */ "final_number ::= TRILLION",
 /*  38 */ "final_number ::= DECIMAL TRILLION",
 /*  39 */ "final_number ::= WHOLE_NUMBER TRILLION",
 /*  40 */ "less_than_trillionth_end_only ::= less_than_trillionth",
 /*  41 */ "less_than_trillionth_end_only ::= less_than_billionth_end_only",
 /*  42 */ "less_than_trillionth ::= BILLION less_than_billionth_end_only",
 /*  43 */ "less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only",
 /*  44 */ "less_than_trillionth ::= less_than_thousand BILLIONTH",
 /*  45 */ "less_than_trillionth ::= less_than_billionth",
 /*  46 */ "final_number ::= BILLIONTH",
 /*  47 */ "final_number ::= DECIMAL BILLIONTH",
 /*  48 */ "less_than_trillionths_end_only ::= less_than_trillionths",
 /*  49 */ "less_than_trillionths_end_only ::= less_than_billionths_end_only",
 /*  50 */ "less_than_trillionths ::= BILLION less_than_billionths_end_only",
 /*  51 */ "less_than_trillionths ::= less_than_thousand BILLION less_than_billionths_end_only",
 /*  52 */ "less_than_trillionths ::= less_than_thousand BILLIONTHS",
 /*  53 */ "less_than_trillionths ::= less_than_billionths",
 /*  54 */ "final_number ::= BILLIONTHS",
 /*  55 */ "final_number ::= DECIMAL BILLIONTHS",
 /*  56 */ "less_than_trillion_end_only ::= less_than_trillion",
 /*  57 */ "less_than_trillion_end_only ::= less_than_billion_end_only",
 /*  58 */ "less_than_trillion ::= BILLION less_than_billion_end_only",
 /*  59 */ "less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only",
 /*  60 */ "less_than_trillion ::= less_than_thousand BILLION",
 /*  61 */ "less_than_trillion ::= less_than_billion",
 /*  62 */ "final_number ::= BILLION",
 /*  63 */ "final_number ::= DECIMAL BILLION",
 /*  64 */ "final_number ::= WHOLE_NUMBER BILLION",
 /*  65 */ "less_than_billionth_end_only ::= less_than_billionth",
 /*  66 */ "less_than_billionth_end_only ::= less_than_millionth_end_only",
 /*  67 */ "less_than_billionth ::= MILLION less_than_millionth_end_only",
 /*  68 */ "less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only",
 /*  69 */ "less_than_billionth ::= less_than_thousand MILLIONTH",
 /*  70 */ "less_than_billionth ::= less_than_millionth",
 /*  71 */ "final_number ::= MILLIONTH",
 /*  72 */ "final_number ::= DECIMAL MILLIONTH",
 /*  73 */ "less_than_billionths_end_only ::= less_than_billionths",
 /*  74 */ "less_than_billionths_end_only ::= less_than_millionths_end_only",
 /*  75 */ "less_than_billionths ::= MILLION less_than_millionths_end_only",
 /*  76 */ "less_than_billionths ::= less_than_thousand MILLION less_than_millionths_end_only",
 /*  77 */ "less_than_billionths ::= less_than_thousand MILLIONTHS",
 /*  78 */ "less_than_billionths ::= less_than_millionths",
 /*  79 */ "final_number ::= MILLIONTHS",
 /*  80 */ "final_number ::= DECIMAL MILLIONTHS",
 /*  81 */ "less_than_billion_end_only ::= less_than_billion",
 /*  82 */ "less_than_billion_end_only ::= less_than_million_end_only",
 /*  83 */ "less_than_billion ::= MILLION less_than_million_end_only",
 /*  84 */ "less_than_billion ::= less_than_thousand MILLION less_than_million_end_only",
 /*  85 */ "less_than_billion ::= less_than_thousand MILLION",
 /*  86 */ "less_than_billion ::= less_than_million",
 /*  87 */ "final_number ::= MILLION",
 /*  88 */ "final_number ::= DECIMAL MILLION",
 /*  89 */ "final_number ::= WHOLE_NUMBER MILLION",
 /*  90 */ "less_than_millionth_end_only ::= less_than_millionth",
 /*  91 */ "less_than_millionth_end_only ::= less_than_thousandth_end_only",
 /*  92 */ "less_than_millionth ::= THOUSAND less_than_thousandth_end_only",
 /*  93 */ "less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only",
 /*  94 */ "less_than_millionth ::= less_than_thousand THOUSANDTH",
 /*  95 */ "less_than_millionth ::= less_than_thousandth",
 /*  96 */ "final_number ::= THOUSANDTH",
 /*  97 */ "final_number ::= DECIMAL THOUSANDTH",
 /*  98 */ "less_than_millionths_end_only ::= less_than_millionths",
 /*  99 */ "less_than_millionths_end_only ::= less_than_thousandths_end_only",
 /* 100 */ "less_than_millionths ::= THOUSAND less_than_thousandths_end_only",
 /* 101 */ "less_than_millionths ::= less_than_thousand THOUSAND less_than_thousandths_end_only",
 /* 102 */ "less_than_millionths ::= less_than_thousand THOUSANDTHS",
 /* 103 */ "less_than_millionths ::= less_than_thousandths",
 /* 104 */ "final_number ::= THOUSANDTHS",
 /* 105 */ "final_number ::= DECIMAL THOUSANDTHS",
 /* 106 */ "less_than_million_end_only ::= less_than_million",
 /* 107 */ "less_than_million_end_only ::= less_than_thousand_end_only",
 /* 108 */ "less_than_million ::= THOUSAND less_than_thousand_end_only",
 /* 109 */ "less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only",
 /* 110 */ "less_than_million ::= less_than_thousand THOUSAND",
 /* 111 */ "less_than_million ::= less_than_thousand",
 /* 112 */ "final_number ::= THOUSAND",
 /* 113 */ "final_number ::= DECIMAL THOUSAND",
 /* 114 */ "final_number ::= WHOLE_NUMBER THOUSAND",
 /* 115 */ "less_than_thousandth_end_only ::= AND less_than_hundredth",
 /* 116 */ "less_than_thousandth_end_only ::= less_than_thousandth",
 /* 117 */ "less_than_thousandth ::= HUNDRED AND less_than_hundredth",
 /* 118 */ "less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth",
 /* 119 */ "less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth",
 /* 120 */ "less_than_thousandth ::= less_than_hundred HUNDREDTH",
 /* 121 */ "less_than_thousandth ::= less_than_hundredth",
 /* 122 */ "final_number ::= HUNDREDTH",
 /* 123 */ "final_number ::= DECIMAL HUNDREDTH",
 /* 124 */ "less_than_thousandths_end_only ::= AND less_than_hundredths",
 /* 125 */ "less_than_thousandths_end_only ::= less_than_thousandths",
 /* 126 */ "less_than_thousandths ::= HUNDRED AND less_than_hundredths",
 /* 127 */ "less_than_thousandths ::= less_than_hundred HUNDRED AND less_than_hundredths",
 /* 128 */ "less_than_thousandths ::= less_than_hundred HUNDRED less_than_hundredths",
 /* 129 */ "less_than_thousandths ::= less_than_hundred HUNDREDTHS",
 /* 130 */ "less_than_thousandths ::= less_than_hundredths",
 /* 131 */ "final_number ::= HUNDREDTHS",
 /* 132 */ "final_number ::= DECIMAL HUNDREDTHS",
 /* 133 */ "less_than_thousand_end_only ::= AND less_than_hundred",
 /* 134 */ "less_than_thousand_end_only ::= less_than_thousand",
 /* 135 */ "less_than_thousand ::= HUNDRED AND less_than_hundred",
 /* 136 */ "less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred",
 /* 137 */ "less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred",
 /* 138 */ "less_than_thousand ::= less_than_hundred HUNDRED",
 /* 139 */ "less_than_thousand ::= less_than_hundred",
 /* 140 */ "final_number ::= HUNDRED",
 /* 141 */ "final_number ::= DECIMAL HUNDRED",
 /* 142 */ "final_number ::= WHOLE_NUMBER HUNDRED",
 /* 143 */ "less_than_hundredth ::= tens less_than_tenth",
 /* 144 */ "less_than_hundredth ::= tenth",
 /* 145 */ "less_than_hundredth ::= less_than_twentieth",
 /* 146 */ "less_than_hundredths ::= tens less_than_tenths",
 /* 147 */ "less_than_hundredths ::= tenths",
 /* 148 */ "less_than_hundredths ::= less_than_twentieths",
 /* 149 */ "less_than_hundred ::= tens less_than_ten",
 /* 150 */ "less_than_hundred ::= tens",
 /* 151 */ "less_than_hundred ::= less_than_twenty",
 /* 152 */ "less_than_twentieth ::= tenth_to_19th",
 /* 153 */ "less_than_twentieth ::= less_than_tenth",
 /* 154 */ "less_than_twentieths ::= tenths_to_19ths",
 /* 155 */ "less_than_twentieths ::= less_than_tenths",
 /* 156 */ "less_than_twenty ::= ten_to_19",
 /* 157 */ "less_than_twenty ::= less_than_ten",
 /* 158 */ "less_than_ten ::= ONE",
 /* 159 */ "less_than_ten ::= TWO",
 /* 160 */ "less_than_ten ::= THREE",
 /* 161 */ "less_than_ten ::= FOUR",
 /* 162 */ "less_than_ten ::= FIVE",
 /* 163 */ "less_than_ten ::= SIX",
 /* 164 */ "less_than_ten ::= SEVEN",
 /* 165 */ "less_than_ten ::= EIGHT",
 /* 166 */ "less_than_ten ::= NINE",
 /* 167 */ "ten_to_19 ::= TEN",
 /* 168 */ "ten_to_19 ::= ELEVEN",
 /* 169 */ "ten_to_19 ::= TWELVE",
 /* 170 */ "ten_to_19 ::= THIRTEEN",
 /* 171 */ "ten_to_19 ::= FOURTEEN",
 /* 172 */ "ten_to_19 ::= FIFTEEN",
 /* 173 */ "ten_to_19 ::= SIXTEEN",
 /* 174 */ "ten_to_19 ::= SEVENTEEN",
 /* 175 */ "ten_to_19 ::= EIGHTEEN",
 /* 176 */ "ten_to_19 ::= NINETEEN",
 /* 177 */ "tens ::= TWENTY",
 /* 178 */ "tens ::= THIRTY",
 /* 179 */ "tens ::= FORTY",
 /* 180 */ "tens ::= FIFTY",
 /* 181 */ "tens ::= SIXTY",
 /* 182 */ "tens ::= SEVENTY",
 /* 183 */ "tens ::= EIGHTY",
 /* 184 */ "tens ::= NINETY",
 /* 185 */ "less_than_tenth ::= FIRST",
 /* 186 */ "less_than_tenth ::= SECOND",
 /* 187 */ "less_than_tenth ::= THIRD",
 /* 188 */ "less_than_tenth ::= FOURTH",
 /* 189 */ "less_than_tenth ::= FIFTH",
 /* 190 */ "less_than_tenth ::= SIXTH",
 /* 191 */ "less_than_tenth ::= SEVENTH",
 /* 192 */ "less_than_tenth ::= EIGHTH",
 /* 193 */ "less_than_tenth ::= NINTH",
 /* 194 */ "tenth_to_19th ::= TENTH",
 /* 195 */ "tenth_to_19th ::= ELEVENTH",
 /* 196 */ "tenth_to_19th ::= TWELFTH",
 /* 197 */ "tenth_to_19th ::= THIRTEENTH",
 /* 198 */ "tenth_to_19th ::= FOURTEENTH",
 /* 199 */ "tenth_to_19th ::= FIFTEENTH",
 /* 200 */ "tenth_to_19th ::= SIXTEENTH",
 /* 201 */ "tenth_to_19th ::= SEVENTEENTH",
 /* 202 */ "tenth_to_19th ::= EIGHTEENTH",
 /* 203 */ "tenth_to_19th ::= NINETEENTH",
 /* 204 */ "tenth ::= TWENTIETH",
 /* 205 */ "tenth ::= THIRTIETH",
 /* 206 */ "tenth ::= FOURTIETH",
 /* 207 */ "tenth ::= FIFTIETH",
 /* 208 */ "tenth ::= SIXTIETH",
 /* 209 */ "tenth ::= SEVENTIETH",
 /* 210 */ "tenth ::= EIGHTIETH",
 /* 211 */ "tenth ::= NINETIETH",
 /* 212 */ "less_than_tenths ::= FIRSTS",
 /* 213 */ "less_than_tenths ::= SECONDS",
 /* 214 */ "less_than_tenths ::= THIRDS",
 /* 215 */ "less_than_tenths ::= FOURTHS",
 /* 216 */ "less_than_tenths ::= FIFTHS",
 /* 217 */ "less_than_tenths ::= SIXTHS",
 /* 218 */ "less_than_tenths ::= SEVENTHS",
 /* 219 */ "less_than_tenths ::= EIGHTHS",
 /* 220 */ "less_than_tenths ::= NINTHS",
 /* 221 */ "tenths_to_19ths ::= TENTHS",
 /* 222 */ "tenths_to_19ths ::= ELEVENTHS",
 /* 223 */ "tenths_to_19ths ::= TWELFTHS",
 /* 224 */ "tenths_to_19ths ::= THIRTEENTHS",
 /* 225 */ "tenths_to_19ths ::= FOURTEENTHS",
 /* 226 */ "tenths_to_19ths ::= FIFTEENTHS",
 /* 227 */ "tenths_to_19ths ::= SIXTEENTHS",
 /* 228 */ "tenths_to_19ths ::= SEVENTEENTHS",
 /* 229 */ "tenths_to_19ths ::= EIGHTEENTHS",
 /* 230 */ "tenths_to_19ths ::= NINETEENTHS",
 /* 231 */ "tenths ::= TWENTIETHS",
 /* 232 */ "tenths ::= THIRTIETHS",
 /* 233 */ "tenths ::= FOURTIETHS",
 /* 234 */ "tenths ::= FIFTIETHS",
 /* 235 */ "tenths ::= SIXTIETHS",
 /* 236 */ "tenths ::= SEVENTIETHS",
 /* 237 */ "tenths ::= EIGHTIETHS",
 /* 238 */ "tenths ::= NINETIETHS",
 /* 239 */ "program ::= numbers",
 /* 240 */ "numbers ::= numbers number",
 /* 241 */ "numbers ::= number",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yytos = NULL;
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    if( yyGrowStack(pParser) ){
      pParser->yystack = &pParser->yystk0;
      pParser->yystksz = 1;
    }
#endif
#ifndef YYNOERRORRECOVERY
    pParser->yyerrcnt = -1;
#endif
    pParser->yytos = pParser->yystack;
    pParser->yystack[0].stateno = 0;
    pParser->yystack[0].major = 0;
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseReset(
  void *p                    /* The parser to be reset */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif

  if( pParser ){
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yytos = NULL;
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    /*if( yyGrowStack(pParser) ){*/
      /*pParser->yystack = &pParser->yystk0;*/
      /*pParser->yystksz = 1;*/
    /*}*/
#endif
#ifndef YYNOERRORRECOVERY
    pParser->yyerrcnt = -1;
#endif
    pParser->yytos = pParser->yystack;
    pParser->yystack[0].stateno = 0;
    pParser->yystack[0].major = 0;
  }
}

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH;
   yypParser->yytos--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH] ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 113, 1 },
  { 113, 2 },
  { 114, 3 },
  { 114, 3 },
  { 114, 2 },
  { 114, 2 },
  { 114, 2 },
  { 114, 2 },
  { 114, 2 },
  { 114, 2 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 3 },
  { 114, 1 },
  { 118, 2 },
  { 118, 2 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 116, 2 },
  { 116, 3 },
  { 116, 2 },
  { 116, 1 },
  { 114, 1 },
  { 114, 2 },
  { 117, 2 },
  { 117, 3 },
  { 117, 2 },
  { 117, 1 },
  { 114, 1 },
  { 114, 2 },
  { 115, 2 },
  { 115, 3 },
  { 115, 2 },
  { 115, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 2 },
  { 119, 1 },
  { 119, 1 },
  { 121, 2 },
  { 121, 3 },
  { 121, 2 },
  { 121, 1 },
  { 114, 1 },
  { 114, 2 },
  { 122, 1 },
  { 122, 1 },
  { 123, 2 },
  { 123, 3 },
  { 123, 2 },
  { 123, 1 },
  { 114, 1 },
  { 114, 2 },
  { 124, 1 },
  { 124, 1 },
  { 125, 2 },
  { 125, 3 },
  { 125, 2 },
  { 125, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 2 },
  { 126, 1 },
  { 126, 1 },
  { 127, 2 },
  { 127, 3 },
  { 127, 2 },
  { 127, 1 },
  { 114, 1 },
  { 114, 2 },
  { 128, 1 },
  { 128, 1 },
  { 129, 2 },
  { 129, 3 },
  { 129, 2 },
  { 129, 1 },
  { 114, 1 },
  { 114, 2 },
  { 130, 1 },
  { 130, 1 },
  { 131, 2 },
  { 131, 3 },
  { 131, 2 },
  { 131, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 2 },
  { 132, 1 },
  { 132, 1 },
  { 133, 2 },
  { 133, 3 },
  { 133, 2 },
  { 133, 1 },
  { 114, 1 },
  { 114, 2 },
  { 134, 1 },
  { 134, 1 },
  { 135, 2 },
  { 135, 3 },
  { 135, 2 },
  { 135, 1 },
  { 114, 1 },
  { 114, 2 },
  { 136, 1 },
  { 136, 1 },
  { 137, 2 },
  { 137, 3 },
  { 137, 2 },
  { 137, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 2 },
  { 138, 2 },
  { 138, 1 },
  { 139, 3 },
  { 139, 4 },
  { 139, 3 },
  { 139, 2 },
  { 139, 1 },
  { 114, 1 },
  { 114, 2 },
  { 140, 2 },
  { 140, 1 },
  { 141, 3 },
  { 141, 4 },
  { 141, 3 },
  { 141, 2 },
  { 141, 1 },
  { 114, 1 },
  { 114, 2 },
  { 142, 2 },
  { 142, 1 },
  { 120, 3 },
  { 120, 4 },
  { 120, 3 },
  { 120, 2 },
  { 120, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 2 },
  { 143, 2 },
  { 143, 1 },
  { 143, 1 },
  { 145, 2 },
  { 145, 1 },
  { 145, 1 },
  { 144, 2 },
  { 144, 1 },
  { 144, 1 },
  { 149, 1 },
  { 149, 1 },
  { 152, 1 },
  { 152, 1 },
  { 154, 1 },
  { 154, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 111, 1 },
  { 112, 2 },
  { 112, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH-1] ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* number ::= final_number */
#line 217 "parser.yy"
{
    if (!yymsp[0].minor.yy0.leave_alone) {
        insertYYSTYPE(&(state->yystypeList), yymsp[0].minor.yy0);
    }
}
#line 2084 "parser.c"
        break;
      case 1: /* number ::= NEGATIVE final_number */
#line 222 "parser.yy"
{
    yymsp[0].minor.yy0.dbl = -yymsp[0].minor.yy0.dbl;
    yymsp[0].minor.yy0.begin = yymsp[-1].minor.yy0.begin;
    yymsp[0].minor.yy0.end = yymsp[0].minor.yy0.end;
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 2094 "parser.c"
        break;
      case 2: /* final_number ::= less_than_quadrillion AND_A QUARTER */
#line 229 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, 1.0, 4.0); }
#line 2099 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* final_number ::= less_than_quadrillion AND_A HALF */
#line 230 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0); }
#line 2105 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 4: /* final_number ::= less_than_quadrillion QUARTERS */
#line 232 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, yymsp[-1].minor.yy0.dbl, 4.0); }
#line 2111 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* final_number ::= ONE QUARTER */
      case 6: /* final_number ::= A QUARTER */ yytestcase(yyruleno==6);
#line 233 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 4.0); }
#line 2118 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* final_number ::= less_than_quadrillion HALVES */
#line 236 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, yymsp[-1].minor.yy0.dbl, 2.0); }
#line 2124 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 8: /* final_number ::= ONE HALF */
#line 237 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0);  }
#line 2130 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* final_number ::= A HALF */
#line 238 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0); }
#line 2136 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 10: /* final_number ::= less_than_quadrillion */
      case 36: /* less_than_quadrillion ::= less_than_trillion */ yytestcase(yyruleno==36);
      case 56: /* less_than_trillion_end_only ::= less_than_trillion */ yytestcase(yyruleno==56);
      case 57: /* less_than_trillion_end_only ::= less_than_billion_end_only */ yytestcase(yyruleno==57);
      case 61: /* less_than_trillion ::= less_than_billion */ yytestcase(yyruleno==61);
      case 81: /* less_than_billion_end_only ::= less_than_billion */ yytestcase(yyruleno==81);
      case 82: /* less_than_billion_end_only ::= less_than_million_end_only */ yytestcase(yyruleno==82);
      case 86: /* less_than_billion ::= less_than_million */ yytestcase(yyruleno==86);
      case 106: /* less_than_million_end_only ::= less_than_million */ yytestcase(yyruleno==106);
      case 107: /* less_than_million_end_only ::= less_than_thousand_end_only */ yytestcase(yyruleno==107);
      case 111: /* less_than_million ::= less_than_thousand */ yytestcase(yyruleno==111);
      case 134: /* less_than_thousand_end_only ::= less_than_thousand */ yytestcase(yyruleno==134);
      case 139: /* less_than_thousand ::= less_than_hundred */ yytestcase(yyruleno==139);
      case 150: /* less_than_hundred ::= tens */ yytestcase(yyruleno==150);
      case 151: /* less_than_hundred ::= less_than_twenty */ yytestcase(yyruleno==151);
      case 156: /* less_than_twenty ::= ten_to_19 */ yytestcase(yyruleno==156);
      case 157: /* less_than_twenty ::= less_than_ten */ yytestcase(yyruleno==157);
#line 240 "parser.yy"
{ COPY_YYSTYPE_BE_DBL(yylhsminor.yy0, yymsp[0].minor.yy0); }
#line 2158 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 11: /* final_number ::= less_than_quadrillionth */
      case 12: /* final_number ::= less_than_quadrillionths */ yytestcase(yyruleno==12);
      case 24: /* less_than_quadrillionth ::= less_than_trillionth */ yytestcase(yyruleno==24);
      case 30: /* less_than_quadrillionths ::= less_than_trillionths */ yytestcase(yyruleno==30);
      case 40: /* less_than_trillionth_end_only ::= less_than_trillionth */ yytestcase(yyruleno==40);
      case 41: /* less_than_trillionth_end_only ::= less_than_billionth_end_only */ yytestcase(yyruleno==41);
      case 45: /* less_than_trillionth ::= less_than_billionth */ yytestcase(yyruleno==45);
      case 48: /* less_than_trillionths_end_only ::= less_than_trillionths */ yytestcase(yyruleno==48);
      case 49: /* less_than_trillionths_end_only ::= less_than_billionths_end_only */ yytestcase(yyruleno==49);
      case 53: /* less_than_trillionths ::= less_than_billionths */ yytestcase(yyruleno==53);
      case 65: /* less_than_billionth_end_only ::= less_than_billionth */ yytestcase(yyruleno==65);
      case 66: /* less_than_billionth_end_only ::= less_than_millionth_end_only */ yytestcase(yyruleno==66);
      case 70: /* less_than_billionth ::= less_than_millionth */ yytestcase(yyruleno==70);
      case 73: /* less_than_billionths_end_only ::= less_than_billionths */ yytestcase(yyruleno==73);
      case 74: /* less_than_billionths_end_only ::= less_than_millionths_end_only */ yytestcase(yyruleno==74);
      case 78: /* less_than_billionths ::= less_than_millionths */ yytestcase(yyruleno==78);
      case 90: /* less_than_millionth_end_only ::= less_than_millionth */ yytestcase(yyruleno==90);
      case 91: /* less_than_millionth_end_only ::= less_than_thousandth_end_only */ yytestcase(yyruleno==91);
      case 95: /* less_than_millionth ::= less_than_thousandth */ yytestcase(yyruleno==95);
      case 98: /* less_than_millionths_end_only ::= less_than_millionths */ yytestcase(yyruleno==98);
      case 99: /* less_than_millionths_end_only ::= less_than_thousandths_end_only */ yytestcase(yyruleno==99);
      case 103: /* less_than_millionths ::= less_than_thousandths */ yytestcase(yyruleno==103);
      case 116: /* less_than_thousandth_end_only ::= less_than_thousandth */ yytestcase(yyruleno==116);
      case 121: /* less_than_thousandth ::= less_than_hundredth */ yytestcase(yyruleno==121);
      case 125: /* less_than_thousandths_end_only ::= less_than_thousandths */ yytestcase(yyruleno==125);
      case 130: /* less_than_thousandths ::= less_than_hundredths */ yytestcase(yyruleno==130);
      case 144: /* less_than_hundredth ::= tenth */ yytestcase(yyruleno==144);
      case 145: /* less_than_hundredth ::= less_than_twentieth */ yytestcase(yyruleno==145);
      case 147: /* less_than_hundredths ::= tenths */ yytestcase(yyruleno==147);
      case 148: /* less_than_hundredths ::= less_than_twentieths */ yytestcase(yyruleno==148);
      case 152: /* less_than_twentieth ::= tenth_to_19th */ yytestcase(yyruleno==152);
      case 153: /* less_than_twentieth ::= less_than_tenth */ yytestcase(yyruleno==153);
      case 154: /* less_than_twentieths ::= tenths_to_19ths */ yytestcase(yyruleno==154);
      case 155: /* less_than_twentieths ::= less_than_tenths */ yytestcase(yyruleno==155);
#line 241 "parser.yy"
{ COPY_YYSTYPE_BE_DBL_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0); }
#line 2197 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 13: /* final_number ::= less_than_quadrillion AND fraction */
#line 244 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, yymsp[0].minor.yy0.frac_num, yymsp[0].minor.yy0.frac_denom); }
#line 2203 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* final_number ::= fraction */
#line 247 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.frac_num = yymsp[0].minor.yy0.frac_num; yylhsminor.yy0.frac_denom = yymsp[0].minor.yy0.frac_denom; yylhsminor.yy0.is_frac = yymsp[0].minor.yy0.is_frac; }
#line 2209 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 15: /* fraction ::= A less_than_quadrillionth */
      case 16: /* fraction ::= AN less_than_quadrillionth */ yytestcase(yyruleno==16);
#line 259 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, yymsp[0].minor.yy0.dbl); }
#line 2216 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 17: /* final_number ::= ZERO_WHOLE_NUMBER */
#line 265 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.leave_alone = true; }
#line 2222 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 18: /* final_number ::= WHOLE_NUMBER */
#line 266 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; }
#line 2228 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 19: /* final_number ::= DECIMAL */
#line 267 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = true; }
#line 2234 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 20: /* final_number ::= ZERO */
#line 268 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 0.0); }
#line 2240 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 21: /* less_than_quadrillionth ::= TRILLION less_than_trillionth_end_only */
      case 27: /* less_than_quadrillionths ::= TRILLION less_than_trillionths_end_only */ yytestcase(yyruleno==27);
#line 274 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF_VALUE(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2247 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 22: /* less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only */
      case 28: /* less_than_quadrillionths ::= less_than_thousand TRILLION less_than_trillionths_end_only */ yytestcase(yyruleno==28);
#line 275 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2254 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 23: /* less_than_quadrillionth ::= less_than_thousand TRILLIONTH */
#line 276 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 2260 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* final_number ::= TRILLIONTH */
#line 279 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 2266 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 26: /* final_number ::= DECIMAL TRILLIONTH */
#line 280 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 2272 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* less_than_quadrillionths ::= less_than_thousand TRILLIONTHS */
#line 286 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 2278 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 31: /* final_number ::= TRILLIONTHS */
#line 289 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 2284 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* final_number ::= DECIMAL TRILLIONTHS */
#line 290 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 2290 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 33: /* less_than_quadrillion ::= TRILLION less_than_trillion_end_only */
#line 296 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_VALUE(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2296 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 34: /* less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only */
#line 297 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2302 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 35: /* less_than_quadrillion ::= less_than_thousand TRILLION */
      case 39: /* final_number ::= WHOLE_NUMBER TRILLION */ yytestcase(yyruleno==39);
#line 298 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2309 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 37: /* final_number ::= TRILLION */
#line 301 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2315 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* final_number ::= DECIMAL TRILLION */
#line 302 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2321 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 42: /* less_than_trillionth ::= BILLION less_than_billionth_end_only */
      case 50: /* less_than_trillionths ::= BILLION less_than_billionths_end_only */ yytestcase(yyruleno==50);
#line 312 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF_VALUE(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2328 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 43: /* less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only */
      case 51: /* less_than_trillionths ::= less_than_thousand BILLION less_than_billionths_end_only */ yytestcase(yyruleno==51);
#line 313 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2335 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 44: /* less_than_trillionth ::= less_than_thousand BILLIONTH */
#line 314 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 2341 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 46: /* final_number ::= BILLIONTH */
#line 317 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 2347 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 47: /* final_number ::= DECIMAL BILLIONTH */
#line 318 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 2353 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 52: /* less_than_trillionths ::= less_than_thousand BILLIONTHS */
#line 327 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 2359 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 54: /* final_number ::= BILLIONTHS */
#line 330 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 2365 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 55: /* final_number ::= DECIMAL BILLIONTHS */
#line 331 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 2371 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 58: /* less_than_trillion ::= BILLION less_than_billion_end_only */
#line 340 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_VALUE(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2377 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 59: /* less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only */
#line 341 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2383 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 60: /* less_than_trillion ::= less_than_thousand BILLION */
      case 64: /* final_number ::= WHOLE_NUMBER BILLION */ yytestcase(yyruleno==64);
#line 342 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2390 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 62: /* final_number ::= BILLION */
#line 345 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2396 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 63: /* final_number ::= DECIMAL BILLION */
#line 346 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2402 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 67: /* less_than_billionth ::= MILLION less_than_millionth_end_only */
      case 75: /* less_than_billionths ::= MILLION less_than_millionths_end_only */ yytestcase(yyruleno==75);
#line 356 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF_VALUE(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2409 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 68: /* less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only */
      case 76: /* less_than_billionths ::= less_than_thousand MILLION less_than_millionths_end_only */ yytestcase(yyruleno==76);
#line 357 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2416 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 69: /* less_than_billionth ::= less_than_thousand MILLIONTH */
#line 358 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2422 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 71: /* final_number ::= MILLIONTH */
#line 361 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2428 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 72: /* final_number ::= DECIMAL MILLIONTH */
#line 362 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2434 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 77: /* less_than_billionths ::= less_than_thousand MILLIONTHS */
#line 371 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2440 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 79: /* final_number ::= MILLIONTHS */
#line 374 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2446 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 80: /* final_number ::= DECIMAL MILLIONTHS */
#line 375 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2452 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 83: /* less_than_billion ::= MILLION less_than_million_end_only */
#line 384 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_VALUE(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2458 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 84: /* less_than_billion ::= less_than_thousand MILLION less_than_million_end_only */
#line 385 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2464 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 85: /* less_than_billion ::= less_than_thousand MILLION */
      case 89: /* final_number ::= WHOLE_NUMBER MILLION */ yytestcase(yyruleno==89);
#line 386 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2471 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 87: /* final_number ::= MILLION */
#line 389 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2477 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 88: /* final_number ::= DECIMAL MILLION */
#line 390 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2483 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 92: /* less_than_millionth ::= THOUSAND less_than_thousandth_end_only */
      case 100: /* less_than_millionths ::= THOUSAND less_than_thousandths_end_only */ yytestcase(yyruleno==100);
#line 400 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF_VALUE(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2490 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 93: /* less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only */
      case 101: /* less_than_millionths ::= less_than_thousand THOUSAND less_than_thousandths_end_only */ yytestcase(yyruleno==101);
#line 401 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2497 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 94: /* less_than_millionth ::= less_than_thousand THOUSANDTH */
#line 402 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2503 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 96: /* final_number ::= THOUSANDTH */
#line 405 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2509 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 97: /* final_number ::= DECIMAL THOUSANDTH */
#line 406 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2515 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 102: /* less_than_millionths ::= less_than_thousand THOUSANDTHS */
#line 415 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2521 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 104: /* final_number ::= THOUSANDTHS */
#line 418 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2527 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 105: /* final_number ::= DECIMAL THOUSANDTHS */
#line 419 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2533 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 108: /* less_than_million ::= THOUSAND less_than_thousand_end_only */
#line 428 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_VALUE(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2539 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 109: /* less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only */
#line 429 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2545 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 110: /* less_than_million ::= less_than_thousand THOUSAND */
      case 114: /* final_number ::= WHOLE_NUMBER THOUSAND */ yytestcase(yyruleno==114);
#line 430 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2552 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 112: /* final_number ::= THOUSAND */
#line 433 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2558 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 113: /* final_number ::= DECIMAL THOUSAND */
#line 434 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2564 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 115: /* less_than_thousandth_end_only ::= AND less_than_hundredth */
      case 124: /* less_than_thousandths_end_only ::= AND less_than_hundredths */ yytestcase(yyruleno==124);
#line 441 "parser.yy"
{ COPY_YYSTYPE_BE_DBL_SUFF(yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2571 "parser.c"
        break;
      case 117: /* less_than_thousandth ::= HUNDRED AND less_than_hundredth */
      case 126: /* less_than_thousandths ::= HUNDRED AND less_than_hundredths */ yytestcase(yyruleno==126);
#line 444 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF_VALUE(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2577 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 118: /* less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth */
      case 127: /* less_than_thousandths ::= less_than_hundred HUNDRED AND less_than_hundredths */ yytestcase(yyruleno==127);
#line 445 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2584 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 119: /* less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth */
      case 128: /* less_than_thousandths ::= less_than_hundred HUNDRED less_than_hundredths */ yytestcase(yyruleno==128);
#line 446 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2591 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 120: /* less_than_thousandth ::= less_than_hundred HUNDREDTH */
#line 447 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2597 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 122: /* final_number ::= HUNDREDTH */
#line 450 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2603 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 123: /* final_number ::= DECIMAL HUNDREDTH */
#line 451 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2609 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 129: /* less_than_thousandths ::= less_than_hundred HUNDREDTHS */
#line 461 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2615 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 131: /* final_number ::= HUNDREDTHS */
#line 464 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2621 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 132: /* final_number ::= DECIMAL HUNDREDTHS */
#line 466 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2627 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 133: /* less_than_thousand_end_only ::= AND less_than_hundred */
#line 472 "parser.yy"
{ COPY_YYSTYPE_BE_DBL(yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2633 "parser.c"
        break;
      case 135: /* less_than_thousand ::= HUNDRED AND less_than_hundred */
#line 475 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_VALUE(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2638 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 136: /* less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred */
#line 476 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2644 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 137: /* less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred */
#line 477 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2650 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 138: /* less_than_thousand ::= less_than_hundred HUNDRED */
      case 142: /* final_number ::= WHOLE_NUMBER HUNDRED */ yytestcase(yyruleno==142);
#line 478 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2657 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 140: /* final_number ::= HUNDRED */
#line 481 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2663 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 141: /* final_number ::= DECIMAL HUNDRED */
#line 482 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2669 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 143: /* less_than_hundredth ::= tens less_than_tenth */
      case 146: /* less_than_hundredths ::= tens less_than_tenths */ yytestcase(yyruleno==146);
#line 489 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2676 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 149: /* less_than_hundred ::= tens less_than_ten */
#line 501 "parser.yy"
{ COPY_YYSTYPE_BE_ADD(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2682 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 158: /* less_than_ten ::= ONE */
#line 526 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0); }
#line 2688 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 159: /* less_than_ten ::= TWO */
#line 527 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0); }
#line 2694 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 160: /* less_than_ten ::= THREE */
#line 528 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0); }
#line 2700 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 161: /* less_than_ten ::= FOUR */
#line 529 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0); }
#line 2706 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 162: /* less_than_ten ::= FIVE */
#line 530 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0); }
#line 2712 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 163: /* less_than_ten ::= SIX */
#line 531 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0); }
#line 2718 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 164: /* less_than_ten ::= SEVEN */
#line 532 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0); }
#line 2724 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 165: /* less_than_ten ::= EIGHT */
#line 533 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0); }
#line 2730 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 166: /* less_than_ten ::= NINE */
#line 534 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0); }
#line 2736 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 167: /* ten_to_19 ::= TEN */
#line 536 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0); }
#line 2742 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 168: /* ten_to_19 ::= ELEVEN */
#line 537 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0); }
#line 2748 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 169: /* ten_to_19 ::= TWELVE */
#line 538 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0); }
#line 2754 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 170: /* ten_to_19 ::= THIRTEEN */
#line 539 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0); }
#line 2760 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 171: /* ten_to_19 ::= FOURTEEN */
#line 540 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0); }
#line 2766 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 172: /* ten_to_19 ::= FIFTEEN */
#line 541 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0); }
#line 2772 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 173: /* ten_to_19 ::= SIXTEEN */
#line 542 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0); }
#line 2778 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 174: /* ten_to_19 ::= SEVENTEEN */
#line 543 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0); }
#line 2784 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 175: /* ten_to_19 ::= EIGHTEEN */
#line 544 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0); }
#line 2790 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 176: /* ten_to_19 ::= NINETEEN */
#line 545 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0); }
#line 2796 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 177: /* tens ::= TWENTY */
#line 547 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0); }
#line 2802 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 178: /* tens ::= THIRTY */
#line 548 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0); }
#line 2808 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 179: /* tens ::= FORTY */
#line 549 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0); }
#line 2814 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 180: /* tens ::= FIFTY */
#line 550 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0); }
#line 2820 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 181: /* tens ::= SIXTY */
#line 551 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0); }
#line 2826 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 182: /* tens ::= SEVENTY */
#line 552 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0); }
#line 2832 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 183: /* tens ::= EIGHTY */
#line 553 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0); }
#line 2838 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 184: /* tens ::= NINETY */
#line 554 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0); }
#line 2844 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 185: /* less_than_tenth ::= FIRST */
#line 556 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0, ST); }
#line 2850 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 186: /* less_than_tenth ::= SECOND */
#line 557 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0, ND); }
#line 2856 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 187: /* less_than_tenth ::= THIRD */
#line 558 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0, RD); }
#line 2862 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 188: /* less_than_tenth ::= FOURTH */
#line 559 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0, TH); }
#line 2868 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 189: /* less_than_tenth ::= FIFTH */
#line 560 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0, TH); }
#line 2874 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 190: /* less_than_tenth ::= SIXTH */
#line 561 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0, TH); }
#line 2880 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 191: /* less_than_tenth ::= SEVENTH */
#line 562 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0, TH); }
#line 2886 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 192: /* less_than_tenth ::= EIGHTH */
#line 563 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0, TH); }
#line 2892 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 193: /* less_than_tenth ::= NINTH */
#line 564 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0, TH); }
#line 2898 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 194: /* tenth_to_19th ::= TENTH */
#line 566 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0, TH); }
#line 2904 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 195: /* tenth_to_19th ::= ELEVENTH */
#line 567 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0, TH); }
#line 2910 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 196: /* tenth_to_19th ::= TWELFTH */
#line 568 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0, TH); }
#line 2916 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 197: /* tenth_to_19th ::= THIRTEENTH */
#line 569 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0, TH); }
#line 2922 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 198: /* tenth_to_19th ::= FOURTEENTH */
#line 570 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0, TH); }
#line 2928 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 199: /* tenth_to_19th ::= FIFTEENTH */
#line 571 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0, TH); }
#line 2934 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 200: /* tenth_to_19th ::= SIXTEENTH */
#line 572 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0, TH); }
#line 2940 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 201: /* tenth_to_19th ::= SEVENTEENTH */
#line 573 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0, TH); }
#line 2946 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 202: /* tenth_to_19th ::= EIGHTEENTH */
#line 574 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0, TH); }
#line 2952 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 203: /* tenth_to_19th ::= NINETEENTH */
#line 575 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0, TH); }
#line 2958 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 204: /* tenth ::= TWENTIETH */
#line 577 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0, TH); }
#line 2964 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 205: /* tenth ::= THIRTIETH */
#line 578 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0, TH); }
#line 2970 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 206: /* tenth ::= FOURTIETH */
#line 579 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0, TH); }
#line 2976 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 207: /* tenth ::= FIFTIETH */
#line 580 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0, TH); }
#line 2982 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 208: /* tenth ::= SIXTIETH */
#line 581 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0, TH); }
#line 2988 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 209: /* tenth ::= SEVENTIETH */
#line 582 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0, TH); }
#line 2994 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 210: /* tenth ::= EIGHTIETH */
#line 583 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0, TH); }
#line 3000 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 211: /* tenth ::= NINETIETH */
#line 584 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0, TH); }
#line 3006 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 212: /* less_than_tenths ::= FIRSTS */
#line 586 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0, STS); }
#line 3012 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 213: /* less_than_tenths ::= SECONDS */
#line 587 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0, NDS); }
#line 3018 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 214: /* less_than_tenths ::= THIRDS */
#line 588 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0, RDS); }
#line 3024 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 215: /* less_than_tenths ::= FOURTHS */
#line 589 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0, THS); }
#line 3030 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 216: /* less_than_tenths ::= FIFTHS */
#line 590 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0, THS); }
#line 3036 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 217: /* less_than_tenths ::= SIXTHS */
#line 591 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0, THS); }
#line 3042 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 218: /* less_than_tenths ::= SEVENTHS */
#line 592 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0, THS); }
#line 3048 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 219: /* less_than_tenths ::= EIGHTHS */
#line 593 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0, THS); }
#line 3054 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 220: /* less_than_tenths ::= NINTHS */
#line 594 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0, THS); }
#line 3060 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 221: /* tenths_to_19ths ::= TENTHS */
#line 596 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0, THS); }
#line 3066 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 222: /* tenths_to_19ths ::= ELEVENTHS */
#line 597 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0, THS); }
#line 3072 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 223: /* tenths_to_19ths ::= TWELFTHS */
#line 598 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0, THS); }
#line 3078 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 224: /* tenths_to_19ths ::= THIRTEENTHS */
#line 599 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0, THS); }
#line 3084 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 225: /* tenths_to_19ths ::= FOURTEENTHS */
#line 600 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0, THS); }
#line 3090 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 226: /* tenths_to_19ths ::= FIFTEENTHS */
#line 601 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0, THS); }
#line 3096 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 227: /* tenths_to_19ths ::= SIXTEENTHS */
#line 602 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0, THS); }
#line 3102 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 228: /* tenths_to_19ths ::= SEVENTEENTHS */
#line 603 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0, THS); }
#line 3108 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 229: /* tenths_to_19ths ::= EIGHTEENTHS */
#line 604 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0, THS); }
#line 3114 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 230: /* tenths_to_19ths ::= NINETEENTHS */
#line 605 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0, THS); }
#line 3120 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 231: /* tenths ::= TWENTIETHS */
#line 607 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0, THS); }
#line 3126 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 232: /* tenths ::= THIRTIETHS */
#line 608 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0, THS); }
#line 3132 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 233: /* tenths ::= FOURTIETHS */
#line 609 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0, THS); }
#line 3138 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 234: /* tenths ::= FIFTIETHS */
#line 610 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0, THS); }
#line 3144 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 235: /* tenths ::= SIXTIETHS */
#line 611 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0, THS); }
#line 3150 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 236: /* tenths ::= SEVENTIETHS */
#line 612 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0, THS); }
#line 3156 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 237: /* tenths ::= EIGHTIETHS */
#line 613 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0, THS); }
#line 3162 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 238: /* tenths ::= NINETIETHS */
#line 614 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0, THS); }
#line 3168 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      default:
      /* (239) program ::= numbers */ yytestcase(yyruleno==239);
      /* (240) numbers ::= numbers number */ yytestcase(yyruleno==240);
      /* (241) numbers ::= number (OPTIMIZED OUT) */ assert(yyruleno!=241);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ){
      yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    }
    yymsp -= yysize-1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yytos -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
#line 203 "parser.yy"

    state->error = PARSE_FAILURE;
#if print_errors
    fprintf(stderr,"Giving up.  Parser is hopelessly lost...\n");
#endif
#line 3221 "parser.c"
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 196 "parser.yy"

    state->error = SYNTAX_ERROR;
#if print_errors
    fprintf(stderr, "Syntax error\n");
#endif
#line 3244 "parser.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
