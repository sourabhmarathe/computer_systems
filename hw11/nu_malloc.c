

#include <stdio.h>

#include "xmalloc.h"

/* HW11 TODO:
 *  - This should call / use your HW06 alloctor,
 *    modified to be thread-safe.
 */

void*
xmalloc(size_t bytes)
{
    fprintf(stderr, "TODO: Call HW06 allocator in nu_malloc.c\n");
    return 0;
}

void
xfree(void* ptr)
{
    fprintf(stderr, "TODO: Call HW06 allocator in nu_malloc.c\n");
}

void*
xrealloc(void* prev, size_t bytes)
{
    fprintf(stderr, "TODO: Implement realloc with HW06 allocator in nu_malloc.c\n");
    return 0;
}

