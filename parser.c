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
#line 37 "parser.c"
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
#define TOKEN_THIRDS                         83
#define TOKEN_FOURTHS                        84
#define TOKEN_FIFTHS                         85
#define TOKEN_SIXTHS                         86
#define TOKEN_SEVENTHS                       87
#define TOKEN_EIGHTHS                        88
#define TOKEN_NINTHS                         89
#define TOKEN_TENTHS                         90
#define TOKEN_ELEVENTHS                      91
#define TOKEN_TWELFTHS                       92
#define TOKEN_THIRTEENTHS                    93
#define TOKEN_FOURTEENTHS                    94
#define TOKEN_FIFTEENTHS                     95
#define TOKEN_SIXTEENTHS                     96
#define TOKEN_SEVENTEENTHS                   97
#define TOKEN_EIGHTEENTHS                    98
#define TOKEN_NINETEENTHS                    99
#define TOKEN_TWENTIETHS                     100
#define TOKEN_THIRTIETHS                     101
#define TOKEN_FOURTIETHS                     102
#define TOKEN_FIFTIETHS                      103
#define TOKEN_SIXTIETHS                      104
#define TOKEN_SEVENTIETHS                    105
#define TOKEN_EIGHTIETHS                     106
#define TOKEN_NINETIETHS                     107
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
#define YYNOCODE 157
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
#define YYNRULE              224
#define YY_MAX_SHIFT         41
#define YY_MIN_SHIFTREDUCE   263
#define YY_MAX_SHIFTREDUCE   486
#define YY_MIN_REDUCE        487
#define YY_MAX_REDUCE        710
#define YY_ERROR_ACTION      711
#define YY_ACCEPT_ACTION     712
#define YY_NO_ACTION         713
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
#define YY_ACTTAB_COUNT (1587)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   708,    4,  389,  389,  389,  392,   37,    9,  395,  395,
 /*    10 */    10,   26,  281,  295,  285,  290,   32,  317,  303,  310,
 /*    20 */   339,  325,  332,  361,  347,  354,  386,  370,  378,  405,
 /*    30 */   406,  407,  408,  409,  410,  411,  412,  413,  414,  415,
 /*    40 */   416,  417,  418,  419,  420,  421,  422,  423,  424,  425,
 /*    50 */   426,  427,  428,  429,  430,  431,  432,  433,  434,  435,
 /*    60 */   436,  437,  438,  439,  440,  441,  442,  443,  444,  445,
 /*    70 */   446,  447,  448,  449,  450,  451,  452,  453,  454,  455,
 /*    80 */   456,  457,  458,  459,  460,  461,  462,  463,  464,  465,
 /*    90 */   466,  467,  468,  469,  470,  471,  472,  473,  474,  475,
 /*   100 */   476,  477,  478,  479,  480,  481,  482,  483,    4,   12,
 /*   110 */   368,  376,   11,   37,    9,   10,  489,   10,   26,  281,
 /*   120 */   295,  285,  290,   32,  317,  303,  310,  339,  325,  332,
 /*   130 */   361,  347,  354,  386,  370,  378,  405,  406,  407,  408,
 /*   140 */   409,  410,  411,  412,  413,  414,  415,  416,  417,  418,
 /*   150 */   419,  420,  421,  422,  423,  424,  425,  426,  427,  428,
 /*   160 */   429,  430,  431,  432,  433,  434,  435,  436,  437,  438,
 /*   170 */   439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
 /*   180 */   449,  450,  451,  452,  453,  454,  455,  456,  457,  458,
 /*   190 */   459,  460,  461,  462,  463,  464,  465,  466,  467,  468,
 /*   200 */   469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
 /*   210 */   479,  480,  481,  482,  483,   37,    9,  268,  271,   10,
 /*   220 */    26,  281,  295,  285,  290,   32,  317,  303,  310,  339,
 /*   230 */   325,  332,  361,  347,  354,  386,  370,  378,  405,  406,
 /*   240 */   407,  408,  409,  410,  411,  412,  413,  414,  415,  416,
 /*   250 */   417,  418,  419,  420,  421,  422,  423,  424,  425,  426,
 /*   260 */   427,  428,  429,  430,  431,  432,  433,  434,  435,  436,
 /*   270 */   437,  438,  439,  440,  441,  442,  443,  444,  445,  446,
 /*   280 */   447,  448,  449,  450,  451,  452,  453,  454,  455,  456,
 /*   290 */   457,  458,  459,  460,  461,  462,  463,  464,  465,  466,
 /*   300 */   467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
 /*   310 */   477,  478,  479,  480,  481,  482,  483,  404,   17,  368,
 /*   320 */    14,  367,  383,  375,   22,  399,  390,  391,  401,  393,
 /*   330 */   394,  403,  397,  398,  400,  402,    6,  345,  352,  276,
 /*   340 */   405,  406,  407,  408,  409,  410,  411,  412,  413,  414,
 /*   350 */   415,  416,  417,  418,  419,  420,  421,  422,  423,  424,
 /*   360 */   425,  426,  427,  428,  429,  430,  431,  432,  433,  434,
 /*   370 */   435,  436,  437,  438,  439,  440,  441,  442,  443,  444,
 /*   380 */   445,  446,  447,  448,  449,  450,  451,  452,  453,  454,
 /*   390 */   455,  456,  457,  458,  459,  460,  461,  462,  463,  464,
 /*   400 */   465,  466,  467,  468,  469,  470,  471,  472,  473,  474,
 /*   410 */   475,  476,  477,  478,  479,  480,  481,  482,  483,  404,
 /*   420 */    19,  368,   13,  366,  382,  374,   22,  399,  390,  391,
 /*   430 */   401,  393,  394,  403,  397,  398,  400,  402,   16,  345,
 /*   440 */   265,  266,  405,  406,  407,  408,  409,  410,  411,  412,
 /*   450 */   413,  414,  415,  416,  417,  418,  419,  420,  421,  422,
 /*   460 */   423,  424,  425,  426,  427,  428,  429,  430,  431,  432,
 /*   470 */   433,  434,  435,  436,  437,  438,  439,  440,  441,  442,
 /*   480 */   443,  444,  445,  446,  447,  448,  449,  450,  451,  452,
 /*   490 */   453,  454,  455,  456,  457,  458,  459,  460,  461,  462,
 /*   500 */   463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
 /*   510 */   473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
 /*   520 */   483,  404,  364,  380,  372,   22,  399,  390,  391,  401,
 /*   530 */   393,  394,  403,  397,  398,  400,  402,    5,  323,  330,
 /*   540 */     6,  345,  352,  489,  405,  406,  407,  408,  409,  410,
 /*   550 */   411,  412,  413,  414,  415,  416,  417,  418,  419,  420,
 /*   560 */   421,  422,  423,  424,  425,  426,  427,  428,  429,  430,
 /*   570 */   431,  432,  433,  434,  435,  436,  437,  438,  439,  440,
 /*   580 */   441,  442,  443,  444,  445,  446,  447,  448,  449,  450,
 /*   590 */   451,  452,  453,  454,  455,  456,  457,  458,  459,  460,
 /*   600 */   461,  462,  463,  464,  465,  466,  467,  468,  469,  470,
 /*   610 */   471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
 /*   620 */   481,  482,  483,  269,  272,  489,  404,  296,  286,  291,
 /*   630 */   489,  318,  304,  311,  340,  326,  333,  362,  348,  355,
 /*   640 */   387,  371,  379,   15,  323,  489,   16,  345,  489,  405,
 /*   650 */   406,  407,  408,  409,  410,  411,  412,  413,  414,  415,
 /*   660 */   416,  417,  418,  419,  420,  421,  422,  423,  424,  425,
 /*   670 */   426,  427,  428,  429,  430,  431,  432,  433,  434,  435,
 /*   680 */   436,  437,  438,  439,  440,  441,  442,  443,  444,  445,
 /*   690 */   446,  447,  448,  449,  450,  451,  452,  453,  454,  455,
 /*   700 */   456,  457,  404,  344,  365,   21,  489,  489,  369,   39,
 /*   710 */   489,   23,  399,  390,  391,  489,  489,  489,  403,  397,
 /*   720 */   398,  489,  402,  489,  489,  405,  406,  407,  408,  409,
 /*   730 */   410,  411,  412,  413,  414,  415,  416,  417,  418,  419,
 /*   740 */   420,  421,  422,  423,  424,  425,  426,  427,  428,  429,
 /*   750 */   430,  431,  432,  433,  434,  435,  436,  437,  438,  439,
 /*   760 */   440,  441,  442,  443,  444,  445,  446,  447,  448,  449,
 /*   770 */   450,  451,  452,  453,  454,  455,  456,  457,  404,  489,
 /*   780 */   489,   18,    1,  283,  288,  489,    3,  301,  308,    5,
 /*   790 */   323,  330,    6,  345,  352,  489,  489,  489,  489,  489,
 /*   800 */   489,  405,  406,  407,  408,  409,  410,  411,  412,  413,
 /*   810 */   414,  415,  416,  417,  418,  419,  420,  421,  422,  423,
 /*   820 */   424,  425,  426,  427,  428,  429,  430,  431,  432,  433,
 /*   830 */   434,  435,  436,  437,  438,  439,  440,  441,  442,  443,
 /*   840 */   444,  445,  446,  447,  448,  449,  450,  451,  452,  453,
 /*   850 */   454,  455,  456,  457,  404,  367,  383,  489,   23,  399,
 /*   860 */   390,  391,  489,  489,  489,  403,  397,  398,   41,  402,
 /*   870 */   489,  267,  489,  489,  270,   25,  489,  405,  406,  407,
 /*   880 */   408,  409,  410,  411,  412,  413,  414,  415,  416,  417,
 /*   890 */   418,  419,  420,  421,  422,  423,  424,  425,  426,  427,
 /*   900 */   428,  429,  430,  431,  432,  433,  434,  435,  436,  437,
 /*   910 */   438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
 /*   920 */   448,  449,  450,  451,  452,  453,  454,  455,  456,  457,
 /*   930 */    28,  282,  298,  287,  305,  292,  312,  299,  302,  306,
 /*   940 */   309,  313,  316,  321,  324,  328,  331,  335,  338,  343,
 /*   950 */   346,  350,  353,  357,  369,   35,  377,   22,  399,  390,
 /*   960 */   391,  401,  393,  394,  403,  397,  398,  400,  402,   20,
 /*   970 */   423,  424,  425,  426,  427,  428,  429,  430,  431,  432,
 /*   980 */   433,  434,  435,  436,  437,  438,  439,  440,  441,  442,
 /*   990 */   443,  444,  445,  446,  447,  448,  449,  450,  451,  452,
 /*  1000 */   453,  454,  455,  456,  457,  489,  489,  423,  424,  425,
 /*  1010 */   426,  427,  428,  429,  430,  431,  432,  433,  434,  435,
 /*  1020 */   436,  437,  438,  439,  440,  441,  442,  443,  444,  445,
 /*  1030 */   446,  447,  448,  449,  450,  451,  452,  453,  454,  455,
 /*  1040 */   456,  457,  712,    2,    2,  263,   34,  274,  275,  277,
 /*  1050 */    27,  489,  284,  489,  289,  489,  294,  489,  302,  489,
 /*  1060 */   309,  489,  316,  489,  324,  489,  331,  489,  338,  489,
 /*  1070 */   346,  489,  353,  489,  369,   35,  377,   22,  399,  390,
 /*  1080 */   391,  401,  393,  394,  403,  397,  398,  400,  402,  485,
 /*  1090 */   263,   34,  274,  275,  277,   27,  489,  284,  489,  289,
 /*  1100 */   489,  294,  489,  302,  489,  309,  489,  316,  489,  324,
 /*  1110 */   489,  331,  489,  338,  489,  346,  489,  353,  489,  369,
 /*  1120 */    35,  377,   22,  399,  390,  391,  401,  393,  394,  403,
 /*  1130 */   397,  398,  400,  402,   30,  489,  489,  489,  489,  489,
 /*  1140 */   489,  300,  320,  307,  327,  314,  334,  321,  324,  328,
 /*  1150 */   331,  335,  338,  343,  346,  350,  353,  357,  369,   35,
 /*  1160 */   377,   22,  399,  390,  391,  401,  393,  394,  403,  397,
 /*  1170 */   398,  400,  402,  264,   34,  274,  275,  277,   27,  489,
 /*  1180 */   284,  489,  289,  489,  294,  489,  302,  489,  309,  489,
 /*  1190 */   316,  489,  324,  489,  331,  489,  338,  489,  346,  489,
 /*  1200 */   353,  489,  369,   35,  377,   22,  399,  390,  391,  401,
 /*  1210 */   393,  394,  403,  397,  398,  400,  402,   36,  431,  432,
 /*  1220 */   433,  434,  435,  436,  437,  438,  439,  489,  489,  489,
 /*  1230 */   322,  342,  329,  349,  336,  356,  343,  346,  350,  353,
 /*  1240 */   357,  369,   35,  377,   22,  399,  390,  391,  401,  393,
 /*  1250 */   394,  403,  397,  398,  400,  402,  404,  366,  382,  489,
 /*  1260 */    23,  399,  390,  391,  489,  489,  489,  403,  397,  398,
 /*  1270 */   489,  402,  489,  489,  489,  489,  489,  489,  489,  405,
 /*  1280 */   406,  407,  408,  409,  410,  411,  412,    3,  301,  308,
 /*  1290 */     5,  323,  330,    6,  345,  352,  489,    8,  301,  489,
 /*  1300 */    15,  323,  489,   16,  345,  431,  432,  433,  434,  435,
 /*  1310 */   436,  437,  438,  439,  489,  489,  489,  489,  297,    7,
 /*  1320 */   283,  381,  319,    8,  301,  341,   15,  323,  363,   16,
 /*  1330 */   345,  388,  458,  459,  460,  461,  462,  463,  464,  465,
 /*  1340 */   344,  365,  351,  373,  358,  369,   35,  377,   22,  399,
 /*  1350 */   390,  391,  401,  393,  394,  403,  397,  398,  400,  402,
 /*  1360 */    31,  282,  298,  489,  489,  489,  367,  299,  302,   24,
 /*  1370 */   399,  390,  391,  321,  324,  489,  489,  489,  398,  343,
 /*  1380 */   346,  404,  489,  489,  369,   38,  489,   23,  399,  390,
 /*  1390 */   391,  489,  489,  489,  403,  397,  398,  489,  402,  489,
 /*  1400 */   489,  489,  489,  489,  405,  406,  407,  408,  409,  410,
 /*  1410 */   411,  412,  489,  366,  489,  489,   24,  399,  390,  391,
 /*  1420 */   489,  489,  489,  489,  489,  398,  489,  489,  489,  489,
 /*  1430 */   431,  432,  433,  434,  435,  436,  437,  438,  439,   33,
 /*  1440 */   489,  489,  489,  489,  489,  489,  300,  320,  489,  489,
 /*  1450 */   489,  489,  321,  324,  489,  489,  489,  489,  343,  346,
 /*  1460 */   489,  489,  489,  369,   38,  489,   23,  399,  390,  391,
 /*  1470 */   489,  489,  489,  403,  397,  398,  278,  402,  489,   29,
 /*  1480 */   364,  284,  489,   24,  399,  390,  391,  302,  489,  489,
 /*  1490 */   489,  489,  398,  324,  489,  489,  489,  489,  489,  346,
 /*  1500 */   489,  489,  489,  369,   38,  489,   23,  399,  390,  391,
 /*  1510 */   489,  489,  489,  403,  397,  398,  489,  402,  489,  279,
 /*  1520 */   489,  489,   29,  489,  284,  489,  489,  489,  489,  489,
 /*  1530 */   302,  489,  489,  489,  489,  489,  324,  489,  489,  489,
 /*  1540 */   489,  489,  346,  489,  489,  489,  369,   38,   40,   23,
 /*  1550 */   399,  390,  391,  489,  489,  489,  403,  397,  398,  489,
 /*  1560 */   402,  322,  342,  489,  489,  489,  489,  343,  346,  489,
 /*  1570 */   489,  489,  369,   38,  489,   23,  399,  390,  391,  489,
 /*  1580 */   489,  489,  403,  397,  398,  489,  402,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     0,    1,  145,  145,  145,  148,    6,    7,  151,  151,
 /*    10 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*    20 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    30 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*    40 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*    50 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*    60 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*    70 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*    80 */    80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
 /*    90 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*   100 */   100,  101,  102,  103,  104,  105,  106,  107,    1,   26,
 /*   110 */    27,   28,    7,    6,    7,   10,  156,   10,   11,   12,
 /*   120 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*   130 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*   140 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*   150 */    43,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*   160 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   170 */    63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
 /*   180 */    73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
 /*   190 */    83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*   200 */    93,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   210 */   103,  104,  105,  106,  107,    6,    7,    3,    4,   10,
 /*   220 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*   230 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*   240 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   250 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*   260 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   270 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*   280 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   290 */    81,   82,   83,   84,   85,   86,   87,   88,   89,   90,
 /*   300 */    91,   92,   93,   94,   95,   96,   97,   98,   99,  100,
 /*   310 */   101,  102,  103,  104,  105,  106,  107,    6,   26,   27,
 /*   320 */     9,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*   330 */   150,  151,  152,  153,  154,  155,   23,   24,   25,  116,
 /*   340 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   350 */    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*   360 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   370 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*   380 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   390 */    79,   80,   81,   82,   83,   84,   85,   86,   87,   88,
 /*   400 */    89,   90,   91,   92,   93,   94,   95,   96,   97,   98,
 /*   410 */    99,  100,  101,  102,  103,  104,  105,  106,  107,    6,
 /*   420 */    26,   27,    9,  141,  142,  143,  144,  145,  146,  147,
 /*   430 */   148,  149,  150,  151,  152,  153,  154,  155,   23,   24,
 /*   440 */     3,    4,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   450 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*   460 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   470 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   480 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*   490 */    77,   78,   79,   80,   81,   82,   83,   84,   85,   86,
 /*   500 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*   510 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   520 */   107,    6,  141,  142,  143,  144,  145,  146,  147,  148,
 /*   530 */   149,  150,  151,  152,  153,  154,  155,   20,   21,   22,
 /*   540 */    23,   24,   25,  156,   29,   30,   31,   32,   33,   34,
 /*   550 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*   560 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   570 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*   580 */    65,   66,   67,   68,   69,   70,   71,   72,   73,   74,
 /*   590 */    75,   76,   77,   78,   79,   80,   81,   82,   83,   84,
 /*   600 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   610 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   620 */   105,  106,  107,    3,    4,  156,    6,   13,   14,   15,
 /*   630 */   156,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*   640 */    26,   27,   28,   20,   21,  156,   23,   24,  156,   29,
 /*   650 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   660 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*   670 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   680 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   690 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   700 */    80,   81,    6,  136,  137,    9,  156,  156,  141,  142,
 /*   710 */   156,  144,  145,  146,  147,  156,  156,  156,  151,  152,
 /*   720 */   153,  156,  155,  156,  156,   29,   30,   31,   32,   33,
 /*   730 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*   740 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   750 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*   760 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*   770 */    74,   75,   76,   77,   78,   79,   80,   81,    6,  156,
 /*   780 */   156,    9,   13,   14,   15,  156,   17,   18,   19,   20,
 /*   790 */    21,   22,   23,   24,   25,  156,  156,  156,  156,  156,
 /*   800 */   156,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*   810 */    38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
 /*   820 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   830 */    58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   840 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   850 */    78,   79,   80,   81,    6,  141,  142,  156,  144,  145,
 /*   860 */   146,  147,  156,  156,  156,  151,  152,  153,    2,  155,
 /*   870 */   156,    5,  156,  156,    8,    9,  156,   29,   30,   31,
 /*   880 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*   890 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*   900 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*   910 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   920 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*   930 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*   940 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*   950 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*   960 */   147,  148,  149,  150,  151,  152,  153,  154,  155,    9,
 /*   970 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   980 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   990 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*  1000 */    77,   78,   79,   80,   81,  156,  156,   47,   48,   49,
 /*  1010 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  1020 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  1030 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*  1040 */    80,   81,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1050 */   117,  156,  119,  156,  121,  156,  123,  156,  125,  156,
 /*  1060 */   127,  156,  129,  156,  131,  156,  133,  156,  135,  156,
 /*  1070 */   137,  156,  139,  156,  141,  142,  143,  144,  145,  146,
 /*  1080 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  111,
 /*  1090 */   112,  113,  114,  115,  116,  117,  156,  119,  156,  121,
 /*  1100 */   156,  123,  156,  125,  156,  127,  156,  129,  156,  131,
 /*  1110 */   156,  133,  156,  135,  156,  137,  156,  139,  156,  141,
 /*  1120 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  1130 */   152,  153,  154,  155,  117,  156,  156,  156,  156,  156,
 /*  1140 */   156,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1150 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1160 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1170 */   153,  154,  155,  112,  113,  114,  115,  116,  117,  156,
 /*  1180 */   119,  156,  121,  156,  123,  156,  125,  156,  127,  156,
 /*  1190 */   129,  156,  131,  156,  133,  156,  135,  156,  137,  156,
 /*  1200 */   139,  156,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  1210 */   149,  150,  151,  152,  153,  154,  155,  117,   55,   56,
 /*  1220 */    57,   58,   59,   60,   61,   62,   63,  156,  156,  156,
 /*  1230 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1240 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  1250 */   150,  151,  152,  153,  154,  155,    6,  141,  142,  156,
 /*  1260 */   144,  145,  146,  147,  156,  156,  156,  151,  152,  153,
 /*  1270 */   156,  155,  156,  156,  156,  156,  156,  156,  156,   29,
 /*  1280 */    30,   31,   32,   33,   34,   35,   36,   17,   18,   19,
 /*  1290 */    20,   21,   22,   23,   24,   25,  156,   17,   18,  156,
 /*  1300 */    20,   21,  156,   23,   24,   55,   56,   57,   58,   59,
 /*  1310 */    60,   61,   62,   63,  156,  156,  156,  156,   13,   13,
 /*  1320 */    14,  117,   17,   17,   18,   20,   20,   21,   23,   23,
 /*  1330 */    24,   26,   82,   83,   84,   85,   86,   87,   88,   89,
 /*  1340 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1350 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  1360 */   117,  118,  119,  156,  156,  156,  141,  124,  125,  144,
 /*  1370 */   145,  146,  147,  130,  131,  156,  156,  156,  153,  136,
 /*  1380 */   137,    6,  156,  156,  141,  142,  156,  144,  145,  146,
 /*  1390 */   147,  156,  156,  156,  151,  152,  153,  156,  155,  156,
 /*  1400 */   156,  156,  156,  156,   29,   30,   31,   32,   33,   34,
 /*  1410 */    35,   36,  156,  141,  156,  156,  144,  145,  146,  147,
 /*  1420 */   156,  156,  156,  156,  156,  153,  156,  156,  156,  156,
 /*  1430 */    55,   56,   57,   58,   59,   60,   61,   62,   63,  117,
 /*  1440 */   156,  156,  156,  156,  156,  156,  124,  125,  156,  156,
 /*  1450 */   156,  156,  130,  131,  156,  156,  156,  156,  136,  137,
 /*  1460 */   156,  156,  156,  141,  142,  156,  144,  145,  146,  147,
 /*  1470 */   156,  156,  156,  151,  152,  153,  114,  155,  156,  117,
 /*  1480 */   141,  119,  156,  144,  145,  146,  147,  125,  156,  156,
 /*  1490 */   156,  156,  153,  131,  156,  156,  156,  156,  156,  137,
 /*  1500 */   156,  156,  156,  141,  142,  156,  144,  145,  146,  147,
 /*  1510 */   156,  156,  156,  151,  152,  153,  156,  155,  156,  114,
 /*  1520 */   156,  156,  117,  156,  119,  156,  156,  156,  156,  156,
 /*  1530 */   125,  156,  156,  156,  156,  156,  131,  156,  156,  156,
 /*  1540 */   156,  156,  137,  156,  156,  156,  141,  142,  117,  144,
 /*  1550 */   145,  146,  147,  156,  156,  156,  151,  152,  153,  156,
 /*  1560 */   155,  130,  131,  156,  156,  156,  156,  136,  137,  156,
 /*  1570 */   156,  156,  141,  142,  156,  144,  145,  146,  147,  156,
 /*  1580 */   156,  156,  151,  152,  153,  156,  155,
};
#define YY_SHIFT_USE_DFLT (1587)
#define YY_SHIFT_COUNT    (41)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (1375)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */   107,  311,    0,  311,  209,  311,  311,  696,  696,  620,
 /*    10 */   848,  848,  413,  515,  515,  696,  696,  772,  848,  960,
 /*    20 */   923,  923, 1250, 1375, 1163,  105,  614,  769, 1270, 1306,
 /*    30 */   517, 1280, 1305,  623,  866,   83,  313,  214,  292,  394,
 /*    40 */   415,  437,
};
#define YY_REDUCE_USE_DFLT (-144)
#define YY_REDUCE_COUNT (25)
#define YY_REDUCE_MIN   (-143)
#define YY_REDUCE_MAX   (1431)
static const short yy_reduce_ofst[] = {
 /*     0 */   933,  813,  978, 1017, 1061, 1100, 1204, 1243, 1322, 1362,
 /*    10 */  1405, 1362,  180,  282,  381, 1431,  567,  714, 1116, 1225,
 /*    20 */  1272, 1339, -143, -142, -141,  223,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   711,  517,  711,  539,  711,  561,  583,  711,  711,  711,
 /*    10 */   711,  711,  608,  711,  711,  711,  711,  608,  711,  711,
 /*    20 */   711,  711,  620,  620,  711,  711,  504,  584,  584,  711,
 /*    30 */   584,  711,  711,  711,  497,  609,  584,  628,  609,  711,
 /*    40 */   711,  711,
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
  "EIGHTIETH",     "NINETIETH",     "FIRSTS",        "THIRDS",      
  "FOURTHS",       "FIFTHS",        "SIXTHS",        "SEVENTHS",    
  "EIGHTHS",       "NINTHS",        "TENTHS",        "ELEVENTHS",   
  "TWELFTHS",      "THIRTEENTHS",   "FOURTEENTHS",   "FIFTEENTHS",  
  "SIXTEENTHS",    "SEVENTEENTHS",  "EIGHTEENTHS",   "NINETEENTHS", 
  "TWENTIETHS",    "THIRTIETHS",    "FOURTIETHS",    "FIFTIETHS",   
  "SIXTIETHS",     "SEVENTIETHS",   "EIGHTIETHS",    "NINETIETHS",  
  "error",         "program",       "numbers",       "number",      
  "final_number",  "less_than_quadrillion",  "less_than_quadrillionth",  "less_than_quadrillionths",
  "fraction",      "less_than_thousand",  "less_than_trillionth_end_only",  "less_than_trillionth",
  "less_than_trillionths_end_only",  "less_than_trillionths",  "less_than_trillion_end_only",  "less_than_trillion",
  "less_than_billionth_end_only",  "less_than_billionth",  "less_than_billionths_end_only",  "less_than_billionths",
  "less_than_billion_end_only",  "less_than_billion",  "less_than_millionth_end_only",  "less_than_millionth",
  "less_than_millionths_end_only",  "less_than_millionths",  "less_than_million_end_only",  "less_than_million",
  "less_than_thousandth_end_only",  "less_than_thousandth",  "less_than_thousandths_end_only",  "less_than_thousandths",
  "less_than_thousand_end_only",  "less_than_hundredth",  "less_than_hundred",  "less_than_hundredths",
  "tens",          "less_than_tenth",  "tenth",         "less_than_twentieth",
  "less_than_tenths",  "tenths",        "less_than_twentieths",  "less_than_ten",
  "less_than_twenty",  "tenth_to_19th",  "tenths_to_19ths",  "ten_to_19",   
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
 /* 196 */ "less_than_tenths ::= THIRDS",
 /* 197 */ "less_than_tenths ::= FOURTHS",
 /* 198 */ "less_than_tenths ::= FIFTHS",
 /* 199 */ "less_than_tenths ::= SIXTHS",
 /* 200 */ "less_than_tenths ::= SEVENTHS",
 /* 201 */ "less_than_tenths ::= EIGHTHS",
 /* 202 */ "less_than_tenths ::= NINTHS",
 /* 203 */ "tenths_to_19ths ::= TENTHS",
 /* 204 */ "tenths_to_19ths ::= ELEVENTHS",
 /* 205 */ "tenths_to_19ths ::= TWELFTHS",
 /* 206 */ "tenths_to_19ths ::= THIRTEENTHS",
 /* 207 */ "tenths_to_19ths ::= FOURTEENTHS",
 /* 208 */ "tenths_to_19ths ::= FIFTEENTHS",
 /* 209 */ "tenths_to_19ths ::= SIXTEENTHS",
 /* 210 */ "tenths_to_19ths ::= SEVENTEENTHS",
 /* 211 */ "tenths_to_19ths ::= EIGHTEENTHS",
 /* 212 */ "tenths_to_19ths ::= NINETEENTHS",
 /* 213 */ "tenths ::= TWENTIETHS",
 /* 214 */ "tenths ::= THIRTIETHS",
 /* 215 */ "tenths ::= FOURTIETHS",
 /* 216 */ "tenths ::= FIFTIETHS",
 /* 217 */ "tenths ::= SIXTIETHS",
 /* 218 */ "tenths ::= SEVENTIETHS",
 /* 219 */ "tenths ::= EIGHTIETHS",
 /* 220 */ "tenths ::= NINETIETHS",
 /* 221 */ "program ::= numbers",
 /* 222 */ "numbers ::= numbers number",
 /* 223 */ "numbers ::= number",
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
  { 111, 1 },
  { 111, 2 },
  { 112, 3 },
  { 112, 3 },
  { 112, 2 },
  { 112, 2 },
  { 112, 2 },
  { 112, 2 },
  { 112, 2 },
  { 112, 2 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 3 },
  { 112, 1 },
  { 116, 2 },
  { 116, 2 },
  { 112, 1 },
  { 112, 1 },
  { 114, 3 },
  { 114, 2 },
  { 114, 1 },
  { 112, 1 },
  { 112, 2 },
  { 115, 3 },
  { 115, 2 },
  { 115, 1 },
  { 112, 1 },
  { 112, 2 },
  { 113, 3 },
  { 113, 2 },
  { 113, 1 },
  { 112, 1 },
  { 112, 2 },
  { 112, 2 },
  { 118, 1 },
  { 118, 1 },
  { 119, 3 },
  { 119, 2 },
  { 119, 1 },
  { 112, 1 },
  { 112, 2 },
  { 120, 1 },
  { 120, 1 },
  { 121, 3 },
  { 121, 2 },
  { 121, 1 },
  { 112, 1 },
  { 112, 2 },
  { 122, 1 },
  { 122, 1 },
  { 123, 3 },
  { 123, 2 },
  { 123, 1 },
  { 112, 1 },
  { 112, 2 },
  { 112, 2 },
  { 124, 1 },
  { 124, 1 },
  { 125, 3 },
  { 125, 2 },
  { 125, 1 },
  { 112, 1 },
  { 112, 2 },
  { 126, 1 },
  { 126, 1 },
  { 127, 3 },
  { 127, 2 },
  { 127, 1 },
  { 112, 1 },
  { 112, 2 },
  { 128, 1 },
  { 128, 1 },
  { 129, 3 },
  { 129, 2 },
  { 129, 1 },
  { 112, 1 },
  { 112, 2 },
  { 112, 2 },
  { 130, 1 },
  { 130, 1 },
  { 131, 3 },
  { 131, 2 },
  { 131, 1 },
  { 112, 1 },
  { 112, 2 },
  { 132, 1 },
  { 132, 1 },
  { 133, 3 },
  { 133, 2 },
  { 133, 1 },
  { 112, 1 },
  { 112, 2 },
  { 134, 1 },
  { 134, 1 },
  { 135, 3 },
  { 135, 2 },
  { 135, 1 },
  { 112, 1 },
  { 112, 2 },
  { 112, 2 },
  { 136, 2 },
  { 136, 1 },
  { 137, 4 },
  { 137, 3 },
  { 137, 2 },
  { 137, 1 },
  { 112, 1 },
  { 112, 2 },
  { 138, 2 },
  { 138, 1 },
  { 139, 4 },
  { 139, 3 },
  { 139, 2 },
  { 139, 1 },
  { 112, 1 },
  { 112, 2 },
  { 140, 2 },
  { 140, 1 },
  { 117, 4 },
  { 117, 3 },
  { 117, 2 },
  { 117, 1 },
  { 112, 1 },
  { 112, 2 },
  { 112, 2 },
  { 141, 2 },
  { 141, 1 },
  { 141, 1 },
  { 143, 2 },
  { 143, 1 },
  { 143, 1 },
  { 142, 2 },
  { 142, 1 },
  { 142, 1 },
  { 147, 1 },
  { 147, 1 },
  { 150, 1 },
  { 150, 1 },
  { 152, 1 },
  { 152, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
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
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
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
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 109, 1 },
  { 110, 2 },
  { 110, 1 },
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
#line 68 "parser.yy"
{
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1711 "parser.c"
        break;
      case 1: /* number ::= NEGATIVE final_number */
#line 71 "parser.yy"
{
    yymsp[0].minor.yy0.dbl = -yymsp[0].minor.yy0.dbl;
    yymsp[0].minor.yy0.begin = yymsp[-1].minor.yy0.begin;
    yymsp[0].minor.yy0.end = yymsp[0].minor.yy0.end;
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1721 "parser.c"
        break;
      case 2: /* final_number ::= less_than_quadrillion AND_A QUARTER */
#line 78 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, 1.0, 4.0); }
#line 1726 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* final_number ::= less_than_quadrillion AND_A HALF */
#line 79 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0); }
#line 1732 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 4: /* final_number ::= less_than_quadrillion QUARTERS */
#line 81 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, yymsp[-1].minor.yy0.dbl, 4.0); }
#line 1738 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* final_number ::= ONE QUARTER */
      case 6: /* final_number ::= A QUARTER */ yytestcase(yyruleno==6);
#line 82 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 4.0); }
#line 1745 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* final_number ::= less_than_quadrillion HALVES */
#line 85 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, yymsp[-1].minor.yy0.dbl, 2.0); }
#line 1751 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 8: /* final_number ::= ONE HALF */
#line 86 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0);  }
#line 1757 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* final_number ::= A HALF */
#line 87 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, 2.0); }
#line 1763 "parser.c"
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
#line 89 "parser.yy"
{ COPY_YYSTYPE_BE_DBL(yylhsminor.yy0, yymsp[0].minor.yy0); }
#line 1785 "parser.c"
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
#line 90 "parser.yy"
{ COPY_YYSTYPE_BE_DBL_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0); }
#line 1824 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 13: /* final_number ::= less_than_quadrillion AND fraction */
#line 93 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET_MULT(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, yymsp[0].minor.yy0.frac_num, yymsp[0].minor.yy0.frac_denom); }
#line 1830 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* final_number ::= fraction */
#line 96 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.frac_num = yymsp[0].minor.yy0.frac_num; yylhsminor.yy0.frac_denom = yymsp[0].minor.yy0.frac_denom; yylhsminor.yy0.is_frac = yymsp[0].minor.yy0.is_frac; }
#line 1836 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 15: /* fraction ::= A less_than_quadrillionth */
      case 16: /* fraction ::= AN less_than_quadrillionth */ yytestcase(yyruleno==16);
#line 108 "parser.yy"
{ COPY_YYSTYPE_FRAC_SET(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 1.0, yymsp[0].minor.yy0.dbl); }
#line 1843 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 17: /* final_number ::= DECIMAL */
#line 114 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = true; }
#line 1849 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 18: /* final_number ::= ZERO */
#line 115 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 0.0); }
#line 1855 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 19: /* less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only */
      case 24: /* less_than_quadrillionths ::= less_than_thousand TRILLION less_than_trillionths_end_only */ yytestcase(yyruleno==24);
#line 121 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1862 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 20: /* less_than_quadrillionth ::= less_than_thousand TRILLIONTH */
#line 122 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 1868 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 22: /* final_number ::= TRILLIONTH */
#line 125 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 1874 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 23: /* final_number ::= DECIMAL TRILLIONTH */
#line 126 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 1880 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* less_than_quadrillionths ::= less_than_thousand TRILLIONTHS */
#line 131 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 1886 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 27: /* final_number ::= TRILLIONTHS */
#line 134 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 1892 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 28: /* final_number ::= DECIMAL TRILLIONTHS */
#line 135 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 1898 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only */
#line 141 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1904 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 30: /* less_than_quadrillion ::= less_than_thousand TRILLION */
      case 34: /* final_number ::= WHOLE_NUMBER TRILLION */ yytestcase(yyruleno==34);
#line 142 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1911 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* final_number ::= TRILLION */
#line 145 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1917 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 33: /* final_number ::= DECIMAL TRILLION */
#line 146 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1923 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 37: /* less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only */
      case 44: /* less_than_trillionths ::= less_than_thousand BILLION less_than_billionths_end_only */ yytestcase(yyruleno==44);
#line 156 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1930 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* less_than_trillionth ::= less_than_thousand BILLIONTH */
#line 157 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 1936 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 40: /* final_number ::= BILLIONTH */
#line 160 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 1942 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 41: /* final_number ::= DECIMAL BILLIONTH */
#line 161 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 1948 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 45: /* less_than_trillionths ::= less_than_thousand BILLIONTHS */
#line 169 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 1954 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 47: /* final_number ::= BILLIONTHS */
#line 172 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 1960 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 48: /* final_number ::= DECIMAL BILLIONTHS */
#line 173 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 1966 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 51: /* less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only */
#line 182 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1972 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 52: /* less_than_trillion ::= less_than_thousand BILLION */
      case 56: /* final_number ::= WHOLE_NUMBER BILLION */ yytestcase(yyruleno==56);
#line 183 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1979 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 54: /* final_number ::= BILLION */
#line 186 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1985 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 55: /* final_number ::= DECIMAL BILLION */
#line 187 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1991 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 59: /* less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only */
      case 66: /* less_than_billionths ::= less_than_thousand MILLION less_than_millionths_end_only */ yytestcase(yyruleno==66);
#line 197 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 1998 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 60: /* less_than_billionth ::= less_than_thousand MILLIONTH */
#line 198 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2004 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 62: /* final_number ::= MILLIONTH */
#line 201 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2010 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 63: /* final_number ::= DECIMAL MILLIONTH */
#line 202 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 2016 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 67: /* less_than_billionths ::= less_than_thousand MILLIONTHS */
#line 210 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2022 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 69: /* final_number ::= MILLIONTHS */
#line 213 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2028 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 70: /* final_number ::= DECIMAL MILLIONTHS */
#line 214 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 2034 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 73: /* less_than_billion ::= less_than_thousand MILLION less_than_million_end_only */
#line 223 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2040 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 74: /* less_than_billion ::= less_than_thousand MILLION */
      case 78: /* final_number ::= WHOLE_NUMBER MILLION */ yytestcase(yyruleno==78);
#line 224 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2047 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 76: /* final_number ::= MILLION */
#line 227 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2053 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 77: /* final_number ::= DECIMAL MILLION */
#line 228 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 2059 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 81: /* less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only */
      case 88: /* less_than_millionths ::= less_than_thousand THOUSAND less_than_thousandths_end_only */ yytestcase(yyruleno==88);
#line 238 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2066 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 82: /* less_than_millionth ::= less_than_thousand THOUSANDTH */
#line 239 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2072 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 84: /* final_number ::= THOUSANDTH */
#line 242 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2078 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 85: /* final_number ::= DECIMAL THOUSANDTH */
#line 243 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 2084 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 89: /* less_than_millionths ::= less_than_thousand THOUSANDTHS */
#line 251 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2090 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 91: /* final_number ::= THOUSANDTHS */
#line 254 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2096 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 92: /* final_number ::= DECIMAL THOUSANDTHS */
#line 255 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 2102 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 95: /* less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only */
#line 264 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2108 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 96: /* less_than_million ::= less_than_thousand THOUSAND */
      case 100: /* final_number ::= WHOLE_NUMBER THOUSAND */ yytestcase(yyruleno==100);
#line 265 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2115 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 98: /* final_number ::= THOUSAND */
#line 268 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2121 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 99: /* final_number ::= DECIMAL THOUSAND */
#line 269 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 2127 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 101: /* less_than_thousandth_end_only ::= AND less_than_hundredth */
      case 109: /* less_than_thousandths_end_only ::= AND less_than_hundredths */ yytestcase(yyruleno==109);
#line 276 "parser.yy"
{ COPY_YYSTYPE_BE_DBL_SUFF(yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2134 "parser.c"
        break;
      case 103: /* less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth */
      case 111: /* less_than_thousandths ::= less_than_hundred HUNDRED AND less_than_hundredths */ yytestcase(yyruleno==111);
#line 279 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2140 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 104: /* less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth */
      case 112: /* less_than_thousandths ::= less_than_hundred HUNDRED less_than_hundredths */ yytestcase(yyruleno==112);
#line 280 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2147 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 105: /* less_than_thousandth ::= less_than_hundred HUNDREDTH */
#line 281 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2153 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 107: /* final_number ::= HUNDREDTH */
#line 284 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2159 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 108: /* final_number ::= DECIMAL HUNDREDTH */
#line 285 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 2165 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 113: /* less_than_thousandths ::= less_than_hundred HUNDREDTHS */
#line 294 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2171 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 115: /* final_number ::= HUNDREDTHS */
#line 297 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2177 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 116: /* final_number ::= DECIMAL HUNDREDTHS */
#line 299 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 2183 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 117: /* less_than_thousand_end_only ::= AND less_than_hundred */
#line 305 "parser.yy"
{ COPY_YYSTYPE_BE_DBL(yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2189 "parser.c"
        break;
      case 119: /* less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred */
#line 308 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2194 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 120: /* less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred */
#line 309 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2200 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 121: /* less_than_thousand ::= less_than_hundred HUNDRED */
      case 125: /* final_number ::= WHOLE_NUMBER HUNDRED */ yytestcase(yyruleno==125);
#line 310 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2207 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 123: /* final_number ::= HUNDRED */
#line 313 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2213 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 124: /* final_number ::= DECIMAL HUNDRED */
#line 314 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2219 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 126: /* less_than_hundredth ::= tens less_than_tenth */
      case 129: /* less_than_hundredths ::= tens less_than_tenths */ yytestcase(yyruleno==129);
#line 321 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2226 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 132: /* less_than_hundred ::= tens less_than_ten */
#line 333 "parser.yy"
{ COPY_YYSTYPE_BE_ADD(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2232 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 141: /* less_than_ten ::= ONE */
#line 358 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0); }
#line 2238 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 142: /* less_than_ten ::= TWO */
#line 359 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0); }
#line 2244 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 143: /* less_than_ten ::= THREE */
#line 360 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0); }
#line 2250 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 144: /* less_than_ten ::= FOUR */
#line 361 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0); }
#line 2256 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 145: /* less_than_ten ::= FIVE */
#line 362 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0); }
#line 2262 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 146: /* less_than_ten ::= SIX */
#line 363 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0); }
#line 2268 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 147: /* less_than_ten ::= SEVEN */
#line 364 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0); }
#line 2274 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 148: /* less_than_ten ::= EIGHT */
#line 365 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0); }
#line 2280 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 149: /* less_than_ten ::= NINE */
#line 366 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0); }
#line 2286 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 150: /* ten_to_19 ::= TEN */
#line 368 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0); }
#line 2292 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 151: /* ten_to_19 ::= ELEVEN */
#line 369 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0); }
#line 2298 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 152: /* ten_to_19 ::= TWELVE */
#line 370 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0); }
#line 2304 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 153: /* ten_to_19 ::= THIRTEEN */
#line 371 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0); }
#line 2310 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 154: /* ten_to_19 ::= FOURTEEN */
#line 372 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0); }
#line 2316 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 155: /* ten_to_19 ::= FIFTEEN */
#line 373 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0); }
#line 2322 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 156: /* ten_to_19 ::= SIXTEEN */
#line 374 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0); }
#line 2328 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 157: /* ten_to_19 ::= SEVENTEEN */
#line 375 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0); }
#line 2334 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 158: /* ten_to_19 ::= EIGHTEEN */
#line 376 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0); }
#line 2340 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 159: /* ten_to_19 ::= NINETEEN */
#line 377 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0); }
#line 2346 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 160: /* tens ::= TWENTY */
#line 379 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0); }
#line 2352 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 161: /* tens ::= THIRTY */
#line 380 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0); }
#line 2358 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 162: /* tens ::= FORTY */
#line 381 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0); }
#line 2364 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 163: /* tens ::= FIFTY */
#line 382 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0); }
#line 2370 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 164: /* tens ::= SIXTY */
#line 383 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0); }
#line 2376 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 165: /* tens ::= SEVENTY */
#line 384 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0); }
#line 2382 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 166: /* tens ::= EIGHTY */
#line 385 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0); }
#line 2388 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 167: /* tens ::= NINETY */
#line 386 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0); }
#line 2394 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 168: /* less_than_tenth ::= FIRST */
#line 388 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0, ST); }
#line 2400 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 169: /* less_than_tenth ::= SECOND */
#line 389 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0, ND); }
#line 2406 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 170: /* less_than_tenth ::= THIRD */
#line 390 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0, RD); }
#line 2412 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 171: /* less_than_tenth ::= FOURTH */
#line 391 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0, TH); }
#line 2418 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 172: /* less_than_tenth ::= FIFTH */
#line 392 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0, TH); }
#line 2424 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 173: /* less_than_tenth ::= SIXTH */
#line 393 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0, TH); }
#line 2430 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 174: /* less_than_tenth ::= SEVENTH */
#line 394 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0, TH); }
#line 2436 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 175: /* less_than_tenth ::= EIGHTH */
#line 395 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0, TH); }
#line 2442 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 176: /* less_than_tenth ::= NINTH */
#line 396 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0, TH); }
#line 2448 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 177: /* tenth_to_19th ::= TENTH */
#line 398 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0, TH); }
#line 2454 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 178: /* tenth_to_19th ::= ELEVENTH */
#line 399 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0, TH); }
#line 2460 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 179: /* tenth_to_19th ::= TWELFTH */
#line 400 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0, TH); }
#line 2466 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 180: /* tenth_to_19th ::= THIRTEENTH */
#line 401 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0, TH); }
#line 2472 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 181: /* tenth_to_19th ::= FOURTEENTH */
#line 402 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0, TH); }
#line 2478 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 182: /* tenth_to_19th ::= FIFTEENTH */
#line 403 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0, TH); }
#line 2484 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 183: /* tenth_to_19th ::= SIXTEENTH */
#line 404 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0, TH); }
#line 2490 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 184: /* tenth_to_19th ::= SEVENTEENTH */
#line 405 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0, TH); }
#line 2496 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 185: /* tenth_to_19th ::= EIGHTEENTH */
#line 406 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0, TH); }
#line 2502 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 186: /* tenth_to_19th ::= NINETEENTH */
#line 407 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0, TH); }
#line 2508 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 187: /* tenth ::= TWENTIETH */
#line 409 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0, TH); }
#line 2514 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 188: /* tenth ::= THIRTIETH */
#line 410 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0, TH); }
#line 2520 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 189: /* tenth ::= FOURTIETH */
#line 411 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0, TH); }
#line 2526 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 190: /* tenth ::= FIFTIETH */
#line 412 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0, TH); }
#line 2532 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 191: /* tenth ::= SIXTIETH */
#line 413 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0, TH); }
#line 2538 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 192: /* tenth ::= SEVENTIETH */
#line 414 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0, TH); }
#line 2544 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 193: /* tenth ::= EIGHTIETH */
#line 415 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0, TH); }
#line 2550 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 194: /* tenth ::= NINETIETH */
#line 416 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0, TH); }
#line 2556 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 195: /* less_than_tenths ::= FIRSTS */
#line 418 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0, STS); }
#line 2562 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 196: /* less_than_tenths ::= THIRDS */
#line 420 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0, RDS); }
#line 2568 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 197: /* less_than_tenths ::= FOURTHS */
#line 421 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0, THS); }
#line 2574 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 198: /* less_than_tenths ::= FIFTHS */
#line 422 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0, THS); }
#line 2580 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 199: /* less_than_tenths ::= SIXTHS */
#line 423 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0, THS); }
#line 2586 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 200: /* less_than_tenths ::= SEVENTHS */
#line 424 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0, THS); }
#line 2592 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 201: /* less_than_tenths ::= EIGHTHS */
#line 425 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0, THS); }
#line 2598 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 202: /* less_than_tenths ::= NINTHS */
#line 426 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0, THS); }
#line 2604 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 203: /* tenths_to_19ths ::= TENTHS */
#line 428 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0, THS); }
#line 2610 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 204: /* tenths_to_19ths ::= ELEVENTHS */
#line 429 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0, THS); }
#line 2616 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 205: /* tenths_to_19ths ::= TWELFTHS */
#line 430 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0, THS); }
#line 2622 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 206: /* tenths_to_19ths ::= THIRTEENTHS */
#line 431 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0, THS); }
#line 2628 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 207: /* tenths_to_19ths ::= FOURTEENTHS */
#line 432 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0, THS); }
#line 2634 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 208: /* tenths_to_19ths ::= FIFTEENTHS */
#line 433 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0, THS); }
#line 2640 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 209: /* tenths_to_19ths ::= SIXTEENTHS */
#line 434 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0, THS); }
#line 2646 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 210: /* tenths_to_19ths ::= SEVENTEENTHS */
#line 435 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0, THS); }
#line 2652 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 211: /* tenths_to_19ths ::= EIGHTEENTHS */
#line 436 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0, THS); }
#line 2658 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 212: /* tenths_to_19ths ::= NINETEENTHS */
#line 437 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0, THS); }
#line 2664 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 213: /* tenths ::= TWENTIETHS */
#line 439 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0, THS); }
#line 2670 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 214: /* tenths ::= THIRTIETHS */
#line 440 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0, THS); }
#line 2676 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 215: /* tenths ::= FOURTIETHS */
#line 441 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0, THS); }
#line 2682 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 216: /* tenths ::= FIFTIETHS */
#line 442 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0, THS); }
#line 2688 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 217: /* tenths ::= SIXTIETHS */
#line 443 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0, THS); }
#line 2694 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 218: /* tenths ::= SEVENTIETHS */
#line 444 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0, THS); }
#line 2700 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 219: /* tenths ::= EIGHTIETHS */
#line 445 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0, THS); }
#line 2706 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 220: /* tenths ::= NINETIETHS */
#line 446 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0, THS); }
#line 2712 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      default:
      /* (221) program ::= numbers */ yytestcase(yyruleno==221);
      /* (222) numbers ::= numbers number */ yytestcase(yyruleno==222);
      /* (223) numbers ::= number (OPTIMIZED OUT) */ assert(yyruleno!=223);
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
#line 54 "parser.yy"

    state->error = PARSE_FAILURE;
#if print_errors
    fprintf(stderr,"Giving up.  Parser is hopelessly lost...\n");
#endif
#line 2765 "parser.c"
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
#line 47 "parser.yy"

    state->error = SYNTAX_ERROR;
#if print_errors
    fprintf(stderr, "Syntax error\n");
#endif
#line 2788 "parser.c"
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
