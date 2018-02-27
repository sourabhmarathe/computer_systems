#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h> 

#include "job.h"

#define QUEUE_SIZE 16
#define atomic _Atomic

typedef struct queue {
    job jobs[QUEUE_SIZE];
    _Atomic unsigned int qii; // Input index.
    _Atomic unsigned int qjj; // Output index.
	sem_t osem;
	sem_t isem; 
	bool end_program; 
} queue;

queue* make_queue();
void   free_queue(queue* qq);

void queue_put(queue* qq, job jj);
job  queue_get(queue* qq);

#endif
