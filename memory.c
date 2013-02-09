/********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#include <stdlib.h>
#include "memory.h"

void* xmalloc(size_t nbytes)
{
    void *ptr = malloc(nbytes);
    if (!ptr) {
        abort();
    }
    return ptr;
}

void* xrealloc(void *ptr, size_t nbytes)
{
    void *nptr = realloc(ptr, nbytes);
    if (nptr == NULL) {
        abort();
    }
    return nptr;
}

void xfree(void* ptr)
{
    if (ptr) {
        free(ptr);
    }
}
