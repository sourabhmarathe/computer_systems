#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h> 
#include <pthread.h> 
typedef struct qnode {
    void*         data;
    struct qnode* prev;
    struct qnode* next;
} qnode;

typedef struct queue {
    qnode* head;
    qnode* tail;

	bool end_program_flag; 
} queue;

void mutex_init(); 
queue* make_queue();
void free_queue(queue* qq);

void queue_put(queue* qq, void* msg);
void* queue_get(queue* qq);

void stop_program(queue* qq); 

#endif
