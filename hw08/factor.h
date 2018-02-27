#ifndef FACTOR_H
#define FACTOR_H

#include <stdint.h>
#include <pthread.h> 

#include "int128.h"
#include "ivec.h"
#include "queue.h"

typedef struct factor_job {
    int128_t number;
    ivec*    factors;
} factor_job;

pthread_t* factor_init(int total_threads);
void factor_cleanup(pthread_t* thread_list, int total_threads);

factor_job* make_job(int128_t nn);
void free_job(factor_job* job);

void submit_job(factor_job* job, queue* qq);
factor_job* get_result(queue* qq);

void* run_jobs(void* thread_list);

ivec* factor(int128_t xx);

#endif
