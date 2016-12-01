#ifndef SCAN_H
#define SCAN_H

/* scan.h
 * Scott Bronson
 * 27 Dec 2004
 *
 * Version VERSION
 *
 * This part of support code to make writing re2c scanners much easier.
 *
 * TODO: add dispose procs.  Normally these will just be null but
 * if they're set, they will ensure that all resources are collected.
 *
 * TODO: probably want to split the re2c-specific code from the general
 * code.  This file is overall very useful, but it's got a few limitations
 * imposed by re2c that should probably be placed in its own layer.
 * That way, future versions of re2c won't have to suffer the same
 * limitations.
 */

// to pull in the definition for size_t
#include <sys/types.h>


/*
 * @file scan.h
 *
 * This file contains the definition of the scanstate, the most
 * important data structure for a scanner.  It also contains some
 * support macros.
 *
 * TERMINOLOGY
 *
 * allocate: scanstates can be dynamically (dynamicscan_create()) or
 * statically (scanstate_init()) allocated.  The buffers they use to
 * hold data may also be either dynamic or static.  Of course, any
 * time you allocate something dynamically, you must call the
 * corresponding free routine when you're done with it.
 *
 * attach: when the scanner is first initialized (scanstate_init())
 * or allocated (dynamicscan_create()), it is not associated with
 * a reader (to supply data) or a scanner (to scan the data).
 * You first need to attach both a ::scanproc and a ::readproc.
 *
 * initialize: prepare an already-allocated scanner for use.
 * After initializing it, you must ATTACH the scanner to a
 * READER.
 *
 * reader: See readproc
 *
 * ::readproc:: reads data into the scanstate for the scanner.
 * Examples are readmem.c (read from a contiguous block in
 * memory), readfp.c (read from a FILE*), readfd.c (read
 * from a Unix file descriptor), etc.
 *
 * scanner The function (or functions, see \ref startstates)
 * that actually performs the scanning.
 * The scanner need not actually be written with the assistance
 * of re2c
 * It accepts a scanstate data structure and returns the next
 * token in the stream.
 *
 * scanproc:
 *
 * ::scanstate: the data structure that retains complete state for the
 * scanner.  Scanners are thread safe: they never, ever use global
 * state.
 */

#ifndef R2SCAN_H
#define R2SCAN_H


// for re2c...
#define YYCTYPE     char
#define YYCURSOR    (ss->cursor)
#define YYLIMIT     (ss->limit)
#define YYMARKER    (ss->marker)

/*
 * Fills the scan buffer with more data.
 *
 * This routine needs to force a return if 0 bytes were read because
 * otherwise the re2c scanner will end up scanning garbage way off
 * the end of the buffer.  There's no (good) way to tell the scanner
 * "the file is at eof so just finish the token that you're on" (right?).
 * It will always lose the token at the end of the file unless the file
 * ends in a token delimiter (usually a newline).
 *
 * We ignore n because there can be less than n bytes left in the file,
 * yet one or more tokens will still match.  Therefore, we should always
 * read as much data as we can, and we should return success even if we
 * have less than n bytes in the buffer.  N is totally useless.
 *
 * The last line is the limitation.  If it weren't there, YYFILL would
 * return with an empty buffer so re2c would know it's at EOF and
 * shut down gracefully.  But re2c can't handle that.
 *
 * If you're using the re2c lib but writing your own re2c scanners,
 * call ss->read directly.
 */
#define YYFILL(n) do { \
    if (ss->last_read < 0) return ss->last_read; \
    if (ss->cursor >= ss->limit) return 0; \
} while(0);


// forward declaration
struct scanstate;

/*
 * Readprocs are provide the scanners with data.
 *
 * Each scanner has a single readproc that feeds it with data.
 * The re2c library includes some standard readprocs 
 * (see \ref readproc_builtins) and makes it easy to write your own
 * if the builtins don't do what you want (see \ref writing_readprocs).
 *
 * @returns: A positive return value specifies the number of bytes
 * that were returned.  Zero means EOF.  Negative means an error
 * occurred (the exact meaning of the error code depends on the
 * readproc).
 *
 * re2c doesn't work well with partial reads so be sure your readproc
 * fills the entire buffer if possible (see \ref readproc_shortreads).
 *
 * See \ref error_handling for more on how errors are propagated
 * through the re2c library.
 */
typedef ssize_t (*readproc)(struct scanstate *ss);


/*
 * Scans the data and returns the next token.
 *
 * A scanner is simply one or more functions that accept a scanstate
 * object and return the next token in that stream.
 * The scanproc will typically be generated with the
 * assistance of re2c, but the re2c library can be useful
 * even when re2c isn't used (see \ref cmpscan).
 *
 * Once you have created the scanstate data structure,
 * pass it to the scanner.  If the scanner returns 0,
 * you hit EOF.  If the scanner returns a negative number,
 * then some sort of error was encountered.  Or, if you're
 * doing nonblocking I/O, then it just might mean that this
 * there's not enough data available to determine the next
 * token.
 */
typedef int (*scanproc)(struct scanstate *ss);



/*
 * Contains all state for a single scan session.
 *
 * This structure is used by a scanner to preserve its state.
 *
 * All charptrs are declared const to help ensure that you don't
 * accidentally end up modifying the buffer as it's being scanned.
 * This means that any time you want to read data into the buffer,
 * you need to cast the pointers to be nonconst.
 */
struct scanstate {
    const char *cursor; /*The current character being looked at by the scanner. This is the same as re2c's YYCURSOR.*/
    const char *limit;  /*The last (uppermost) valid character in the current buffer. This is the same as re2c's YYLIMIT.*/
    const char *marker; /*Used internally by re2c engine to handle backtracking. This is the same as re2c's YYMARKER.*/
    const char *token;  /*The start of the current token.*/
    ssize_t last_read;  /*The return value from the last time the ::readproc was called. If last_read is 0 (eof) or negative (error), then the readproc should not be called.*/
    const char *bufptr; /*The buffer currently in use.*/
    size_t bufsiz;      /*The maximum number of bytes that the buffer can hold.*/
};
typedef struct scanstate scanstate;


/*
 * Initializes a given scanstate structure.
 *
 * Call this to prepare a scanner for use.  Some calls, such as readmem_init()
 * and dynscan_create
 *
 * @param ss The scanstate to initialize.  There is no need to clear this
 *   memory first -- scanstate_init initializes every field.
 * @param bufptr The scan buffer.
 * @param bufsiz Size, in bytes, of bufptr.  Pass 0 when bufptr is NULL.
 */
void scanstate_init(scanstate *ss, const char *bufptr, size_t bufsiz);

#endif

#endif // SCAN_H
