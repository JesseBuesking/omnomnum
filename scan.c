#include <string.h>
#include <assert.h>
#include "scan.h"

/*
 * Initializes the scanstate structure.
 *
 * @param ss The scanstate structure to initialize.
 * @param bufptr The buffer that the scanner should use.
 * @param bufsiz The size in bytes of the buffer passed in bufptr.
 *
 * Initializes the given scanstate structure and tells it to use
 * the provided buffer.
 *
 * If you don't need to buffer the data
 * (for instance, when using readmem.h), pass NULL for bufptr
 * and 0 for bufsiz.
 *
 * You probably want to attach a reader after calling this routine.
 *
 * Also see scanstate_reset() and dynscan_create().
 */
void scanstate_init(scanstate *ss, const char *bufptr, size_t bufsiz)
{
    ss->cursor = bufptr;
    ss->marker = NULL;
    ss->token = bufptr;
    ss->last_read = 1;
    ss->bufptr = bufptr;
    ss->bufsiz = bufsiz;
    ss->limit = ss->bufptr + ss->bufsiz;
}
