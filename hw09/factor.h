#ifndef FACTOR_H
#define FACTOR_H

#include <stdint.h>

#include "job.h"

void submit_job(job jj);
job  get_result();

void run_jobs();

void factor(int64_t xx, int64_t* size, int64_t* ys);

void factor_init(int num_procs, pid_t* processes);
pid_t output_init(int64_t count); 
void factor_wait_done(pid_t* processes, int procs, pid_t process);

void factor_finish(int worker_count); 
void factor_cleanup(void);

// FIXME: These shouldn't need to be declared here.
void work_off_jobs();
void print_results(int64_t);

#endif
