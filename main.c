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
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s [OPTIONS] [FILE...]\n", prog_name);
    fprintf(stderr, "\n");
    fprintf(stderr, "Normalizes numbers in text. Reads from stdin if no files specified.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  --precision N          Set decimal precision (default: 6)\n");
    fprintf(stderr, "  --parse-second         Parse 'second' as ordinal '2nd'\n");
    fprintf(stderr, "  --no-parse-fractions   Disable fraction parsing (keep fractions as-is)\n");
    fprintf(stderr, "  --reduce-fractions     Reduce fractions to lowest terms (e.g., 2/4 -> 1/2)\n");
    fprintf(stderr, "  -h, --help             Show this help message\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "  echo 'one and a half' | %s\n", prog_name);
    fprintf(stderr, "  %s --no-parse-fractions input.txt\n", prog_name);
    fprintf(stderr, "  %s --precision 3 --parse-second < input.txt\n", prog_name);
    fprintf(stderr, "  %s --reduce-fractions < input.txt\n", prog_name);
}

void process_input(FILE *fp, ParserState *state) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Save user-configured settings that should persist across lines
    int saved_precision = state->precision;
    bool saved_parse_second = state->parse_second;
    bool saved_parse_fractions = state->parse_fractions;
    bool saved_reduce_fractions = state->reduce_fractions;

    while ((read = getline(&line, &len, fp)) != -1) {
        normalize(line, read, state);
        printf("%s", state->result);
        if (read > 0 && line[read-1] != '\n') {
            printf("\n");
        }
        resetParserState(state);
        // Restore user settings after reset
        state->precision = saved_precision;
        state->parse_second = saved_parse_second;
        state->parse_fractions = saved_parse_fractions;
        state->reduce_fractions = saved_reduce_fractions;
    }

    if (line) {
        free(line);
    }
}

int main(int argc, char *argv[]) {
    ParserState state;
    initParserState(&state);
    initOmNomNum();

    int precision = 6;
    bool parse_second = false;
    bool parse_fractions = true;
    bool reduce_fractions = false;
    int file_count = 0;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            freeParserState(&state);
            freeOmNomNum();
            return 0;
        } else if (strcmp(argv[i], "--precision") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --precision requires an argument\n");
                print_usage(argv[0]);
                freeParserState(&state);
                freeOmNomNum();
                return 1;
            }
            precision = atoi(argv[++i]);
            if (precision < 0) {
                fprintf(stderr, "Error: precision must be non-negative\n");
                freeParserState(&state);
                freeOmNomNum();
                return 1;
            }
        } else if (strcmp(argv[i], "--parse-second") == 0) {
            parse_second = true;
        } else if (strcmp(argv[i], "--no-parse-fractions") == 0) {
            parse_fractions = false;
        } else if (strcmp(argv[i], "--reduce-fractions") == 0) {
            reduce_fractions = true;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: unknown option '%s'\n", argv[i]);
            print_usage(argv[0]);
            freeParserState(&state);
            freeOmNomNum();
            return 1;
        } else {
            // This is a file argument
            file_count++;
        }
    }

    // Apply settings to parser state
    state.precision = precision;
    state.parse_second = parse_second;
    state.parse_fractions = parse_fractions;
    state.reduce_fractions = reduce_fractions;

    // Process input
    if (file_count == 0) {
        // Read from stdin
        process_input(stdin, &state);
    } else {
        // Process each file
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                FILE *fp = fopen(argv[i], "r");
                if (!fp) {
                    fprintf(stderr, "Error: cannot open file '%s': %s\n", argv[i], strerror(errno));
                    freeParserState(&state);
                    freeOmNomNum();
                    return 1;
                }
                process_input(fp, &state);
                fclose(fp);
            } else if (strcmp(argv[i], "--precision") == 0) {
                // Skip the next argument (already processed)
                i++;
            }
        }
    }

    freeParserState(&state);
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
