/********************************************************************
 * fand - control fans using sysfs
 * Copyright 2013 Michael Ekstrand <michael@elehack.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 ********************************************************************/

#include <stdlib.h>
#include "util.h"

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
