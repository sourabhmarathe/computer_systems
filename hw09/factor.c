
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include "job.h"
#include "queue.h"
#include "factor.h"

static queue* iqueue;
static queue* oqueue;

static int worker_count = 0;
static pid_t workers[64];
static pid_t printer;

void
work_off_jobs(void)
{
    while (1) {
        job jj = queue_get(iqueue);
        if (jj.number < 0) {
            break;
        }

        factor(jj.number, &(jj.count), &(jj.factors[0]));

        queue_put(oqueue, jj);
    }
}

void
print_results(int64_t count)
{
    int64_t oks = 0;

    for (int64_t ii = 0; ii < count; ++ii) {
        job res = get_result();

        printf("%ld: ", res.number);
        int64_t prod = 1;
        for (int64_t jj = 0; jj < res.count; ++jj) {
            int64_t xx = res.factors[jj];
            prod *= xx;
            printf("%ld ", xx);
        }
        printf("\n");

        if (prod == res.number) {
            ++oks;
        }
        else {
            fprintf(stderr, "Warning: bad factorization");
        }
    }

    printf("Factored %ld / %ld numbers.\n", oks, count);
}

void
factor_wait_done(pid_t* processes, int procs, pid_t process)
{
	int status; 
	waitpid(process, &status, 0);  
	for (int ii = 0; ii < procs; ++ii) {
		waitpid(processes[ii], &status, 0); 
	}
	return; 
}

void
factor_init(int num_procs, pid_t* processes)
{
    if (iqueue == 0) iqueue = make_queue();
    if (oqueue == 0) oqueue = make_queue();

	int status, output_status; 
	// Spawn num_procs worker procs and a printing proc.
	for (int ii = 0; ii < num_procs; ++ii) {
    	processes[ii] = fork();
		assert(processes[ii] != -1);  
		if (processes[ii] == 0) {
			work_off_jobs(); 
			exit(0); 
		}
	}
}

pid_t
output_init(int64_t count) 
{	
	// One more printing thread
	pid_t outprocess = fork(); 
	assert(outprocess != -1);
	if (outprocess == 0) {
		print_results(count); 
		exit(0); 
	}
	return outprocess; 
}
	

void
factor_finish(int worker_count)
{
    job done = make_job(-1);

    for (int ii = 0; ii < worker_count; ++ii) {
        submit_job(done);
	}
}

void 
factor_cleanup(void) 
{
    free_queue(iqueue);
    iqueue = 0;
    free_queue(oqueue);
    oqueue = 0;
}

void
submit_job(job jj)
{
    queue_put(iqueue, jj);
}

job
get_result()
{
    return queue_get(oqueue);
}

static
int64_t
isqrt(int64_t xx)
{
    double yy = ceil(sqrt((double)xx));
    return (int64_t) yy;
}

void
factor(int64_t xx, int64_t* size, int64_t* ys)
{
    int jj = 0;

    while (xx % 2 == 0) {
        ys[jj++] = 2;
        xx /= 2;
    }

    for (int64_t ii = 3; ii <= isqrt(xx); ii += 2) {
        int64_t x1 = xx / ii;
        if (x1 * ii == xx) {
            ys[jj++] = ii;
            xx = x1;
            ii = 1;
        }
    }

    ys[jj++] = xx;
    *size = jj;
}

