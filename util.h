/********************************************************************
 * fand - control fans using sysfs
 ********************************************************************/

#ifndef FAND_MEMORY_H
#define FAND_MEMORY_H

void* xmalloc(size_t nbytes);
void* xrealloc(void *ptr, size_t nbytes);
void xfree(void* ptr);

#endif //!defined(FAND_MEMORY_H)
