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

#define maxi(a, b) (a > b ? a : b)
#define mini(a, b) (a < b ? a : b)
#line 50 "parser.yy"

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
#line 76 "parser.c"
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
#define TOKEN_BILLION                        11
#define TOKEN_BILLIONTH                      12
#define TOKEN_MILLION                        13
#define TOKEN_MILLIONTH                      14
#define TOKEN_THOUSAND                       15
#define TOKEN_THOUSANDTH                     16
#define TOKEN_AND                            17
#define TOKEN_HUNDRED                        18
#define TOKEN_HUNDREDTH                      19
#define TOKEN_ONE                            20
#define TOKEN_TWO                            21
#define TOKEN_THREE                          22
#define TOKEN_FOUR                           23
#define TOKEN_FIVE                           24
#define TOKEN_SIX                            25
#define TOKEN_SEVEN                          26
#define TOKEN_EIGHT                          27
#define TOKEN_NINE                           28
#define TOKEN_TEN                            29
#define TOKEN_ELEVEN                         30
#define TOKEN_TWELVE                         31
#define TOKEN_THIRTEEN                       32
#define TOKEN_FOURTEEN                       33
#define TOKEN_FIFTEEN                        34
#define TOKEN_SIXTEEN                        35
#define TOKEN_SEVENTEEN                      36
#define TOKEN_EIGHTEEN                       37
#define TOKEN_NINETEEN                       38
#define TOKEN_TWENTY                         39
#define TOKEN_THIRTY                         40
#define TOKEN_FORTY                          41
#define TOKEN_FIFTY                          42
#define TOKEN_SIXTY                          43
#define TOKEN_SEVENTY                        44
#define TOKEN_EIGHTY                         45
#define TOKEN_NINETY                         46
#define TOKEN_FIRST                          47
#define TOKEN_SECOND                         48
#define TOKEN_THIRD                          49
#define TOKEN_FOURTH                         50
#define TOKEN_FIFTH                          51
#define TOKEN_SIXTH                          52
#define TOKEN_SEVENTH                        53
#define TOKEN_EIGHTH                         54
#define TOKEN_NINTH                          55
#define TOKEN_TENTH                          56
#define TOKEN_ELEVENTH                       57
#define TOKEN_TWELFTH                        58
#define TOKEN_THIRTEENTH                     59
#define TOKEN_FOURTEENTH                     60
#define TOKEN_FIFTEENTH                      61
#define TOKEN_SIXTEENTH                      62
#define TOKEN_SEVENTEENTH                    63
#define TOKEN_EIGHTEENTH                     64
#define TOKEN_NINETEENTH                     65
#define TOKEN_TWENTIETH                      66
#define TOKEN_THIRTIETH                      67
#define TOKEN_FOURTIETH                      68
#define TOKEN_FIFTIETH                       69
#define TOKEN_SIXTIETH                       70
#define TOKEN_SEVENTIETH                     71
#define TOKEN_EIGHTIETH                      72
#define TOKEN_NINETIETH                      73
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
#define YYNOCODE 108
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
#define YYNRULE              145
#define YY_MAX_SHIFT         18
#define YY_MIN_SHIFTREDUCE   158
#define YY_MAX_SHIFTREDUCE   302
#define YY_MIN_REDUCE        303
#define YY_MAX_REDUCE        447
#define YY_ERROR_ACTION      448
#define YY_ACCEPT_ACTION     449
#define YY_NO_ACTION         450
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
#define YY_ACTTAB_COUNT (639)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   445,    4,    5,  195,    6,  209,  305,   11,  167,  176,
 /*    10 */   171,  190,  183,  204,  197,  218,  211,  305,  234,  226,
 /*    20 */   246,  247,  248,  249,  250,  251,  252,  253,  254,  255,
 /*    30 */   256,  257,  258,  259,  260,  261,  262,  263,  264,  265,
 /*    40 */   266,  267,  268,  269,  270,  271,  272,  273,  274,  275,
 /*    50 */   276,  277,  278,  279,  280,  281,  282,  283,  284,  285,
 /*    60 */   286,  287,  288,  289,  290,  291,  292,  293,  294,  295,
 /*    70 */   296,  297,  298,  299,    4,  236,    7,  224,  239,  305,
 /*    80 */    11,  167,  176,  171,  190,  183,  204,  197,  218,  211,
 /*    90 */   305,  234,  226,  246,  247,  248,  249,  250,  251,  252,
 /*   100 */   253,  254,  255,  256,  257,  258,  259,  260,  261,  262,
 /*   110 */   263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
 /*   120 */   273,  274,  275,  276,  277,  278,  279,  280,  281,  282,
 /*   130 */   283,  284,  285,  286,  287,  288,  289,  290,  291,  292,
 /*   140 */   293,  294,  295,  296,  297,  298,  299,   11,  167,  176,
 /*   150 */   171,  190,  183,  204,  197,  218,  211,  305,  234,  226,
 /*   160 */   246,  247,  248,  249,  250,  251,  252,  253,  254,  255,
 /*   170 */   256,  257,  258,  259,  260,  261,  262,  263,  264,  265,
 /*   180 */   266,  267,  268,  269,  270,  271,  272,  273,  274,  275,
 /*   190 */   276,  277,  278,  279,  280,  281,  282,  283,  284,  285,
 /*   200 */   286,  287,  288,  289,  290,  291,  292,  293,  294,  295,
 /*   210 */   296,  297,  298,  299,    9,    6,  209,  246,  247,  248,
 /*   220 */   249,  250,  251,  252,  253,  254,  255,  256,  257,  258,
 /*   230 */   259,  260,  261,  262,  263,  264,  265,  266,  267,  268,
 /*   240 */   269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
 /*   250 */   279,  280,  281,  282,  283,  284,  285,  286,  287,  288,
 /*   260 */   289,  290,  291,  292,  293,  294,  295,  296,  297,  298,
 /*   270 */   299,    8,  305,  305,  246,  247,  248,  249,  250,  251,
 /*   280 */   252,  253,  254,  255,  256,  257,  258,  259,  260,  261,
 /*   290 */   262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
 /*   300 */   272,  273,  274,  275,  276,  277,  278,  279,  280,  281,
 /*   310 */   282,  283,  284,  285,  286,  287,  288,  289,  290,  291,
 /*   320 */   292,  293,  294,  295,  296,  297,  298,  299,  246,  247,
 /*   330 */   248,  249,  250,  251,  252,  253,  254,  255,  256,  257,
 /*   340 */   258,  259,  260,  261,  262,  263,  264,  265,  266,  267,
 /*   350 */   268,  269,  270,  271,  272,  273,  274,  275,  276,  277,
 /*   360 */   278,  279,  280,  281,  282,  283,  284,  285,  286,  287,
 /*   370 */   288,  289,  290,  291,  292,  293,  294,  295,  296,  297,
 /*   380 */   298,  299,   13,  168,  178,  173,  185,  179,  182,  186,
 /*   390 */   189,  193,  196,  200,  203,  207,  210,  214,  225,   16,
 /*   400 */    10,  243,  237,  238,  245,  241,  242,  244,  449,    2,
 /*   410 */     2,  158,   15,  165,   12,   18,  170,  161,  175,  163,
 /*   420 */   182,  160,  189,  162,  196,  305,  203,  305,  210,  305,
 /*   430 */   225,   16,   10,  243,  237,  238,  245,  241,  242,  244,
 /*   440 */   301,  158,   15,  165,   12,  305,  170,  305,  175,  305,
 /*   450 */   182,  305,  189,  305,  196,  305,  203,  305,  210,  305,
 /*   460 */   225,   16,   10,  243,  237,  238,  245,  241,  242,  244,
 /*   470 */    14,  305,  305,  305,  305,  180,  192,  187,  199,  193,
 /*   480 */   196,  200,  203,  207,  210,  214,  225,   16,   10,  243,
 /*   490 */   237,  238,  245,  241,  242,  244,  305,  159,   15,  165,
 /*   500 */    12,  305,  170,  305,  175,  305,  182,  305,  189,  305,
 /*   510 */   196,  305,  203,  305,  210,  305,  225,   16,   10,  243,
 /*   520 */   237,  238,  245,  241,  242,  244,   17,    1,  169,    3,
 /*   530 */   181,    5,  195,    6,  209,  194,  206,  201,  213,  207,
 /*   540 */   210,  214,  225,   16,   10,  243,  237,  238,  245,  241,
 /*   550 */   242,  244,  229,  305,  305,  305,  246,  247,  248,  249,
 /*   560 */   250,  251,  252,  253,  254,  208,  221,  215,  225,   16,
 /*   570 */    10,  243,  237,  238,  245,  241,  242,  244,  305,  305,
 /*   580 */   305,  305,  305,  273,  274,  275,  276,  277,  278,  279,
 /*   590 */   280,  281,  223,  231,   10,  243,  237,  238,  245,  241,
 /*   600 */   242,  244,  222,  230,   10,  243,  237,  238,  245,  241,
 /*   610 */   242,  244,  220,  228,   10,  243,  237,  238,  245,  241,
 /*   620 */   242,  244,  177,  172,  191,  184,  205,  198,  219,  212,
 /*   630 */   305,  235,  227,    3,  181,    5,  195,    6,  209,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     0,    1,   13,   14,   15,   16,  107,    7,    8,    9,
 /*    10 */    10,   11,   12,   13,   14,   15,   16,  107,   18,   19,
 /*    20 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    30 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*    40 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*    50 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*    60 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*    70 */    70,   71,   72,   73,    1,  100,   18,   19,  103,  107,
 /*    80 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*    90 */   107,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*   100 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   110 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*   120 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   130 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   140 */    67,   68,   69,   70,   71,   72,   73,    7,    8,    9,
 /*   150 */    10,   11,   12,   13,   14,   15,   16,  107,   18,   19,
 /*   160 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*   170 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   180 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*   190 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   200 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   210 */    70,   71,   72,   73,   17,   15,   16,   20,   21,   22,
 /*   220 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*   230 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*   240 */    43,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*   250 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   260 */    63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
 /*   270 */    73,   17,  107,  107,   20,   21,   22,   23,   24,   25,
 /*   280 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   290 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*   300 */    46,   47,   48,   49,   50,   51,   52,   53,   54,   55,
 /*   310 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*   320 */    66,   67,   68,   69,   70,   71,   72,   73,   20,   21,
 /*   330 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*   340 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*   350 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*   360 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*   370 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   380 */    72,   73,   81,   82,   83,   84,   85,   86,   87,   88,
 /*   390 */    89,   90,   91,   92,   93,   94,   95,   96,   97,   98,
 /*   400 */    99,  100,  101,  102,  103,  104,  105,  106,   75,   76,
 /*   410 */    77,   78,   79,   80,   81,    2,   83,    4,   85,    6,
 /*   420 */    87,    3,   89,    5,   91,  107,   93,  107,   95,  107,
 /*   430 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   440 */    77,   78,   79,   80,   81,  107,   83,  107,   85,  107,
 /*   450 */    87,  107,   89,  107,   91,  107,   93,  107,   95,  107,
 /*   460 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   470 */    81,  107,  107,  107,  107,   86,   87,   88,   89,   90,
 /*   480 */    91,   92,   93,   94,   95,   96,   97,   98,   99,  100,
 /*   490 */   101,  102,  103,  104,  105,  106,  107,   78,   79,   80,
 /*   500 */    81,  107,   83,  107,   85,  107,   87,  107,   89,  107,
 /*   510 */    91,  107,   93,  107,   95,  107,   97,   98,   99,  100,
 /*   520 */   101,  102,  103,  104,  105,  106,   81,    9,   10,   11,
 /*   530 */    12,   13,   14,   15,   16,   90,   91,   92,   93,   94,
 /*   540 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   550 */   105,  106,   81,  107,  107,  107,   20,   21,   22,   23,
 /*   560 */    24,   25,   26,   27,   28,   94,   95,   96,   97,   98,
 /*   570 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  107,
 /*   580 */   107,  107,  107,   47,   48,   49,   50,   51,   52,   53,
 /*   590 */    54,   55,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   600 */   105,  106,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   610 */   105,  106,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   620 */   105,  106,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   630 */   107,   18,   19,   11,   12,   13,   14,   15,   16,
};
#define YY_SHIFT_USE_DFLT (639)
#define YY_SHIFT_COUNT    (18)
#define YY_SHIFT_MIN      (-11)
#define YY_SHIFT_MAX      (622)
static const short yy_shift_ofst[] = {
 /*     0 */    73,  197,    0,  197,  140,  197,  197,  254,  308,  308,
 /*    10 */   536,  613,  518,  622,  -11,  413,   58,  200,  418,
};
#define YY_REDUCE_USE_DFLT (-26)
#define YY_REDUCE_COUNT (10)
#define YY_REDUCE_MIN   (-25)
#define YY_REDUCE_MAX   (515)
static const short yy_reduce_ofst[] = {
 /*     0 */   333,  301,  363,  389,  419,  445,  471,  495,  505,  515,
 /*    10 */   -25,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   448,  319,  448,  333,  448,  347,  361,  377,  448,  448,
 /*    10 */   385,  311,  362,  362,  362,  309,  378,  362,  448,
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
  "ZERO",          "TRILLION",      "TRILLIONTH",    "BILLION",     
  "BILLIONTH",     "MILLION",       "MILLIONTH",     "THOUSAND",    
  "THOUSANDTH",    "AND",           "HUNDRED",       "HUNDREDTH",   
  "ONE",           "TWO",           "THREE",         "FOUR",        
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
  "EIGHTIETH",     "NINETIETH",     "error",         "program",     
  "numbers",       "number",        "final_number",  "less_than_quadrillion",
  "less_than_quadrillionth",  "less_than_thousand",  "less_than_trillionth_end_only",  "less_than_trillionth",
  "less_than_trillion_end_only",  "less_than_trillion",  "less_than_billionth_end_only",  "less_than_billionth",
  "less_than_billion_end_only",  "less_than_billion",  "less_than_millionth_end_only",  "less_than_millionth",
  "less_than_million_end_only",  "less_than_million",  "less_than_thousandth_end_only",  "less_than_thousandth",
  "less_than_thousand_end_only",  "less_than_hundredth",  "less_than_hundred",  "tens",        
  "first_to_9th",  "tenth",         "less_than_twentieth",  "less_than_ten",
  "less_than_twenty",  "tenth_to_19th",  "ten_to_19",   
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
 /*   8 */ "final_number ::= NUMBER",
 /*   9 */ "final_number ::= ZERO",
 /*  10 */ "less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only",
 /*  11 */ "less_than_quadrillionth ::= less_than_thousand TRILLIONTH",
 /*  12 */ "less_than_quadrillionth ::= less_than_trillionth",
 /*  13 */ "final_number ::= TRILLIONTH",
 /*  14 */ "final_number ::= NUMBER TRILLIONTH",
 /*  15 */ "less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only",
 /*  16 */ "less_than_quadrillion ::= less_than_thousand TRILLION",
 /*  17 */ "less_than_quadrillion ::= less_than_trillion",
 /*  18 */ "final_number ::= TRILLION",
 /*  19 */ "final_number ::= NUMBER TRILLION",
 /*  20 */ "less_than_trillionth_end_only ::= less_than_trillionth",
 /*  21 */ "less_than_trillionth_end_only ::= less_than_billionth_end_only",
 /*  22 */ "less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only",
 /*  23 */ "less_than_trillionth ::= less_than_thousand BILLIONTH",
 /*  24 */ "less_than_trillionth ::= less_than_billionth",
 /*  25 */ "final_number ::= BILLIONTH",
 /*  26 */ "final_number ::= NUMBER BILLIONTH",
 /*  27 */ "less_than_trillion_end_only ::= less_than_trillion",
 /*  28 */ "less_than_trillion_end_only ::= less_than_billion_end_only",
 /*  29 */ "less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only",
 /*  30 */ "less_than_trillion ::= less_than_thousand BILLION",
 /*  31 */ "less_than_trillion ::= less_than_billion",
 /*  32 */ "final_number ::= BILLION",
 /*  33 */ "final_number ::= NUMBER BILLION",
 /*  34 */ "less_than_billionth_end_only ::= less_than_billionth",
 /*  35 */ "less_than_billionth_end_only ::= less_than_millionth_end_only",
 /*  36 */ "less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only",
 /*  37 */ "less_than_billionth ::= less_than_thousand MILLIONTH",
 /*  38 */ "less_than_billionth ::= less_than_millionth",
 /*  39 */ "final_number ::= MILLIONTH",
 /*  40 */ "final_number ::= NUMBER MILLIONTH",
 /*  41 */ "less_than_billion_end_only ::= less_than_billion",
 /*  42 */ "less_than_billion_end_only ::= less_than_million_end_only",
 /*  43 */ "less_than_billion ::= less_than_thousand MILLION less_than_million_end_only",
 /*  44 */ "less_than_billion ::= less_than_thousand MILLION",
 /*  45 */ "less_than_billion ::= less_than_million",
 /*  46 */ "final_number ::= MILLION",
 /*  47 */ "final_number ::= NUMBER MILLION",
 /*  48 */ "less_than_millionth_end_only ::= less_than_millionth",
 /*  49 */ "less_than_millionth_end_only ::= less_than_thousandth_end_only",
 /*  50 */ "less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only",
 /*  51 */ "less_than_millionth ::= less_than_thousand THOUSANDTH",
 /*  52 */ "less_than_millionth ::= less_than_thousandth",
 /*  53 */ "final_number ::= THOUSANDTH",
 /*  54 */ "final_number ::= NUMBER THOUSANDTH",
 /*  55 */ "less_than_million_end_only ::= less_than_million",
 /*  56 */ "less_than_million_end_only ::= less_than_thousand_end_only",
 /*  57 */ "less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only",
 /*  58 */ "less_than_million ::= less_than_thousand THOUSAND",
 /*  59 */ "less_than_million ::= less_than_thousand",
 /*  60 */ "final_number ::= THOUSAND",
 /*  61 */ "final_number ::= NUMBER THOUSAND",
 /*  62 */ "less_than_thousandth_end_only ::= AND less_than_hundredth",
 /*  63 */ "less_than_thousandth_end_only ::= less_than_thousandth",
 /*  64 */ "less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth",
 /*  65 */ "less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth",
 /*  66 */ "less_than_thousandth ::= less_than_hundred HUNDREDTH",
 /*  67 */ "less_than_thousandth ::= less_than_hundredth",
 /*  68 */ "final_number ::= HUNDREDTH",
 /*  69 */ "final_number ::= NUMBER HUNDREDTH",
 /*  70 */ "less_than_thousand_end_only ::= AND less_than_hundred",
 /*  71 */ "less_than_thousand_end_only ::= less_than_thousand",
 /*  72 */ "less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred",
 /*  73 */ "less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred",
 /*  74 */ "less_than_thousand ::= less_than_hundred HUNDRED",
 /*  75 */ "less_than_thousand ::= less_than_hundred",
 /*  76 */ "final_number ::= HUNDRED",
 /*  77 */ "final_number ::= NUMBER HUNDRED",
 /*  78 */ "less_than_hundredth ::= tens first_to_9th",
 /*  79 */ "less_than_hundredth ::= tenth",
 /*  80 */ "less_than_hundredth ::= less_than_twentieth",
 /*  81 */ "less_than_hundred ::= tens less_than_ten",
 /*  82 */ "less_than_hundred ::= tens",
 /*  83 */ "less_than_hundred ::= less_than_twenty",
 /*  84 */ "less_than_twentieth ::= tenth_to_19th",
 /*  85 */ "less_than_twentieth ::= first_to_9th",
 /*  86 */ "less_than_twenty ::= ten_to_19",
 /*  87 */ "less_than_twenty ::= less_than_ten",
 /*  88 */ "less_than_ten ::= ONE",
 /*  89 */ "less_than_ten ::= TWO",
 /*  90 */ "less_than_ten ::= THREE",
 /*  91 */ "less_than_ten ::= FOUR",
 /*  92 */ "less_than_ten ::= FIVE",
 /*  93 */ "less_than_ten ::= SIX",
 /*  94 */ "less_than_ten ::= SEVEN",
 /*  95 */ "less_than_ten ::= EIGHT",
 /*  96 */ "less_than_ten ::= NINE",
 /*  97 */ "ten_to_19 ::= TEN",
 /*  98 */ "ten_to_19 ::= ELEVEN",
 /*  99 */ "ten_to_19 ::= TWELVE",
 /* 100 */ "ten_to_19 ::= THIRTEEN",
 /* 101 */ "ten_to_19 ::= FOURTEEN",
 /* 102 */ "ten_to_19 ::= FIFTEEN",
 /* 103 */ "ten_to_19 ::= SIXTEEN",
 /* 104 */ "ten_to_19 ::= SEVENTEEN",
 /* 105 */ "ten_to_19 ::= EIGHTEEN",
 /* 106 */ "ten_to_19 ::= NINETEEN",
 /* 107 */ "tens ::= TWENTY",
 /* 108 */ "tens ::= THIRTY",
 /* 109 */ "tens ::= FORTY",
 /* 110 */ "tens ::= FIFTY",
 /* 111 */ "tens ::= SIXTY",
 /* 112 */ "tens ::= SEVENTY",
 /* 113 */ "tens ::= EIGHTY",
 /* 114 */ "tens ::= NINETY",
 /* 115 */ "first_to_9th ::= FIRST",
 /* 116 */ "first_to_9th ::= SECOND",
 /* 117 */ "first_to_9th ::= THIRD",
 /* 118 */ "first_to_9th ::= FOURTH",
 /* 119 */ "first_to_9th ::= FIFTH",
 /* 120 */ "first_to_9th ::= SIXTH",
 /* 121 */ "first_to_9th ::= SEVENTH",
 /* 122 */ "first_to_9th ::= EIGHTH",
 /* 123 */ "first_to_9th ::= NINTH",
 /* 124 */ "tenth_to_19th ::= TENTH",
 /* 125 */ "tenth_to_19th ::= ELEVENTH",
 /* 126 */ "tenth_to_19th ::= TWELFTH",
 /* 127 */ "tenth_to_19th ::= THIRTEENTH",
 /* 128 */ "tenth_to_19th ::= FOURTEENTH",
 /* 129 */ "tenth_to_19th ::= FIFTEENTH",
 /* 130 */ "tenth_to_19th ::= SIXTEENTH",
 /* 131 */ "tenth_to_19th ::= SEVENTEENTH",
 /* 132 */ "tenth_to_19th ::= EIGHTEENTH",
 /* 133 */ "tenth_to_19th ::= NINETEENTH",
 /* 134 */ "tenth ::= TWENTIETH",
 /* 135 */ "tenth ::= THIRTIETH",
 /* 136 */ "tenth ::= FOURTIETH",
 /* 137 */ "tenth ::= FIFTIETH",
 /* 138 */ "tenth ::= SIXTIETH",
 /* 139 */ "tenth ::= SEVENTIETH",
 /* 140 */ "tenth ::= EIGHTIETH",
 /* 141 */ "tenth ::= NINETIETH",
 /* 142 */ "program ::= numbers",
 /* 143 */ "numbers ::= numbers number",
 /* 144 */ "numbers ::= number",
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
  { 77, 1 },
  { 77, 2 },
  { 78, 3 },
  { 78, 2 },
  { 78, 3 },
  { 78, 2 },
  { 78, 1 },
  { 78, 1 },
  { 78, 1 },
  { 78, 1 },
  { 80, 3 },
  { 80, 2 },
  { 80, 1 },
  { 78, 1 },
  { 78, 2 },
  { 79, 3 },
  { 79, 2 },
  { 79, 1 },
  { 78, 1 },
  { 78, 2 },
  { 82, 1 },
  { 82, 1 },
  { 83, 3 },
  { 83, 2 },
  { 83, 1 },
  { 78, 1 },
  { 78, 2 },
  { 84, 1 },
  { 84, 1 },
  { 85, 3 },
  { 85, 2 },
  { 85, 1 },
  { 78, 1 },
  { 78, 2 },
  { 86, 1 },
  { 86, 1 },
  { 87, 3 },
  { 87, 2 },
  { 87, 1 },
  { 78, 1 },
  { 78, 2 },
  { 88, 1 },
  { 88, 1 },
  { 89, 3 },
  { 89, 2 },
  { 89, 1 },
  { 78, 1 },
  { 78, 2 },
  { 90, 1 },
  { 90, 1 },
  { 91, 3 },
  { 91, 2 },
  { 91, 1 },
  { 78, 1 },
  { 78, 2 },
  { 92, 1 },
  { 92, 1 },
  { 93, 3 },
  { 93, 2 },
  { 93, 1 },
  { 78, 1 },
  { 78, 2 },
  { 94, 2 },
  { 94, 1 },
  { 95, 4 },
  { 95, 3 },
  { 95, 2 },
  { 95, 1 },
  { 78, 1 },
  { 78, 2 },
  { 96, 2 },
  { 96, 1 },
  { 81, 4 },
  { 81, 3 },
  { 81, 2 },
  { 81, 1 },
  { 78, 1 },
  { 78, 2 },
  { 97, 2 },
  { 97, 1 },
  { 97, 1 },
  { 98, 2 },
  { 98, 1 },
  { 98, 1 },
  { 102, 1 },
  { 102, 1 },
  { 104, 1 },
  { 104, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 75, 1 },
  { 76, 2 },
  { 76, 1 },
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
#line 113 "parser.yy"
{
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1349 "parser.c"
        break;
      case 1: /* number ::= NEGATIVE final_number */
#line 116 "parser.yy"
{
    yymsp[0].minor.yy0.dbl = -yymsp[0].minor.yy0.dbl;
    yymsp[0].minor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin);
    yymsp[0].minor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end);
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1359 "parser.c"
        break;
      case 2: /* final_number ::= less_than_quadrillion AND_A QUARTER */
#line 123 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-2].minor.yy0.dbl + 0.25; yylhsminor.yy0.is_dbl = true; }
#line 1364 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* final_number ::= less_than_quadrillion QUARTERS */
#line 124 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl / 4.0; yylhsminor.yy0.is_dbl = true; }
#line 1370 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 4: /* final_number ::= less_than_quadrillion AND_A HALF */
#line 125 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-2].minor.yy0.dbl + 0.5; yylhsminor.yy0.is_dbl = true; }
#line 1376 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* final_number ::= less_than_quadrillion HALVES */
#line 126 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl / 2.0; yylhsminor.yy0.is_dbl = true; }
#line 1382 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 6: /* final_number ::= less_than_quadrillion */
      case 17: /* less_than_quadrillion ::= less_than_trillion */ yytestcase(yyruleno==17);
      case 27: /* less_than_trillion_end_only ::= less_than_trillion */ yytestcase(yyruleno==27);
      case 28: /* less_than_trillion_end_only ::= less_than_billion_end_only */ yytestcase(yyruleno==28);
      case 31: /* less_than_trillion ::= less_than_billion */ yytestcase(yyruleno==31);
      case 41: /* less_than_billion_end_only ::= less_than_billion */ yytestcase(yyruleno==41);
      case 42: /* less_than_billion_end_only ::= less_than_million_end_only */ yytestcase(yyruleno==42);
      case 45: /* less_than_billion ::= less_than_million */ yytestcase(yyruleno==45);
      case 55: /* less_than_million_end_only ::= less_than_million */ yytestcase(yyruleno==55);
      case 56: /* less_than_million_end_only ::= less_than_thousand_end_only */ yytestcase(yyruleno==56);
      case 59: /* less_than_million ::= less_than_thousand */ yytestcase(yyruleno==59);
      case 71: /* less_than_thousand_end_only ::= less_than_thousand */ yytestcase(yyruleno==71);
      case 75: /* less_than_thousand ::= less_than_hundred */ yytestcase(yyruleno==75);
      case 82: /* less_than_hundred ::= tens */ yytestcase(yyruleno==82);
      case 83: /* less_than_hundred ::= less_than_twenty */ yytestcase(yyruleno==83);
      case 86: /* less_than_twenty ::= ten_to_19 */ yytestcase(yyruleno==86);
      case 87: /* less_than_twenty ::= less_than_ten */ yytestcase(yyruleno==87);
#line 127 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; }
#line 1404 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* final_number ::= less_than_quadrillionth */
      case 12: /* less_than_quadrillionth ::= less_than_trillionth */ yytestcase(yyruleno==12);
      case 20: /* less_than_trillionth_end_only ::= less_than_trillionth */ yytestcase(yyruleno==20);
      case 21: /* less_than_trillionth_end_only ::= less_than_billionth_end_only */ yytestcase(yyruleno==21);
      case 24: /* less_than_trillionth ::= less_than_billionth */ yytestcase(yyruleno==24);
      case 34: /* less_than_billionth_end_only ::= less_than_billionth */ yytestcase(yyruleno==34);
      case 35: /* less_than_billionth_end_only ::= less_than_millionth_end_only */ yytestcase(yyruleno==35);
      case 38: /* less_than_billionth ::= less_than_millionth */ yytestcase(yyruleno==38);
      case 52: /* less_than_millionth ::= less_than_thousandth */ yytestcase(yyruleno==52);
      case 63: /* less_than_thousandth_end_only ::= less_than_thousandth */ yytestcase(yyruleno==63);
      case 67: /* less_than_thousandth ::= less_than_hundredth */ yytestcase(yyruleno==67);
      case 79: /* less_than_hundredth ::= tenth */ yytestcase(yyruleno==79);
      case 80: /* less_than_hundredth ::= less_than_twentieth */ yytestcase(yyruleno==80);
      case 84: /* less_than_twentieth ::= tenth_to_19th */ yytestcase(yyruleno==84);
      case 85: /* less_than_twentieth ::= first_to_9th */ yytestcase(yyruleno==85);
#line 128 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; yylhsminor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; }
#line 1424 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 8: /* final_number ::= NUMBER */
#line 145 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = true; }
#line 1430 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* final_number ::= ZERO */
#line 146 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 0.0; }
#line 1436 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 10: /* less_than_quadrillionth ::= less_than_thousand TRILLION less_than_trillionth_end_only */
#line 172 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1442 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 11: /* less_than_quadrillionth ::= less_than_thousand TRILLIONTH */
#line 173 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; yylhsminor.yy0.suffix = TH; }
#line 1448 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 13: /* final_number ::= TRILLIONTH */
#line 176 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000000000.0; yylhsminor.yy0.suffix = TH; }
#line 1454 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* final_number ::= NUMBER TRILLIONTH */
#line 177 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1460 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 15: /* less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only */
#line 188 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1466 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 16: /* less_than_quadrillion ::= less_than_thousand TRILLION */
#line 189 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1472 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 18: /* final_number ::= TRILLION */
#line 192 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000000000.0; }
#line 1478 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 19: /* final_number ::= NUMBER TRILLION */
#line 193 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000000.0; yylhsminor.yy0.is_dbl = true; }
#line 1484 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 22: /* less_than_trillionth ::= less_than_thousand BILLION less_than_billionth_end_only */
#line 202 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1490 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 23: /* less_than_trillionth ::= less_than_thousand BILLIONTH */
#line 203 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; yylhsminor.yy0.suffix = TH; }
#line 1496 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* final_number ::= BILLIONTH */
#line 206 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000000.0; yylhsminor.yy0.suffix = TH; }
#line 1502 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 26: /* final_number ::= NUMBER BILLIONTH */
#line 207 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1508 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only */
#line 221 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1514 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 30: /* less_than_trillion ::= less_than_thousand BILLION */
#line 222 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1520 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* final_number ::= BILLION */
#line 225 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000000.0; }
#line 1526 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 33: /* final_number ::= NUMBER BILLION */
#line 226 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000.0; yylhsminor.yy0.is_dbl = true; }
#line 1532 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 36: /* less_than_billionth ::= less_than_thousand MILLION less_than_millionth_end_only */
#line 235 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1538 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 37: /* less_than_billionth ::= less_than_thousand MILLIONTH */
#line 236 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; yylhsminor.yy0.suffix = TH; }
#line 1544 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 39: /* final_number ::= MILLIONTH */
#line 239 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000.0; yylhsminor.yy0.suffix = TH; }
#line 1550 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 40: /* final_number ::= NUMBER MILLIONTH */
#line 240 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1556 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 43: /* less_than_billion ::= less_than_thousand MILLION less_than_million_end_only */
#line 254 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1562 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 44: /* less_than_billion ::= less_than_thousand MILLION */
#line 255 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1568 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 46: /* final_number ::= MILLION */
#line 258 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000.0; }
#line 1574 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 47: /* final_number ::= NUMBER MILLION */
#line 259 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000.0; yylhsminor.yy0.is_dbl = true; }
#line 1580 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 48: /* less_than_millionth_end_only ::= less_than_millionth */
      case 49: /* less_than_millionth_end_only ::= less_than_thousandth_end_only */ yytestcase(yyruleno==49);
#line 265 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1587 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 50: /* less_than_millionth ::= less_than_thousand THOUSAND less_than_thousandth_end_only */
#line 268 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1593 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 51: /* less_than_millionth ::= less_than_thousand THOUSANDTH */
#line 269 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; yylhsminor.yy0.suffix = TH; }
#line 1599 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 53: /* final_number ::= THOUSANDTH */
#line 272 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000.0; yylhsminor.yy0.suffix = TH; }
#line 1605 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 54: /* final_number ::= NUMBER THOUSANDTH */
#line 273 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1611 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 57: /* less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only */
#line 287 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1617 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 58: /* less_than_million ::= less_than_thousand THOUSAND */
#line 288 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1623 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 60: /* final_number ::= THOUSAND */
#line 291 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000.0; }
#line 1629 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 61: /* final_number ::= NUMBER THOUSAND */
#line 292 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000.0; yylhsminor.yy0.is_dbl = true; }
#line 1635 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 62: /* less_than_thousandth_end_only ::= AND less_than_hundredth */
#line 298 "parser.yy"
{ yymsp[-1].minor.yy0.begin = yymsp[0].minor.yy0.begin; yymsp[-1].minor.yy0.end = yymsp[0].minor.yy0.end; yymsp[-1].minor.yy0.dbl = yymsp[0].minor.yy0.dbl; yymsp[-1].minor.yy0.suffix = yymsp[0].minor.yy0.suffix; yymsp[-1].minor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; }
#line 1641 "parser.c"
        break;
      case 64: /* less_than_thousandth ::= less_than_hundred HUNDRED AND less_than_hundredth */
#line 301 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-3].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-3].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-3].minor.yy0.dbl * 100.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-3].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1646 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 65: /* less_than_thousandth ::= less_than_hundred HUNDRED less_than_hundredth */
#line 302 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 100.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1652 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 66: /* less_than_thousandth ::= less_than_hundred HUNDREDTH */
#line 303 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 100.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; yylhsminor.yy0.suffix = TH; }
#line 1658 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 68: /* final_number ::= HUNDREDTH */
#line 306 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 100.0; yylhsminor.yy0.suffix = TH; }
#line 1664 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 69: /* final_number ::= NUMBER HUNDREDTH */
#line 307 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 100.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1670 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 70: /* less_than_thousand_end_only ::= AND less_than_hundred */
#line 318 "parser.yy"
{ yymsp[-1].minor.yy0.begin = yymsp[0].minor.yy0.begin; yymsp[-1].minor.yy0.end = yymsp[0].minor.yy0.end; yymsp[-1].minor.yy0.dbl = yymsp[0].minor.yy0.dbl; yymsp[-1].minor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; }
#line 1676 "parser.c"
        break;
      case 72: /* less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred */
#line 321 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-3].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-3].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-3].minor.yy0.dbl * 100.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-3].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1681 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 73: /* less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred */
#line 322 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 100.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1687 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 74: /* less_than_thousand ::= less_than_hundred HUNDRED */
#line 323 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 100.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1693 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 76: /* final_number ::= HUNDRED */
#line 326 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 100.0; }
#line 1699 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 77: /* final_number ::= NUMBER HUNDRED */
#line 327 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 100.0; yylhsminor.yy0.is_dbl = true; }
#line 1705 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 78: /* less_than_hundredth ::= tens first_to_9th */
#line 333 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1711 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 81: /* less_than_hundred ::= tens less_than_ten */
#line 345 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1717 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 88: /* less_than_ten ::= ONE */
#line 370 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1.0; }
#line 1723 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 89: /* less_than_ten ::= TWO */
#line 371 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 2.0; }
#line 1729 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 90: /* less_than_ten ::= THREE */
#line 372 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 3.0; }
#line 1735 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 91: /* less_than_ten ::= FOUR */
#line 373 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 4.0; }
#line 1741 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 92: /* less_than_ten ::= FIVE */
#line 374 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 5.0; }
#line 1747 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 93: /* less_than_ten ::= SIX */
#line 375 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 6.0; }
#line 1753 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 94: /* less_than_ten ::= SEVEN */
#line 376 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 7.0; }
#line 1759 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 95: /* less_than_ten ::= EIGHT */
#line 377 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 8.0; }
#line 1765 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 96: /* less_than_ten ::= NINE */
#line 378 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 9.0; }
#line 1771 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 97: /* ten_to_19 ::= TEN */
#line 380 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 10.0; }
#line 1777 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 98: /* ten_to_19 ::= ELEVEN */
#line 381 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 11.0; }
#line 1783 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 99: /* ten_to_19 ::= TWELVE */
#line 382 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 12.0; }
#line 1789 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 100: /* ten_to_19 ::= THIRTEEN */
#line 383 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 13.0; }
#line 1795 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 101: /* ten_to_19 ::= FOURTEEN */
#line 384 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 14.0; }
#line 1801 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 102: /* ten_to_19 ::= FIFTEEN */
#line 385 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 15.0; }
#line 1807 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 103: /* ten_to_19 ::= SIXTEEN */
#line 386 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 16.0; }
#line 1813 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 104: /* ten_to_19 ::= SEVENTEEN */
#line 387 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 17.0; }
#line 1819 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 105: /* ten_to_19 ::= EIGHTEEN */
#line 388 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 18.0; }
#line 1825 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 106: /* ten_to_19 ::= NINETEEN */
#line 389 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 19.0; }
#line 1831 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 107: /* tens ::= TWENTY */
#line 391 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 20.0; }
#line 1837 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 108: /* tens ::= THIRTY */
#line 392 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 30.0; }
#line 1843 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 109: /* tens ::= FORTY */
#line 393 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 40.0; }
#line 1849 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 110: /* tens ::= FIFTY */
#line 394 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 50.0; }
#line 1855 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 111: /* tens ::= SIXTY */
#line 395 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 60.0; }
#line 1861 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 112: /* tens ::= SEVENTY */
#line 396 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 70.0; }
#line 1867 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 113: /* tens ::= EIGHTY */
#line 397 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 80.0; }
#line 1873 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 114: /* tens ::= NINETY */
#line 398 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 90.0; }
#line 1879 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 115: /* first_to_9th ::= FIRST */
#line 400 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1.0; yylhsminor.yy0.suffix = ST; }
#line 1885 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 116: /* first_to_9th ::= SECOND */
#line 401 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 2.0; yylhsminor.yy0.suffix = ND; }
#line 1891 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 117: /* first_to_9th ::= THIRD */
#line 402 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 3.0; yylhsminor.yy0.suffix = RD; }
#line 1897 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 118: /* first_to_9th ::= FOURTH */
#line 403 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 4.0; yylhsminor.yy0.suffix = TH; }
#line 1903 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 119: /* first_to_9th ::= FIFTH */
#line 404 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 5.0; yylhsminor.yy0.suffix = TH; }
#line 1909 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 120: /* first_to_9th ::= SIXTH */
#line 405 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 6.0; yylhsminor.yy0.suffix = TH; }
#line 1915 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 121: /* first_to_9th ::= SEVENTH */
#line 406 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 7.0; yylhsminor.yy0.suffix = TH; }
#line 1921 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 122: /* first_to_9th ::= EIGHTH */
#line 407 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 8.0; yylhsminor.yy0.suffix = TH; }
#line 1927 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 123: /* first_to_9th ::= NINTH */
#line 408 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 9.0; yylhsminor.yy0.suffix = TH; }
#line 1933 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 124: /* tenth_to_19th ::= TENTH */
#line 410 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 10.0; yylhsminor.yy0.suffix = TH; }
#line 1939 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 125: /* tenth_to_19th ::= ELEVENTH */
#line 411 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 11.0; yylhsminor.yy0.suffix = TH; }
#line 1945 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 126: /* tenth_to_19th ::= TWELFTH */
#line 412 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 12.0; yylhsminor.yy0.suffix = TH; }
#line 1951 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 127: /* tenth_to_19th ::= THIRTEENTH */
#line 413 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 13.0; yylhsminor.yy0.suffix = TH; }
#line 1957 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 128: /* tenth_to_19th ::= FOURTEENTH */
#line 414 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 14.0; yylhsminor.yy0.suffix = TH; }
#line 1963 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 129: /* tenth_to_19th ::= FIFTEENTH */
#line 415 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 15.0; yylhsminor.yy0.suffix = TH; }
#line 1969 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 130: /* tenth_to_19th ::= SIXTEENTH */
#line 416 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 16.0; yylhsminor.yy0.suffix = TH; }
#line 1975 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 131: /* tenth_to_19th ::= SEVENTEENTH */
#line 417 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 17.0; yylhsminor.yy0.suffix = TH; }
#line 1981 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 132: /* tenth_to_19th ::= EIGHTEENTH */
#line 418 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 18.0; yylhsminor.yy0.suffix = TH; }
#line 1987 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 133: /* tenth_to_19th ::= NINETEENTH */
#line 419 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 19.0; yylhsminor.yy0.suffix = TH; }
#line 1993 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 134: /* tenth ::= TWENTIETH */
#line 421 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 20.0; yylhsminor.yy0.suffix = TH; }
#line 1999 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 135: /* tenth ::= THIRTIETH */
#line 422 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 30.0; yylhsminor.yy0.suffix = TH; }
#line 2005 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 136: /* tenth ::= FOURTIETH */
#line 423 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 40.0; yylhsminor.yy0.suffix = TH; }
#line 2011 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 137: /* tenth ::= FIFTIETH */
#line 424 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 50.0; yylhsminor.yy0.suffix = TH; }
#line 2017 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 138: /* tenth ::= SIXTIETH */
#line 425 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 60.0; yylhsminor.yy0.suffix = TH; }
#line 2023 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 139: /* tenth ::= SEVENTIETH */
#line 426 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 70.0; yylhsminor.yy0.suffix = TH; }
#line 2029 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 140: /* tenth ::= EIGHTIETH */
#line 427 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 80.0; yylhsminor.yy0.suffix = TH; }
#line 2035 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 141: /* tenth ::= NINETIETH */
#line 428 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 90.0; yylhsminor.yy0.suffix = TH; }
#line 2041 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      default:
      /* (142) program ::= numbers */ yytestcase(yyruleno==142);
      /* (143) numbers ::= numbers number */ yytestcase(yyruleno==143);
      /* (144) numbers ::= number (OPTIMIZED OUT) */ assert(yyruleno!=144);
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
#line 94 "parser.yy"

    state->error = PARSE_FAILURE;
#if print_errors
    fprintf(stderr,"Giving up.  Parser is hopelessly lost...\n");
#endif
#line 2094 "parser.c"
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
#line 87 "parser.yy"

    state->error = SYNTAX_ERROR;
#if print_errors
    fprintf(stderr, "Syntax error\n");
#endif
#line 2117 "parser.c"
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
