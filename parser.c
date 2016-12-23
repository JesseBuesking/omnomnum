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
#line 165 "parser.c"
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
#define TOKEN_DECIMAL                        11
#define TOKEN_ZERO                           12
#define TOKEN_TRILLION                       13
#define TOKEN_TRILLIONTH                     14
#define TOKEN_TRILLIONTHS                    15
#define TOKEN_WHOLE_NUMBER                   16
#define TOKEN_BILLION                        17
#define TOKEN_BILLIONTH                      18
#define TOKEN_BILLIONTHS                     19
#define TOKEN_MILLION                        20
#define TOKEN_MILLIONTH                      21
#define TOKEN_MILLIONTHS                     22
#define TOKEN_THOUSAND                       23
#define TOKEN_THOUSANDTH                     24
#define TOKEN_THOUSANDTHS                    25
#define TOKEN_HUNDRED                        26
#define TOKEN_HUNDREDTH                      27
#define TOKEN_HUNDREDTHS                     28
#define TOKEN_TWO                            29
#define TOKEN_THREE                          30
#define TOKEN_FOUR                           31
#define TOKEN_FIVE                           32
#define TOKEN_SIX                            33
#define TOKEN_SEVEN                          34
#define TOKEN_EIGHT                          35
#define TOKEN_NINE                           36
#define TOKEN_TEN                            37
#define TOKEN_ELEVEN                         38
#define TOKEN_TWELVE                         39
#define TOKEN_THIRTEEN                       40
#define TOKEN_FOURTEEN                       41
#define TOKEN_FIFTEEN                        42
#define TOKEN_SIXTEEN                        43
#define TOKEN_SEVENTEEN                      44
#define TOKEN_EIGHTEEN                       45
#define TOKEN_NINETEEN                       46
#define TOKEN_TWENTY                         47
#define TOKEN_THIRTY                         48
#define TOKEN_FORTY                          49
#define TOKEN_FIFTY                          50
#define TOKEN_SIXTY                          51
#define TOKEN_SEVENTY                        52
#define TOKEN_EIGHTY                         53
#define TOKEN_NINETY                         54
#define TOKEN_FIRST                          55
#define TOKEN_SECOND                         56
#define TOKEN_THIRD                          57
#define TOKEN_FOURTH                         58
#define TOKEN_FIFTH                          59
#define TOKEN_SIXTH                          60
#define TOKEN_SEVENTH                        61
#define TOKEN_EIGHTH                         62
#define TOKEN_NINTH                          63
#define TOKEN_TENTH                          64
#define TOKEN_ELEVENTH                       65
#define TOKEN_TWELFTH                        66
#define TOKEN_THIRTEENTH                     67
#define TOKEN_FOURTEENTH                     68
#define TOKEN_FIFTEENTH                      69
#define TOKEN_SIXTEENTH                      70
#define TOKEN_SEVENTEENTH                    71
#define TOKEN_EIGHTEENTH                     72
#define TOKEN_NINETEENTH                     73
#define TOKEN_TWENTIETH                      74
#define TOKEN_THIRTIETH                      75
#define TOKEN_FOURTIETH                      76
#define TOKEN_FIFTIETH                       77
#define TOKEN_SIXTIETH                       78
#define TOKEN_SEVENTIETH                     79
#define TOKEN_EIGHTIETH                      80
#define TOKEN_NINETIETH                      81
#define TOKEN_FIRSTS                         82
#define TOKEN_SECONDS                        83
#define TOKEN_THIRDS                         84
#define TOKEN_FOURTHS                        85
#define TOKEN_FIFTHS                         86
#define TOKEN_SIXTHS                         87
#define TOKEN_SEVENTHS                       88
#define TOKEN_EIGHTHS                        89
#define TOKEN_NINTHS                         90
#define TOKEN_TENTHS                         91
#define TOKEN_ELEVENTHS                      92
#define TOKEN_TWELFTHS                       93
#define TOKEN_THIRTEENTHS                    94
#define TOKEN_FOURTEENTHS                    95
#define TOKEN_FIFTEENTHS                     96
#define TOKEN_SIXTEENTHS                     97
#define TOKEN_SEVENTEENTHS                   98
#define TOKEN_EIGHTEENTHS                    99
#define TOKEN_NINETEENTHS                    100
#define TOKEN_TWENTIETHS                     101
#define TOKEN_THIRTIETHS                     102
#define TOKEN_FOURTIETHS                     103
#define TOKEN_FIFTIETHS                      104
#define TOKEN_SIXTIETHS                      105
#define TOKEN_SEVENTIETHS                    106
#define TOKEN_EIGHTIETHS                     107
#define TOKEN_NINETIETHS                     108
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
#define YYNOCODE 158
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
#define YYNSTATE             42
#define YYNRULE              225
#define YY_MAX_SHIFT         41
#define YY_MIN_SHIFTREDUCE   264
#define YY_MAX_SHIFTREDUCE   488
#define YY_MIN_REDUCE        489
#define YY_MAX_REDUCE        713
#define YY_ERROR_ACTION      714
#define YY_ACCEPT_ACTION     715
#define YY_NO_ACTION         716
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
#define YY_ACTTAB_COUNT (1596)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   711,    4,  390,  390,  390,  393,   37,    9,  396,  396,
 /*    10 */    10,   26,  282,  296,  286,  291,   32,  318,  304,  311,
 /*    20 */   340,  326,  333,  362,  348,  355,  387,  371,  379,  406,
 /*    30 */   407,  408,  409,  410,  411,  412,  413,  414,  415,  416,
 /*    40 */   417,  418,  419,  420,  421,  422,  423,  424,  425,  426,
 /*    50 */   427,  428,  429,  430,  431,  432,  433,  434,  435,  436,
 /*    60 */   437,  438,  439,  440,  441,  442,  443,  444,  445,  446,
 /*    70 */   447,  448,  449,  450,  451,  452,  453,  454,  455,  456,
 /*    80 */   457,  458,  459,  460,  461,  462,  463,  464,  465,  466,
 /*    90 */   467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
 /*   100 */   477,  478,  479,  480,  481,  482,  483,  484,  485,    4,
 /*   110 */    12,  369,  377,   11,   37,    9,   10,  491,   10,   26,
 /*   120 */   282,  296,  286,  291,   32,  318,  304,  311,  340,  326,
 /*   130 */   333,  362,  348,  355,  387,  371,  379,  406,  407,  408,
 /*   140 */   409,  410,  411,  412,  413,  414,  415,  416,  417,  418,
 /*   150 */   419,  420,  421,  422,  423,  424,  425,  426,  427,  428,
 /*   160 */   429,  430,  431,  432,  433,  434,  435,  436,  437,  438,
 /*   170 */   439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
 /*   180 */   449,  450,  451,  452,  453,  454,  455,  456,  457,  458,
 /*   190 */   459,  460,  461,  462,  463,  464,  465,  466,  467,  468,
 /*   200 */   469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
 /*   210 */   479,  480,  481,  482,  483,  484,  485,   37,    9,  269,
 /*   220 */   272,   10,   26,  282,  296,  286,  291,   32,  318,  304,
 /*   230 */   311,  340,  326,  333,  362,  348,  355,  387,  371,  379,
 /*   240 */   406,  407,  408,  409,  410,  411,  412,  413,  414,  415,
 /*   250 */   416,  417,  418,  419,  420,  421,  422,  423,  424,  425,
 /*   260 */   426,  427,  428,  429,  430,  431,  432,  433,  434,  435,
 /*   270 */   436,  437,  438,  439,  440,  441,  442,  443,  444,  445,
 /*   280 */   446,  447,  448,  449,  450,  451,  452,  453,  454,  455,
 /*   290 */   456,  457,  458,  459,  460,  461,  462,  463,  464,  465,
 /*   300 */   466,  467,  468,  469,  470,  471,  472,  473,  474,  475,
 /*   310 */   476,  477,  478,  479,  480,  481,  482,  483,  484,  485,
 /*   320 */   405,   17,  369,   14,  368,  384,  376,   22,  400,  391,
 /*   330 */   392,  402,  394,  395,  404,  398,  399,  401,  403,    6,
 /*   340 */   346,  353,  277,  406,  407,  408,  409,  410,  411,  412,
 /*   350 */   413,  414,  415,  416,  417,  418,  419,  420,  421,  422,
 /*   360 */   423,  424,  425,  426,  427,  428,  429,  430,  431,  432,
 /*   370 */   433,  434,  435,  436,  437,  438,  439,  440,  441,  442,
 /*   380 */   443,  444,  445,  446,  447,  448,  449,  450,  451,  452,
 /*   390 */   453,  454,  455,  456,  457,  458,  459,  460,  461,  462,
 /*   400 */   463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
 /*   410 */   473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
 /*   420 */   483,  484,  485,  405,   19,  369,   13,  367,  383,  375,
 /*   430 */    22,  400,  391,  392,  402,  394,  395,  404,  398,  399,
 /*   440 */   401,  403,   16,  346,  266,  267,  406,  407,  408,  409,
 /*   450 */   410,  411,  412,  413,  414,  415,  416,  417,  418,  419,
 /*   460 */   420,  421,  422,  423,  424,  425,  426,  427,  428,  429,
 /*   470 */   430,  431,  432,  433,  434,  435,  436,  437,  438,  439,
 /*   480 */   440,  441,  442,  443,  444,  445,  446,  447,  448,  449,
 /*   490 */   450,  451,  452,  453,  454,  455,  456,  457,  458,  459,
 /*   500 */   460,  461,  462,  463,  464,  465,  466,  467,  468,  469,
 /*   510 */   470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
 /*   520 */   480,  481,  482,  483,  484,  485,  405,  365,  381,  373,
 /*   530 */    22,  400,  391,  392,  402,  394,  395,  404,  398,  399,
 /*   540 */   401,  403,    5,  324,  331,    6,  346,  353,  491,  406,
 /*   550 */   407,  408,  409,  410,  411,  412,  413,  414,  415,  416,
 /*   560 */   417,  418,  419,  420,  421,  422,  423,  424,  425,  426,
 /*   570 */   427,  428,  429,  430,  431,  432,  433,  434,  435,  436,
 /*   580 */   437,  438,  439,  440,  441,  442,  443,  444,  445,  446,
 /*   590 */   447,  448,  449,  450,  451,  452,  453,  454,  455,  456,
 /*   600 */   457,  458,  459,  460,  461,  462,  463,  464,  465,  466,
 /*   610 */   467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
 /*   620 */   477,  478,  479,  480,  481,  482,  483,  484,  485,  270,
 /*   630 */   273,  491,  405,  297,  287,  292,  491,  319,  305,  312,
 /*   640 */   341,  327,  334,  363,  349,  356,  388,  372,  380,   15,
 /*   650 */   324,  491,   16,  346,  491,  406,  407,  408,  409,  410,
 /*   660 */   411,  412,  413,  414,  415,  416,  417,  418,  419,  420,
 /*   670 */   421,  422,  423,  424,  425,  426,  427,  428,  429,  430,
 /*   680 */   431,  432,  433,  434,  435,  436,  437,  438,  439,  440,
 /*   690 */   441,  442,  443,  444,  445,  446,  447,  448,  449,  450,
 /*   700 */   451,  452,  453,  454,  455,  456,  457,  458,  405,  345,
 /*   710 */   366,   21,  491,  491,  370,   39,  491,   23,  400,  391,
 /*   720 */   392,  491,  491,  491,  404,  398,  399,  491,  403,  491,
 /*   730 */   491,  406,  407,  408,  409,  410,  411,  412,  413,  414,
 /*   740 */   415,  416,  417,  418,  419,  420,  421,  422,  423,  424,
 /*   750 */   425,  426,  427,  428,  429,  430,  431,  432,  433,  434,
 /*   760 */   435,  436,  437,  438,  439,  440,  441,  442,  443,  444,
 /*   770 */   445,  446,  447,  448,  449,  450,  451,  452,  453,  454,
 /*   780 */   455,  456,  457,  458,  405,  491,  491,   18,    1,  284,
 /*   790 */   289,  491,    3,  302,  309,    5,  324,  331,    6,  346,
 /*   800 */   353,  491,  491,  491,  491,  491,  491,  406,  407,  408,
 /*   810 */   409,  410,  411,  412,  413,  414,  415,  416,  417,  418,
 /*   820 */   419,  420,  421,  422,  423,  424,  425,  426,  427,  428,
 /*   830 */   429,  430,  431,  432,  433,  434,  435,  436,  437,  438,
 /*   840 */   439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
 /*   850 */   449,  450,  451,  452,  453,  454,  455,  456,  457,  458,
 /*   860 */   405,  368,  384,  491,   23,  400,  391,  392,  491,  491,
 /*   870 */   491,  404,  398,  399,   41,  403,  491,  268,  491,  491,
 /*   880 */   271,   25,  491,  406,  407,  408,  409,  410,  411,  412,
 /*   890 */   413,  414,  415,  416,  417,  418,  419,  420,  421,  422,
 /*   900 */   423,  424,  425,  426,  427,  428,  429,  430,  431,  432,
 /*   910 */   433,  434,  435,  436,  437,  438,  439,  440,  441,  442,
 /*   920 */   443,  444,  445,  446,  447,  448,  449,  450,  451,  452,
 /*   930 */   453,  454,  455,  456,  457,  458,   28,  283,  299,  288,
 /*   940 */   306,  293,  313,  300,  303,  307,  310,  314,  317,  322,
 /*   950 */   325,  329,  332,  336,  339,  344,  347,  351,  354,  358,
 /*   960 */   370,   35,  378,   22,  400,  391,  392,  402,  394,  395,
 /*   970 */   404,  398,  399,  401,  403,   20,  424,  425,  426,  427,
 /*   980 */   428,  429,  430,  431,  432,  433,  434,  435,  436,  437,
 /*   990 */   438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
 /*  1000 */   448,  449,  450,  451,  452,  453,  454,  455,  456,  457,
 /*  1010 */   458,  491,  491,  424,  425,  426,  427,  428,  429,  430,
 /*  1020 */   431,  432,  433,  434,  435,  436,  437,  438,  439,  440,
 /*  1030 */   441,  442,  443,  444,  445,  446,  447,  448,  449,  450,
 /*  1040 */   451,  452,  453,  454,  455,  456,  457,  458,  715,    2,
 /*  1050 */     2,  264,   34,  275,  276,  278,   27,  491,  285,  491,
 /*  1060 */   290,  491,  295,  491,  303,  491,  310,  491,  317,  491,
 /*  1070 */   325,  491,  332,  491,  339,  491,  347,  491,  354,  491,
 /*  1080 */   370,   35,  378,   22,  400,  391,  392,  402,  394,  395,
 /*  1090 */   404,  398,  399,  401,  403,  487,  264,   34,  275,  276,
 /*  1100 */   278,   27,  491,  285,  491,  290,  491,  295,  491,  303,
 /*  1110 */   491,  310,  491,  317,  491,  325,  491,  332,  491,  339,
 /*  1120 */   491,  347,  491,  354,  491,  370,   35,  378,   22,  400,
 /*  1130 */   391,  392,  402,  394,  395,  404,  398,  399,  401,  403,
 /*  1140 */    30,  491,  491,  491,  491,  491,  491,  301,  321,  308,
 /*  1150 */   328,  315,  335,  322,  325,  329,  332,  336,  339,  344,
 /*  1160 */   347,  351,  354,  358,  370,   35,  378,   22,  400,  391,
 /*  1170 */   392,  402,  394,  395,  404,  398,  399,  401,  403,  491,
 /*  1180 */   265,   34,  275,  276,  278,   27,  491,  285,  491,  290,
 /*  1190 */   491,  295,  491,  303,  491,  310,  491,  317,  491,  325,
 /*  1200 */   491,  332,  491,  339,  491,  347,  491,  354,  491,  370,
 /*  1210 */    35,  378,   22,  400,  391,  392,  402,  394,  395,  404,
 /*  1220 */   398,  399,  401,  403,   36,  432,  433,  434,  435,  436,
 /*  1230 */   437,  438,  439,  440,  491,  491,  491,  323,  343,  330,
 /*  1240 */   350,  337,  357,  344,  347,  351,  354,  358,  370,   35,
 /*  1250 */   378,   22,  400,  391,  392,  402,  394,  395,  404,  398,
 /*  1260 */   399,  401,  403,  405,  367,  383,  491,   23,  400,  391,
 /*  1270 */   392,  491,  491,  491,  404,  398,  399,  491,  403,  491,
 /*  1280 */   491,  491,  491,  491,  491,  491,  406,  407,  408,  409,
 /*  1290 */   410,  411,  412,  413,    3,  302,  309,    5,  324,  331,
 /*  1300 */     6,  346,  353,  491,    8,  302,  491,   15,  324,  491,
 /*  1310 */    16,  346,  432,  433,  434,  435,  436,  437,  438,  439,
 /*  1320 */   440,  491,  491,  491,  491,  491,    7,  284,  491,  382,
 /*  1330 */     8,  302,  491,   15,  324,  491,   16,  346,  491,  459,
 /*  1340 */   460,  461,  462,  463,  464,  465,  466,  467,  345,  366,
 /*  1350 */   352,  374,  359,  370,   35,  378,   22,  400,  391,  392,
 /*  1360 */   402,  394,  395,  404,  398,  399,  401,  403,   31,  283,
 /*  1370 */   299,  491,  491,  491,  368,  300,  303,   24,  400,  391,
 /*  1380 */   392,  322,  325,  491,  491,  491,  399,  344,  347,  405,
 /*  1390 */   491,  491,  370,   38,  491,   23,  400,  391,  392,  491,
 /*  1400 */   491,  491,  404,  398,  399,  491,  403,  491,  491,  491,
 /*  1410 */   491,  491,  406,  407,  408,  409,  410,  411,  412,  413,
 /*  1420 */   491,  367,  491,  491,   24,  400,  391,  392,  491,  491,
 /*  1430 */   491,  491,  491,  399,  491,  491,  491,  491,  432,  433,
 /*  1440 */   434,  435,  436,  437,  438,  439,  440,  298,   33,  491,
 /*  1450 */   491,  320,  491,  491,  342,  301,  321,  364,  491,  491,
 /*  1460 */   389,  322,  325,  491,  491,  491,  491,  344,  347,  491,
 /*  1470 */   491,  491,  370,   38,  491,   23,  400,  391,  392,  491,
 /*  1480 */   491,  491,  404,  398,  399,  279,  403,  491,   29,  365,
 /*  1490 */   285,  491,   24,  400,  391,  392,  303,  491,  491,  491,
 /*  1500 */   491,  399,  325,  491,  491,  491,  491,  491,  347,  491,
 /*  1510 */   491,  491,  370,   38,  491,   23,  400,  391,  392,  491,
 /*  1520 */   491,  491,  404,  398,  399,  491,  403,  491,  280,  491,
 /*  1530 */   491,   29,  491,  285,  491,  491,  491,  491,  491,  303,
 /*  1540 */   491,  491,  491,  491,  491,  325,  491,  491,  491,  491,
 /*  1550 */   491,  347,  491,  491,  491,  370,   38,   40,   23,  400,
 /*  1560 */   391,  392,  491,  491,  491,  404,  398,  399,  491,  403,
 /*  1570 */   323,  343,  491,  491,  491,  491,  344,  347,  491,  491,
 /*  1580 */   491,  370,   38,  491,   23,  400,  391,  392,  491,  491,
 /*  1590 */   491,  404,  398,  399,  491,  403,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     0,    1,  146,  146,  146,  149,    6,    7,  152,  152,
 /*    10 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*    20 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    30 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*    40 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*    50 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*    60 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*    70 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*    80 */    80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
 /*    90 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*   100 */   100,  101,  102,  103,  104,  105,  106,  107,  108,    1,
 /*   110 */    26,   27,   28,    7,    6,    7,   10,  157,   10,   11,
 /*   120 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*   130 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*   140 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*   150 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*   160 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*   170 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   180 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*   190 */    82,   83,   84,   85,   86,   87,   88,   89,   90,   91,
 /*   200 */    92,   93,   94,   95,   96,   97,   98,   99,  100,  101,
 /*   210 */   102,  103,  104,  105,  106,  107,  108,    6,    7,    3,
 /*   220 */     4,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   230 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*   240 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   250 */    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*   260 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   270 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*   280 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   290 */    79,   80,   81,   82,   83,   84,   85,   86,   87,   88,
 /*   300 */    89,   90,   91,   92,   93,   94,   95,   96,   97,   98,
 /*   310 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*   320 */     6,   26,   27,    9,  142,  143,  144,  145,  146,  147,
 /*   330 */   148,  149,  150,  151,  152,  153,  154,  155,  156,   23,
 /*   340 */    24,   25,  117,   29,   30,   31,   32,   33,   34,   35,
 /*   350 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*   360 */    46,   47,   48,   49,   50,   51,   52,   53,   54,   55,
 /*   370 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*   380 */    66,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*   390 */    76,   77,   78,   79,   80,   81,   82,   83,   84,   85,
 /*   400 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*   410 */    96,   97,   98,   99,  100,  101,  102,  103,  104,  105,
 /*   420 */   106,  107,  108,    6,   26,   27,    9,  142,  143,  144,
 /*   430 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*   440 */   155,  156,   23,   24,    3,    4,   29,   30,   31,   32,
 /*   450 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*   460 */    43,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*   470 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   480 */    63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
 /*   490 */    73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
 /*   500 */    83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*   510 */    93,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   520 */   103,  104,  105,  106,  107,  108,    6,  142,  143,  144,
 /*   530 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*   540 */   155,  156,   20,   21,   22,   23,   24,   25,  157,   29,
 /*   550 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   560 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*   570 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   580 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   590 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   600 */    80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
 /*   610 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*   620 */   100,  101,  102,  103,  104,  105,  106,  107,  108,    3,
 /*   630 */     4,  157,    6,   13,   14,   15,  157,   17,   18,   19,
 /*   640 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   20,
 /*   650 */    21,  157,   23,   24,  157,   29,   30,   31,   32,   33,
 /*   660 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*   670 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   680 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*   690 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*   700 */    74,   75,   76,   77,   78,   79,   80,   81,    6,  137,
 /*   710 */   138,    9,  157,  157,  142,  143,  157,  145,  146,  147,
 /*   720 */   148,  157,  157,  157,  152,  153,  154,  157,  156,  157,
 /*   730 */   157,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*   740 */    38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
 /*   750 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   760 */    58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   770 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   780 */    78,   79,   80,   81,    6,  157,  157,    9,   13,   14,
 /*   790 */    15,  157,   17,   18,   19,   20,   21,   22,   23,   24,
 /*   800 */    25,  157,  157,  157,  157,  157,  157,   29,   30,   31,
 /*   810 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*   820 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*   830 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*   840 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   850 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*   860 */     6,  142,  143,  157,  145,  146,  147,  148,  157,  157,
 /*   870 */   157,  152,  153,  154,    2,  156,  157,    5,  157,  157,
 /*   880 */     8,    9,  157,   29,   30,   31,   32,   33,   34,   35,
 /*   890 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*   900 */    46,   47,   48,   49,   50,   51,   52,   53,   54,   55,
 /*   910 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*   920 */    66,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*   930 */    76,   77,   78,   79,   80,   81,  118,  119,  120,  121,
 /*   940 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*   950 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*   960 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*   970 */   152,  153,  154,  155,  156,    9,   47,   48,   49,   50,
 /*   980 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   990 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*  1000 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*  1010 */    81,  157,  157,   47,   48,   49,   50,   51,   52,   53,
 /*  1020 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*  1030 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*  1040 */    74,   75,   76,   77,   78,   79,   80,   81,  110,  111,
 /*  1050 */   112,  113,  114,  115,  116,  117,  118,  157,  120,  157,
 /*  1060 */   122,  157,  124,  157,  126,  157,  128,  157,  130,  157,
 /*  1070 */   132,  157,  134,  157,  136,  157,  138,  157,  140,  157,
 /*  1080 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  1090 */   152,  153,  154,  155,  156,  112,  113,  114,  115,  116,
 /*  1100 */   117,  118,  157,  120,  157,  122,  157,  124,  157,  126,
 /*  1110 */   157,  128,  157,  130,  157,  132,  157,  134,  157,  136,
 /*  1120 */   157,  138,  157,  140,  157,  142,  143,  144,  145,  146,
 /*  1130 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  1140 */   118,  157,  157,  157,  157,  157,  157,  125,  126,  127,
 /*  1150 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1160 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  1170 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1180 */   113,  114,  115,  116,  117,  118,  157,  120,  157,  122,
 /*  1190 */   157,  124,  157,  126,  157,  128,  157,  130,  157,  132,
 /*  1200 */   157,  134,  157,  136,  157,  138,  157,  140,  157,  142,
 /*  1210 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1220 */   153,  154,  155,  156,  118,   55,   56,   57,   58,   59,
 /*  1230 */    60,   61,   62,   63,  157,  157,  157,  131,  132,  133,
 /*  1240 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1250 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  1260 */   154,  155,  156,    6,  142,  143,  157,  145,  146,  147,
 /*  1270 */   148,  157,  157,  157,  152,  153,  154,  157,  156,  157,
 /*  1280 */   157,  157,  157,  157,  157,  157,   29,   30,   31,   32,
 /*  1290 */    33,   34,   35,   36,   17,   18,   19,   20,   21,   22,
 /*  1300 */    23,   24,   25,  157,   17,   18,  157,   20,   21,  157,
 /*  1310 */    23,   24,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  1320 */    63,  157,  157,  157,  157,  157,   13,   14,  157,  118,
 /*  1330 */    17,   18,  157,   20,   21,  157,   23,   24,  157,   82,
 /*  1340 */    83,   84,   85,   86,   87,   88,   89,   90,  137,  138,
 /*  1350 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  1360 */   149,  150,  151,  152,  153,  154,  155,  156,  118,  119,
 /*  1370 */   120,  157,  157,  157,  142,  125,  126,  145,  146,  147,
 /*  1380 */   148,  131,  132,  157,  157,  157,  154,  137,  138,    6,
 /*  1390 */   157,  157,  142,  143,  157,  145,  146,  147,  148,  157,
 /*  1400 */   157,  157,  152,  153,  154,  157,  156,  157,  157,  157,
 /*  1410 */   157,  157,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1420 */   157,  142,  157,  157,  145,  146,  147,  148,  157,  157,
 /*  1430 */   157,  157,  157,  154,  157,  157,  157,  157,   55,   56,
 /*  1440 */    57,   58,   59,   60,   61,   62,   63,   13,  118,  157,
 /*  1450 */   157,   17,  157,  157,   20,  125,  126,   23,  157,  157,
 /*  1460 */    26,  131,  132,  157,  157,  157,  157,  137,  138,  157,
 /*  1470 */   157,  157,  142,  143,  157,  145,  146,  147,  148,  157,
 /*  1480 */   157,  157,  152,  153,  154,  115,  156,  157,  118,  142,
 /*  1490 */   120,  157,  145,  146,  147,  148,  126,  157,  157,  157,
 /*  1500 */   157,  154,  132,  157,  157,  157,  157,  157,  138,  157,
 /*  1510 */   157,  157,  142,  143,  157,  145,  146,  147,  148,  157,
 /*  1520 */   157,  157,  152,  153,  154,  157,  156,  157,  115,  157,
 /*  1530 */   157,  118,  157,  120,  157,  157,  157,  157,  157,  126,
 /*  1540 */   157,  157,  157,  157,  157,  132,  157,  157,  157,  157,
 /*  1550 */   157,  138,  157,  157,  157,  142,  143,  118,  145,  146,
 /*  1560 */   147,  148,  157,  157,  157,  152,  153,  154,  157,  156,
 /*  1570 */   131,  132,  157,  157,  157,  157,  137,  138,  157,  157,
 /*  1580 */   157,  142,  143,  157,  145,  146,  147,  148,  157,  157,
 /*  1590 */   157,  152,  153,  154,  157,  156,
};
#define YY_SHIFT_USE_DFLT (1596)
#define YY_SHIFT_COUNT    (41)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (1434)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */   108,  314,    0,  314,  211,  314,  314,  702,  702,  626,
 /*    10 */   854,  854,  417,  520,  520,  702,  702,  778,  854,  966,
 /*    20 */   929,  929, 1257, 1383, 1170,  106,  620,  775, 1277, 1313,
 /*    30 */   522, 1287, 1434,  629,  872,   84,  316,  216,  295,  398,
 /*    40 */   419,  441,
};
#define YY_REDUCE_USE_DFLT (-145)
#define YY_REDUCE_COUNT (25)
#define YY_REDUCE_MIN   (-144)
#define YY_REDUCE_MAX   (1439)
static const short yy_reduce_ofst[] = {
 /*     0 */   938,  818,  983, 1022, 1067, 1106, 1211, 1250, 1330, 1370,
 /*    10 */  1413, 1370,  182,  285,  385, 1439,  572,  719, 1122, 1232,
 /*    20 */  1279, 1347, -144, -143, -142,  225,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   714,  519,  714,  541,  714,  563,  585,  714,  714,  714,
 /*    10 */   714,  714,  610,  714,  714,  714,  714,  610,  714,  714,
 /*    20 */   714,  714,  622,  622,  714,  714,  506,  586,  586,  714,
 /*    30 */   586,  714,  714,  714,  499,  611,  586,  630,  611,  714,
 /*    40 */   714,  714,
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
  "HALVES",        "AND",           "AN",            "DECIMAL",     
  "ZERO",          "TRILLION",      "TRILLIONTH",    "TRILLIONTHS", 
  "WHOLE_NUMBER",  "BILLION",       "BILLIONTH",     "BILLIONTHS",  
  "MILLION",       "MILLIONTH",     "MILLIONTHS",    "THOUSAND",    
  "THOUSANDTH",    "THOUSANDTHS",   "HUNDRED",       "HUNDREDTH",   
  "HUNDREDTHS",    "TWO",           "THREE",         "FOUR",        
  "FIVE",          "SIX",           "SEVEN",         "EIGHT",       
  "NINE",          "TEN",           "ELEVEN",        "TWELVE",      
  "THIRTEEN",      "FOURTEEN",      "FIFTEEN",       "SIXTEEN",     
  "SEVENTEEN",     "EIGHTEEN",      "NINETEEN",      "TWENTY",      
  "THIRTY",        "FORTY",         "FIFTY",         "SIXTY",       
  "SEVENTY",       "EIGHTY",        "NINETY",        "FIRST",       
  "SECOND",        "THIRD",         "FOURTH",        "FIFTH",       
  "SIXTH",         "SEVENTH",       "EIGHTH",        "NINTH",       
  "TENTH",         "ELEVENTH",      "TWELFTH",       "THIRTEENTH",  
  "FOURTEENTH",    "FIFTEENTH",     "SIXTEENTH",     "SEVENTEENTH", 
  "EIGHTEENTH",    "NINETEENTH",    "TWENTIETH",     "THIRTIETH",   
  "FOURTIETH",     "FIFTIETH",      "SIXTIETH",      "SEVENTIETH",  
  "EIGHTIETH",     "NINETIETH",     "FIRSTS",        "SECONDS",     
  "THIRDS",        "FOURTHS",       "FIFTHS",        "SIXTHS",      
  "SEVENTHS",      "EIGHTHS",       "NINTHS",        "TENTHS",      
  "ELEVENTHS",     "TWELFTHS",      "THIRTEENTHS",   "FOURTEENTHS", 
  "FIFTEENTHS",    "SIXTEENTHS",    "SEVENTEENTHS",  "EIGHTEENTHS", 
  "NINETEENTHS",   "TWENTIETHS",    "THIRTIETHS",    "FOURTIETHS",  
  "FIFTIETHS",     "SIXTIETHS",     "SEVENTIETHS",   "EIGHTIETHS",  
  "NINETIETHS",    "error",         "program",       "numbers",     
  "number",        "final_number",  "less_than_quadrillion",  "less_than_quadrillionth",
  "less_than_quadrillionths",  "fraction",      "less_than_thousand",  "less_than_trillionth_end_only",
  "less_than_trillionth",  "less_than_trillionths_end_only",  "less_than_trillionths",  "less_than_trillion_end_only",
  "less_than_trillion",  "less_than_billionth_end_only",  "less_than_billionth",  "less_than_billionths_end_only",
  "less_than_billionths",  "less_than_billion_end_only",  "less_than_billion",  "less_than_millionth_end_only",
  "less_than_millionth",  "less_than_millionths_end_only",  "less_than_millionths",  "less_than_million_end_only",
  "less_than_million",  "less_than_thousandth_end_only",  "less_than_thousandth",  "less_than_thousandths_end_only",
  "less_than_thousandths",  "less_than_thousand_end_only",  "less_than_hundredth",  "less_than_hundred",
  "less_than_hundredths",  "tens",          "less_than_tenth",  "tenth",       
  "less_than_twentieth",  "less_than_tenths",  "tenths",        "less_than_twentieths",
  "less_than_ten",  "less_than_twenty",  "tenth_to_19th",  "tenths_to_19ths",
  "ten_to_19",   
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
 /*  17 */ "final_number ::= DECIMAL",
 /*  18 */ "final_number ::= ZERO",
 /*  19 */ "less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only",
 /*  20 */ "less_than_quadrillionth ::= less_than_thousand TRILLIONTH",
 /*  21 */ "less_than_quadrillionth ::= less_than_trillionth",
 /*  22 */ "final_number ::= TRILLIONTH",
 /*  23 */ "final_number ::= DECIMAL TRILLIONTH",
 /*  24 */ "less_than_quadrillionths ::= less_than_thousand TRILLION less_than_trillionths_end_only",
 /*  25 */ "less_than_quadrillionths ::= less_than_thousand TRILLIONTHS",
 /*  26 */ "less_than_quadrillionths ::= less_than_trillionths",
 /*  27 */ "final_number ::= TRILLIONTHS",
 /*  28 */ "final_number ::= DECIMAL TRILLIONTHS",
 /*  29 */ "less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only",
 /*  30 */ "less_than_quadrillion ::= less_than_thousand TRILLION",
 /*  31 */ "less_than_quadrillion ::= less_than_trillion",
 /*  32 */ "final_number ::= TRILLION",
 /*  33 */ "final_number ::= DECIMAL TRILLION",
 /*  34 */ "final_number ::= WHOLE_NUMBER TRILLION",
 /*  35 */ "less_than_trillionth_end_only ::= less_than_trillionth",
 /*  36 */ "less_than_trillionth_end_only ::= less_than_billionth_end_only",
 /*  37 */ "less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only",
 /*  38 */ "less_than_trillionth ::= less_than_thousand BILLIONTH",
 /*  39 */ "less_than_trillionth ::= less_than_billionth",
 /*  40 */ "final_number ::= BILLIONTH",
 /*  41 */ "final_number ::= DECIMAL BILLIONTH",
 /*  42 */ "less_than_trillionths_end_only ::= less_than_trillionths",
 /*  43 */ "less_than_trillionths_end_only ::= less_than_billionths_end_only",
 /*  44 */ "less_than_trillionths ::= less_than_thousand BILLION less_than_billionths_end_only",
 /*  45 */ "less_than_trillionths ::= less_than_thousand BILLIONTHS",
 /*  46 */ "less_than_trillionths ::= less_than_billionths",
 /*  47 */ "final_number ::= BILLIONTHS",
 /*  48 */ "final_number ::= DECIMAL BILLIONTHS",
 /*  49 */ "less_than_trillion_end_only ::= less_than_trillion",
 /*  50 */ "less_than_trillion_end_only ::= less_than_billion_end_only",
 /*  51 */ "less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only",
 /*  52 */ "less_than_trillion ::= less_than_thousand BILLION",
 /*  53 */ "less_than_trillion ::= less_than_billion",
 /*  54 */ "final_number ::= BILLION",
 /*  55 */ "final_number ::= DECIMAL BILLION",
 /*  56 */ "final_number ::= WHOLE_NUMBER BILLION",
 /*  57 */ "less_than_billionth_end_only ::= less_than_billionth",
 /*  58 */ "less_than_billionth_end_only ::= less_than_millionth_end_only",
 /*  59 */ "less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only",
 /*  60 */ "less_than_billionth ::= less_than_thousand MILLIONTH",
 /*  61 */ "less_than_billionth ::= less_than_millionth",
 /*  62 */ "final_number ::= MILLIONTH",
 /*  63 */ "final_number ::= DECIMAL MILLIONTH",
 /*  64 */ "less_than_billionths_end_only ::= less_than_billionths",
 /*  65 */ "less_than_billionths_end_only ::= less_than_millionths_end_only",
 /*  66 */ "less_than_billionths ::= less_than_thousand MILLION less_than_millionths_end_only",
 /*  67 */ "less_than_billionths ::= less_than_thousand MILLIONTHS",
 /*  68 */ "less_than_billionths ::= less_than_millionths",
 /*  69 */ "final_number ::= MILLIONTHS",
 /*  70 */ "final_number ::= DECIMAL MILLIONTHS",
 /*  71 */ "less_than_billion_end_only ::= less_than_billion",
 /*  72 */ "less_than_billion_end_only ::= less_than_million_end_only",
 /*  73 */ "less_than_billion ::= less_than_thousand MILLION less_than_million_end_only",
 /*  74 */ "less_than_billion ::= less_than_thousand MILLION",
 /*  75 */ "less_than_billion ::= less_than_million",
 /*  76 */ "final_number ::= MILLION",
 /*  77 */ "final_number ::= DECIMAL MILLION",
 /*  78 */ "final_number ::= WHOLE_NUMBER MILLION",
 /*  79 */ "less_than_millionth_end_only ::= less_than_millionth",
 /*  80 */ "less_than_millionth_end_only ::= less_than_thousandth_end_only",
 /*  81 */ "less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only",
 /*  82 */ "less_than_millionth ::= less_than_thousand THOUSANDTH",
 /*  83 */ "less_than_millionth ::= less_than_thousandth",
 /*  84 */ "final_number ::= THOUSANDTH",
 /*  85 */ "final_number ::= DECIMAL THOUSANDTH",
 /*  86 */ "less_than_millionths_end_only ::= less_than_millionths",
 /*  87 */ "less_than_millionths_end_only ::= less_than_thousandths_end_only",
 /*  88 */ "less_than_millionths ::= less_than_thousand THOUSAND less_than_thousandths_end_only",
 /*  89 */ "less_than_millionths ::= less_than_thousand THOUSANDTHS",
 /*  90 */ "less_than_millionths ::= less_than_thousandths",
 /*  91 */ "final_number ::= THOUSANDTHS",
 /*  92 */ "final_number ::= DECIMAL THOUSANDTHS",
 /*  93 */ "less_than_million_end_only ::= less_than_million",
 /*  94 */ "less_than_million_end_only ::= less_than_thousand_end_only",
 /*  95 */ "less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only",
 /*  96 */ "less_than_million ::= less_than_thousand THOUSAND",
 /*  97 */ "less_than_million ::= less_than_thousand",
 /*  98 */ "final_number ::= THOUSAND",
 /*  99 */ "final_number ::= DECIMAL THOUSAND",
 /* 100 */ "final_number ::= WHOLE_NUMBER THOUSAND",
 /* 101 */ "less_than_thousandth_end_only ::= AND less_than_hundredth",
 /* 102 */ "less_than_thousandth_end_only ::= less_than_thousandth",
 /* 103 */ "less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth",
 /* 104 */ "less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth",
 /* 105 */ "less_than_thousandth ::= less_than_hundred HUNDREDTH",
 /* 106 */ "less_than_thousandth ::= less_than_hundredth",
 /* 107 */ "final_number ::= HUNDREDTH",
 /* 108 */ "final_number ::= DECIMAL HUNDREDTH",
 /* 109 */ "less_than_thousandths_end_only ::= AND less_than_hundredths",
 /* 110 */ "less_than_thousandths_end_only ::= less_than_thousandths",
 /* 111 */ "less_than_thousandths ::= less_than_hundred HUNDRED AND less_than_hundredths",
 /* 112 */ "less_than_thousandths ::= less_than_hundred HUNDRED less_than_hundredths",
 /* 113 */ "less_than_thousandths ::= less_than_hundred HUNDREDTHS",
 /* 114 */ "less_than_thousandths ::= less_than_hundredths",
 /* 115 */ "final_number ::= HUNDREDTHS",
 /* 116 */ "final_number ::= DECIMAL HUNDREDTHS",
 /* 117 */ "less_than_thousand_end_only ::= AND less_than_hundred",
 /* 118 */ "less_than_thousand_end_only ::= less_than_thousand",
 /* 119 */ "less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred",
 /* 120 */ "less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred",
 /* 121 */ "less_than_thousand ::= less_than_hundred HUNDRED",
 /* 122 */ "less_than_thousand ::= less_than_hundred",
 /* 123 */ "final_number ::= HUNDRED",
 /* 124 */ "final_number ::= DECIMAL HUNDRED",
 /* 125 */ "final_number ::= WHOLE_NUMBER HUNDRED",
 /* 126 */ "less_than_hundredth ::= tens less_than_tenth",
 /* 127 */ "less_than_hundredth ::= tenth",
 /* 128 */ "less_than_hundredth ::= less_than_twentieth",
 /* 129 */ "less_than_hundredths ::= tens less_than_tenths",
 /* 130 */ "less_than_hundredths ::= tenths",
 /* 131 */ "less_than_hundredths ::= less_than_twentieths",
 /* 132 */ "less_than_hundred ::= tens less_than_ten",
 /* 133 */ "less_than_hundred ::= tens",
 /* 134 */ "less_than_hundred ::= less_than_twenty",
 /* 135 */ "less_than_twentieth ::= tenth_to_19th",
 /* 136 */ "less_than_twentieth ::= less_than_tenth",
 /* 137 */ "less_than_twentieths ::= tenths_to_19ths",
 /* 138 */ "less_than_twentieths ::= less_than_tenths",
 /* 139 */ "less_than_twenty ::= ten_to_19",
 /* 140 */ "less_than_twenty ::= less_than_ten",
 /* 141 */ "less_than_ten ::= ONE",
 /* 142 */ "less_than_ten ::= TWO",
 /* 143 */ "less_than_ten ::= THREE",
 /* 144 */ "less_than_ten ::= FOUR",
 /* 145 */ "less_than_ten ::= FIVE",
 /* 146 */ "less_than_ten ::= SIX",
 /* 147 */ "less_than_ten ::= SEVEN",
 /* 148 */ "less_than_ten ::= EIGHT",
 /* 149 */ "less_than_ten ::= NINE",
 /* 150 */ "ten_to_19 ::= TEN",
 /* 151 */ "ten_to_19 ::= ELEVEN",
 /* 152 */ "ten_to_19 ::= TWELVE",
 /* 153 */ "ten_to_19 ::= THIRTEEN",
 /* 154 */ "ten_to_19 ::= FOURTEEN",
 /* 155 */ "ten_to_19 ::= FIFTEEN",
 /* 156 */ "ten_to_19 ::= SIXTEEN",
 /* 157 */ "ten_to_19 ::= SEVENTEEN",
 /* 158 */ "ten_to_19 ::= EIGHTEEN",
 /* 159 */ "ten_to_19 ::= NINETEEN",
 /* 160 */ "tens ::= TWENTY",
 /* 161 */ "tens ::= THIRTY",
 /* 162 */ "tens ::= FORTY",
 /* 163 */ "tens ::= FIFTY",
 /* 164 */ "tens ::= SIXTY",
 /* 165 */ "tens ::= SEVENTY",
 /* 166 */ "tens ::= EIGHTY",
 /* 167 */ "tens ::= NINETY",
 /* 168 */ "less_than_tenth ::= FIRST",
 /* 169 */ "less_than_tenth ::= SECOND",
 /* 170 */ "less_than_tenth ::= THIRD",
 /* 171 */ "less_than_tenth ::= FOURTH",
 /* 172 */ "less_than_tenth ::= FIFTH",
 /* 173 */ "less_than_tenth ::= SIXTH",
 /* 174 */ "less_than_tenth ::= SEVENTH",
 /* 175 */ "less_than_tenth ::= EIGHTH",
 /* 176 */ "less_than_tenth ::= NINTH",
 /* 177 */ "tenth_to_19th ::= TENTH",
 /* 178 */ "tenth_to_19th ::= ELEVENTH",
 /* 179 */ "tenth_to_19th ::= TWELFTH",
 /* 180 */ "tenth_to_19th ::= THIRTEENTH",
 /* 181 */ "tenth_to_19th ::= FOURTEENTH",
 /* 182 */ "tenth_to_19th ::= FIFTEENTH",
 /* 183 */ "tenth_to_19th ::= SIXTEENTH",
 /* 184 */ "tenth_to_19th ::= SEVENTEENTH",
 /* 185 */ "tenth_to_19th ::= EIGHTEENTH",
 /* 186 */ "tenth_to_19th ::= NINETEENTH",
 /* 187 */ "tenth ::= TWENTIETH",
 /* 188 */ "tenth ::= THIRTIETH",
 /* 189 */ "tenth ::= FOURTIETH",
 /* 190 */ "tenth ::= FIFTIETH",
 /* 191 */ "tenth ::= SIXTIETH",
 /* 192 */ "tenth ::= SEVENTIETH",
 /* 193 */ "tenth ::= EIGHTIETH",
 /* 194 */ "tenth ::= NINETIETH",
 /* 195 */ "less_than_tenths ::= FIRSTS",
 /* 196 */ "less_than_tenths ::= SECONDS",
 /* 197 */ "less_than_tenths ::= THIRDS",
 /* 198 */ "less_than_tenths ::= FOURTHS",
 /* 199 */ "less_than_tenths ::= FIFTHS",
 /* 200 */ "less_than_tenths ::= SIXTHS",
 /* 201 */ "less_than_tenths ::= SEVENTHS",
 /* 202 */ "less_than_tenths ::= EIGHTHS",
 /* 203 */ "less_than_tenths ::= NINTHS",
 /* 204 */ "tenths_to_19ths ::= TENTHS",
 /* 205 */ "tenths_to_19ths ::= ELEVENTHS",
 /* 206 */ "tenths_to_19ths ::= TWELFTHS",
 /* 207 */ "tenths_to_19ths ::= THIRTEENTHS",
 /* 208 */ "tenths_to_19ths ::= FOURTEENTHS",
 /* 209 */ "tenths_to_19ths ::= FIFTEENTHS",
 /* 210 */ "tenths_to_19ths ::= SIXTEENTHS",
 /* 211 */ "tenths_to_19ths ::= SEVENTEENTHS",
 /* 212 */ "tenths_to_19ths ::= EIGHTEENTHS",
 /* 213 */ "tenths_to_19ths ::= NINETEENTHS",
 /* 214 */ "tenths ::= TWENTIETHS",
 /* 215 */ "tenths ::= THIRTIETHS",
 /* 216 */ "tenths ::= FOURTIETHS",
 /* 217 */ "tenths ::= FIFTIETHS",
 /* 218 */ "tenths ::= SIXTIETHS",
 /* 219 */ "tenths ::= SEVENTIETHS",
 /* 220 */ "tenths ::= EIGHTIETHS",
 /* 221 */ "tenths ::= NINETIETHS",
 /* 222 */ "program ::= numbers",
 /* 223 */ "numbers ::= numbers number",
 /* 224 */ "numbers ::= number",
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
  { 112, 1 },
  { 112, 2 },
  { 113, 3 },
  { 113, 3 },
  { 113, 2 },
  { 113, 2 },
  { 113, 2 },
  { 113, 2 },
  { 113, 2 },
  { 113, 2 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 3 },
  { 113, 1 },
  { 117, 2 },
  { 117, 2 },
  { 113, 1 },
  { 113, 1 },
  { 115, 3 },
  { 115, 2 },
  { 115, 1 },
  { 113, 1 },
  { 113, 2 },
  { 116, 3 },
  { 116, 2 },
  { 116, 1 },
  { 113, 1 },
  { 113, 2 },
  { 114, 3 },
  { 114, 2 },
  { 114, 1 },
  { 113, 1 },
  { 113, 2 },
  { 113, 2 },
  { 119, 1 },
  { 119, 1 },
  { 120, 3 },
  { 120, 2 },
  { 120, 1 },
  { 113, 1 },
  { 113, 2 },
  { 121, 1 },
  { 121, 1 },
  { 122, 3 },
  { 122, 2 },
  { 122, 1 },
  { 113, 1 },
  { 113, 2 },
  { 123, 1 },
  { 123, 1 },
  { 124, 3 },
  { 124, 2 },
  { 124, 1 },
  { 113, 1 },
  { 113, 2 },
  { 113, 2 },
  { 125, 1 },
  { 125, 1 },
  { 126, 3 },
  { 126, 2 },
  { 126, 1 },
  { 113, 1 },
  { 113, 2 },
  { 127, 1 },
  { 127, 1 },
  { 128, 3 },
  { 128, 2 },
  { 128, 1 },
  { 113, 1 },
  { 113, 2 },
  { 129, 1 },
  { 129, 1 },
  { 130, 3 },
  { 130, 2 },
  { 130, 1 },
  { 113, 1 },
  { 113, 2 },
  { 113, 2 },
  { 131, 1 },
  { 131, 1 },
  { 132, 3 },
  { 132, 2 },
  { 132, 1 },
  { 113, 1 },
  { 113, 2 },
  { 133, 1 },
  { 133, 1 },
  { 134, 3 },
  { 134, 2 },
  { 134, 1 },
  { 113, 1 },
  { 113, 2 },
  { 135, 1 },
  { 135, 1 },
  { 136, 3 },
  { 136, 2 },
  { 136, 1 },
  { 113, 1 },
  { 113, 2 },
  { 113, 2 },
  { 137, 2 },
  { 137, 1 },
  { 138, 4 },
  { 138, 3 },
  { 138, 2 },
  { 138, 1 },
  { 113, 1 },
  { 113, 2 },
  { 139, 2 },
  { 139, 1 },
  { 140, 4 },
  { 140, 3 },
  { 140, 2 },
  { 140, 1 },
  { 113, 1 },
  { 113, 2 },
  { 141, 2 },
  { 141, 1 },
  { 118, 4 },
  { 118, 3 },
  { 118, 2 },
  { 118, 1 },
  { 113, 1 },
  { 113, 2 },
  { 113, 2 },
  { 142, 2 },
  { 142, 1 },
  { 142, 1 },
  { 144, 2 },
  { 144, 1 },
  { 144, 1 },
  { 143, 2 },
  { 143, 1 },
  { 143, 1 },
  { 148, 1 },
  { 148, 1 },
  { 151, 1 },
  { 151, 1 },
  { 153, 1 },
  { 153, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
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
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
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
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 110, 1 },
  { 111, 2 },
  { 111, 1 },
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
#line 196 "parser.yy"
{
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1845 "parser.c"
        break;
      case 1: /* number ::= NEGATIVE final_number */
#line 199 "parser.yy"
{
    yymsp[0].minor.yy0.dbl = -yymsp[0].minor.yy0.dbl;
    yymsp[0].minor.yy0.begin = yymsp[-1].minor.yy0.begin;
    yymsp[0].minor.yy0.end = yymsp[0].minor.yy0.end;
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1855 "parser.c"
        break;
      case 2: /* final_number ::= less_than_quadrillion AND_A QUARTER */
#line 206 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, 1.0, 4.0); }
#line 1860 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* final_number ::= less_than_quadrillion AND_A HALF */
#line 207 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0); }
#line 1866 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 4: /* final_number ::= less_than_quadrillion QUARTERS */
#line 209 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, yymsp[-1].minor.yy0.dbl, 4.0); }
#line 1872 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* final_number ::= ONE QUARTER */
      case 6: /* final_number ::= A QUARTER */ yytestcase(yyruleno==6);
#line 210 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 4.0); }
#line 1879 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* final_number ::= less_than_quadrillion HALVES */
#line 213 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, yymsp[-1].minor.yy0.dbl, 2.0); }
#line 1885 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 8: /* final_number ::= ONE HALF */
#line 214 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0);  }
#line 1891 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* final_number ::= A HALF */
#line 215 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0); }
#line 1897 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 10: /* final_number ::= less_than_quadrillion */
      case 31: /* less_than_quadrillion ::= less_than_trillion */ yytestcase(yyruleno==31);
      case 49: /* less_than_trillion_end_only ::= less_than_trillion */ yytestcase(yyruleno==49);
      case 50: /* less_than_trillion_end_only ::= less_than_billion_end_only */ yytestcase(yyruleno==50);
      case 53: /* less_than_trillion ::= less_than_billion */ yytestcase(yyruleno==53);
      case 71: /* less_than_billion_end_only ::= less_than_billion */ yytestcase(yyruleno==71);
      case 72: /* less_than_billion_end_only ::= less_than_million_end_only */ yytestcase(yyruleno==72);
      case 75: /* less_than_billion ::= less_than_million */ yytestcase(yyruleno==75);
      case 93: /* less_than_million_end_only ::= less_than_million */ yytestcase(yyruleno==93);
      case 94: /* less_than_million_end_only ::= less_than_thousand_end_only */ yytestcase(yyruleno==94);
      case 97: /* less_than_million ::= less_than_thousand */ yytestcase(yyruleno==97);
      case 118: /* less_than_thousand_end_only ::= less_than_thousand */ yytestcase(yyruleno==118);
      case 122: /* less_than_thousand ::= less_than_hundred */ yytestcase(yyruleno==122);
      case 133: /* less_than_hundred ::= tens */ yytestcase(yyruleno==133);
      case 134: /* less_than_hundred ::= less_than_twenty */ yytestcase(yyruleno==134);
      case 139: /* less_than_twenty ::= ten_to_19 */ yytestcase(yyruleno==139);
      case 140: /* less_than_twenty ::= less_than_ten */ yytestcase(yyruleno==140);
#line 217 "parser.yy"
{ COPY_YYSTYPE_BE_DBL(yylhsminor.yy0, yymsp[0].minor.yy0); }
#line 1919 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 11: /* final_number ::= less_than_quadrillionth */
      case 12: /* final_number ::= less_than_quadrillionths */ yytestcase(yyruleno==12);
      case 21: /* less_than_quadrillionth ::= less_than_trillionth */ yytestcase(yyruleno==21);
      case 26: /* less_than_quadrillionths ::= less_than_trillionths */ yytestcase(yyruleno==26);
      case 35: /* less_than_trillionth_end_only ::= less_than_trillionth */ yytestcase(yyruleno==35);
      case 36: /* less_than_trillionth_end_only ::= less_than_billionth_end_only */ yytestcase(yyruleno==36);
      case 39: /* less_than_trillionth ::= less_than_billionth */ yytestcase(yyruleno==39);
      case 42: /* less_than_trillionths_end_only ::= less_than_trillionths */ yytestcase(yyruleno==42);
      case 43: /* less_than_trillionths_end_only ::= less_than_billionths_end_only */ yytestcase(yyruleno==43);
      case 46: /* less_than_trillionths ::= less_than_billionths */ yytestcase(yyruleno==46);
      case 57: /* less_than_billionth_end_only ::= less_than_billionth */ yytestcase(yyruleno==57);
      case 58: /* less_than_billionth_end_only ::= less_than_millionth_end_only */ yytestcase(yyruleno==58);
      case 61: /* less_than_billionth ::= less_than_millionth */ yytestcase(yyruleno==61);
      case 64: /* less_than_billionths_end_only ::= less_than_billionths */ yytestcase(yyruleno==64);
      case 65: /* less_than_billionths_end_only ::= less_than_millionths_end_only */ yytestcase(yyruleno==65);
      case 68: /* less_than_billionths ::= less_than_millionths */ yytestcase(yyruleno==68);
      case 79: /* less_than_millionth_end_only ::= less_than_millionth */ yytestcase(yyruleno==79);
      case 80: /* less_than_millionth_end_only ::= less_than_thousandth_end_only */ yytestcase(yyruleno==80);
      case 83: /* less_than_millionth ::= less_than_thousandth */ yytestcase(yyruleno==83);
      case 86: /* less_than_millionths_end_only ::= less_than_millionths */ yytestcase(yyruleno==86);
      case 87: /* less_than_millionths_end_only ::= less_than_thousandths_end_only */ yytestcase(yyruleno==87);
      case 90: /* less_than_millionths ::= less_than_thousandths */ yytestcase(yyruleno==90);
      case 102: /* less_than_thousandth_end_only ::= less_than_thousandth */ yytestcase(yyruleno==102);
      case 106: /* less_than_thousandth ::= less_than_hundredth */ yytestcase(yyruleno==106);
      case 110: /* less_than_thousandths_end_only ::= less_than_thousandths */ yytestcase(yyruleno==110);
      case 114: /* less_than_thousandths ::= less_than_hundredths */ yytestcase(yyruleno==114);
      case 127: /* less_than_hundredth ::= tenth */ yytestcase(yyruleno==127);
      case 128: /* less_than_hundredth ::= less_than_twentieth */ yytestcase(yyruleno==128);
      case 130: /* less_than_hundredths ::= tenths */ yytestcase(yyruleno==130);
      case 131: /* less_than_hundredths ::= less_than_twentieths */ yytestcase(yyruleno==131);
      case 135: /* less_than_twentieth ::= tenth_to_19th */ yytestcase(yyruleno==135);
      case 136: /* less_than_twentieth ::= less_than_tenth */ yytestcase(yyruleno==136);
      case 137: /* less_than_twentieths ::= tenths_to_19ths */ yytestcase(yyruleno==137);
      case 138: /* less_than_twentieths ::= less_than_tenths */ yytestcase(yyruleno==138);
#line 218 "parser.yy"
{ COPY_YYSTYPE_BE_DBL_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0); }
#line 1958 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 13: /* final_number ::= less_than_quadrillion AND fraction */
#line 221 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, yymsp[0].minor.yy0.frac_num, yymsp[0].minor.yy0.frac_denom); }
#line 1964 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* final_number ::= fraction */
#line 224 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.frac_num = yymsp[0].minor.yy0.frac_num; yylhsminor.yy0.frac_denom = yymsp[0].minor.yy0.frac_denom; yylhsminor.yy0.is_frac = yymsp[0].minor.yy0.is_frac; }
#line 1970 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 15: /* fraction ::= A less_than_quadrillionth */
      case 16: /* fraction ::= AN less_than_quadrillionth */ yytestcase(yyruleno==16);
#line 236 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, yymsp[0].minor.yy0.dbl); }
#line 1977 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 17: /* final_number ::= DECIMAL */
#line 242 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = true; }
#line 1983 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 18: /* final_number ::= ZERO */
#line 243 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 0.0); }
#line 1989 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 19: /* less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only */
      case 24: /* less_than_quadrillionths ::= less_than_thousand TRILLION less_than_trillionths_end_only */ yytestcase(yyruleno==24);
#line 249 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1996 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 20: /* less_than_quadrillionth ::= less_than_thousand TRILLIONTH */
#line 250 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 2002 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 22: /* final_number ::= TRILLIONTH */
#line 253 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 2008 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 23: /* final_number ::= DECIMAL TRILLIONTH */
#line 254 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 2014 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* less_than_quadrillionths ::= less_than_thousand TRILLIONTHS */
#line 259 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 2020 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 27: /* final_number ::= TRILLIONTHS */
#line 262 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 2026 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 28: /* final_number ::= DECIMAL TRILLIONTHS */
#line 263 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 2032 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only */
#line 269 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2038 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 30: /* less_than_quadrillion ::= less_than_thousand TRILLION */
      case 34: /* final_number ::= WHOLE_NUMBER TRILLION */ yytestcase(yyruleno==34);
#line 270 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2045 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* final_number ::= TRILLION */
#line 273 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2051 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 33: /* final_number ::= DECIMAL TRILLION */
#line 274 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 2057 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 37: /* less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only */
      case 44: /* less_than_trillionths ::= less_than_thousand BILLION less_than_billionths_end_only */ yytestcase(yyruleno==44);
#line 284 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2064 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* less_than_trillionth ::= less_than_thousand BILLIONTH */
#line 285 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 2070 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 40: /* final_number ::= BILLIONTH */
#line 288 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 2076 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 41: /* final_number ::= DECIMAL BILLIONTH */
#line 289 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 2082 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 45: /* less_than_trillionths ::= less_than_thousand BILLIONTHS */
#line 297 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 2088 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 47: /* final_number ::= BILLIONTHS */
#line 300 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 2094 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 48: /* final_number ::= DECIMAL BILLIONTHS */
#line 301 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 2100 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 51: /* less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only */
#line 310 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2106 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 52: /* less_than_trillion ::= less_than_thousand BILLION */
      case 56: /* final_number ::= WHOLE_NUMBER BILLION */ yytestcase(yyruleno==56);
#line 311 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2113 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 54: /* final_number ::= BILLION */
#line 314 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2119 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 55: /* final_number ::= DECIMAL BILLION */
#line 315 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 2125 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 59: /* less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only */
      case 66: /* less_than_billionths ::= less_than_thousand MILLION less_than_millionths_end_only */ yytestcase(yyruleno==66);
#line 325 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2132 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 60: /* less_than_billionth ::= less_than_thousand MILLIONTH */
#line 326 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2138 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 62: /* final_number ::= MILLIONTH */
#line 329 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2144 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 63: /* final_number ::= DECIMAL MILLIONTH */
#line 330 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2150 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 67: /* less_than_billionths ::= less_than_thousand MILLIONTHS */
#line 338 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2156 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 69: /* final_number ::= MILLIONTHS */
#line 341 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2162 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 70: /* final_number ::= DECIMAL MILLIONTHS */
#line 342 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2168 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 73: /* less_than_billion ::= less_than_thousand MILLION less_than_million_end_only */
#line 351 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2174 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 74: /* less_than_billion ::= less_than_thousand MILLION */
      case 78: /* final_number ::= WHOLE_NUMBER MILLION */ yytestcase(yyruleno==78);
#line 352 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2181 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 76: /* final_number ::= MILLION */
#line 355 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2187 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 77: /* final_number ::= DECIMAL MILLION */
#line 356 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2193 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 81: /* less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only */
      case 88: /* less_than_millionths ::= less_than_thousand THOUSAND less_than_thousandths_end_only */ yytestcase(yyruleno==88);
#line 366 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2200 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 82: /* less_than_millionth ::= less_than_thousand THOUSANDTH */
#line 367 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2206 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 84: /* final_number ::= THOUSANDTH */
#line 370 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2212 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 85: /* final_number ::= DECIMAL THOUSANDTH */
#line 371 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2218 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 89: /* less_than_millionths ::= less_than_thousand THOUSANDTHS */
#line 379 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2224 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 91: /* final_number ::= THOUSANDTHS */
#line 382 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2230 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 92: /* final_number ::= DECIMAL THOUSANDTHS */
#line 383 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2236 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 95: /* less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only */
#line 392 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2242 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 96: /* less_than_million ::= less_than_thousand THOUSAND */
      case 100: /* final_number ::= WHOLE_NUMBER THOUSAND */ yytestcase(yyruleno==100);
#line 393 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2249 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 98: /* final_number ::= THOUSAND */
#line 396 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2255 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 99: /* final_number ::= DECIMAL THOUSAND */
#line 397 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2261 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 101: /* less_than_thousandth_end_only ::= AND less_than_hundredth */
      case 109: /* less_than_thousandths_end_only ::= AND less_than_hundredths */ yytestcase(yyruleno==109);
#line 404 "parser.yy"
{ COPY_YYSTYPE_BE_DBL_SUFF(yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2268 "parser.c"
        break;
      case 103: /* less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth */
      case 111: /* less_than_thousandths ::= less_than_hundred HUNDRED AND less_than_hundredths */ yytestcase(yyruleno==111);
#line 407 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2274 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 104: /* less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth */
      case 112: /* less_than_thousandths ::= less_than_hundred HUNDRED less_than_hundredths */ yytestcase(yyruleno==112);
#line 408 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2281 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 105: /* less_than_thousandth ::= less_than_hundred HUNDREDTH */
#line 409 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2287 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 107: /* final_number ::= HUNDREDTH */
#line 412 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2293 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 108: /* final_number ::= DECIMAL HUNDREDTH */
#line 413 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2299 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 113: /* less_than_thousandths ::= less_than_hundred HUNDREDTHS */
#line 422 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2305 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 115: /* final_number ::= HUNDREDTHS */
#line 425 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2311 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 116: /* final_number ::= DECIMAL HUNDREDTHS */
#line 427 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2317 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 117: /* less_than_thousand_end_only ::= AND less_than_hundred */
#line 433 "parser.yy"
{ COPY_YYSTYPE_BE_DBL(yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2323 "parser.c"
        break;
      case 119: /* less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred */
#line 436 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2328 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 120: /* less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred */
#line 437 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2334 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 121: /* less_than_thousand ::= less_than_hundred HUNDRED */
      case 125: /* final_number ::= WHOLE_NUMBER HUNDRED */ yytestcase(yyruleno==125);
#line 438 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2341 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 123: /* final_number ::= HUNDRED */
#line 441 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2347 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 124: /* final_number ::= DECIMAL HUNDRED */
#line 442 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2353 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 126: /* less_than_hundredth ::= tens less_than_tenth */
      case 129: /* less_than_hundredths ::= tens less_than_tenths */ yytestcase(yyruleno==129);
#line 449 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2360 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 132: /* less_than_hundred ::= tens less_than_ten */
#line 461 "parser.yy"
{ COPY_YYSTYPE_BE_ADD(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2366 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 141: /* less_than_ten ::= ONE */
#line 486 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0); }
#line 2372 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 142: /* less_than_ten ::= TWO */
#line 487 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0); }
#line 2378 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 143: /* less_than_ten ::= THREE */
#line 488 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0); }
#line 2384 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 144: /* less_than_ten ::= FOUR */
#line 489 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0); }
#line 2390 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 145: /* less_than_ten ::= FIVE */
#line 490 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0); }
#line 2396 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 146: /* less_than_ten ::= SIX */
#line 491 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0); }
#line 2402 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 147: /* less_than_ten ::= SEVEN */
#line 492 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0); }
#line 2408 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 148: /* less_than_ten ::= EIGHT */
#line 493 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0); }
#line 2414 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 149: /* less_than_ten ::= NINE */
#line 494 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0); }
#line 2420 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 150: /* ten_to_19 ::= TEN */
#line 496 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0); }
#line 2426 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 151: /* ten_to_19 ::= ELEVEN */
#line 497 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0); }
#line 2432 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 152: /* ten_to_19 ::= TWELVE */
#line 498 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0); }
#line 2438 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 153: /* ten_to_19 ::= THIRTEEN */
#line 499 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0); }
#line 2444 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 154: /* ten_to_19 ::= FOURTEEN */
#line 500 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0); }
#line 2450 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 155: /* ten_to_19 ::= FIFTEEN */
#line 501 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0); }
#line 2456 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 156: /* ten_to_19 ::= SIXTEEN */
#line 502 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0); }
#line 2462 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 157: /* ten_to_19 ::= SEVENTEEN */
#line 503 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0); }
#line 2468 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 158: /* ten_to_19 ::= EIGHTEEN */
#line 504 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0); }
#line 2474 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 159: /* ten_to_19 ::= NINETEEN */
#line 505 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0); }
#line 2480 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 160: /* tens ::= TWENTY */
#line 507 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0); }
#line 2486 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 161: /* tens ::= THIRTY */
#line 508 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0); }
#line 2492 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 162: /* tens ::= FORTY */
#line 509 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0); }
#line 2498 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 163: /* tens ::= FIFTY */
#line 510 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0); }
#line 2504 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 164: /* tens ::= SIXTY */
#line 511 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0); }
#line 2510 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 165: /* tens ::= SEVENTY */
#line 512 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0); }
#line 2516 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 166: /* tens ::= EIGHTY */
#line 513 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0); }
#line 2522 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 167: /* tens ::= NINETY */
#line 514 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0); }
#line 2528 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 168: /* less_than_tenth ::= FIRST */
#line 516 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0, ST); }
#line 2534 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 169: /* less_than_tenth ::= SECOND */
#line 517 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0, ND); }
#line 2540 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 170: /* less_than_tenth ::= THIRD */
#line 518 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0, RD); }
#line 2546 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 171: /* less_than_tenth ::= FOURTH */
#line 519 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0, TH); }
#line 2552 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 172: /* less_than_tenth ::= FIFTH */
#line 520 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0, TH); }
#line 2558 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 173: /* less_than_tenth ::= SIXTH */
#line 521 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0, TH); }
#line 2564 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 174: /* less_than_tenth ::= SEVENTH */
#line 522 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0, TH); }
#line 2570 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 175: /* less_than_tenth ::= EIGHTH */
#line 523 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0, TH); }
#line 2576 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 176: /* less_than_tenth ::= NINTH */
#line 524 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0, TH); }
#line 2582 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 177: /* tenth_to_19th ::= TENTH */
#line 526 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0, TH); }
#line 2588 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 178: /* tenth_to_19th ::= ELEVENTH */
#line 527 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0, TH); }
#line 2594 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 179: /* tenth_to_19th ::= TWELFTH */
#line 528 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0, TH); }
#line 2600 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 180: /* tenth_to_19th ::= THIRTEENTH */
#line 529 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0, TH); }
#line 2606 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 181: /* tenth_to_19th ::= FOURTEENTH */
#line 530 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0, TH); }
#line 2612 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 182: /* tenth_to_19th ::= FIFTEENTH */
#line 531 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0, TH); }
#line 2618 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 183: /* tenth_to_19th ::= SIXTEENTH */
#line 532 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0, TH); }
#line 2624 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 184: /* tenth_to_19th ::= SEVENTEENTH */
#line 533 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0, TH); }
#line 2630 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 185: /* tenth_to_19th ::= EIGHTEENTH */
#line 534 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0, TH); }
#line 2636 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 186: /* tenth_to_19th ::= NINETEENTH */
#line 535 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0, TH); }
#line 2642 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 187: /* tenth ::= TWENTIETH */
#line 537 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0, TH); }
#line 2648 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 188: /* tenth ::= THIRTIETH */
#line 538 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0, TH); }
#line 2654 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 189: /* tenth ::= FOURTIETH */
#line 539 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0, TH); }
#line 2660 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 190: /* tenth ::= FIFTIETH */
#line 540 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0, TH); }
#line 2666 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 191: /* tenth ::= SIXTIETH */
#line 541 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0, TH); }
#line 2672 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 192: /* tenth ::= SEVENTIETH */
#line 542 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0, TH); }
#line 2678 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 193: /* tenth ::= EIGHTIETH */
#line 543 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0, TH); }
#line 2684 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 194: /* tenth ::= NINETIETH */
#line 544 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0, TH); }
#line 2690 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 195: /* less_than_tenths ::= FIRSTS */
#line 546 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0, STS); }
#line 2696 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 196: /* less_than_tenths ::= SECONDS */
#line 547 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0, NDS); }
#line 2702 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 197: /* less_than_tenths ::= THIRDS */
#line 548 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0, RDS); }
#line 2708 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 198: /* less_than_tenths ::= FOURTHS */
#line 549 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0, THS); }
#line 2714 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 199: /* less_than_tenths ::= FIFTHS */
#line 550 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0, THS); }
#line 2720 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 200: /* less_than_tenths ::= SIXTHS */
#line 551 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0, THS); }
#line 2726 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 201: /* less_than_tenths ::= SEVENTHS */
#line 552 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0, THS); }
#line 2732 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 202: /* less_than_tenths ::= EIGHTHS */
#line 553 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0, THS); }
#line 2738 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 203: /* less_than_tenths ::= NINTHS */
#line 554 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0, THS); }
#line 2744 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 204: /* tenths_to_19ths ::= TENTHS */
#line 556 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0, THS); }
#line 2750 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 205: /* tenths_to_19ths ::= ELEVENTHS */
#line 557 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0, THS); }
#line 2756 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 206: /* tenths_to_19ths ::= TWELFTHS */
#line 558 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0, THS); }
#line 2762 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 207: /* tenths_to_19ths ::= THIRTEENTHS */
#line 559 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0, THS); }
#line 2768 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 208: /* tenths_to_19ths ::= FOURTEENTHS */
#line 560 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0, THS); }
#line 2774 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 209: /* tenths_to_19ths ::= FIFTEENTHS */
#line 561 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0, THS); }
#line 2780 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 210: /* tenths_to_19ths ::= SIXTEENTHS */
#line 562 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0, THS); }
#line 2786 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 211: /* tenths_to_19ths ::= SEVENTEENTHS */
#line 563 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0, THS); }
#line 2792 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 212: /* tenths_to_19ths ::= EIGHTEENTHS */
#line 564 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0, THS); }
#line 2798 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 213: /* tenths_to_19ths ::= NINETEENTHS */
#line 565 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0, THS); }
#line 2804 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 214: /* tenths ::= TWENTIETHS */
#line 567 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0, THS); }
#line 2810 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 215: /* tenths ::= THIRTIETHS */
#line 568 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0, THS); }
#line 2816 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 216: /* tenths ::= FOURTIETHS */
#line 569 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0, THS); }
#line 2822 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 217: /* tenths ::= FIFTIETHS */
#line 570 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0, THS); }
#line 2828 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 218: /* tenths ::= SIXTIETHS */
#line 571 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0, THS); }
#line 2834 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 219: /* tenths ::= SEVENTIETHS */
#line 572 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0, THS); }
#line 2840 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 220: /* tenths ::= EIGHTIETHS */
#line 573 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0, THS); }
#line 2846 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 221: /* tenths ::= NINETIETHS */
#line 574 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0, THS); }
#line 2852 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      default:
      /* (222) program ::= numbers */ yytestcase(yyruleno==222);
      /* (223) numbers ::= numbers number */ yytestcase(yyruleno==223);
      /* (224) numbers ::= number (OPTIMIZED OUT) */ assert(yyruleno!=224);
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
#line 182 "parser.yy"

    state->error = PARSE_FAILURE;
#if print_errors
    fprintf(stderr,"Giving up.  Parser is hopelessly lost...\n");
#endif
#line 2905 "parser.c"
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
#line 175 "parser.yy"

    state->error = SYNTAX_ERROR;
#if print_errors
    fprintf(stderr, "Syntax error\n");
#endif
#line 2928 "parser.c"
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
