#include <stdint.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "hw06_mem.h"

static const int64_t CHUNK_SIZE = 1000*1000;
// You should update these counters on memory allocation / deallocation events.
// These counters should only go up, and should provide totals for the entire
// execution of the program.

// Linked list data structure
typedef struct Free_cell Free_cell;
struct Free_cell {
	int64_t size;
	Free_cell* next; 
};
static const int64_t CELL_SIZE = (int64_t) sizeof(uint64_t);
Free_cell* head_cell = 0; 

int64_t
nu_free_list_length()
{
    int64_t count = 1;
	Free_cell* curr_cell = head_cell;
	if (head_cell == 0) {
		return 0; 
	}
	while (curr_cell != 0 && curr_cell->next != NULL) {
		count++;
		if (curr_cell->next == NULL) {
			return count;
		}
		curr_cell = curr_cell->next; 
	}
	return count;
}

void insert_free_cell(Free_cell* new_cell) {
	// Inserts a new cell into the free_list 
	if ((uint64_t)head_cell == 0) {
		head_cell = new_cell;
		head_cell->next = 0; 
		return;
	}
	if ((uint64_t)new_cell <= (uint64_t)head_cell) {
	 	new_cell->next = head_cell; 
		head_cell = new_cell; 
		return;
	}
	for (Free_cell* pp = head_cell; pp != 0; pp = pp->next) {
		if (pp->next == 0) {  
			pp->next = new_cell;
			new_cell->next = 0;
			return;
		}
		else if ((uint64_t)new_cell < (uint64_t)pp->next) { 
			new_cell->next = pp->next;
			pp->next = new_cell;
			return;
		}
	}
}

void remove_free_cell(Free_cell* old_cell) {
	// Deletes a new cell from the free list
	if ((uint64_t)head_cell == (uint64_t)old_cell) {
		head_cell = old_cell->next; 
		return;
	}
	for (Free_cell* pp = head_cell; pp != 0; pp = pp->next) {
		if ((uint64_t)pp->next == (uint64_t)old_cell) {
			pp->next = old_cell->next; 
			return;
		}
	}
}

void*
hw06_malloc(size_t usize)
{
    // Allocate small blocks of memory by allocating 64k chunks
    // and then satisfying multiple requests from that.
    //
    // Allocate large blocks (>= 64k) of memory directly with
    // mmap.
	uint64_t alloc_size = usize + sizeof(uint64_t);
	if (alloc_size < sizeof(Free_cell)) {
		alloc_size = sizeof(Free_cell); 
	}
	void* addr; 
	if (alloc_size <= CHUNK_SIZE) {
		Free_cell* new_cell;
		// Iterate over the free list
		for (Free_cell* pp = head_cell; pp != 0; pp = pp->next) {
			// Check for free cells that are eligible to be allocated
			if (pp->size >= alloc_size) {
				if (pp->size - alloc_size < sizeof(Free_cell)) {
					addr = pp; 
					remove_free_cell(pp); 
					*(uint64_t*)addr = pp->size; 
					return addr + sizeof(uint64_t); 
				}
				// Get the address 
				addr = pp;
				// Insert a new_cell into the free_list and delete pp from the list
				new_cell = (Free_cell*) (addr + alloc_size); 
				new_cell->size = pp->size - alloc_size;
				remove_free_cell(pp); 
				insert_free_cell(new_cell);
				*(uint64_t*)addr = alloc_size;
				// Add allocation size to the address and return to user
				return addr + sizeof(uint64_t);
			}
		}
		// Make a new Free_cell and allocate it 
		addr = mmap(0, CHUNK_SIZE + sizeof(uint64_t), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
		if (CHUNK_SIZE - usize < sizeof(Free_cell)) {
			*(uint64_t*)addr = CHUNK_SIZE + sizeof(uint64_t);
			return addr + sizeof(uint64_t); 
		}
		new_cell = (Free_cell*) (addr + alloc_size); 
		new_cell->size = CHUNK_SIZE - usize;
		*((int64_t*) addr) = alloc_size; 
		// Insert the new_cell into the free list 
		insert_free_cell(new_cell);		
		// Return the location of the allocated memory
		return (addr + sizeof(uint64_t)); 
	}
	else {
		addr = mmap(0, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
		*((uint64_t*) addr) = alloc_size; 
		return (addr + sizeof(uint64_t));
	}
}

void
hw06_free(void* addr) 
{
    // Free small blocks by saving them for reuse.
    //   - Stick together adjacent small blocks into bigger blocks.
    //   - Advanced: If too many full chunks have been freed (> 4 maybe?)
    //     return some of them with munmap.
    // Free large blocks with munmap.
	uint64_t alloc_size = *(uint64_t*)(addr - sizeof(uint64_t)); 
	if (alloc_size < sizeof(Free_cell)) {
		assert(-1);
	}
	if (alloc_size > CHUNK_SIZE) {
		addr = addr - sizeof(uint64_t); 
		munmap(addr, alloc_size);
		return;
	}
	else {
		// Insert free_cell back into the free list
		Free_cell* free_cell = (Free_cell*) (addr - sizeof(uint64_t));
		free_cell->size = alloc_size; 
		insert_free_cell(free_cell);
		// Coalesce the list 
		for (Free_cell* pp = head_cell; pp != 0; pp = pp->next) {
			if ((uint64_t) pp + pp->size == (uint64_t) pp->next) {
				pp->size = pp->next->size + pp->size;
				pp->next = pp->next->next;
			}
		}
		return; 	
	}
}

