/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -C -l -c -t -m 100 -k '1,2,3,$' denom_words.gperf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "denom_words.gperf"

#include <string.h>
#line 4 "denom_words.gperf"
struct denom_word { const char *name; double value; };

#define TOTAL_KEYWORDS 67
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 14
#define MIN_HASH_VALUE 8
#define MAX_HASH_VALUE 102
/* maximum key range = 95, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103,   0,  29, 103,
       13,  11,   3,  32,   0,   3, 103, 103,  36,   3,
        0,  14, 103,  56,  39,   6,  16,   0,  26,  31,
       24, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
      103, 103, 103, 103, 103, 103
    };
  return len + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]] + asso_values[(unsigned char)str[len - 1]];
}

const struct denom_word *
in_word_set (register const char *str, register size_t len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  5,  9,  0,  0,  9, 10,
       5,  6, 10,  8,  9, 10, 11,  6,  0,  6,  9, 10,  9, 10,
       9, 10,  7,  6,  5, 10, 11, 10, 11,  5,  5,  6, 10,  8,
       9,  4,  0,  6,  0, 11,  9, 10,  7,  9,  6, 10, 11,  9,
      10,  8, 10,  7, 11, 12, 10, 11,  8,  7,  8,  9, 10, 13,
       8,  9,  8,  9, 10, 11, 14,  9,  0,  0,  0,  0,  0,  0,
      10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  7
    };
  static const struct denom_word wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 39 "denom_words.gperf"
      {"ninth", 9},
#line 31 "denom_words.gperf"
      {"hundredth", 100},
      {""}, {""},
#line 37 "denom_words.gperf"
      {"ninetieth", 90},
#line 35 "denom_words.gperf"
      {"nineteenth", 19},
#line 18 "denom_words.gperf"
      {"fifth", 5},
#line 40 "denom_words.gperf"
      {"ninths", 9},
#line 32 "denom_words.gperf"
      {"hundredths", 100},
#line 20 "denom_words.gperf"
      {"fiftieth", 50},
#line 16 "denom_words.gperf"
      {"fifteenth", 15},
#line 38 "denom_words.gperf"
      {"ninetieths", 90},
#line 36 "denom_words.gperf"
      {"nineteenths", 19},
#line 19 "denom_words.gperf"
      {"fifths", 5},
      {""},
#line 26 "denom_words.gperf"
      {"fourth", 4},
#line 21 "denom_words.gperf"
      {"fiftieths", 50},
#line 17 "denom_words.gperf"
      {"fifteenths", 15},
#line 28 "denom_words.gperf"
      {"fourtieth", 40},
#line 24 "denom_words.gperf"
      {"fourteenth", 14},
#line 63 "denom_words.gperf"
      {"thirtieth", 30},
#line 61 "denom_words.gperf"
      {"thirteenth", 13},
#line 27 "denom_words.gperf"
      {"fourths", 4},
#line 60 "denom_words.gperf"
      {"thirds", 3},
#line 57 "denom_words.gperf"
      {"tenth", 10},
#line 29 "denom_words.gperf"
      {"fourtieths", 40},
#line 25 "denom_words.gperf"
      {"fourteenths", 14},
#line 64 "denom_words.gperf"
      {"thirtieths", 30},
#line 62 "denom_words.gperf"
      {"thirteenths", 13},
#line 59 "denom_words.gperf"
      {"third", 3},
#line 53 "denom_words.gperf"
      {"sixth", 6},
#line 58 "denom_words.gperf"
      {"tenths", 10},
#line 65 "denom_words.gperf"
      {"thousandth", 1000},
#line 55 "denom_words.gperf"
      {"sixtieth", 60},
#line 51 "denom_words.gperf"
      {"sixteenth", 16},
#line 30 "denom_words.gperf"
      {"half", 2},
      {""},
#line 54 "denom_words.gperf"
      {"sixths", 6},
      {""},
#line 66 "denom_words.gperf"
      {"thousandths", 1000},
#line 56 "denom_words.gperf"
      {"sixtieths", 60},
#line 52 "denom_words.gperf"
      {"sixteenths", 16},
#line 47 "denom_words.gperf"
      {"seventh", 7},
#line 33 "denom_words.gperf"
      {"millionth", 1000000},
#line 10 "denom_words.gperf"
      {"eighth", 8},
#line 49 "denom_words.gperf"
      {"seventieth", 70},
#line 45 "denom_words.gperf"
      {"seventeenth", 17},
#line 12 "denom_words.gperf"
      {"eightieth", 80},
#line 8 "denom_words.gperf"
      {"eighteenth", 18},
#line 48 "denom_words.gperf"
      {"sevenths", 7},
#line 34 "denom_words.gperf"
      {"millionths", 1000000},
#line 11 "denom_words.gperf"
      {"eighths", 8},
#line 50 "denom_words.gperf"
      {"seventieths", 70},
#line 46 "denom_words.gperf"
      {"seventeenths", 17},
#line 13 "denom_words.gperf"
      {"eightieths", 80},
#line 9 "denom_words.gperf"
      {"eighteenths", 18},
#line 22 "denom_words.gperf"
      {"fortieth", 40},
#line 69 "denom_words.gperf"
      {"twelfth", 12},
#line 14 "denom_words.gperf"
      {"eleventh", 11},
#line 71 "denom_words.gperf"
      {"twentieth", 20},
#line 67 "denom_words.gperf"
      {"trillionth", 1000000000000.0},
#line 41 "denom_words.gperf"
      {"quadrillionth", 1000000000000000.0},
#line 44 "denom_words.gperf"
      {"quarters", 4},
#line 23 "denom_words.gperf"
      {"fortieths", 40},
#line 70 "denom_words.gperf"
      {"twelfths", 12},
#line 15 "denom_words.gperf"
      {"elevenths", 11},
#line 72 "denom_words.gperf"
      {"twentieths", 20},
#line 68 "denom_words.gperf"
      {"trillionths", 1000000000000.0},
#line 42 "denom_words.gperf"
      {"quadrillionths", 1000000000000000.0},
#line 6 "denom_words.gperf"
      {"billionth", 1000000000},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 7 "denom_words.gperf"
      {"billionths", 1000000000},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 43 "denom_words.gperf"
      {"quarter", 4}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
#line 73 "denom_words.gperf"

