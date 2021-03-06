/* OmNomNum 0.0.2 -- Gobbles up numbers in strings.
 *
 * Copyright (c) 2017, Jesse Buesking <jessebuesking at gmail dot com>
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

#include "omnomnum.h"
#include <stdbool.h>

void test_single(const char *data, bool verbose, bool parse_second, int precision) {
    ParserState state;
    initParserState(&state);
    state.parse_second = parse_second;
    state.precision = precision;

    if (verbose) printf("Input:  '%s'\n", data);
    normalize(data, strlen(data), &state);
    if (verbose) printf("Output: '%s'\n", state.result);
    resetParserState(&state);

    freeParserState(&state);
}

void test_loop(const char *data) {
    ParserState state;
    initParserState(&state);

    int i;
    for (i = 0; i < 1000; i++) {
        normalize(data, strlen(data), &state);
        resetParserState(&state);
    }

    freeParserState(&state);
}

int main() {
    /* I use main just for running examples through valgrind, hence you'll
     * probably see some lines of code commented out...
     */

    initOmNomNum();

    test_single("0.002", true, false, 3);

    /*test_loop("two hundred");*/
    /*test_loop("two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello two hundred hello");*/
    /*test_loop("two hundred hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world");*/

    freeOmNomNum();

    return 0;
}


/*require 'omnomnum'*/
/*require 'numerizer'*/
/*txt = File.open('english-numerals.txt').read().split("\n")*/
/*txt.each_with_index do |row, index|*/
  /*o = OmNomNum.normalize(row).gsub(/\s+/, ' ')*/
  /*n = Numerizer.numerize(row).gsub(/\s+/, ' ')*/
  /*puts "[#{index}]\n\t'#{row}'\n\t'#{o}'\n\t'#{n}'" unless o == n*/
/*end*/
