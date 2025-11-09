#include "scanner.def.h"
#include "parser.h"

/* Lemon 3.50 compatibility: provide ParseReset wrapper */
void ParseInit(void *yypRawParser ParseCTX_PDECL);

void ParseReset(void *p) {
    /* Reinitialize the parser state */
    ParseInit(p ParseCTX_PARAM);
}
