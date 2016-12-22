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

#include "scanner.h"

#define TOKEN_SEPARATOR 10000
#define TOKEN_CHARACTERS 10001

int omnomnum_scanner_start(ParserState *state, void *pParser, YYSTYPE *yylval, scanstate *ss) {
fast_path:
    ss->token = ss->cursor;
    /*!re2c
        SEPARATOR       = [ \r\n\t\f\-]+;
        //ALL_OTHERS    = [^ \r\n\t\f\-\x00]+; // not a separator
        ALL_OTHERS      = [^]; // not a separator
        // see http://re2c.org/examples/example_07.html for ideas of other valid
        // types of numbers
        WHOLE_NUMBER    = [0-9]+;
        DECIMAL         = [0-9]* "." [0-9]+;
        THREE_PART_DATE = [0-9]{2,4} "/" [0-9]{2,4} "/" [0-9]{2,4};
        TWO_PART_DATE   = [0-9]+ "/" [0-9]+;

        // Decimal marks https://en.wikipedia.org/wiki/Decimal_mark
        // english unofficial
        DECIMAL_EN_UN   = [0-9]{1,3} ("," [0-9]{3})+ ("." [0-9]+)*;
        // TODO: figure out word boundaries so that the decimal part can be
        // optional, otherwise this will start matching too much
        DECIMAL_SI      = [0-9]{1,3} (" " [0-9]{3})+ "." [0-9]+;
        DECIMAL_SI_FR   = [0-9]{1,3} (" " [0-9]{3})+ "," [0-9]+;
        DECIMAL_IRE     = [0-9]{1,3} ("," [0-9]{3})+ ("·" [0-9]+)*;
        DECIMAL_ARG     = [0-9]{1,3} ("." [0-9]{3})+ ("," [0-9]+)*;
        DECIMAL_IND     = [0-9]{1,2} ("," [0-9]{2})* ("," [0-9]{3}) ("." [0-9]+)*;
        DECIMAL_SWI     = [0-9]{1,3} ("'" [0-9]{3})+ ("." [0-9]+)*;
        DECIMAL_CHI     = [0-9]{1,4} ("," [0-9]{4})+ ("." [0-9]+)*;

        'a' { return TOKEN_A; }
        'an' { return TOKEN_AN; }
        'and' { return TOKEN_AND; }
        'and a' { return TOKEN_AND_A; }

        'negative' { return TOKEN_NEGATIVE; }

        'zero' { return TOKEN_ZERO; }
        '1' | 'one' { return TOKEN_ONE; }
        'two' { return TOKEN_TWO; }
        'three' { return TOKEN_THREE; }
        'four' { return TOKEN_FOUR; }
        'five' { return TOKEN_FIVE; }
        'six' { return TOKEN_SIX; }
        'seven' { return TOKEN_SEVEN; }
        'eight' { return TOKEN_EIGHT; }
        'nine' { return TOKEN_NINE; }
        'ten' { return TOKEN_TEN; }
        'eleven' { return TOKEN_ELEVEN; }
        'twelve' { return TOKEN_TWELVE; }
        'thirteen' { return TOKEN_THIRTEEN; }
        'fourteen' { return TOKEN_FOURTEEN; }
        'fifteen' { return TOKEN_FIFTEEN; }
        'sixteen' { return TOKEN_SIXTEEN; }
        'seventeen' { return TOKEN_SEVENTEEN; }
        'eighteen' { return TOKEN_EIGHTEEN; }
        'nineteen' { return TOKEN_NINETEEN; }
        'ninteen' { return TOKEN_NINETEEN; }

        'twenty' { return TOKEN_TWENTY; }
        'thirty' { return TOKEN_THIRTY; }
        'forty' { return TOKEN_FORTY; }
        'fourty' { return TOKEN_FORTY; }
        'fifty' { return TOKEN_FIFTY; }
        'sixty' { return TOKEN_SIXTY; }
        'seventy' { return TOKEN_SEVENTY; }
        'eighty' { return TOKEN_EIGHTY; }
        'ninety' { return TOKEN_NINETY; }

        'a hundred' { return TOKEN_HUNDRED; }
        'hundred' { return TOKEN_HUNDRED; }
        'a thousand' { return TOKEN_THOUSAND; }
        'thousand' { return TOKEN_THOUSAND; }
        'a million' { return TOKEN_MILLION; }
        'million' { return TOKEN_MILLION; }
        'a billion' { return TOKEN_BILLION; }
        'billion' { return TOKEN_BILLION; }
        'a trillion' { return TOKEN_TRILLION; }
        'trillion' { return TOKEN_TRILLION; }

        'first' { return TOKEN_FIRST; }
        'second' {
            if (!state->parse_second) {
                // dont' parse "second" by default. easiest way to do this is to
                // treat it as a character token
                if (state->is_parsing) {
                    if (state->last_token != TOKEN_SEPARATOR) {
                        // number followed by character... e.g. "oneself"
                    } else {
                        // finish whatever we had and reset
                        Parse(pParser, 0, *yylval, state);
                    }

                    ParseReset(pParser);
                    state->is_parsing = false;
                }

                state->last_token = TOKEN_CHARACTERS;
                goto fast_path;
            } else {
                return TOKEN_SECOND;
            }
        }
        'third' { return TOKEN_THIRD; }
        'fourth' { return TOKEN_FOURTH; }
        'fifth' { return TOKEN_FIFTH; }
        'sixth' { return TOKEN_SIXTH; }
        'seventh' { return TOKEN_SEVENTH; }
        'eighth' { return TOKEN_EIGHTH; }
        'nineth' { return TOKEN_NINTH; }
        'ninth' { return TOKEN_NINTH; }

        'tenth' { return TOKEN_TENTH; }
        'eleventh' { return TOKEN_ELEVENTH; }
        'twelfth' { return TOKEN_TWELFTH; }
        'thirteenth' { return TOKEN_THIRTEENTH; }
        'fourteenth' { return TOKEN_FOURTEENTH; }
        'fifteenth' { return TOKEN_FIFTEENTH; }
        'sixteenth' { return TOKEN_SIXTEENTH; }
        'seventeenth' { return TOKEN_SEVENTEENTH; }
        'eighteenth' { return TOKEN_EIGHTEENTH; }
        'nineteenth' { return TOKEN_NINETEENTH; }

        'twentieth' { return TOKEN_TWENTIETH; }
        'thirtieth' { return TOKEN_THIRTIETH; }
        'fourtieth' { return TOKEN_FOURTIETH; }
        'fiftieth' { return TOKEN_FIFTIETH; }
        'sixtieth' { return TOKEN_SIXTIETH; }
        'seventieth' { return TOKEN_SEVENTIETH; }
        'eightieth' { return TOKEN_EIGHTIETH; }
        'ninetieth' { return TOKEN_NINETIETH; }

        'hundredth' { return TOKEN_HUNDREDTH; }
        'thousandth' { return TOKEN_THOUSANDTH; }
        'millionth' { return TOKEN_MILLIONTH; }
        'billionth' { return TOKEN_BILLIONTH; }
        'trillionth' { return TOKEN_TRILLIONTH; }

        'quarter' { return TOKEN_QUARTER; }
        'half' { return TOKEN_HALF; }

        'firsts' { return TOKEN_FIRSTS; }
        // TODO: handle like SECOND above
        //'seconds' { return TOKEN_SECONDS; }
        'thirds' { return TOKEN_THIRDS; }
        'fourths' { return TOKEN_FOURTHS; }
        'fifths' { return TOKEN_FIFTHS; }
        'sixths' { return TOKEN_SIXTHS; }
        'sevenths' { return TOKEN_SEVENTHS; }
        'eighths' { return TOKEN_EIGHTHS; }
        'nineths' { return TOKEN_NINTHS; }
        'ninths' { return TOKEN_NINTHS; }

        'tenths' { return TOKEN_TENTHS; }
        'elevenths' { return TOKEN_ELEVENTHS; }
        'twelfths' { return TOKEN_TWELFTHS; }
        'thirteenths' { return TOKEN_THIRTEENTHS; }
        'fourteenths' { return TOKEN_FOURTEENTHS; }
        'fifteenths' { return TOKEN_FIFTEENTHS; }
        'sixteenths' { return TOKEN_SIXTEENTHS; }
        'seventeenths' { return TOKEN_SEVENTEENTHS; }
        'eighteenths' { return TOKEN_EIGHTEENTHS; }
        'nineteenths' { return TOKEN_NINETEENTHS; }

        'twentieths' { return TOKEN_TWENTIETHS; }
        'thirtieths' { return TOKEN_THIRTIETHS; }
        'fourtieths' { return TOKEN_FOURTIETHS; }
        'fiftieths' { return TOKEN_FIFTIETHS; }
        'sixtieths' { return TOKEN_SIXTIETHS; }
        'seventieths' { return TOKEN_SEVENTIETHS; }
        'eightieths' { return TOKEN_EIGHTIETHS; }
        'ninetieths' { return TOKEN_NINETIETHS; }

        'hundredths' { return TOKEN_HUNDREDTHS; }
        'thousandths' { return TOKEN_THOUSANDTHS; }
        'millionths' { return TOKEN_MILLIONTHS; }
        'billionths' { return TOKEN_BILLIONTHS; }
        'trillionths' { return TOKEN_TRILLIONTHS; }

        'quarters' { return TOKEN_QUARTERS; }
        'halves' { return TOKEN_HALVES; }

        SEPARATOR {
            if (!state->is_parsing) {
                // not parsing and we've found a character. gobble it and continue
                state->last_token = TOKEN_SEPARATOR;
                goto fast_path;
            }

            return TOKEN_SEPARATOR;
        }
        THREE_PART_DATE {
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) {
                    // number followed by character... e.g. "oneself"
                } else {
                    // finish whatever we had and reset
                    Parse(pParser, 0, *yylval, state);
                }

                ParseReset(pParser);
                state->is_parsing = false;
            }

            state->last_token = TOKEN_CHARACTERS;
            goto fast_path;
        }
        TWO_PART_DATE {
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) {
                    // number followed by character... e.g. "oneself"
                } else {
                    // finish whatever we had and reset
                    Parse(pParser, 0, *yylval, state);
                }

                ParseReset(pParser);
                state->is_parsing = false;
            }

            state->last_token = TOKEN_CHARACTERS;
            goto fast_path;
        }
        DECIMAL_EN_UN {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ',');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_SI {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ' ');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_SI_FR {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ' ');
            replace_char_inplace(tmp, len, ',', '.');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_IRE {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ',');
            len = replace_two_byte_char_inplace((unsigned char*)tmp, len, (unsigned char*)"·", '.');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_ARG {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            printf("%s\n", tmp);
            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, '.');
            printf("%s\n", tmp);
            replace_char_inplace(tmp, len, ',', '.');
            printf("%s\n", tmp);

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_IND {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ',');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_SWI {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, '\'');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        DECIMAL_CHI {
            // create duplicate of string (to be modified inplace)
            size_t token_len = ss->cursor - ss->token;
            char* tmp = (char*)malloc(token_len + 1);
            strncpy(tmp, ss->token, token_len);
            tmp[token_len] = '\0';

            size_t len = remove_char_inplace(tmp, ss->cursor - ss->token, ',');

            // turn string version of number into double
            sds string_value = sdsnewlen(tmp, len);
            free(tmp);

            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }
        WHOLE_NUMBER {
            // turn string version of number into double
            sds string_value = sdsnewlen(ss->token, ss->cursor - ss->token);
            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_WHOLE_NUMBER;
        }
        DECIMAL {
            // turn string version of number into double
            sds string_value = sdsnewlen(ss->token, ss->cursor - ss->token);
            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return TOKEN_DECIMAL;
        }

        // MUST COME LAST
        ALL_OTHERS {
            if (state->is_parsing) {
                if (state->last_token != TOKEN_SEPARATOR) {
                    // number followed by character... e.g. "oneself"
                } else {
                    // finish whatever we had and reset
                    Parse(pParser, 0, *yylval, state);
                }

                ParseReset(pParser);
                state->is_parsing = false;
            }

            state->last_token = TOKEN_CHARACTERS;
            goto fast_path;
        }
    */
}
