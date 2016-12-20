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
#define TOKEN_QUARTERS                        4
#define TOKEN_HALF                            5
#define TOKEN_HALVES                          6
#define TOKEN_NUMBER                          7
#define TOKEN_ZERO                            8
#define TOKEN_TRILLION                        9
#define TOKEN_TRILLIONTH                     10
#define TOKEN_TRILLIONTHS                    11
#define TOKEN_BILLION                        12
#define TOKEN_BILLIONTH                      13
#define TOKEN_BILLIONTHS                     14
#define TOKEN_MILLION                        15
#define TOKEN_MILLIONTH                      16
#define TOKEN_MILLIONTHS                     17
#define TOKEN_THOUSAND                       18
#define TOKEN_THOUSANDTH                     19
#define TOKEN_THOUSANDTHS                    20
#define TOKEN_AND                            21
#define TOKEN_HUNDRED                        22
#define TOKEN_HUNDREDTH                      23
#define TOKEN_HUNDREDTHS                     24
#define TOKEN_ONE                            25
#define TOKEN_TWO                            26
#define TOKEN_THREE                          27
#define TOKEN_FOUR                           28
#define TOKEN_FIVE                           29
#define TOKEN_SIX                            30
#define TOKEN_SEVEN                          31
#define TOKEN_EIGHT                          32
#define TOKEN_NINE                           33
#define TOKEN_TEN                            34
#define TOKEN_ELEVEN                         35
#define TOKEN_TWELVE                         36
#define TOKEN_THIRTEEN                       37
#define TOKEN_FOURTEEN                       38
#define TOKEN_FIFTEEN                        39
#define TOKEN_SIXTEEN                        40
#define TOKEN_SEVENTEEN                      41
#define TOKEN_EIGHTEEN                       42
#define TOKEN_NINETEEN                       43
#define TOKEN_TWENTY                         44
#define TOKEN_THIRTY                         45
#define TOKEN_FORTY                          46
#define TOKEN_FIFTY                          47
#define TOKEN_SIXTY                          48
#define TOKEN_SEVENTY                        49
#define TOKEN_EIGHTY                         50
#define TOKEN_NINETY                         51
#define TOKEN_FIRST                          52
#define TOKEN_SECOND                         53
#define TOKEN_THIRD                          54
#define TOKEN_FOURTH                         55
#define TOKEN_FIFTH                          56
#define TOKEN_SIXTH                          57
#define TOKEN_SEVENTH                        58
#define TOKEN_EIGHTH                         59
#define TOKEN_NINTH                          60
#define TOKEN_TENTH                          61
#define TOKEN_ELEVENTH                       62
#define TOKEN_TWELFTH                        63
#define TOKEN_THIRTEENTH                     64
#define TOKEN_FOURTEENTH                     65
#define TOKEN_FIFTEENTH                      66
#define TOKEN_SIXTEENTH                      67
#define TOKEN_SEVENTEENTH                    68
#define TOKEN_EIGHTEENTH                     69
#define TOKEN_NINETEENTH                     70
#define TOKEN_TWENTIETH                      71
#define TOKEN_THIRTIETH                      72
#define TOKEN_FOURTIETH                      73
#define TOKEN_FIFTIETH                       74
#define TOKEN_SIXTIETH                       75
#define TOKEN_SEVENTIETH                     76
#define TOKEN_EIGHTIETH                      77
#define TOKEN_NINETIETH                      78
#define TOKEN_FIRSTS                         79
#define TOKEN_THIRDS                         80
#define TOKEN_FOURTHS                        81
#define TOKEN_FIFTHS                         82
#define TOKEN_SIXTHS                         83
#define TOKEN_SEVENTHS                       84
#define TOKEN_EIGHTHS                        85
#define TOKEN_NINTHS                         86
#define TOKEN_TENTHS                         87
#define TOKEN_ELEVENTHS                      88
#define TOKEN_TWELFTHS                       89
#define TOKEN_THIRTEENTHS                    90
#define TOKEN_FOURTEENTHS                    91
#define TOKEN_FIFTEENTHS                     92
#define TOKEN_SIXTEENTHS                     93
#define TOKEN_SEVENTEENTHS                   94
#define TOKEN_EIGHTEENTHS                    95
#define TOKEN_NINETEENTHS                    96
#define TOKEN_TWENTIETHS                     97
#define TOKEN_THIRTIETHS                     98
#define TOKEN_FOURTIETHS                     99
#define TOKEN_FIFTIETHS                      100
#define TOKEN_SIXTIETHS                      101
#define TOKEN_SEVENTIETHS                    102
#define TOKEN_EIGHTIETHS                     103
#define TOKEN_NINETIETHS                     104
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
#define YYNOCODE 153
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
#define YYNSTATE             19
#define YYNRULE              211
#define YY_MAX_SHIFT         18
#define YY_MIN_SHIFTREDUCE   227
#define YY_MAX_SHIFTREDUCE   437
#define YY_MIN_REDUCE        438
#define YY_MAX_REDUCE        648
#define YY_ERROR_ACTION      649
#define YY_ACCEPT_ACTION     650
#define YY_NO_ACTION         651
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
#define YY_ACTTAB_COUNT (935)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   646,    4,    7,  320,  328,  440,  440,   11,  237,  251,
 /*    10 */   241,  246,  272,  258,  265,  293,  279,  286,  314,  300,
 /*    20 */   307,  440,  338,  322,  330,  355,  356,  357,  358,  359,
 /*    30 */   360,  361,  362,  363,  364,  365,  366,  367,  368,  369,
 /*    40 */   370,  371,  372,  373,  374,  375,  376,  377,  378,  379,
 /*    50 */   380,  381,  382,  383,  384,  385,  386,  387,  388,  389,
 /*    60 */   390,  391,  392,  393,  394,  395,  396,  397,  398,  399,
 /*    70 */   400,  401,  402,  403,  404,  405,  406,  407,  408,  409,
 /*    80 */   410,  411,  412,  413,  414,  415,  416,  417,  418,  419,
 /*    90 */   420,  421,  422,  423,  424,  425,  426,  427,  428,  429,
 /*   100 */   430,  431,  432,  433,  434,    4,    6,  298,  305,  440,
 /*   110 */   440,   11,  237,  251,  241,  246,  272,  258,  265,  293,
 /*   120 */   279,  286,  314,  300,  307,  440,  338,  322,  330,  355,
 /*   130 */   356,  357,  358,  359,  360,  361,  362,  363,  364,  365,
 /*   140 */   366,  367,  368,  369,  370,  371,  372,  373,  374,  375,
 /*   150 */   376,  377,  378,  379,  380,  381,  382,  383,  384,  385,
 /*   160 */   386,  387,  388,  389,  390,  391,  392,  393,  394,  395,
 /*   170 */   396,  397,  398,  399,  400,  401,  402,  403,  404,  405,
 /*   180 */   406,  407,  408,  409,  410,  411,  412,  413,  414,  415,
 /*   190 */   416,  417,  418,  419,  420,  421,  422,  423,  424,  425,
 /*   200 */   426,  427,  428,  429,  430,  431,  432,  433,  434,   11,
 /*   210 */   237,  251,  241,  246,  272,  258,  265,  293,  279,  286,
 /*   220 */   314,  300,  307,  440,  338,  322,  330,  355,  356,  357,
 /*   230 */   358,  359,  360,  361,  362,  363,  364,  365,  366,  367,
 /*   240 */   368,  369,  370,  371,  372,  373,  374,  375,  376,  377,
 /*   250 */   378,  379,  380,  381,  382,  383,  384,  385,  386,  387,
 /*   260 */   388,  389,  390,  391,  392,  393,  394,  395,  396,  397,
 /*   270 */   398,  399,  400,  401,  402,  403,  404,  405,  406,  407,
 /*   280 */   408,  409,  410,  411,  412,  413,  414,  415,  416,  417,
 /*   290 */   418,  419,  420,  421,  422,  423,  424,  425,  426,  427,
 /*   300 */   428,  429,  430,  431,  432,  433,  434,    9,  229,  440,
 /*   310 */   231,  355,  356,  357,  358,  359,  360,  361,  362,  363,
 /*   320 */   364,  365,  366,  367,  368,  369,  370,  371,  372,  373,
 /*   330 */   374,  375,  376,  377,  378,  379,  380,  381,  382,  383,
 /*   340 */   384,  385,  386,  387,  388,  389,  390,  391,  392,  393,
 /*   350 */   394,  395,  396,  397,  398,  399,  400,  401,  402,  403,
 /*   360 */   404,  405,  406,  407,  408,  409,  410,  411,  412,  413,
 /*   370 */   414,  415,  416,  417,  418,  419,  420,  421,  422,  423,
 /*   380 */   424,  425,  426,  427,  428,  429,  430,  431,  432,  433,
 /*   390 */   434,    8,  440,  440,  440,  355,  356,  357,  358,  359,
 /*   400 */   360,  361,  362,  363,  364,  365,  366,  367,  368,  369,
 /*   410 */   370,  371,  372,  373,  374,  375,  376,  377,  378,  379,
 /*   420 */   380,  381,  382,  383,  384,  385,  386,  387,  388,  389,
 /*   430 */   390,  391,  392,  393,  394,  395,  396,  397,  398,  399,
 /*   440 */   400,  401,  402,  403,  404,  405,  406,  407,  408,  409,
 /*   450 */   410,  411,  412,  413,  414,  415,  416,  417,  418,  419,
 /*   460 */   420,  421,  422,  423,  424,  425,  426,  427,  428,  429,
 /*   470 */   430,  431,  432,  433,  434,  355,  356,  357,  358,  359,
 /*   480 */   360,  361,  362,  363,  364,  365,  366,  367,  368,  369,
 /*   490 */   370,  371,  372,  373,  374,  375,  376,  377,  378,  379,
 /*   500 */   380,  381,  382,  383,  384,  385,  386,  387,  388,  389,
 /*   510 */   390,  391,  392,  393,  394,  395,  396,  397,  398,  399,
 /*   520 */   400,  401,  402,  403,  404,  405,  406,  407,  408,  409,
 /*   530 */   410,  411,  412,  413,  414,  415,  416,  417,  418,  419,
 /*   540 */   420,  421,  422,  423,  424,  425,  426,  427,  428,  429,
 /*   550 */   430,  431,  432,  433,  434,   13,  238,  253,  243,  260,
 /*   560 */   248,  267,  254,  257,  261,  264,  268,  271,  275,  278,
 /*   570 */   282,  285,  289,  292,  296,  299,  303,  306,  310,  321,
 /*   580 */    15,  329,   10,  350,  341,  342,  352,  344,  345,  354,
 /*   590 */   348,  349,  351,  353,  650,    3,    3,  227,   17,  234,
 /*   600 */   235,   12,   18,  240,  230,  245,  232,  250,  440,  257,
 /*   610 */   440,  264,  440,  271,  440,  278,  440,  285,  440,  292,
 /*   620 */   440,  299,  440,  306,  440,  321,   15,  329,   10,  350,
 /*   630 */   341,  342,  352,  344,  345,  354,  348,  349,  351,  353,
 /*   640 */    14,    5,  277,  284,    6,  298,  305,  255,  274,  262,
 /*   650 */   281,  269,  288,  275,  278,  282,  285,  289,  292,  296,
 /*   660 */   299,  303,  306,  310,  321,   15,  329,   10,  350,  341,
 /*   670 */   342,  352,  344,  345,  354,  348,  349,  351,  353,  436,
 /*   680 */   227,   17,  234,  235,   12,  440,  240,  440,  245,  440,
 /*   690 */   250,  440,  257,  440,  264,  440,  271,  440,  278,  440,
 /*   700 */   285,  440,  292,  440,  299,  440,  306,  440,  321,   15,
 /*   710 */   329,   10,  350,  341,  342,  352,  344,  345,  354,  348,
 /*   720 */   349,  351,  353,  228,   17,  234,  235,   12,  440,  240,
 /*   730 */   440,  245,  440,  250,  440,  257,  440,  264,  440,  271,
 /*   740 */   440,  278,  440,  285,  440,  292,  440,  299,  440,  306,
 /*   750 */   440,  321,   15,  329,   10,  350,  341,  342,  352,  344,
 /*   760 */   345,  354,  348,  349,  351,  353,   16,    1,  239,  244,
 /*   770 */     2,  256,  263,    5,  277,  284,    6,  298,  305,  276,
 /*   780 */   295,  283,  302,  290,  309,  296,  299,  303,  306,  310,
 /*   790 */   321,   15,  329,   10,  350,  341,  342,  352,  344,  345,
 /*   800 */   354,  348,  349,  351,  353,  355,  356,  357,  358,  359,
 /*   810 */   360,  361,  362,  363,  319,  335,  327,   10,  350,  341,
 /*   820 */   342,  352,  344,  345,  354,  348,  349,  351,  353,  440,
 /*   830 */   440,  440,  382,  383,  384,  385,  386,  387,  388,  389,
 /*   840 */   390,  440,  340,  440,  440,  343,  440,  440,  346,  333,
 /*   850 */     2,  256,  263,    5,  277,  284,    6,  298,  305,  409,
 /*   860 */   410,  411,  412,  413,  414,  415,  416,  440,  297,  317,
 /*   870 */   304,  325,  311,  321,   15,  329,   10,  350,  341,  342,
 /*   880 */   352,  344,  345,  354,  348,  349,  351,  353,  440,  318,
 /*   890 */   334,  326,   10,  350,  341,  342,  352,  344,  345,  354,
 /*   900 */   348,  349,  351,  353,  316,  332,  324,   10,  350,  341,
 /*   910 */   342,  352,  344,  345,  354,  348,  349,  351,  353,  252,
 /*   920 */   242,  247,  273,  259,  266,  294,  280,  287,  315,  301,
 /*   930 */   308,  440,  339,  323,  331,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     0,    1,   22,   23,   24,  152,  152,    7,    8,    9,
 /*    10 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*    20 */    20,  152,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    30 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*    40 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*    50 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*    60 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*    70 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*    80 */    80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
 /*    90 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*   100 */   100,  101,  102,  103,  104,    1,   18,   19,   20,  152,
 /*   110 */   152,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   120 */    16,   17,   18,   19,   20,  152,   22,   23,   24,   25,
 /*   130 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   140 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*   150 */    46,   47,   48,   49,   50,   51,   52,   53,   54,   55,
 /*   160 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*   170 */    66,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*   180 */    76,   77,   78,   79,   80,   81,   82,   83,   84,   85,
 /*   190 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*   200 */    96,   97,   98,   99,  100,  101,  102,  103,  104,    7,
 /*   210 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   220 */    18,   19,   20,  152,   22,   23,   24,   25,   26,   27,
 /*   230 */    28,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*   240 */    38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
 /*   250 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   260 */    58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   270 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   280 */    78,   79,   80,   81,   82,   83,   84,   85,   86,   87,
 /*   290 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*   300 */    98,   99,  100,  101,  102,  103,  104,   21,    3,  152,
 /*   310 */     5,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   320 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*   330 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   340 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*   350 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*   360 */    74,   75,   76,   77,   78,   79,   80,   81,   82,   83,
 /*   370 */    84,   85,   86,   87,   88,   89,   90,   91,   92,   93,
 /*   380 */    94,   95,   96,   97,   98,   99,  100,  101,  102,  103,
 /*   390 */   104,   21,  152,  152,  152,   25,   26,   27,   28,   29,
 /*   400 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   410 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*   420 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   430 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   440 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   450 */    80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
 /*   460 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*   470 */   100,  101,  102,  103,  104,   25,   26,   27,   28,   29,
 /*   480 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   490 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*   500 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   510 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   520 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   530 */    80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
 /*   540 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*   550 */   100,  101,  102,  103,  104,  113,  114,  115,  116,  117,
 /*   560 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*   570 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*   580 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*   590 */   148,  149,  150,  151,  106,  107,  108,  109,  110,  111,
 /*   600 */   112,  113,    2,  115,    4,  117,    6,  119,  152,  121,
 /*   610 */   152,  123,  152,  125,  152,  127,  152,  129,  152,  131,
 /*   620 */   152,  133,  152,  135,  152,  137,  138,  139,  140,  141,
 /*   630 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*   640 */   113,   15,   16,   17,   18,   19,   20,  120,  121,  122,
 /*   650 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*   660 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*   670 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  108,
 /*   680 */   109,  110,  111,  112,  113,  152,  115,  152,  117,  152,
 /*   690 */   119,  152,  121,  152,  123,  152,  125,  152,  127,  152,
 /*   700 */   129,  152,  131,  152,  133,  152,  135,  152,  137,  138,
 /*   710 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*   720 */   149,  150,  151,  109,  110,  111,  112,  113,  152,  115,
 /*   730 */   152,  117,  152,  119,  152,  121,  152,  123,  152,  125,
 /*   740 */   152,  127,  152,  129,  152,  131,  152,  133,  152,  135,
 /*   750 */   152,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*   760 */   146,  147,  148,  149,  150,  151,  113,    9,   10,   11,
 /*   770 */    12,   13,   14,   15,   16,   17,   18,   19,   20,  126,
 /*   780 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*   790 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*   800 */   147,  148,  149,  150,  151,   25,   26,   27,   28,   29,
 /*   810 */    30,   31,   32,   33,  137,  138,  139,  140,  141,  142,
 /*   820 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*   830 */   152,  152,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   840 */    60,  152,  141,  152,  152,  144,  152,  152,  147,  113,
 /*   850 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   79,
 /*   860 */    80,   81,   82,   83,   84,   85,   86,  152,  132,  133,
 /*   870 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*   880 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  137,
 /*   890 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*   900 */   148,  149,  150,  151,  137,  138,  139,  140,  141,  142,
 /*   910 */   143,  144,  145,  146,  147,  148,  149,  150,  151,    9,
 /*   920 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*   930 */    20,  152,   22,   23,   24,
};
#define YY_SHIFT_USE_DFLT (935)
#define YY_SHIFT_COUNT    (18)
#define YY_SHIFT_MIN      (-20)
#define YY_SHIFT_MAX      (910)
static const short yy_shift_ofst[] = {
 /*     0 */   104,  286,  286,    0,  202,  286,  286,  370,  450,  450,
 /*    10 */   780,  910,  758,  838,  626,  -20,   88,  600,  305,
};
#define YY_REDUCE_USE_DFLT (-1)
#define YY_REDUCE_COUNT (10)
#define YY_REDUCE_MIN   (0)
#define YY_REDUCE_MAX   (767)
static const short yy_reduce_ofst[] = {
 /*     0 */   488,  442,  527,  571,  614,  653,  736,  677,  752,  767,
 /*    10 */   701,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   649,  460,  481,  649,  649,  502,  523,  547,  649,  649,
 /*    10 */   558,  447,  524,  524,  524,  548,  524,  444,  649,
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
  "QUARTERS",      "HALF",          "HALVES",        "NUMBER",      
  "ZERO",          "TRILLION",      "TRILLIONTH",    "TRILLIONTHS", 
  "BILLION",       "BILLIONTH",     "BILLIONTHS",    "MILLION",     
  "MILLIONTH",     "MILLIONTHS",    "THOUSAND",      "THOUSANDTH",  
  "THOUSANDTHS",   "AND",           "HUNDRED",       "HUNDREDTH",   
  "HUNDREDTHS",    "ONE",           "TWO",           "THREE",       
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
  "THIRDS",        "FOURTHS",       "FIFTHS",        "SIXTHS",      
  "SEVENTHS",      "EIGHTHS",       "NINTHS",        "TENTHS",      
  "ELEVENTHS",     "TWELFTHS",      "THIRTEENTHS",   "FOURTEENTHS", 
  "FIFTEENTHS",    "SIXTEENTHS",    "SEVENTEENTHS",  "EIGHTEENTHS", 
  "NINETEENTHS",   "TWENTIETHS",    "THIRTIETHS",    "FOURTIETHS",  
  "FIFTIETHS",     "SIXTIETHS",     "SEVENTIETHS",   "EIGHTIETHS",  
  "NINETIETHS",    "error",         "program",       "numbers",     
  "number",        "final_number",  "less_than_quadrillion",  "less_than_quadrillionth",
  "less_than_quadrillionths",  "less_than_thousand",  "less_than_trillionth_end_only",  "less_than_trillionth",
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
 /*   3 */ "final_number ::= less_than_quadrillion QUARTERS",
 /*   4 */ "final_number ::= less_than_quadrillion AND_A HALF",
 /*   5 */ "final_number ::= less_than_quadrillion HALVES",
 /*   6 */ "final_number ::= less_than_quadrillion",
 /*   7 */ "final_number ::= less_than_quadrillionth",
 /*   8 */ "final_number ::= less_than_quadrillionths",
 /*   9 */ "final_number ::= NUMBER",
 /*  10 */ "final_number ::= ZERO",
 /*  11 */ "less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only",
 /*  12 */ "less_than_quadrillionth ::= less_than_thousand TRILLIONTH",
 /*  13 */ "less_than_quadrillionth ::= less_than_trillionth",
 /*  14 */ "final_number ::= TRILLIONTH",
 /*  15 */ "final_number ::= NUMBER TRILLIONTH",
 /*  16 */ "less_than_quadrillionths ::= less_than_thousand TRILLION less_than_trillionths_end_only",
 /*  17 */ "less_than_quadrillionths ::= less_than_thousand TRILLIONTHS",
 /*  18 */ "less_than_quadrillionths ::= less_than_trillionths",
 /*  19 */ "final_number ::= TRILLIONTHS",
 /*  20 */ "final_number ::= NUMBER TRILLIONTHS",
 /*  21 */ "less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only",
 /*  22 */ "less_than_quadrillion ::= less_than_thousand TRILLION",
 /*  23 */ "less_than_quadrillion ::= less_than_trillion",
 /*  24 */ "final_number ::= TRILLION",
 /*  25 */ "final_number ::= NUMBER TRILLION",
 /*  26 */ "less_than_trillionth_end_only ::= less_than_trillionth",
 /*  27 */ "less_than_trillionth_end_only ::= less_than_billionth_end_only",
 /*  28 */ "less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only",
 /*  29 */ "less_than_trillionth ::= less_than_thousand BILLIONTH",
 /*  30 */ "less_than_trillionth ::= less_than_billionth",
 /*  31 */ "final_number ::= BILLIONTH",
 /*  32 */ "final_number ::= NUMBER BILLIONTH",
 /*  33 */ "less_than_trillionths_end_only ::= less_than_trillionths",
 /*  34 */ "less_than_trillionths_end_only ::= less_than_billionths_end_only",
 /*  35 */ "less_than_trillionths ::= less_than_thousand BILLION less_than_billionths_end_only",
 /*  36 */ "less_than_trillionths ::= less_than_thousand BILLIONTHS",
 /*  37 */ "less_than_trillionths ::= less_than_billionths",
 /*  38 */ "final_number ::= BILLIONTHS",
 /*  39 */ "final_number ::= NUMBER BILLIONTHS",
 /*  40 */ "less_than_trillion_end_only ::= less_than_trillion",
 /*  41 */ "less_than_trillion_end_only ::= less_than_billion_end_only",
 /*  42 */ "less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only",
 /*  43 */ "less_than_trillion ::= less_than_thousand BILLION",
 /*  44 */ "less_than_trillion ::= less_than_billion",
 /*  45 */ "final_number ::= BILLION",
 /*  46 */ "final_number ::= NUMBER BILLION",
 /*  47 */ "less_than_billionth_end_only ::= less_than_billionth",
 /*  48 */ "less_than_billionth_end_only ::= less_than_millionth_end_only",
 /*  49 */ "less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only",
 /*  50 */ "less_than_billionth ::= less_than_thousand MILLIONTH",
 /*  51 */ "less_than_billionth ::= less_than_millionth",
 /*  52 */ "final_number ::= MILLIONTH",
 /*  53 */ "final_number ::= NUMBER MILLIONTH",
 /*  54 */ "less_than_billionths_end_only ::= less_than_billionths",
 /*  55 */ "less_than_billionths_end_only ::= less_than_millionths_end_only",
 /*  56 */ "less_than_billionths ::= less_than_thousand MILLION less_than_millionths_end_only",
 /*  57 */ "less_than_billionths ::= less_than_thousand MILLIONTHS",
 /*  58 */ "less_than_billionths ::= less_than_millionths",
 /*  59 */ "final_number ::= MILLIONTHS",
 /*  60 */ "final_number ::= NUMBER MILLIONTHS",
 /*  61 */ "less_than_billion_end_only ::= less_than_billion",
 /*  62 */ "less_than_billion_end_only ::= less_than_million_end_only",
 /*  63 */ "less_than_billion ::= less_than_thousand MILLION less_than_million_end_only",
 /*  64 */ "less_than_billion ::= less_than_thousand MILLION",
 /*  65 */ "less_than_billion ::= less_than_million",
 /*  66 */ "final_number ::= MILLION",
 /*  67 */ "final_number ::= NUMBER MILLION",
 /*  68 */ "less_than_millionth_end_only ::= less_than_millionth",
 /*  69 */ "less_than_millionth_end_only ::= less_than_thousandth_end_only",
 /*  70 */ "less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only",
 /*  71 */ "less_than_millionth ::= less_than_thousand THOUSANDTH",
 /*  72 */ "less_than_millionth ::= less_than_thousandth",
 /*  73 */ "final_number ::= THOUSANDTH",
 /*  74 */ "final_number ::= NUMBER THOUSANDTH",
 /*  75 */ "less_than_millionths_end_only ::= less_than_millionths",
 /*  76 */ "less_than_millionths_end_only ::= less_than_thousandths_end_only",
 /*  77 */ "less_than_millionths ::= less_than_thousand THOUSAND less_than_thousandths_end_only",
 /*  78 */ "less_than_millionths ::= less_than_thousand THOUSANDTHS",
 /*  79 */ "less_than_millionths ::= less_than_thousandths",
 /*  80 */ "final_number ::= THOUSANDTHS",
 /*  81 */ "final_number ::= NUMBER THOUSANDTHS",
 /*  82 */ "less_than_million_end_only ::= less_than_million",
 /*  83 */ "less_than_million_end_only ::= less_than_thousand_end_only",
 /*  84 */ "less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only",
 /*  85 */ "less_than_million ::= less_than_thousand THOUSAND",
 /*  86 */ "less_than_million ::= less_than_thousand",
 /*  87 */ "final_number ::= THOUSAND",
 /*  88 */ "final_number ::= NUMBER THOUSAND",
 /*  89 */ "less_than_thousandth_end_only ::= AND less_than_hundredth",
 /*  90 */ "less_than_thousandth_end_only ::= less_than_thousandth",
 /*  91 */ "less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth",
 /*  92 */ "less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth",
 /*  93 */ "less_than_thousandth ::= less_than_hundred HUNDREDTH",
 /*  94 */ "less_than_thousandth ::= less_than_hundredth",
 /*  95 */ "final_number ::= HUNDREDTH",
 /*  96 */ "final_number ::= NUMBER HUNDREDTH",
 /*  97 */ "less_than_thousandths_end_only ::= AND less_than_hundredths",
 /*  98 */ "less_than_thousandths_end_only ::= less_than_thousandths",
 /*  99 */ "less_than_thousandths ::= less_than_hundred HUNDRED AND less_than_hundredths",
 /* 100 */ "less_than_thousandths ::= less_than_hundred HUNDRED less_than_hundredths",
 /* 101 */ "less_than_thousandths ::= less_than_hundred HUNDREDTHS",
 /* 102 */ "less_than_thousandths ::= less_than_hundredths",
 /* 103 */ "final_number ::= HUNDREDTHS",
 /* 104 */ "final_number ::= NUMBER HUNDREDTHS",
 /* 105 */ "less_than_thousand_end_only ::= AND less_than_hundred",
 /* 106 */ "less_than_thousand_end_only ::= less_than_thousand",
 /* 107 */ "less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred",
 /* 108 */ "less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred",
 /* 109 */ "less_than_thousand ::= less_than_hundred HUNDRED",
 /* 110 */ "less_than_thousand ::= less_than_hundred",
 /* 111 */ "final_number ::= HUNDRED",
 /* 112 */ "final_number ::= NUMBER HUNDRED",
 /* 113 */ "less_than_hundredth ::= tens less_than_tenth",
 /* 114 */ "less_than_hundredth ::= tenth",
 /* 115 */ "less_than_hundredth ::= less_than_twentieth",
 /* 116 */ "less_than_hundredths ::= tens less_than_tenths",
 /* 117 */ "less_than_hundredths ::= tenths",
 /* 118 */ "less_than_hundredths ::= less_than_twentieths",
 /* 119 */ "less_than_hundred ::= tens less_than_ten",
 /* 120 */ "less_than_hundred ::= tens",
 /* 121 */ "less_than_hundred ::= less_than_twenty",
 /* 122 */ "less_than_twentieth ::= tenth_to_19th",
 /* 123 */ "less_than_twentieth ::= less_than_tenth",
 /* 124 */ "less_than_twentieths ::= tenths_to_19ths",
 /* 125 */ "less_than_twentieths ::= less_than_tenths",
 /* 126 */ "less_than_twenty ::= ten_to_19",
 /* 127 */ "less_than_twenty ::= less_than_ten",
 /* 128 */ "less_than_ten ::= ONE",
 /* 129 */ "less_than_ten ::= TWO",
 /* 130 */ "less_than_ten ::= THREE",
 /* 131 */ "less_than_ten ::= FOUR",
 /* 132 */ "less_than_ten ::= FIVE",
 /* 133 */ "less_than_ten ::= SIX",
 /* 134 */ "less_than_ten ::= SEVEN",
 /* 135 */ "less_than_ten ::= EIGHT",
 /* 136 */ "less_than_ten ::= NINE",
 /* 137 */ "ten_to_19 ::= TEN",
 /* 138 */ "ten_to_19 ::= ELEVEN",
 /* 139 */ "ten_to_19 ::= TWELVE",
 /* 140 */ "ten_to_19 ::= THIRTEEN",
 /* 141 */ "ten_to_19 ::= FOURTEEN",
 /* 142 */ "ten_to_19 ::= FIFTEEN",
 /* 143 */ "ten_to_19 ::= SIXTEEN",
 /* 144 */ "ten_to_19 ::= SEVENTEEN",
 /* 145 */ "ten_to_19 ::= EIGHTEEN",
 /* 146 */ "ten_to_19 ::= NINETEEN",
 /* 147 */ "tens ::= TWENTY",
 /* 148 */ "tens ::= THIRTY",
 /* 149 */ "tens ::= FORTY",
 /* 150 */ "tens ::= FIFTY",
 /* 151 */ "tens ::= SIXTY",
 /* 152 */ "tens ::= SEVENTY",
 /* 153 */ "tens ::= EIGHTY",
 /* 154 */ "tens ::= NINETY",
 /* 155 */ "less_than_tenth ::= FIRST",
 /* 156 */ "less_than_tenth ::= SECOND",
 /* 157 */ "less_than_tenth ::= THIRD",
 /* 158 */ "less_than_tenth ::= FOURTH",
 /* 159 */ "less_than_tenth ::= FIFTH",
 /* 160 */ "less_than_tenth ::= SIXTH",
 /* 161 */ "less_than_tenth ::= SEVENTH",
 /* 162 */ "less_than_tenth ::= EIGHTH",
 /* 163 */ "less_than_tenth ::= NINTH",
 /* 164 */ "tenth_to_19th ::= TENTH",
 /* 165 */ "tenth_to_19th ::= ELEVENTH",
 /* 166 */ "tenth_to_19th ::= TWELFTH",
 /* 167 */ "tenth_to_19th ::= THIRTEENTH",
 /* 168 */ "tenth_to_19th ::= FOURTEENTH",
 /* 169 */ "tenth_to_19th ::= FIFTEENTH",
 /* 170 */ "tenth_to_19th ::= SIXTEENTH",
 /* 171 */ "tenth_to_19th ::= SEVENTEENTH",
 /* 172 */ "tenth_to_19th ::= EIGHTEENTH",
 /* 173 */ "tenth_to_19th ::= NINETEENTH",
 /* 174 */ "tenth ::= TWENTIETH",
 /* 175 */ "tenth ::= THIRTIETH",
 /* 176 */ "tenth ::= FOURTIETH",
 /* 177 */ "tenth ::= FIFTIETH",
 /* 178 */ "tenth ::= SIXTIETH",
 /* 179 */ "tenth ::= SEVENTIETH",
 /* 180 */ "tenth ::= EIGHTIETH",
 /* 181 */ "tenth ::= NINETIETH",
 /* 182 */ "less_than_tenths ::= FIRSTS",
 /* 183 */ "less_than_tenths ::= THIRDS",
 /* 184 */ "less_than_tenths ::= FOURTHS",
 /* 185 */ "less_than_tenths ::= FIFTHS",
 /* 186 */ "less_than_tenths ::= SIXTHS",
 /* 187 */ "less_than_tenths ::= SEVENTHS",
 /* 188 */ "less_than_tenths ::= EIGHTHS",
 /* 189 */ "less_than_tenths ::= NINTHS",
 /* 190 */ "tenths_to_19ths ::= TENTHS",
 /* 191 */ "tenths_to_19ths ::= ELEVENTHS",
 /* 192 */ "tenths_to_19ths ::= TWELFTHS",
 /* 193 */ "tenths_to_19ths ::= THIRTEENTHS",
 /* 194 */ "tenths_to_19ths ::= FOURTEENTHS",
 /* 195 */ "tenths_to_19ths ::= FIFTEENTHS",
 /* 196 */ "tenths_to_19ths ::= SIXTEENTHS",
 /* 197 */ "tenths_to_19ths ::= SEVENTEENTHS",
 /* 198 */ "tenths_to_19ths ::= EIGHTEENTHS",
 /* 199 */ "tenths_to_19ths ::= NINETEENTHS",
 /* 200 */ "tenths ::= TWENTIETHS",
 /* 201 */ "tenths ::= THIRTIETHS",
 /* 202 */ "tenths ::= FOURTIETHS",
 /* 203 */ "tenths ::= FIFTIETHS",
 /* 204 */ "tenths ::= SIXTIETHS",
 /* 205 */ "tenths ::= SEVENTIETHS",
 /* 206 */ "tenths ::= EIGHTIETHS",
 /* 207 */ "tenths ::= NINETIETHS",
 /* 208 */ "program ::= numbers",
 /* 209 */ "numbers ::= numbers number",
 /* 210 */ "numbers ::= number",
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
  { 108, 1 },
  { 108, 2 },
  { 109, 3 },
  { 109, 2 },
  { 109, 3 },
  { 109, 2 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 111, 3 },
  { 111, 2 },
  { 111, 1 },
  { 109, 1 },
  { 109, 2 },
  { 112, 3 },
  { 112, 2 },
  { 112, 1 },
  { 109, 1 },
  { 109, 2 },
  { 110, 3 },
  { 110, 2 },
  { 110, 1 },
  { 109, 1 },
  { 109, 2 },
  { 114, 1 },
  { 114, 1 },
  { 115, 3 },
  { 115, 2 },
  { 115, 1 },
  { 109, 1 },
  { 109, 2 },
  { 116, 1 },
  { 116, 1 },
  { 117, 3 },
  { 117, 2 },
  { 117, 1 },
  { 109, 1 },
  { 109, 2 },
  { 118, 1 },
  { 118, 1 },
  { 119, 3 },
  { 119, 2 },
  { 119, 1 },
  { 109, 1 },
  { 109, 2 },
  { 120, 1 },
  { 120, 1 },
  { 121, 3 },
  { 121, 2 },
  { 121, 1 },
  { 109, 1 },
  { 109, 2 },
  { 122, 1 },
  { 122, 1 },
  { 123, 3 },
  { 123, 2 },
  { 123, 1 },
  { 109, 1 },
  { 109, 2 },
  { 124, 1 },
  { 124, 1 },
  { 125, 3 },
  { 125, 2 },
  { 125, 1 },
  { 109, 1 },
  { 109, 2 },
  { 126, 1 },
  { 126, 1 },
  { 127, 3 },
  { 127, 2 },
  { 127, 1 },
  { 109, 1 },
  { 109, 2 },
  { 128, 1 },
  { 128, 1 },
  { 129, 3 },
  { 129, 2 },
  { 129, 1 },
  { 109, 1 },
  { 109, 2 },
  { 130, 1 },
  { 130, 1 },
  { 131, 3 },
  { 131, 2 },
  { 131, 1 },
  { 109, 1 },
  { 109, 2 },
  { 132, 2 },
  { 132, 1 },
  { 133, 4 },
  { 133, 3 },
  { 133, 2 },
  { 133, 1 },
  { 109, 1 },
  { 109, 2 },
  { 134, 2 },
  { 134, 1 },
  { 135, 4 },
  { 135, 3 },
  { 135, 2 },
  { 135, 1 },
  { 109, 1 },
  { 109, 2 },
  { 136, 2 },
  { 136, 1 },
  { 113, 4 },
  { 113, 3 },
  { 113, 2 },
  { 113, 1 },
  { 109, 1 },
  { 109, 2 },
  { 137, 2 },
  { 137, 1 },
  { 137, 1 },
  { 139, 2 },
  { 139, 1 },
  { 139, 1 },
  { 138, 2 },
  { 138, 1 },
  { 138, 1 },
  { 143, 1 },
  { 143, 1 },
  { 146, 1 },
  { 146, 1 },
  { 148, 1 },
  { 148, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 106, 1 },
  { 107, 2 },
  { 107, 1 },
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
#line 1544 "parser.c"
        break;
      case 1: /* number ::= NEGATIVE final_number */
#line 71 "parser.yy"
{
    yymsp[0].minor.yy0.dbl = -yymsp[0].minor.yy0.dbl;
    yymsp[0].minor.yy0.begin = yymsp[-1].minor.yy0.begin;
    yymsp[0].minor.yy0.end = yymsp[0].minor.yy0.end;
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1554 "parser.c"
        break;
      case 2: /* final_number ::= less_than_quadrillion AND_A QUARTER */
#line 79 "parser.yy"
{ COPY_YYSTYPE_BE2(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[-2].minor.yy0.dbl + 0.25; yylhsminor.yy0.is_dbl = true; }
#line 1559 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* final_number ::= less_than_quadrillion QUARTERS */
#line 80 "parser.yy"
{ COPY_YYSTYPE_BE2(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl / 4.0; yylhsminor.yy0.is_dbl = true; }
#line 1565 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 4: /* final_number ::= less_than_quadrillion AND_A HALF */
#line 81 "parser.yy"
{ COPY_YYSTYPE_BE2(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[-2].minor.yy0.dbl + 0.5; yylhsminor.yy0.is_dbl = true; }
#line 1571 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* final_number ::= less_than_quadrillion HALVES */
#line 82 "parser.yy"
{ COPY_YYSTYPE_BE2(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl / 2.0; yylhsminor.yy0.is_dbl = true; }
#line 1577 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 6: /* final_number ::= less_than_quadrillion */
      case 23: /* less_than_quadrillion ::= less_than_trillion */ yytestcase(yyruleno==23);
      case 40: /* less_than_trillion_end_only ::= less_than_trillion */ yytestcase(yyruleno==40);
      case 41: /* less_than_trillion_end_only ::= less_than_billion_end_only */ yytestcase(yyruleno==41);
      case 44: /* less_than_trillion ::= less_than_billion */ yytestcase(yyruleno==44);
      case 61: /* less_than_billion_end_only ::= less_than_billion */ yytestcase(yyruleno==61);
      case 62: /* less_than_billion_end_only ::= less_than_million_end_only */ yytestcase(yyruleno==62);
      case 65: /* less_than_billion ::= less_than_million */ yytestcase(yyruleno==65);
      case 82: /* less_than_million_end_only ::= less_than_million */ yytestcase(yyruleno==82);
      case 83: /* less_than_million_end_only ::= less_than_thousand_end_only */ yytestcase(yyruleno==83);
      case 86: /* less_than_million ::= less_than_thousand */ yytestcase(yyruleno==86);
      case 106: /* less_than_thousand_end_only ::= less_than_thousand */ yytestcase(yyruleno==106);
      case 110: /* less_than_thousand ::= less_than_hundred */ yytestcase(yyruleno==110);
      case 120: /* less_than_hundred ::= tens */ yytestcase(yyruleno==120);
      case 121: /* less_than_hundred ::= less_than_twenty */ yytestcase(yyruleno==121);
      case 126: /* less_than_twenty ::= ten_to_19 */ yytestcase(yyruleno==126);
      case 127: /* less_than_twenty ::= less_than_ten */ yytestcase(yyruleno==127);
#line 84 "parser.yy"
{ COPY_YYSTYPE_BE_DBL(yylhsminor.yy0, yymsp[0].minor.yy0); }
#line 1599 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* final_number ::= less_than_quadrillionth */
      case 8: /* final_number ::= less_than_quadrillionths */ yytestcase(yyruleno==8);
      case 13: /* less_than_quadrillionth ::= less_than_trillionth */ yytestcase(yyruleno==13);
      case 18: /* less_than_quadrillionths ::= less_than_trillionths */ yytestcase(yyruleno==18);
      case 26: /* less_than_trillionth_end_only ::= less_than_trillionth */ yytestcase(yyruleno==26);
      case 27: /* less_than_trillionth_end_only ::= less_than_billionth_end_only */ yytestcase(yyruleno==27);
      case 30: /* less_than_trillionth ::= less_than_billionth */ yytestcase(yyruleno==30);
      case 33: /* less_than_trillionths_end_only ::= less_than_trillionths */ yytestcase(yyruleno==33);
      case 34: /* less_than_trillionths_end_only ::= less_than_billionths_end_only */ yytestcase(yyruleno==34);
      case 37: /* less_than_trillionths ::= less_than_billionths */ yytestcase(yyruleno==37);
      case 47: /* less_than_billionth_end_only ::= less_than_billionth */ yytestcase(yyruleno==47);
      case 48: /* less_than_billionth_end_only ::= less_than_millionth_end_only */ yytestcase(yyruleno==48);
      case 51: /* less_than_billionth ::= less_than_millionth */ yytestcase(yyruleno==51);
      case 54: /* less_than_billionths_end_only ::= less_than_billionths */ yytestcase(yyruleno==54);
      case 55: /* less_than_billionths_end_only ::= less_than_millionths_end_only */ yytestcase(yyruleno==55);
      case 58: /* less_than_billionths ::= less_than_millionths */ yytestcase(yyruleno==58);
      case 68: /* less_than_millionth_end_only ::= less_than_millionth */ yytestcase(yyruleno==68);
      case 69: /* less_than_millionth_end_only ::= less_than_thousandth_end_only */ yytestcase(yyruleno==69);
      case 72: /* less_than_millionth ::= less_than_thousandth */ yytestcase(yyruleno==72);
      case 75: /* less_than_millionths_end_only ::= less_than_millionths */ yytestcase(yyruleno==75);
      case 76: /* less_than_millionths_end_only ::= less_than_thousandths_end_only */ yytestcase(yyruleno==76);
      case 79: /* less_than_millionths ::= less_than_thousandths */ yytestcase(yyruleno==79);
      case 90: /* less_than_thousandth_end_only ::= less_than_thousandth */ yytestcase(yyruleno==90);
      case 94: /* less_than_thousandth ::= less_than_hundredth */ yytestcase(yyruleno==94);
      case 98: /* less_than_thousandths_end_only ::= less_than_thousandths */ yytestcase(yyruleno==98);
      case 102: /* less_than_thousandths ::= less_than_hundredths */ yytestcase(yyruleno==102);
      case 114: /* less_than_hundredth ::= tenth */ yytestcase(yyruleno==114);
      case 115: /* less_than_hundredth ::= less_than_twentieth */ yytestcase(yyruleno==115);
      case 117: /* less_than_hundredths ::= tenths */ yytestcase(yyruleno==117);
      case 118: /* less_than_hundredths ::= less_than_twentieths */ yytestcase(yyruleno==118);
      case 122: /* less_than_twentieth ::= tenth_to_19th */ yytestcase(yyruleno==122);
      case 123: /* less_than_twentieth ::= less_than_tenth */ yytestcase(yyruleno==123);
      case 124: /* less_than_twentieths ::= tenths_to_19ths */ yytestcase(yyruleno==124);
      case 125: /* less_than_twentieths ::= less_than_tenths */ yytestcase(yyruleno==125);
#line 85 "parser.yy"
{ COPY_YYSTYPE_BE_DBL_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0); }
#line 1638 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* final_number ::= NUMBER */
#line 101 "parser.yy"
{ COPY_YYSTYPE_BE(yylhsminor.yy0, yymsp[0].minor.yy0); yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = true; }
#line 1644 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 10: /* final_number ::= ZERO */
#line 102 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 0.0); }
#line 1650 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 11: /* less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only */
      case 16: /* less_than_quadrillionths ::= less_than_thousand TRILLION less_than_trillionths_end_only */ yytestcase(yyruleno==16);
#line 128 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1657 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 12: /* less_than_quadrillionth ::= less_than_thousand TRILLIONTH */
#line 129 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 1663 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* final_number ::= TRILLIONTH */
#line 132 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 1669 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 15: /* final_number ::= NUMBER TRILLIONTH */
#line 133 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, TH); }
#line 1675 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 17: /* less_than_quadrillionths ::= less_than_thousand TRILLIONTHS */
#line 138 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 1681 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 19: /* final_number ::= TRILLIONTHS */
#line 141 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 1687 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 20: /* final_number ::= NUMBER TRILLIONTHS */
#line 142 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F, THS); }
#line 1693 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 21: /* less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only */
#line 148 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1699 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 22: /* less_than_quadrillion ::= less_than_thousand TRILLION */
#line 149 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1705 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 24: /* final_number ::= TRILLION */
#line 152 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1711 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* final_number ::= NUMBER TRILLION */
#line 153 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, TRILLION_F); }
#line 1717 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 28: /* less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only */
      case 35: /* less_than_trillionths ::= less_than_thousand BILLION less_than_billionths_end_only */ yytestcase(yyruleno==35);
#line 162 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1724 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* less_than_trillionth ::= less_than_thousand BILLIONTH */
#line 163 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 1730 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 31: /* final_number ::= BILLIONTH */
#line 166 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 1736 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* final_number ::= NUMBER BILLIONTH */
#line 167 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, TH); }
#line 1742 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 36: /* less_than_trillionths ::= less_than_thousand BILLIONTHS */
#line 175 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 1748 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* final_number ::= BILLIONTHS */
#line 178 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 1754 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 39: /* final_number ::= NUMBER BILLIONTHS */
#line 179 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F, THS); }
#line 1760 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 42: /* less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only */
#line 188 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1766 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 43: /* less_than_trillion ::= less_than_thousand BILLION */
#line 189 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1772 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 45: /* final_number ::= BILLION */
#line 192 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1778 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 46: /* final_number ::= NUMBER BILLION */
#line 193 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, BILLION_F); }
#line 1784 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 49: /* less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only */
      case 56: /* less_than_billionths ::= less_than_thousand MILLION less_than_millionths_end_only */ yytestcase(yyruleno==56);
#line 202 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 1791 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 50: /* less_than_billionth ::= less_than_thousand MILLIONTH */
#line 203 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 1797 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 52: /* final_number ::= MILLIONTH */
#line 206 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 1803 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 53: /* final_number ::= NUMBER MILLIONTH */
#line 207 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, TH); }
#line 1809 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 57: /* less_than_billionths ::= less_than_thousand MILLIONTHS */
#line 215 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 1815 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 59: /* final_number ::= MILLIONTHS */
#line 218 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 1821 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 60: /* final_number ::= NUMBER MILLIONTHS */
#line 219 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F, THS); }
#line 1827 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 63: /* less_than_billion ::= less_than_thousand MILLION less_than_million_end_only */
#line 228 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 1833 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 64: /* less_than_billion ::= less_than_thousand MILLION */
#line 229 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 1839 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 66: /* final_number ::= MILLION */
#line 232 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 1845 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 67: /* final_number ::= NUMBER MILLION */
#line 233 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, MILLION_F); }
#line 1851 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 70: /* less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only */
      case 77: /* less_than_millionths ::= less_than_thousand THOUSAND less_than_thousandths_end_only */ yytestcase(yyruleno==77);
#line 242 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 1858 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 71: /* less_than_millionth ::= less_than_thousand THOUSANDTH */
#line 243 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 1864 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 73: /* final_number ::= THOUSANDTH */
#line 246 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 1870 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 74: /* final_number ::= NUMBER THOUSANDTH */
#line 247 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, TH); }
#line 1876 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 78: /* less_than_millionths ::= less_than_thousand THOUSANDTHS */
#line 255 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 1882 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 80: /* final_number ::= THOUSANDTHS */
#line 258 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 1888 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 81: /* final_number ::= NUMBER THOUSANDTHS */
#line 259 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F, THS); }
#line 1894 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 84: /* less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only */
#line 268 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 1900 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 85: /* less_than_million ::= less_than_thousand THOUSAND */
#line 269 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 1906 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 87: /* final_number ::= THOUSAND */
#line 272 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 1912 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 88: /* final_number ::= NUMBER THOUSAND */
#line 273 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, THOUSAND_F); }
#line 1918 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 89: /* less_than_thousandth_end_only ::= AND less_than_hundredth */
      case 97: /* less_than_thousandths_end_only ::= AND less_than_hundredths */ yytestcase(yyruleno==97);
#line 279 "parser.yy"
{ COPY_YYSTYPE_BE_DBL_SUFF(yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 1925 "parser.c"
        break;
      case 91: /* less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth */
      case 99: /* less_than_thousandths ::= less_than_hundred HUNDRED AND less_than_hundredths */ yytestcase(yyruleno==99);
#line 282 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 1931 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 92: /* less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth */
      case 100: /* less_than_thousandths ::= less_than_hundred HUNDRED less_than_hundredths */ yytestcase(yyruleno==100);
#line 283 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD_SUFF(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 1938 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 93: /* less_than_thousandth ::= less_than_hundred HUNDREDTH */
#line 284 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 1944 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 95: /* final_number ::= HUNDREDTH */
#line 287 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 1950 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 96: /* final_number ::= NUMBER HUNDREDTH */
#line 288 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, TH); }
#line 1956 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 101: /* less_than_thousandths ::= less_than_hundred HUNDREDTHS */
#line 297 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 1962 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 103: /* final_number ::= HUNDREDTHS */
#line 300 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 1968 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 104: /* final_number ::= NUMBER HUNDREDTHS */
#line 301 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F, THS); }
#line 1974 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 105: /* less_than_thousand_end_only ::= AND less_than_hundred */
#line 307 "parser.yy"
{ COPY_YYSTYPE_BE_DBL(yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 1980 "parser.c"
        break;
      case 107: /* less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred */
#line 310 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 1985 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 108: /* less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred */
#line 311 "parser.yy"
{ COPY_YYSTYPE_BE_MUL_ADD(yylhsminor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 1991 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 109: /* less_than_thousand ::= less_than_hundred HUNDRED */
#line 312 "parser.yy"
{ COPY_YYSTYPE_BE_MUL(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 1997 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 111: /* final_number ::= HUNDRED */
#line 315 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2003 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 112: /* final_number ::= NUMBER HUNDRED */
#line 316 "parser.yy"
{ COPY_YYSTYPE_DBL_NUM(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, HUNDRED_F); }
#line 2009 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 113: /* less_than_hundredth ::= tens less_than_tenth */
      case 116: /* less_than_hundredths ::= tens less_than_tenths */ yytestcase(yyruleno==116);
#line 322 "parser.yy"
{ COPY_YYSTYPE_BE_ADD_SUFF(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2016 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 119: /* less_than_hundred ::= tens less_than_ten */
#line 334 "parser.yy"
{ COPY_YYSTYPE_BE_ADD(yylhsminor.yy0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0); }
#line 2022 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 128: /* less_than_ten ::= ONE */
#line 359 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0); }
#line 2028 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 129: /* less_than_ten ::= TWO */
#line 360 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0); }
#line 2034 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 130: /* less_than_ten ::= THREE */
#line 361 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0); }
#line 2040 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 131: /* less_than_ten ::= FOUR */
#line 362 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0); }
#line 2046 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 132: /* less_than_ten ::= FIVE */
#line 363 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0); }
#line 2052 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 133: /* less_than_ten ::= SIX */
#line 364 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0); }
#line 2058 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 134: /* less_than_ten ::= SEVEN */
#line 365 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0); }
#line 2064 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 135: /* less_than_ten ::= EIGHT */
#line 366 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0); }
#line 2070 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 136: /* less_than_ten ::= NINE */
#line 367 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0); }
#line 2076 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 137: /* ten_to_19 ::= TEN */
#line 369 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0); }
#line 2082 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 138: /* ten_to_19 ::= ELEVEN */
#line 370 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0); }
#line 2088 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 139: /* ten_to_19 ::= TWELVE */
#line 371 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0); }
#line 2094 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 140: /* ten_to_19 ::= THIRTEEN */
#line 372 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0); }
#line 2100 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 141: /* ten_to_19 ::= FOURTEEN */
#line 373 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0); }
#line 2106 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 142: /* ten_to_19 ::= FIFTEEN */
#line 374 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0); }
#line 2112 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 143: /* ten_to_19 ::= SIXTEEN */
#line 375 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0); }
#line 2118 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 144: /* ten_to_19 ::= SEVENTEEN */
#line 376 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0); }
#line 2124 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 145: /* ten_to_19 ::= EIGHTEEN */
#line 377 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0); }
#line 2130 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 146: /* ten_to_19 ::= NINETEEN */
#line 378 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0); }
#line 2136 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 147: /* tens ::= TWENTY */
#line 380 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0); }
#line 2142 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 148: /* tens ::= THIRTY */
#line 381 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0); }
#line 2148 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 149: /* tens ::= FORTY */
#line 382 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0); }
#line 2154 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 150: /* tens ::= FIFTY */
#line 383 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0); }
#line 2160 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 151: /* tens ::= SIXTY */
#line 384 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0); }
#line 2166 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 152: /* tens ::= SEVENTY */
#line 385 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0); }
#line 2172 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 153: /* tens ::= EIGHTY */
#line 386 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0); }
#line 2178 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 154: /* tens ::= NINETY */
#line 387 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0); }
#line 2184 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 155: /* less_than_tenth ::= FIRST */
#line 389 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0, ST); }
#line 2190 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 156: /* less_than_tenth ::= SECOND */
#line 390 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 2.0, ND); }
#line 2196 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 157: /* less_than_tenth ::= THIRD */
#line 391 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0, RD); }
#line 2202 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 158: /* less_than_tenth ::= FOURTH */
#line 392 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0, TH); }
#line 2208 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 159: /* less_than_tenth ::= FIFTH */
#line 393 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0, TH); }
#line 2214 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 160: /* less_than_tenth ::= SIXTH */
#line 394 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0, TH); }
#line 2220 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 161: /* less_than_tenth ::= SEVENTH */
#line 395 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0, TH); }
#line 2226 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 162: /* less_than_tenth ::= EIGHTH */
#line 396 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0, TH); }
#line 2232 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 163: /* less_than_tenth ::= NINTH */
#line 397 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0, TH); }
#line 2238 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 164: /* tenth_to_19th ::= TENTH */
#line 399 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0, TH); }
#line 2244 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 165: /* tenth_to_19th ::= ELEVENTH */
#line 400 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0, TH); }
#line 2250 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 166: /* tenth_to_19th ::= TWELFTH */
#line 401 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0, TH); }
#line 2256 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 167: /* tenth_to_19th ::= THIRTEENTH */
#line 402 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0, TH); }
#line 2262 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 168: /* tenth_to_19th ::= FOURTEENTH */
#line 403 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0, TH); }
#line 2268 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 169: /* tenth_to_19th ::= FIFTEENTH */
#line 404 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0, TH); }
#line 2274 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 170: /* tenth_to_19th ::= SIXTEENTH */
#line 405 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0, TH); }
#line 2280 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 171: /* tenth_to_19th ::= SEVENTEENTH */
#line 406 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0, TH); }
#line 2286 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 172: /* tenth_to_19th ::= EIGHTEENTH */
#line 407 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0, TH); }
#line 2292 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 173: /* tenth_to_19th ::= NINETEENTH */
#line 408 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0, TH); }
#line 2298 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 174: /* tenth ::= TWENTIETH */
#line 410 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0, TH); }
#line 2304 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 175: /* tenth ::= THIRTIETH */
#line 411 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0, TH); }
#line 2310 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 176: /* tenth ::= FOURTIETH */
#line 412 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0, TH); }
#line 2316 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 177: /* tenth ::= FIFTIETH */
#line 413 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0, TH); }
#line 2322 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 178: /* tenth ::= SIXTIETH */
#line 414 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0, TH); }
#line 2328 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 179: /* tenth ::= SEVENTIETH */
#line 415 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0, TH); }
#line 2334 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 180: /* tenth ::= EIGHTIETH */
#line 416 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0, TH); }
#line 2340 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 181: /* tenth ::= NINETIETH */
#line 417 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0, TH); }
#line 2346 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 182: /* less_than_tenths ::= FIRSTS */
#line 419 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 1.0, STS); }
#line 2352 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 183: /* less_than_tenths ::= THIRDS */
#line 421 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 3.0, RDS); }
#line 2358 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 184: /* less_than_tenths ::= FOURTHS */
#line 422 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 4.0, THS); }
#line 2364 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 185: /* less_than_tenths ::= FIFTHS */
#line 423 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 5.0, THS); }
#line 2370 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 186: /* less_than_tenths ::= SIXTHS */
#line 424 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 6.0, THS); }
#line 2376 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 187: /* less_than_tenths ::= SEVENTHS */
#line 425 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 7.0, THS); }
#line 2382 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 188: /* less_than_tenths ::= EIGHTHS */
#line 426 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 8.0, THS); }
#line 2388 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 189: /* less_than_tenths ::= NINTHS */
#line 427 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 9.0, THS); }
#line 2394 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 190: /* tenths_to_19ths ::= TENTHS */
#line 429 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 10.0, THS); }
#line 2400 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 191: /* tenths_to_19ths ::= ELEVENTHS */
#line 430 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 11.0, THS); }
#line 2406 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 192: /* tenths_to_19ths ::= TWELFTHS */
#line 431 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 12.0, THS); }
#line 2412 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 193: /* tenths_to_19ths ::= THIRTEENTHS */
#line 432 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 13.0, THS); }
#line 2418 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 194: /* tenths_to_19ths ::= FOURTEENTHS */
#line 433 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 14.0, THS); }
#line 2424 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 195: /* tenths_to_19ths ::= FIFTEENTHS */
#line 434 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 15.0, THS); }
#line 2430 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 196: /* tenths_to_19ths ::= SIXTEENTHS */
#line 435 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 16.0, THS); }
#line 2436 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 197: /* tenths_to_19ths ::= SEVENTEENTHS */
#line 436 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 17.0, THS); }
#line 2442 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 198: /* tenths_to_19ths ::= EIGHTEENTHS */
#line 437 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 18.0, THS); }
#line 2448 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 199: /* tenths_to_19ths ::= NINETEENTHS */
#line 438 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 19.0, THS); }
#line 2454 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 200: /* tenths ::= TWENTIETHS */
#line 440 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 20.0, THS); }
#line 2460 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 201: /* tenths ::= THIRTIETHS */
#line 441 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 30.0, THS); }
#line 2466 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 202: /* tenths ::= FOURTIETHS */
#line 442 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 40.0, THS); }
#line 2472 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 203: /* tenths ::= FIFTIETHS */
#line 443 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 50.0, THS); }
#line 2478 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 204: /* tenths ::= SIXTIETHS */
#line 444 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 60.0, THS); }
#line 2484 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 205: /* tenths ::= SEVENTIETHS */
#line 445 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 70.0, THS); }
#line 2490 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 206: /* tenths ::= EIGHTIETHS */
#line 446 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 80.0, THS); }
#line 2496 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 207: /* tenths ::= NINETIETHS */
#line 447 "parser.yy"
{ COPY_YYSTYPE_BE_VALUE_SUFF(yylhsminor.yy0, yymsp[0].minor.yy0, 90.0, THS); }
#line 2502 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      default:
      /* (208) program ::= numbers */ yytestcase(yyruleno==208);
      /* (209) numbers ::= numbers number */ yytestcase(yyruleno==209);
      /* (210) numbers ::= number (OPTIMIZED OUT) */ assert(yyruleno!=210);
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
#line 2555 "parser.c"
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
#line 2578 "parser.c"
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
