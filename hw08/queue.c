#include <stdlib.h>
#include <assert.h>
#include <pthread.h> 

#include "queue.h"

queue* iqueue;
queue* oqueue;

pthread_mutex_t mutex; 
pthread_cond_t empty; 

queue* 
make_queue()
{
    queue* qq = malloc(sizeof(queue));
    qq->head = 0;
    qq->tail = 0;
	qq->end_program_flag = false; 
	return qq;
}

void 
mutex_init() 
{
	pthread_mutex_init(&mutex, 0); 
	pthread_cond_init(&empty, 0); 
}

void 
free_queue(queue* qq)
{
    assert(qq->head == 0 && qq->tail == 0);
    free(qq);
}

void 
queue_put(queue* qq, void* msg)
{
	pthread_mutex_lock(&mutex);
    qnode* node = malloc(sizeof(qnode));
    node->data = msg;
    node->prev = 0;
    node->next = 0;
    
    node->next = qq->head;
    qq->head = node;

    if (node->next) {
        node->next->prev = node;
    } 
    else {
        qq->tail = node;
    }

	// Give signal to any threads waiting on get to go ahead
	pthread_cond_broadcast(&empty);
	pthread_mutex_unlock(&mutex);
}

void* 
queue_get(queue* qq)
{
	pthread_mutex_lock(&mutex);

    while (!qq->tail) {
		// if the queue is empty then you need to stop
		if (qq->end_program_flag) {
			pthread_mutex_unlock(&mutex);
			return 0; 
		}
	    int rv = pthread_cond_wait(&empty, &mutex);
		assert(rv == 0); 
		if (qq->end_program_flag) {
			pthread_mutex_unlock(&mutex);
			return 0; 
		}
	}

    qnode* node = qq->tail;

    if (node->prev) {
        qq->tail = node->prev;
        node->prev->next = 0;
    }
    else {
        qq->head = 0;
        qq->tail = 0;
    }

    void* msg = node->data;
    free(node);

    pthread_mutex_unlock(&mutex); 
	return msg;
}

void 
stop_program(queue* qq) 
{
	pthread_mutex_lock(&mutex);
	qq->end_program_flag = true; 
	pthread_cond_broadcast(&empty);
	pthread_mutex_unlock(&mutex); 
	return; 
}

void
queue_cleanup(queue* qq) 
{
	stop_program(qq);
	free_queue(qq);
	return;
}
