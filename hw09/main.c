
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job.h"
#include "factor.h"

int
main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Usage:\n");
        printf("  ./main procs start count\n");
        return 1;
    }

    int procs = atoi(argv[1]);
    int64_t start = atol(argv[2]);
    int64_t count = atol(argv[3]);

	pid_t* processes = malloc(sizeof(pid_t) * procs);
	factor_init(procs, processes);  
  	pid_t output_process = output_init(count);

    for (int64_t ii = 0; ii < count; ++ii) {
        job jj = make_job(start + ii);
        submit_job(jj);
    }

	factor_finish(procs); 
    factor_wait_done(processes, procs, output_process);
    factor_cleanup();
	free(processes); 

    return 0;
}
