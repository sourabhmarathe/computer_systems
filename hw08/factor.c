
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <pthread.h> 

#include "queue.h"
#include "factor.h"

extern queue* iqueue;
extern queue* oqueue;

pthread_t*  
factor_init(int total_threads)
{
	iqueue = make_queue();
	oqueue = make_queue();
	pthread_t* threads = malloc(sizeof(pthread_t) * total_threads);
	for (int ii = 0; ii < total_threads; ++ii) {
		pthread_create(&threads[ii], 0, run_jobs, threads);
	}
	return threads; 
}

void
factor_cleanup(pthread_t* threads, int total_threads)
{
	for (int ii = 0; ii < total_threads; ++ii) {
		int rv = pthread_join(threads[ii], 0);
		assert(rv == 0); 
	}
	free(threads);
	return; 
}

factor_job* 
make_job(int128_t nn)
{
    factor_job* job = malloc(sizeof(factor_job));
    job->number  = nn;
    job->factors = 0;
    return job;
}

void 
free_job(factor_job* job)
{
    if (job->factors) {
        free_ivec(job->factors);
    }
    free(job);
}

void 
submit_job(factor_job* job, queue* qq)
{
    queue_put(qq, job);
}

factor_job* 
get_result(queue* qq)
{
    return queue_get(qq);
}

static
int128_t
isqrt(int128_t xx)
{
    double yy = ceil(sqrt((double)xx));
    return (int128_t) yy;
}

ivec*
factor(int128_t xx)
{
    ivec* ys = make_ivec();

    while (xx % 2 == 0) {
        ivec_push(ys, 2);
        xx /= 2;
    }

    for (int128_t ii = 3; ii <= isqrt(xx); ii += 2) {
        int128_t x1 = xx / ii;
        if (x1 * ii == xx) {
            ivec_push(ys, ii);
            xx = x1;
            ii = 1;
        }
    }

    ivec_push(ys, xx);

    return ys;
}

void* 
run_jobs(void* thread_list)
{
	pthread_t* threads = (pthread_t*) thread_list; 
    factor_job* job;
    while ((job = queue_get(iqueue))) {
        job->factors = factor(job->number);
        queue_put(oqueue, job);
    }
	return 0; 
}

