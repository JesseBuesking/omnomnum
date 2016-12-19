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
#define TOKEN_CHARACTERS                      1
#define TOKEN_NEGATIVE                        2
#define TOKEN_AND_A                           3
#define TOKEN_QUARTER                         4
#define TOKEN_QUARTERS                        5
#define TOKEN_HALF                            6
#define TOKEN_HALVES                          7
#define TOKEN_NUMBER                          8
#define TOKEN_ZERO                            9
#define TOKEN_TRILLION                       10
#define TOKEN_TRILLIONTH                     11
#define TOKEN_BILLION                        12
#define TOKEN_BILLIONTH                      13
#define TOKEN_MILLION                        14
#define TOKEN_MILLIONTH                      15
#define TOKEN_THOUSAND                       16
#define TOKEN_THOUSANDTH                     17
#define TOKEN_HUNDRED                        18
#define TOKEN_AND                            19
#define TOKEN_HUNDREDTH                      20
#define TOKEN_ONE                            21
#define TOKEN_TWO                            22
#define TOKEN_THREE                          23
#define TOKEN_FOUR                           24
#define TOKEN_FIVE                           25
#define TOKEN_SIX                            26
#define TOKEN_SEVEN                          27
#define TOKEN_EIGHT                          28
#define TOKEN_NINE                           29
#define TOKEN_TEN                            30
#define TOKEN_ELEVEN                         31
#define TOKEN_TWELVE                         32
#define TOKEN_THIRTEEN                       33
#define TOKEN_FOURTEEN                       34
#define TOKEN_FIFTEEN                        35
#define TOKEN_SIXTEEN                        36
#define TOKEN_SEVENTEEN                      37
#define TOKEN_EIGHTEEN                       38
#define TOKEN_NINETEEN                       39
#define TOKEN_TWENTY                         40
#define TOKEN_THIRTY                         41
#define TOKEN_FORTY                          42
#define TOKEN_FIFTY                          43
#define TOKEN_SIXTY                          44
#define TOKEN_SEVENTY                        45
#define TOKEN_EIGHTY                         46
#define TOKEN_NINETY                         47
#define TOKEN_FIRST                          48
#define TOKEN_SECOND                         49
#define TOKEN_THIRD                          50
#define TOKEN_FOURTH                         51
#define TOKEN_FIFTH                          52
#define TOKEN_SIXTH                          53
#define TOKEN_SEVENTH                        54
#define TOKEN_EIGHTH                         55
#define TOKEN_NINTH                          56
#define TOKEN_TENTH                          57
#define TOKEN_ELEVENTH                       58
#define TOKEN_TWELFTH                        59
#define TOKEN_THIRTEENTH                     60
#define TOKEN_FOURTEENTH                     61
#define TOKEN_FIFTEENTH                      62
#define TOKEN_SIXTEENTH                      63
#define TOKEN_SEVENTEENTH                    64
#define TOKEN_EIGHTEENTH                     65
#define TOKEN_NINETEENTH                     66
#define TOKEN_TWENTIETH                      67
#define TOKEN_THIRTIETH                      68
#define TOKEN_FOURTIETH                      69
#define TOKEN_FIFTIETH                       70
#define TOKEN_SIXTIETH                       71
#define TOKEN_SEVENTIETH                     72
#define TOKEN_EIGHTIETH                      73
#define TOKEN_NINETIETH                      74
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
#define YYNOCODE 112
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
#define YYNSTATE             24
#define YYNRULE              149
#define YY_MAX_SHIFT         23
#define YY_MIN_SHIFTREDUCE   161
#define YY_MAX_SHIFTREDUCE   309
#define YY_MIN_REDUCE        310
#define YY_MAX_REDUCE        458
#define YY_ERROR_ACTION      459
#define YY_ACCEPT_ACTION     460
#define YY_NO_ACTION         461
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
#define YY_ACTTAB_COUNT (726)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   307,    3,    6,  198,    7,  211,  237,   14,  170,  180,
 /*    10 */   175,  193,  186,  206,  199,  219,  212,  235,   11,  227,
 /*    20 */   246,  247,  248,  249,  250,  251,  252,  253,  254,  255,
 /*    30 */   256,  257,  258,  259,  260,  261,  262,  263,  264,  265,
 /*    40 */   266,  267,  268,  269,  270,  271,  272,  273,  274,  275,
 /*    50 */   276,  277,  278,  279,  280,  281,  282,  283,  284,  285,
 /*    60 */   286,  287,  288,  289,  290,  291,  292,  293,  294,  295,
 /*    70 */   296,  297,  298,  299,  306,    3,  237,  450,    8,  241,
 /*    80 */   226,   14,  170,  180,  175,  193,  186,  206,  199,  219,
 /*    90 */   212,  235,   11,  227,  246,  247,  248,  249,  250,  251,
 /*   100 */   252,  253,  254,  255,  256,  257,  258,  259,  260,  261,
 /*   110 */   262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
 /*   120 */   272,  273,  274,  275,  276,  277,  278,  279,  280,  281,
 /*   130 */   282,  283,  284,  285,  286,  287,  288,  289,  290,  291,
 /*   140 */   292,  293,  294,  295,  296,  297,  298,  299,   14,  170,
 /*   150 */   180,  175,  193,  186,  206,  199,  219,  212,  235,   11,
 /*   160 */   227,  246,  247,  248,  249,  250,  251,  252,  253,  254,
 /*   170 */   255,  256,  257,  258,  259,  260,  261,  262,  263,  264,
 /*   180 */   265,  266,  267,  268,  269,  270,  271,  272,  273,  274,
 /*   190 */   275,  276,  277,  278,  279,  280,  281,  282,  283,  284,
 /*   200 */   285,  286,  287,  288,  289,  290,  291,  292,  293,  294,
 /*   210 */   295,  296,  297,  298,  299,    9,  449,  246,  247,  248,
 /*   220 */   249,  250,  251,  252,  253,  254,  255,  256,  257,  258,
 /*   230 */   259,  260,  261,  262,  263,  264,  265,  266,  267,  268,
 /*   240 */   269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
 /*   250 */   279,  280,  281,  282,  283,  284,  285,  286,  287,  288,
 /*   260 */   289,  290,  291,  292,  293,  294,  295,  296,  297,  298,
 /*   270 */   299,   10,  312,  246,  247,  248,  249,  250,  251,  252,
 /*   280 */   253,  254,  255,  256,  257,  258,  259,  260,  261,  262,
 /*   290 */   263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
 /*   300 */   273,  274,  275,  276,  277,  278,  279,  280,  281,  282,
 /*   310 */   283,  284,  285,  286,  287,  288,  289,  290,  291,  292,
 /*   320 */   293,  294,  295,  296,  297,  298,  299,  246,  247,  248,
 /*   330 */   249,  250,  251,  252,  253,  254,  255,  256,  257,  258,
 /*   340 */   259,  260,  261,  262,  263,  264,  265,  266,  267,  268,
 /*   350 */   269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
 /*   360 */   279,  280,  281,  282,  283,  284,  285,  286,  287,  288,
 /*   370 */   289,  290,  291,  292,  293,  294,  295,  296,  297,  298,
 /*   380 */   299,  265,  266,  267,  268,  269,  270,  271,  272,  273,
 /*   390 */   274,  275,  276,  277,  278,  279,  280,  281,  282,  283,
 /*   400 */   284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
 /*   410 */   294,  295,  296,  297,  298,  299,  460,   23,   22,    2,
 /*   420 */     1,    2,  161,   18,  168,   15,  173,  172,  312,  179,
 /*   430 */   184,  183,  312,  192,  197,  196,  312,  205,  210,  209,
 /*   440 */   312,   19,  225,  223,   12,  240,  238,  239,  245,  243,
 /*   450 */   244,  304,  163,    2,  165,    2,  161,   18,  168,   15,
 /*   460 */   173,  172,  312,  179,  184,  183,  312,  192,  197,  196,
 /*   470 */   312,  205,  210,  209,  312,   19,  225,  223,   12,  240,
 /*   480 */   238,  239,  245,  243,  244,  302,  312,    1,  308,  161,
 /*   490 */    18,  168,   15,  173,  172,  312,  179,  184,  183,  312,
 /*   500 */   192,  197,  196,  312,  205,  210,  209,  312,   19,  225,
 /*   510 */   223,   12,  240,  238,  239,  245,  243,  244,  162,   18,
 /*   520 */   168,   15,  173,  172,  312,  179,  184,  183,  312,  192,
 /*   530 */   197,  196,  312,  205,  210,  209,  312,   19,  225,  223,
 /*   540 */    12,  240,  238,  239,  245,  243,  244,   16,  171,  312,
 /*   550 */   177,  188,  184,  183,  189,  192,  197,  196,  202,  205,
 /*   560 */   210,  209,  215,   19,  225,  223,   12,  240,  238,  239,
 /*   570 */   245,  243,  244,   17,    7,  211,  312,  312,  182,  312,
 /*   580 */   190,  201,  197,  196,  202,  205,  210,  209,  215,   19,
 /*   590 */   225,  223,   12,  240,  238,  239,  245,  243,  244,  230,
 /*   600 */   246,  247,  248,  249,  250,  251,  252,  253,  254,   21,
 /*   610 */   312,  164,  208,  166,  216,   19,  225,  223,   12,  240,
 /*   620 */   238,  239,  245,  243,  244,  312,  312,  273,  274,  275,
 /*   630 */   276,  277,  278,  279,  280,  281,   20,    5,  185,    6,
 /*   640 */   198,    7,  211,  312,  312,  195,  312,  203,  214,  210,
 /*   650 */   209,  215,   19,  225,  223,   12,  240,  238,  239,  245,
 /*   660 */   243,  244,  181,  176,  194,  187,  207,  200,  220,  213,
 /*   670 */   236,  312,  228,  232,  222,  312,   12,  240,  238,  239,
 /*   680 */   245,  243,  244,  229,  224,  312,   12,  240,  238,  239,
 /*   690 */   245,  243,  244,  231,  221,  312,   12,  240,  238,  239,
 /*   700 */   245,  243,  244,  273,  274,  275,  276,  277,  278,  279,
 /*   710 */   280,  281,    4,  174,    5,  185,    6,  198,    7,  211,
 /*   720 */   224,  312,   13,  240,  238,  239,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,   14,   15,   16,   17,  105,    8,    9,   10,
 /*    10 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*    20 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*    30 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*    40 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*    50 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*    60 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*    70 */    71,   72,   73,   74,    1,    2,  105,    0,   18,  108,
 /*    80 */    20,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*    90 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*   100 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   110 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*   120 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   130 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   140 */    67,   68,   69,   70,   71,   72,   73,   74,    8,    9,
 /*   150 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*   160 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*   170 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   180 */    40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
 /*   190 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   200 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   210 */    70,   71,   72,   73,   74,   19,    0,   21,   22,   23,
 /*   220 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   230 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*   240 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   250 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*   260 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*   270 */    74,   19,  111,   21,   22,   23,   24,   25,   26,   27,
 /*   280 */    28,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*   290 */    38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
 /*   300 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   310 */    58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   320 */    68,   69,   70,   71,   72,   73,   74,   21,   22,   23,
 /*   330 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   340 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*   350 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   360 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*   370 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*   380 */    74,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*   390 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   400 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*   410 */    69,   70,   71,   72,   73,   74,   76,   77,   78,   79,
 /*   420 */    80,   81,   82,   83,   84,   85,   86,   87,  111,   89,
 /*   430 */    90,   91,  111,   93,   94,   95,  111,   97,   98,   99,
 /*   440 */   111,  101,  102,  103,  104,  105,  106,  107,  108,  109,
 /*   450 */   110,   77,    4,   79,    6,   81,   82,   83,   84,   85,
 /*   460 */    86,   87,  111,   89,   90,   91,  111,   93,   94,   95,
 /*   470 */   111,   97,   98,   99,  111,  101,  102,  103,  104,  105,
 /*   480 */   106,  107,  108,  109,  110,   78,  111,   80,   81,   82,
 /*   490 */    83,   84,   85,   86,   87,  111,   89,   90,   91,  111,
 /*   500 */    93,   94,   95,  111,   97,   98,   99,  111,  101,  102,
 /*   510 */   103,  104,  105,  106,  107,  108,  109,  110,   82,   83,
 /*   520 */    84,   85,   86,   87,  111,   89,   90,   91,  111,   93,
 /*   530 */    94,   95,  111,   97,   98,   99,  111,  101,  102,  103,
 /*   540 */   104,  105,  106,  107,  108,  109,  110,   85,   86,  111,
 /*   550 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*   560 */    98,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*   570 */   108,  109,  110,   85,   16,   17,  111,  111,   90,  111,
 /*   580 */    92,   93,   94,   95,   96,   97,   98,   99,  100,  101,
 /*   590 */   102,  103,  104,  105,  106,  107,  108,  109,  110,   85,
 /*   600 */    21,   22,   23,   24,   25,   26,   27,   28,   29,    3,
 /*   610 */   111,    5,   98,    7,  100,  101,  102,  103,  104,  105,
 /*   620 */   106,  107,  108,  109,  110,  111,  111,   48,   49,   50,
 /*   630 */    51,   52,   53,   54,   55,   56,   85,   12,   13,   14,
 /*   640 */    15,   16,   17,  111,  111,   94,  111,   96,   97,   98,
 /*   650 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*   660 */   109,  110,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   670 */    18,  111,   20,  101,  102,  111,  104,  105,  106,  107,
 /*   680 */   108,  109,  110,  101,  102,  111,  104,  105,  106,  107,
 /*   690 */   108,  109,  110,  101,  102,  111,  104,  105,  106,  107,
 /*   700 */   108,  109,  110,   48,   49,   50,   51,   52,   53,   54,
 /*   710 */    55,   56,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   720 */   102,  111,  104,  105,  106,  107,
};
#define YY_SHIFT_USE_DFLT (726)
#define YY_SHIFT_COUNT    (23)
#define YY_SHIFT_MIN      (-12)
#define YY_SHIFT_MAX      (702)
static const short yy_shift_ofst[] = {
 /*     0 */    -1,   73,   -1,  140,  196,  196,  196,  196,  252,  306,
 /*    10 */   306,  341,  579,  655,  652,  702,  625,  -12,  606,   60,
 /*    20 */   558,  448,   77,  216,
};
#define YY_REDUCE_USE_DFLT (-100)
#define YY_REDUCE_COUNT (13)
#define YY_REDUCE_MIN   (-99)
#define YY_REDUCE_MAX   (618)
static const short yy_reduce_ofst[] = {
 /*     0 */   340,  374,  407,  436,  462,  488,  551,  514,  572,  582,
 /*    10 */   592,  618,  -29,  -99,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   459,  454,  452,  459,  327,  340,  353,  366,  382,  459,
 /*    10 */   459,  459,  391,  459,  318,  367,  367,  367,  316,  383,
 /*    20 */   367,  459,  459,  459,
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
  "$",             "CHARACTERS",    "NEGATIVE",      "AND_A",       
  "QUARTER",       "QUARTERS",      "HALF",          "HALVES",      
  "NUMBER",        "ZERO",          "TRILLION",      "TRILLIONTH",  
  "BILLION",       "BILLIONTH",     "MILLION",       "MILLIONTH",   
  "THOUSAND",      "THOUSANDTH",    "HUNDRED",       "AND",         
  "HUNDREDTH",     "ONE",           "TWO",           "THREE",       
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
  "SEVENTIETH",    "EIGHTIETH",     "NINETIETH",     "error",       
  "program",       "num_nonnum",    "nonnum_num",    "numbers",     
  "nonnum",        "number",        "final_number",  "less_than_quadrillion",
  "first_to_999999999999999th",  "less_than_thousand",  "first_to_999999999999th",  "trillionths", 
  "less_than_trillion_end_only",  "less_than_trillion",  "first_to_999999999th",  "billionths",  
  "less_than_billion_end_only",  "less_than_billion",  "first_to_999999th",  "millionths",  
  "less_than_million_end_only",  "less_than_million",  "first_to_999th",  "thousandths", 
  "less_than_thousand_end_only",  "less_than_hundred",  "first_to_99th",  "hundredths",  
  "tens",          "first_to_9th",  "tenth",         "tenth_to_19th",
  "less_than_ten",  "less_than_twenty",  "ten_to_19",   
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
 /*   7 */ "final_number ::= first_to_999999999999999th",
 /*   8 */ "final_number ::= NUMBER",
 /*   9 */ "final_number ::= ZERO",
 /*  10 */ "first_to_999999999999999th ::= less_than_thousand TRILLION first_to_999999999999th",
 /*  11 */ "first_to_999999999999999th ::= trillionths",
 /*  12 */ "first_to_999999999999999th ::= first_to_999999999999th",
 /*  13 */ "trillionths ::= less_than_thousand TRILLIONTH",
 /*  14 */ "final_number ::= TRILLIONTH",
 /*  15 */ "final_number ::= NUMBER TRILLIONTH",
 /*  16 */ "less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only",
 /*  17 */ "less_than_quadrillion ::= less_than_thousand TRILLION",
 /*  18 */ "less_than_quadrillion ::= less_than_trillion",
 /*  19 */ "final_number ::= TRILLION",
 /*  20 */ "final_number ::= NUMBER TRILLION",
 /*  21 */ "first_to_999999999999th ::= less_than_thousand BILLION first_to_999999999th",
 /*  22 */ "first_to_999999999999th ::= billionths",
 /*  23 */ "first_to_999999999999th ::= first_to_999999999th",
 /*  24 */ "billionths ::= less_than_thousand BILLIONTH",
 /*  25 */ "final_number ::= BILLIONTH",
 /*  26 */ "final_number ::= NUMBER BILLIONTH",
 /*  27 */ "less_than_trillion_end_only ::= less_than_trillion",
 /*  28 */ "less_than_trillion_end_only ::= less_than_billion_end_only",
 /*  29 */ "less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only",
 /*  30 */ "less_than_trillion ::= less_than_thousand BILLION",
 /*  31 */ "less_than_trillion ::= less_than_billion",
 /*  32 */ "final_number ::= BILLION",
 /*  33 */ "final_number ::= NUMBER BILLION",
 /*  34 */ "first_to_999999999th ::= less_than_thousand MILLION first_to_999999th",
 /*  35 */ "first_to_999999999th ::= millionths",
 /*  36 */ "first_to_999999999th ::= first_to_999999th",
 /*  37 */ "millionths ::= less_than_thousand MILLIONTH",
 /*  38 */ "final_number ::= MILLIONTH",
 /*  39 */ "final_number ::= NUMBER MILLIONTH",
 /*  40 */ "less_than_billion_end_only ::= less_than_billion",
 /*  41 */ "less_than_billion_end_only ::= less_than_million_end_only",
 /*  42 */ "less_than_billion ::= less_than_thousand MILLION less_than_million_end_only",
 /*  43 */ "less_than_billion ::= less_than_thousand MILLION",
 /*  44 */ "less_than_billion ::= less_than_million",
 /*  45 */ "final_number ::= MILLION",
 /*  46 */ "final_number ::= NUMBER MILLION",
 /*  47 */ "first_to_999999th ::= less_than_thousand THOUSAND first_to_999th",
 /*  48 */ "first_to_999999th ::= thousandths",
 /*  49 */ "first_to_999999th ::= first_to_999th",
 /*  50 */ "thousandths ::= less_than_thousand THOUSANDTH",
 /*  51 */ "final_number ::= THOUSANDTH",
 /*  52 */ "final_number ::= NUMBER THOUSANDTH",
 /*  53 */ "less_than_million_end_only ::= less_than_million",
 /*  54 */ "less_than_million_end_only ::= less_than_thousand_end_only",
 /*  55 */ "less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only",
 /*  56 */ "less_than_million ::= less_than_thousand THOUSAND",
 /*  57 */ "less_than_million ::= less_than_thousand",
 /*  58 */ "final_number ::= THOUSAND",
 /*  59 */ "final_number ::= NUMBER THOUSAND",
 /*  60 */ "first_to_999th ::= less_than_hundred HUNDRED AND first_to_99th",
 /*  61 */ "first_to_999th ::= less_than_hundred HUNDRED first_to_99th",
 /*  62 */ "first_to_999th ::= hundredths",
 /*  63 */ "first_to_999th ::= AND first_to_99th",
 /*  64 */ "first_to_999th ::= first_to_99th",
 /*  65 */ "hundredths ::= less_than_hundred HUNDREDTH",
 /*  66 */ "final_number ::= HUNDREDTH",
 /*  67 */ "final_number ::= NUMBER HUNDREDTH",
 /*  68 */ "less_than_thousand_end_only ::= AND less_than_hundred",
 /*  69 */ "less_than_thousand_end_only ::= less_than_thousand",
 /*  70 */ "less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred",
 /*  71 */ "less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred",
 /*  72 */ "less_than_thousand ::= less_than_hundred HUNDRED",
 /*  73 */ "less_than_thousand ::= less_than_hundred",
 /*  74 */ "final_number ::= HUNDRED",
 /*  75 */ "final_number ::= NUMBER HUNDRED",
 /*  76 */ "first_to_99th ::= tens first_to_9th",
 /*  77 */ "first_to_99th ::= tenth",
 /*  78 */ "first_to_99th ::= tenth_to_19th",
 /*  79 */ "first_to_99th ::= first_to_9th",
 /*  80 */ "less_than_hundred ::= tens less_than_ten",
 /*  81 */ "less_than_hundred ::= tens",
 /*  82 */ "less_than_hundred ::= less_than_twenty",
 /*  83 */ "less_than_twenty ::= ten_to_19",
 /*  84 */ "less_than_twenty ::= less_than_ten",
 /*  85 */ "less_than_ten ::= ONE",
 /*  86 */ "less_than_ten ::= TWO",
 /*  87 */ "less_than_ten ::= THREE",
 /*  88 */ "less_than_ten ::= FOUR",
 /*  89 */ "less_than_ten ::= FIVE",
 /*  90 */ "less_than_ten ::= SIX",
 /*  91 */ "less_than_ten ::= SEVEN",
 /*  92 */ "less_than_ten ::= EIGHT",
 /*  93 */ "less_than_ten ::= NINE",
 /*  94 */ "ten_to_19 ::= TEN",
 /*  95 */ "ten_to_19 ::= ELEVEN",
 /*  96 */ "ten_to_19 ::= TWELVE",
 /*  97 */ "ten_to_19 ::= THIRTEEN",
 /*  98 */ "ten_to_19 ::= FOURTEEN",
 /*  99 */ "ten_to_19 ::= FIFTEEN",
 /* 100 */ "ten_to_19 ::= SIXTEEN",
 /* 101 */ "ten_to_19 ::= SEVENTEEN",
 /* 102 */ "ten_to_19 ::= EIGHTEEN",
 /* 103 */ "ten_to_19 ::= NINETEEN",
 /* 104 */ "tens ::= TWENTY",
 /* 105 */ "tens ::= THIRTY",
 /* 106 */ "tens ::= FORTY",
 /* 107 */ "tens ::= FIFTY",
 /* 108 */ "tens ::= SIXTY",
 /* 109 */ "tens ::= SEVENTY",
 /* 110 */ "tens ::= EIGHTY",
 /* 111 */ "tens ::= NINETY",
 /* 112 */ "first_to_9th ::= FIRST",
 /* 113 */ "first_to_9th ::= SECOND",
 /* 114 */ "first_to_9th ::= THIRD",
 /* 115 */ "first_to_9th ::= FOURTH",
 /* 116 */ "first_to_9th ::= FIFTH",
 /* 117 */ "first_to_9th ::= SIXTH",
 /* 118 */ "first_to_9th ::= SEVENTH",
 /* 119 */ "first_to_9th ::= EIGHTH",
 /* 120 */ "first_to_9th ::= NINTH",
 /* 121 */ "tenth_to_19th ::= TENTH",
 /* 122 */ "tenth_to_19th ::= ELEVENTH",
 /* 123 */ "tenth_to_19th ::= TWELFTH",
 /* 124 */ "tenth_to_19th ::= THIRTEENTH",
 /* 125 */ "tenth_to_19th ::= FOURTEENTH",
 /* 126 */ "tenth_to_19th ::= FIFTEENTH",
 /* 127 */ "tenth_to_19th ::= SIXTEENTH",
 /* 128 */ "tenth_to_19th ::= SEVENTEENTH",
 /* 129 */ "tenth_to_19th ::= EIGHTEENTH",
 /* 130 */ "tenth_to_19th ::= NINETEENTH",
 /* 131 */ "tenth ::= TWENTIETH",
 /* 132 */ "tenth ::= THIRTIETH",
 /* 133 */ "tenth ::= FOURTIETH",
 /* 134 */ "tenth ::= FIFTIETH",
 /* 135 */ "tenth ::= SIXTIETH",
 /* 136 */ "tenth ::= SEVENTIETH",
 /* 137 */ "tenth ::= EIGHTIETH",
 /* 138 */ "tenth ::= NINETIETH",
 /* 139 */ "program ::= num_nonnum",
 /* 140 */ "program ::= nonnum_num",
 /* 141 */ "num_nonnum ::= numbers nonnum_num",
 /* 142 */ "num_nonnum ::= numbers",
 /* 143 */ "nonnum_num ::= nonnum num_nonnum",
 /* 144 */ "nonnum_num ::= nonnum",
 /* 145 */ "nonnum ::= nonnum CHARACTERS",
 /* 146 */ "nonnum ::= CHARACTERS",
 /* 147 */ "numbers ::= numbers number",
 /* 148 */ "numbers ::= number",
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
  { 81, 1 },
  { 81, 2 },
  { 82, 3 },
  { 82, 2 },
  { 82, 3 },
  { 82, 2 },
  { 82, 1 },
  { 82, 1 },
  { 82, 1 },
  { 82, 1 },
  { 84, 3 },
  { 84, 1 },
  { 84, 1 },
  { 87, 2 },
  { 82, 1 },
  { 82, 2 },
  { 83, 3 },
  { 83, 2 },
  { 83, 1 },
  { 82, 1 },
  { 82, 2 },
  { 86, 3 },
  { 86, 1 },
  { 86, 1 },
  { 91, 2 },
  { 82, 1 },
  { 82, 2 },
  { 88, 1 },
  { 88, 1 },
  { 89, 3 },
  { 89, 2 },
  { 89, 1 },
  { 82, 1 },
  { 82, 2 },
  { 90, 3 },
  { 90, 1 },
  { 90, 1 },
  { 95, 2 },
  { 82, 1 },
  { 82, 2 },
  { 92, 1 },
  { 92, 1 },
  { 93, 3 },
  { 93, 2 },
  { 93, 1 },
  { 82, 1 },
  { 82, 2 },
  { 94, 3 },
  { 94, 1 },
  { 94, 1 },
  { 99, 2 },
  { 82, 1 },
  { 82, 2 },
  { 96, 1 },
  { 96, 1 },
  { 97, 3 },
  { 97, 2 },
  { 97, 1 },
  { 82, 1 },
  { 82, 2 },
  { 98, 4 },
  { 98, 3 },
  { 98, 1 },
  { 98, 2 },
  { 98, 1 },
  { 103, 2 },
  { 82, 1 },
  { 82, 2 },
  { 100, 2 },
  { 100, 1 },
  { 85, 4 },
  { 85, 3 },
  { 85, 2 },
  { 85, 1 },
  { 82, 1 },
  { 82, 2 },
  { 102, 2 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 101, 2 },
  { 101, 1 },
  { 101, 1 },
  { 109, 1 },
  { 109, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 76, 1 },
  { 76, 1 },
  { 77, 2 },
  { 77, 1 },
  { 78, 2 },
  { 78, 1 },
  { 80, 2 },
  { 80, 1 },
  { 79, 2 },
  { 79, 1 },
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
#line 123 "parser.yy"
{
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1379 "parser.c"
        break;
      case 1: /* number ::= NEGATIVE final_number */
#line 126 "parser.yy"
{
    yymsp[0].minor.yy0.dbl = -yymsp[0].minor.yy0.dbl;
    yymsp[0].minor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin);
    yymsp[0].minor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end);
    insertYYSTYPE(&state->yystypeList, yymsp[0].minor.yy0);
}
#line 1389 "parser.c"
        break;
      case 2: /* final_number ::= less_than_quadrillion AND_A QUARTER */
#line 133 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-2].minor.yy0.dbl + 0.25; yylhsminor.yy0.is_dbl = true; }
#line 1394 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 3: /* final_number ::= less_than_quadrillion QUARTERS */
#line 134 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl / 4.0; yylhsminor.yy0.is_dbl = true; }
#line 1400 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 4: /* final_number ::= less_than_quadrillion AND_A HALF */
#line 135 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-2].minor.yy0.dbl + 0.5; yylhsminor.yy0.is_dbl = true; }
#line 1406 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 5: /* final_number ::= less_than_quadrillion HALVES */
#line 136 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl / 2.0; yylhsminor.yy0.is_dbl = true; }
#line 1412 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 6: /* final_number ::= less_than_quadrillion */
      case 18: /* less_than_quadrillion ::= less_than_trillion */ yytestcase(yyruleno==18);
      case 27: /* less_than_trillion_end_only ::= less_than_trillion */ yytestcase(yyruleno==27);
      case 28: /* less_than_trillion_end_only ::= less_than_billion_end_only */ yytestcase(yyruleno==28);
      case 31: /* less_than_trillion ::= less_than_billion */ yytestcase(yyruleno==31);
      case 40: /* less_than_billion_end_only ::= less_than_billion */ yytestcase(yyruleno==40);
      case 41: /* less_than_billion_end_only ::= less_than_million_end_only */ yytestcase(yyruleno==41);
      case 44: /* less_than_billion ::= less_than_million */ yytestcase(yyruleno==44);
      case 53: /* less_than_million_end_only ::= less_than_million */ yytestcase(yyruleno==53);
      case 54: /* less_than_million_end_only ::= less_than_thousand_end_only */ yytestcase(yyruleno==54);
      case 57: /* less_than_million ::= less_than_thousand */ yytestcase(yyruleno==57);
      case 69: /* less_than_thousand_end_only ::= less_than_thousand */ yytestcase(yyruleno==69);
      case 73: /* less_than_thousand ::= less_than_hundred */ yytestcase(yyruleno==73);
      case 81: /* less_than_hundred ::= tens */ yytestcase(yyruleno==81);
      case 82: /* less_than_hundred ::= less_than_twenty */ yytestcase(yyruleno==82);
      case 83: /* less_than_twenty ::= ten_to_19 */ yytestcase(yyruleno==83);
      case 84: /* less_than_twenty ::= less_than_ten */ yytestcase(yyruleno==84);
#line 137 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; }
#line 1434 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 7: /* final_number ::= first_to_999999999999999th */
      case 11: /* first_to_999999999999999th ::= trillionths */ yytestcase(yyruleno==11);
      case 12: /* first_to_999999999999999th ::= first_to_999999999999th */ yytestcase(yyruleno==12);
      case 22: /* first_to_999999999999th ::= billionths */ yytestcase(yyruleno==22);
      case 23: /* first_to_999999999999th ::= first_to_999999999th */ yytestcase(yyruleno==23);
      case 35: /* first_to_999999999th ::= millionths */ yytestcase(yyruleno==35);
      case 36: /* first_to_999999999th ::= first_to_999999th */ yytestcase(yyruleno==36);
      case 48: /* first_to_999999th ::= thousandths */ yytestcase(yyruleno==48);
      case 49: /* first_to_999999th ::= first_to_999th */ yytestcase(yyruleno==49);
      case 62: /* first_to_999th ::= hundredths */ yytestcase(yyruleno==62);
      case 64: /* first_to_999th ::= first_to_99th */ yytestcase(yyruleno==64);
      case 77: /* first_to_99th ::= tenth */ yytestcase(yyruleno==77);
      case 78: /* first_to_99th ::= tenth_to_19th */ yytestcase(yyruleno==78);
      case 79: /* first_to_99th ::= first_to_9th */ yytestcase(yyruleno==79);
#line 138 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; yylhsminor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; }
#line 1453 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 8: /* final_number ::= NUMBER */
#line 149 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = true; }
#line 1459 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 9: /* final_number ::= ZERO */
#line 150 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 0.0; }
#line 1465 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 10: /* first_to_999999999999999th ::= less_than_thousand TRILLION first_to_999999999999th */
#line 173 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1471 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 13: /* trillionths ::= less_than_thousand TRILLIONTH */
#line 177 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1477 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 14: /* final_number ::= TRILLIONTH */
#line 179 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000000000.0; yylhsminor.yy0.suffix = TH; }
#line 1483 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 15: /* final_number ::= NUMBER TRILLIONTH */
#line 180 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1489 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 16: /* less_than_quadrillion ::= less_than_thousand TRILLION less_than_trillion_end_only */
#line 190 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1495 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 17: /* less_than_quadrillion ::= less_than_thousand TRILLION */
#line 191 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1501 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 19: /* final_number ::= TRILLION */
#line 194 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000000000.0; }
#line 1507 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 20: /* final_number ::= NUMBER TRILLION */
#line 195 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000000.0; yylhsminor.yy0.is_dbl = true; }
#line 1513 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 21: /* first_to_999999999999th ::= less_than_thousand BILLION first_to_999999999th */
#line 201 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1519 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 24: /* billionths ::= less_than_thousand BILLIONTH */
#line 205 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1525 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 25: /* final_number ::= BILLIONTH */
#line 207 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000000.0; yylhsminor.yy0.suffix = TH; }
#line 1531 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 26: /* final_number ::= NUMBER BILLIONTH */
#line 208 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1537 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 29: /* less_than_trillion ::= less_than_thousand BILLION less_than_billion_end_only */
#line 221 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1543 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 30: /* less_than_trillion ::= less_than_thousand BILLION */
#line 222 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1549 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 32: /* final_number ::= BILLION */
#line 225 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000000.0; }
#line 1555 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 33: /* final_number ::= NUMBER BILLION */
#line 226 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000000.0; yylhsminor.yy0.is_dbl = true; }
#line 1561 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 34: /* first_to_999999999th ::= less_than_thousand MILLION first_to_999999th */
#line 232 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1567 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 37: /* millionths ::= less_than_thousand MILLIONTH */
#line 236 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1573 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* final_number ::= MILLIONTH */
#line 238 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000.0; yylhsminor.yy0.suffix = TH; }
#line 1579 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 39: /* final_number ::= NUMBER MILLIONTH */
#line 239 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1585 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 42: /* less_than_billion ::= less_than_thousand MILLION less_than_million_end_only */
#line 252 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1591 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 43: /* less_than_billion ::= less_than_thousand MILLION */
#line 253 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1597 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 45: /* final_number ::= MILLION */
#line 256 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000000.0; }
#line 1603 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 46: /* final_number ::= NUMBER MILLION */
#line 257 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000000.0; yylhsminor.yy0.is_dbl = true; }
#line 1609 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 47: /* first_to_999999th ::= less_than_thousand THOUSAND first_to_999th */
#line 263 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1615 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 50: /* thousandths ::= less_than_thousand THOUSANDTH */
#line 267 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1621 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 51: /* final_number ::= THOUSANDTH */
#line 269 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000.0; yylhsminor.yy0.suffix = TH; }
#line 1627 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 52: /* final_number ::= NUMBER THOUSANDTH */
#line 270 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1633 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 55: /* less_than_million ::= less_than_thousand THOUSAND less_than_thousand_end_only */
#line 283 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 1000.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1639 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 56: /* less_than_million ::= less_than_thousand THOUSAND */
#line 284 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1645 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 58: /* final_number ::= THOUSAND */
#line 287 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1000.0; }
#line 1651 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 59: /* final_number ::= NUMBER THOUSAND */
#line 288 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 1000.0; yylhsminor.yy0.is_dbl = true; }
#line 1657 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 60: /* first_to_999th ::= less_than_hundred HUNDRED AND first_to_99th */
#line 294 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-3].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-3].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-3].minor.yy0.dbl * 100.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-3].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1663 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 61: /* first_to_999th ::= less_than_hundred HUNDRED first_to_99th */
#line 295 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 100.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; }
#line 1669 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 63: /* first_to_999th ::= AND first_to_99th */
#line 297 "parser.yy"
{ yymsp[-1].minor.yy0.begin = yymsp[0].minor.yy0.begin; yymsp[-1].minor.yy0.end = yymsp[0].minor.yy0.end; yymsp[-1].minor.yy0.dbl = yymsp[0].minor.yy0.dbl; yymsp[-1].minor.yy0.suffix = yymsp[0].minor.yy0.suffix; yymsp[-1].minor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; }
#line 1675 "parser.c"
        break;
      case 65: /* hundredths ::= less_than_hundred HUNDREDTH */
#line 300 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 100.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1680 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 66: /* final_number ::= HUNDREDTH */
#line 302 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 100.0; yylhsminor.yy0.suffix = TH; }
#line 1686 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 67: /* final_number ::= NUMBER HUNDREDTH */
#line 303 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 100.0; yylhsminor.yy0.suffix = TH; yylhsminor.yy0.is_dbl = true; }
#line 1692 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 68: /* less_than_thousand_end_only ::= AND less_than_hundred */
#line 313 "parser.yy"
{ yymsp[-1].minor.yy0.begin = yymsp[0].minor.yy0.begin; yymsp[-1].minor.yy0.end = yymsp[0].minor.yy0.end; yymsp[-1].minor.yy0.dbl = yymsp[0].minor.yy0.dbl; yymsp[-1].minor.yy0.is_dbl = yymsp[0].minor.yy0.is_dbl; }
#line 1698 "parser.c"
        break;
      case 70: /* less_than_thousand ::= less_than_hundred HUNDRED AND less_than_hundred */
#line 316 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-3].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-3].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-3].minor.yy0.dbl * 100.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-3].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1703 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 71: /* less_than_thousand ::= less_than_hundred HUNDRED less_than_hundred */
#line 317 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-2].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-2].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = (yymsp[-2].minor.yy0.dbl * 100.0) + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-2].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1709 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 72: /* less_than_thousand ::= less_than_hundred HUNDRED */
#line 318 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 100.0; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl; }
#line 1715 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 74: /* final_number ::= HUNDRED */
#line 321 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 100.0; }
#line 1721 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 75: /* final_number ::= NUMBER HUNDRED */
#line 322 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl * 100.0; yylhsminor.yy0.is_dbl = true; }
#line 1727 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 76: /* first_to_99th ::= tens first_to_9th */
#line 328 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.suffix = yymsp[0].minor.yy0.suffix; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1733 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 80: /* less_than_hundred ::= tens less_than_ten */
#line 337 "parser.yy"
{ yylhsminor.yy0.begin = mini(yymsp[-1].minor.yy0.begin, yymsp[0].minor.yy0.begin); yylhsminor.yy0.end = maxi(yymsp[-1].minor.yy0.end, yymsp[0].minor.yy0.end); yylhsminor.yy0.dbl = yymsp[-1].minor.yy0.dbl + yymsp[0].minor.yy0.dbl; yylhsminor.yy0.is_dbl = yymsp[-1].minor.yy0.is_dbl || yymsp[0].minor.yy0.is_dbl; }
#line 1739 "parser.c"
  yymsp[-1].minor.yy0 = yylhsminor.yy0;
        break;
      case 85: /* less_than_ten ::= ONE */
#line 352 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1.0; }
#line 1745 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 86: /* less_than_ten ::= TWO */
#line 353 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 2.0; }
#line 1751 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 87: /* less_than_ten ::= THREE */
#line 354 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 3.0; }
#line 1757 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 88: /* less_than_ten ::= FOUR */
#line 355 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 4.0; }
#line 1763 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 89: /* less_than_ten ::= FIVE */
#line 356 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 5.0; }
#line 1769 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 90: /* less_than_ten ::= SIX */
#line 357 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 6.0; }
#line 1775 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 91: /* less_than_ten ::= SEVEN */
#line 358 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 7.0; }
#line 1781 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 92: /* less_than_ten ::= EIGHT */
#line 359 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 8.0; }
#line 1787 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 93: /* less_than_ten ::= NINE */
#line 360 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 9.0; }
#line 1793 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 94: /* ten_to_19 ::= TEN */
#line 362 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 10.0; }
#line 1799 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 95: /* ten_to_19 ::= ELEVEN */
#line 363 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 11.0; }
#line 1805 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 96: /* ten_to_19 ::= TWELVE */
#line 364 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 12.0; }
#line 1811 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 97: /* ten_to_19 ::= THIRTEEN */
#line 365 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 13.0; }
#line 1817 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 98: /* ten_to_19 ::= FOURTEEN */
#line 366 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 14.0; }
#line 1823 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 99: /* ten_to_19 ::= FIFTEEN */
#line 367 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 15.0; }
#line 1829 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 100: /* ten_to_19 ::= SIXTEEN */
#line 368 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 16.0; }
#line 1835 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 101: /* ten_to_19 ::= SEVENTEEN */
#line 369 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 17.0; }
#line 1841 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 102: /* ten_to_19 ::= EIGHTEEN */
#line 370 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 18.0; }
#line 1847 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 103: /* ten_to_19 ::= NINETEEN */
#line 371 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 19.0; }
#line 1853 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 104: /* tens ::= TWENTY */
#line 373 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 20.0; }
#line 1859 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 105: /* tens ::= THIRTY */
#line 374 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 30.0; }
#line 1865 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 106: /* tens ::= FORTY */
#line 375 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 40.0; }
#line 1871 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 107: /* tens ::= FIFTY */
#line 376 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 50.0; }
#line 1877 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 108: /* tens ::= SIXTY */
#line 377 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 60.0; }
#line 1883 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 109: /* tens ::= SEVENTY */
#line 378 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 70.0; }
#line 1889 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 110: /* tens ::= EIGHTY */
#line 379 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 80.0; }
#line 1895 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 111: /* tens ::= NINETY */
#line 380 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 90.0; }
#line 1901 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 112: /* first_to_9th ::= FIRST */
#line 382 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 1.0; yylhsminor.yy0.suffix = ST; }
#line 1907 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 113: /* first_to_9th ::= SECOND */
#line 383 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 2.0; yylhsminor.yy0.suffix = ND; }
#line 1913 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 114: /* first_to_9th ::= THIRD */
#line 384 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 3.0; yylhsminor.yy0.suffix = RD; }
#line 1919 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 115: /* first_to_9th ::= FOURTH */
#line 385 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 4.0; yylhsminor.yy0.suffix = TH; }
#line 1925 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 116: /* first_to_9th ::= FIFTH */
#line 386 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 5.0; yylhsminor.yy0.suffix = TH; }
#line 1931 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 117: /* first_to_9th ::= SIXTH */
#line 387 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 6.0; yylhsminor.yy0.suffix = TH; }
#line 1937 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 118: /* first_to_9th ::= SEVENTH */
#line 388 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 7.0; yylhsminor.yy0.suffix = TH; }
#line 1943 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 119: /* first_to_9th ::= EIGHTH */
#line 389 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 8.0; yylhsminor.yy0.suffix = TH; }
#line 1949 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 120: /* first_to_9th ::= NINTH */
#line 390 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 9.0; yylhsminor.yy0.suffix = TH; }
#line 1955 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 121: /* tenth_to_19th ::= TENTH */
#line 392 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 10.0; yylhsminor.yy0.suffix = TH; }
#line 1961 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 122: /* tenth_to_19th ::= ELEVENTH */
#line 393 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 11.0; yylhsminor.yy0.suffix = TH; }
#line 1967 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 123: /* tenth_to_19th ::= TWELFTH */
#line 394 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 12.0; yylhsminor.yy0.suffix = TH; }
#line 1973 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 124: /* tenth_to_19th ::= THIRTEENTH */
#line 395 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 13.0; yylhsminor.yy0.suffix = TH; }
#line 1979 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 125: /* tenth_to_19th ::= FOURTEENTH */
#line 396 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 14.0; yylhsminor.yy0.suffix = TH; }
#line 1985 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 126: /* tenth_to_19th ::= FIFTEENTH */
#line 397 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 15.0; yylhsminor.yy0.suffix = TH; }
#line 1991 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 127: /* tenth_to_19th ::= SIXTEENTH */
#line 398 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 16.0; yylhsminor.yy0.suffix = TH; }
#line 1997 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 128: /* tenth_to_19th ::= SEVENTEENTH */
#line 399 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 17.0; yylhsminor.yy0.suffix = TH; }
#line 2003 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 129: /* tenth_to_19th ::= EIGHTEENTH */
#line 400 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 18.0; yylhsminor.yy0.suffix = TH; }
#line 2009 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 130: /* tenth_to_19th ::= NINETEENTH */
#line 401 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 19.0; yylhsminor.yy0.suffix = TH; }
#line 2015 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 131: /* tenth ::= TWENTIETH */
#line 403 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 20.0; yylhsminor.yy0.suffix = TH; }
#line 2021 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 132: /* tenth ::= THIRTIETH */
#line 404 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 30.0; yylhsminor.yy0.suffix = TH; }
#line 2027 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 133: /* tenth ::= FOURTIETH */
#line 405 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 40.0; yylhsminor.yy0.suffix = TH; }
#line 2033 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 134: /* tenth ::= FIFTIETH */
#line 406 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 50.0; yylhsminor.yy0.suffix = TH; }
#line 2039 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 135: /* tenth ::= SIXTIETH */
#line 407 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 60.0; yylhsminor.yy0.suffix = TH; }
#line 2045 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 136: /* tenth ::= SEVENTIETH */
#line 408 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 70.0; yylhsminor.yy0.suffix = TH; }
#line 2051 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 137: /* tenth ::= EIGHTIETH */
#line 409 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 80.0; yylhsminor.yy0.suffix = TH; }
#line 2057 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 138: /* tenth ::= NINETIETH */
#line 410 "parser.yy"
{ yylhsminor.yy0.begin = yymsp[0].minor.yy0.begin; yylhsminor.yy0.end = yymsp[0].minor.yy0.end; yylhsminor.yy0.dbl = 90.0; yylhsminor.yy0.suffix = TH; }
#line 2063 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      default:
      /* (139) program ::= num_nonnum */ yytestcase(yyruleno==139);
      /* (140) program ::= nonnum_num */ yytestcase(yyruleno==140);
      /* (141) num_nonnum ::= numbers nonnum_num */ yytestcase(yyruleno==141);
      /* (142) num_nonnum ::= numbers */ yytestcase(yyruleno==142);
      /* (143) nonnum_num ::= nonnum num_nonnum */ yytestcase(yyruleno==143);
      /* (144) nonnum_num ::= nonnum */ yytestcase(yyruleno==144);
      /* (145) nonnum ::= nonnum CHARACTERS */ yytestcase(yyruleno==145);
      /* (146) nonnum ::= CHARACTERS */ yytestcase(yyruleno==146);
      /* (147) numbers ::= numbers number */ yytestcase(yyruleno==147);
      /* (148) numbers ::= number (OPTIMIZED OUT) */ assert(yyruleno!=148);
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
#line 2123 "parser.c"
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
#line 2146 "parser.c"
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
