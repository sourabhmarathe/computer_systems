
#include <stdlib.h>

#include "hw06_mem.h"

typedef struct Free_cell Free_cell;
struct Free_cell {
	uint64_t size; 
	Free_cell* next; 
};
static const uint64_t CELL_SIZE = (uint64_t) sizeof(Free_cell);
Free_cell* top_cell = 0;

Free_cell* pop_cell(void) {
	Free_cell* top = top_cell;
	top_cell = top_cell->next; 
	return top; 
}

void push_cell(Free_cell* new_cell) {
	new_cell->next = top_cell;
	top_cell = new_cell;
	return;
}

void* 
nu_malloc(size_t size)
{
    // TODO: Allocate memory using a technique optimized
    //       for sizeof(struct icell) allocations.
    uint64_t alloc_size = size + sizeof(uint64_t);
	if (size == sizeof(uint64_t)) {
        if (top_cell != 0) {
			void* addr; 
			Free_cell* curr_cell = pop_cell();
			addr = curr_cell;
			*(uint64_t *)addr = curr_cell->size; 
			return addr + sizeof(uint64_t);
		}
    }
	return hw06_malloc(size);
}

void 
nu_free(void* ptr)
{
    uint64_t alloc_size = *(uint64_t*)(ptr - sizeof(uint64_t));
	if (alloc_size == CELL_SIZE + sizeof(uint64_t)) {
		Free_cell* new_cell = (Free_cell*) (ptr - sizeof(uint64_t));
		new_cell->size = alloc_size; 
		push_cell(new_cell);
		return;
	}
    hw06_free(ptr);
}
