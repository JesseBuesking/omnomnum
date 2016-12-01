#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "readmem.h"

/*Returns EOF because we're out of data.*/
static ssize_t readmem_read(__attribute__((unused))scanstate *ss)
{
    return 0;
}

/*Initializes the given scanstate to read from the given string.*/
scanstate* readmem_init(scanstate *ss, const char *data, size_t len)
{
    scanstate_init(ss, data, len);
    ss->limit = ss->bufptr + ss->bufsiz;
    ss->read = readmem_read;
    return ss;
}
