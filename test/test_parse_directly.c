/* Direct parser test to check if Parse() is actually using the stack */

#include "../omnomnum.h"
#include "../parser.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    initOmNomNum();

    ParserState state;
    initParserState(&state);

    // Manually create a parser
    void *pParser = ParseAlloc(malloc);
    printf("Parser allocated: %p\n", pParser);

#ifdef YYTRACKMAXSTACKDEPTH
    printf("Initial stack depth: %d\n", ParseStackPeak(pParser));
#endif

    // Send some tokens to the parser
    YYSTYPE yylval;
    yylval.begin = 0;
    yylval.end = 3;

    // Simulate parsing "one hundred"
    // TOKEN_WORD_ONE = some value, let's say we push a few tokens
    printf("\nParsing some tokens...\n");

    // Just send an end token
    Parse(pParser, 0, yylval, &state);

#ifdef YYTRACKMAXSTACKDEPTH
    printf("Stack depth after parsing: %d\n", ParseStackPeak(pParser));
#else
    printf("YYTRACKMAXSTACKDEPTH not defined!\n");
#endif

    ParseFree(pParser, free);
    freeParserState(&state);
    freeOmNomNum();

    return 0;
}
