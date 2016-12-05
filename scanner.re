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

ScannerValue omnomnum_scanner_start(ParserState *state, void *pParser, YYSTYPE *yylval, scanstate *ss) {
    ScannerValue value;
fast_path:
    ss->token = ss->cursor;
    /*!re2c
        SEPARATOR       = [ \r\n\t\f\-]+;
        END             = "\x00";
        //CHARS         = [^ \r\n\t\f\-\x00]+; // not a separator
        CHARS           = [^]; // not a separator
        // see http://re2c.org/examples/example_07.html for ideas of other valid
        // types of numbers
        NUMBER          = ([0-9]* "." [0-9]+) | ([0-9]+);
        THREE_PART_DATE = [0-9]{2,4} "/" [0-9]{2,4} "/" [0-9]{2,4};
        TWO_PART_DATE   = [0-9]+ "/" [0-9]+;

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
        NUMBER {
            value.token = TOKEN_NUMBER;

            // turn string version of number into double
            sds string_value = sdsnewlen(ss->token, ss->cursor - ss->token);
            sscanf(string_value, "%lf", &(*yylval).dbl);
            sdsfree(string_value);

            return value;
        }

        'and' { value.token = TOKEN_AND; return value; }
        'and a' { value.token = TOKEN_AND_A; return value; }

        'negative' { value.token = TOKEN_NEGATIVE; return value; }

        'zero' { value.token = TOKEN_ZERO; return value; }
        'one' { value.token = TOKEN_ONE; return value; }
        'two' { value.token = TOKEN_TWO; return value; }
        'three' { value.token = TOKEN_THREE; return value; }
        'four' { value.token = TOKEN_FOUR; return value; }
        'five' { value.token = TOKEN_FIVE; return value; }
        'six' { value.token = TOKEN_SIX; return value; }
        'seven' { value.token = TOKEN_SEVEN; return value; }
        'eight' { value.token = TOKEN_EIGHT; return value; }
        'nine' { value.token = TOKEN_NINE; return value; }
        'ten' { value.token = TOKEN_TEN; return value; }
        'eleven' { value.token = TOKEN_ELEVEN; return value; }
        'twelve' { value.token = TOKEN_TWELVE; return value; }
        'thirteen' { value.token = TOKEN_THIRTEEN; return value; }
        'fourteen' { value.token = TOKEN_FOURTEEN; return value; }
        'fifteen' { value.token = TOKEN_FIFTEEN; return value; }
        'sixteen' { value.token = TOKEN_SIXTEEN; return value; }
        'seventeen' { value.token = TOKEN_SEVENTEEN; return value; }
        'eighteen' { value.token = TOKEN_EIGHTEEN; return value; }
        'nineteen' { value.token = TOKEN_NINETEEN; return value; }
        'ninteen' { value.token = TOKEN_NINETEEN; return value; }

        'twenty' { value.token = TOKEN_TWENTY; return value; }
        'thirty' { value.token = TOKEN_THIRTY; return value; }
        'forty' { value.token = TOKEN_FORTY; return value; }
        'fourty' { value.token = TOKEN_FORTY; return value; }
        'fifty' { value.token = TOKEN_FIFTY; return value; }
        'sixty' { value.token = TOKEN_SIXTY; return value; }
        'seventy' { value.token = TOKEN_SEVENTY; return value; }
        'eighty' { value.token = TOKEN_EIGHTY; return value; }
        'ninety' { value.token = TOKEN_NINETY; return value; }

        'a hundred' { value.token = TOKEN_HUNDRED; return value; }
        'hundred' { value.token = TOKEN_HUNDRED; return value; }
        'a thousand' { value.token = TOKEN_THOUSAND; return value; }
        'thousand' { value.token = TOKEN_THOUSAND; return value; }
        'a million' { value.token = TOKEN_MILLION; return value; }
        'million' { value.token = TOKEN_MILLION; return value; }
        'a billion' { value.token = TOKEN_BILLION; return value; }
        'billion' { value.token = TOKEN_BILLION; return value; }
        'a trillion' { value.token = TOKEN_TRILLION; return value; }
        'trillion' { value.token = TOKEN_TRILLION; return value; }

        'first' { value.token = TOKEN_FIRST; return value; }
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
                value.token = TOKEN_SECOND;
                return value;
            }
        }
        'third' { value.token = TOKEN_THIRD; return value; }
        'fourth' { value.token = TOKEN_FOURTH; return value; }
        'fifth' { value.token = TOKEN_FIFTH; return value; }
        'sixth' { value.token = TOKEN_SIXTH; return value; }
        'seventh' { value.token = TOKEN_SEVENTH; return value; }
        'eighth' { value.token = TOKEN_EIGHTH; return value; }
        'nineth' { value.token = TOKEN_NINTH; return value; }
        'ninth' { value.token = TOKEN_NINTH; return value; }

        'tenth' { value.token = TOKEN_TENTH; return value; }
        'eleventh' { value.token = TOKEN_ELEVENTH; return value; }
        'twelfth' { value.token = TOKEN_TWELFTH; return value; }
        'thirteenth' { value.token = TOKEN_THIRTEENTH; return value; }
        'fourteenth' { value.token = TOKEN_FOURTEENTH; return value; }
        'fifteenth' { value.token = TOKEN_FIFTEENTH; return value; }
        'sixteenth' { value.token = TOKEN_SIXTEENTH; return value; }
        'seventeenth' { value.token = TOKEN_SEVENTEENTH; return value; }
        'eighteenth' { value.token = TOKEN_EIGHTEENTH; return value; }
        'nineteenth' { value.token = TOKEN_NINETEENTH; return value; }

        'twentieth' { value.token = TOKEN_TWENTIETH; return value; }
        'thirtieth' { value.token = TOKEN_THIRTIETH; return value; }
        'fourtieth' { value.token = TOKEN_FOURTIETH; return value; }
        'fiftieth' { value.token = TOKEN_FIFTIETH; return value; }
        'sixtieth' { value.token = TOKEN_SIXTIETH; return value; }
        'seventieth' { value.token = TOKEN_SEVENTIETH; return value; }
        'eightieth' { value.token = TOKEN_EIGHTIETH; return value; }
        'ninetieth' { value.token = TOKEN_NINETIETH; return value; }

        'hundredth' { value.token = TOKEN_HUNDREDTH; return value; }
        'thousandth' { value.token = TOKEN_THOUSANDTH; return value; }
        'millionth' { value.token = TOKEN_MILLIONTH; return value; }
        'billionth' { value.token = TOKEN_BILLIONTH; return value; }
        'trillionth' { value.token = TOKEN_TRILLIONTH; return value; }

        'quarter' { value.token = TOKEN_QUARTER; return value; }
        'half' { value.token = TOKEN_HALF; return value; }

        //'firsts' { value.token = TOKEN_FIRSTS; return value; }
        //'seconds' { value.token = TOKEN_SECONDS; return value; }
        //'thirds' { value.token = TOKEN_THIRDS; return value; }
        //'fourths' { value.token = TOKEN_FOURTHS; return value; }
        //'fifths' { value.token = TOKEN_FIFTHS; return value; }
        //'sixths' { value.token = TOKEN_SIXTHS; return value; }
        //'sevenths' { value.token = TOKEN_SEVENTHS; return value; }
        //'eighths' { value.token = TOKEN_EIGHTHS; return value; }
        //'nineths' { value.token = TOKEN_NINTHS; return value; }
        //'ninths' { value.token = TOKEN_NINTHS; return value; }

        //'tenths' { value.token = TOKEN_TENTHS; return value; }
        //'elevenths' { value.token = TOKEN_ELEVENTHS; return value; }
        //'twelfths' { value.token = TOKEN_TWELFTHS; return value; }
        //'thirteenths' { value.token = TOKEN_THIRTEENTHS; return value; }
        //'fourteenths' { value.token = TOKEN_FOURTEENTHS; return value; }
        //'fifteenths' { value.token = TOKEN_FIFTEENTHS; return value; }
        //'sixteenths' { value.token = TOKEN_SIXTEENTHS; return value; }
        //'seventeenths' { value.token = TOKEN_SEVENTEENTHS; return value; }
        //'eighteenths' { value.token = TOKEN_EIGHTEENTHS; return value; }
        //'nineteenths' { value.token = TOKEN_NINETEENTHS; return value; }

        //'twentieths' { value.token = TOKEN_TWENTIETHS; return value; }
        //'thirtieths' { value.token = TOKEN_THIRTIETHS; return value; }
        //'fourtieths' { value.token = TOKEN_FOURTIETHS; return value; }
        //'fiftieths' { value.token = TOKEN_FIFTIETHS; return value; }
        //'sixtieths' { value.token = TOKEN_SIXTIETHS; return value; }
        //'seventieths' { value.token = TOKEN_SEVENTIETHS; return value; }
        //'eightieths' { value.token = TOKEN_EIGHTIETHS; return value; }
        //'ninetieths' { value.token = TOKEN_NINETIETHS; return value; }

        //'hundredths' { value.token = TOKEN_HUNDREDTHS; return value; }
        //'thousandths' { value.token = TOKEN_THOUSANDTHS; return value; }
        //'millionths' { value.token = TOKEN_MILLIONTHS; return value; }
        //'billionths' { value.token = TOKEN_BILLIONTHS; return value; }
        //'trillionths' { value.token = TOKEN_TRILLIONTHS; return value; }

        //'quarters' { value.token = TOKEN_QUARTERS; return value; }
        //'halves' { value.token = TOKEN_HALVES; return value; }

        //SEPARATOR { goto std; }
        SEPARATOR {
            if (!state->is_parsing) {
                // not parsing and we've found a character. gobble it and continue
                state->last_token = TOKEN_SEPARATOR;
                goto fast_path;
            }

            value.token = TOKEN_SEPARATOR;
            return value;
        }

        END { value.token = 0; return value; }

        CHARS {
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
