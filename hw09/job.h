#ifndef JOB_H
#define JOB_H

#include <stdint.h>

typedef struct job {
    int64_t number;
    int64_t count; // # of factors
    int64_t factors[68];
} job;

job make_job(int64_t nn);

#endif
