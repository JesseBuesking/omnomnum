/* scan.c
 * Scott Bronson
 * 28 Dec 2004
 */

#include <string.h>
#include <assert.h>
#include "scan.h"

/** @file scan.c
 *
 * This file contains routines to manipulate scanstate structures.
 */

/** Initializes the scanstate structure.
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
    ss->limit = bufptr;
    ss->marker = NULL;
    ss->token = bufptr;
    ss->last_read = 1;
    ss->bufptr = bufptr;
    ss->bufsiz = bufsiz;
    ss->read = NULL;
    ss->state = NULL;
}


/** Resets the given scanstate as if it was just created but doesn't
 * modify the scanner or the readproc.
 *
 * Doesn't modify:
 * - the reader or the readref.
 * - the scanner or the scanref.
 * - the userproc or userref.
 * - the buffer or buffer size
 *
 * If you want to reset the reader and scanner, you probably want
 * to detach and reattach them.
 */
void scanstate_reset(scanstate *ss)
{
    ss->cursor = ss->bufptr;
    ss->limit = ss->bufptr;
    ss->marker = NULL;
    ss->token = ss->bufptr;
    ss->last_read = 1;
}

