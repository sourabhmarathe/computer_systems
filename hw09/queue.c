#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdbool.h> 

#include "queue.h"

#define QUEUE_SIZE 16
// TODO: Make this an interprocess queue.

queue*
make_queue()
{
    int pages = 1 + sizeof(queue) / 4096;
    queue* qq = mmap(0, pages * 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0); 
	qq->qii = 0;
	qq->qjj = 0; 
	sem_init(&(qq->isem), 1, QUEUE_SIZE);
	sem_init(&(qq->osem), 1, 0);  
	qq->end_program = false; 
    return qq;
}

void
free_queue(queue* qq)
{
    assert(qq->qii == qq->qjj);
   	munmap(&qq, sizeof(qq));
}

void
queue_put(queue* qq, job msg)
{
	int rv;
	rv = sem_wait(&qq->isem);
	assert(rv == 0); 

    unsigned int ii = atomic_fetch_add(&(qq->qii), 1);
    qq->jobs[ii % QUEUE_SIZE] = msg;

	rv = sem_post(&qq->osem);
	assert(rv == 0); 
}

job
queue_get(queue* qq)
{	
	int rv; 
	rv = sem_wait(&qq->osem);

    unsigned int jj = atomic_fetch_add(&(qq->qjj), 1);
    job available_job = qq->jobs[jj % QUEUE_SIZE];
	
	rv = sem_post(&qq->isem);
	assert(rv == 0); 
	return available_job; 
}

