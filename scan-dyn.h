#ifndef SCAN_DYN_H
#define SCAN_DYN_H

/* scan-dyn.h
 * Scott Bronson
 * 30 Dec 2004
 *
 * Creates a scanner entirely located on the heap.  You must make
 * sure to eventually call dynscan_free for every scanner created
 * with dynscan_create.
 */

#include "scan.h"


scanstate* dynscan_create(size_t bufsiz);
void dynscan_free(scanstate *ss);

#endif // SCAN_DYN_H
