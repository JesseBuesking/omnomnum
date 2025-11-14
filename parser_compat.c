#include "scanner.def.h"
#include "parser.h"

/* Lemon 3.50 compatibility: provide ParseReset wrapper */
void ParseInit(void *yypRawParser ParseCTX_PDECL);

void ParseReset(void *p) {
    /* Reinitialize the parser state */
#ifdef YYTRACKMAXSTACKDEPTH
    /* Preserve high-water mark across resets for stack depth profiling */
    extern void ParseStackPeakSet(void *p, int value);
    int saved_yyhwm = ParseStackPeak(p);
#endif

    ParseInit(p ParseCTX_PARAM);

#ifdef YYTRACKMAXSTACKDEPTH
    /* Restore the high-water mark */
    ParseStackPeakSet(p, saved_yyhwm);
#endif
}
