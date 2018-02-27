

#include <stdio.h>

#include "xmalloc.h"

/* HW11 TODO:
 *  - This should call / use your HW06 alloctor,
 *    modified to be safe and efficient for
 *    use in a threaded program.
 */

void*
xmalloc(size_t bytes)
{
    fprintf(stderr, "TODO: Implement parallel allocator in par_malloc.c\n");
    return 0;
}

void
xfree(void* ptr)
{
    fprintf(stderr, "TODO: Implement parallel allocator in par_malloc.c\n");
}

void*
xrealloc(void* prev, size_t bytes)
{
    fprintf(stderr, "TODO: Implement parallel allocator in par_malloc.c\n");
    return 0;
}

