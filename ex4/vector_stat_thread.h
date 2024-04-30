#ifndef VECTOR_STAT_THREAD_H

#define VECTOR_STAT_THREAD_H

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

struct ThreadArgs {

    int *v;
    int start;
    int end;
    int min;
    int max;

    int *sv;
    int count;

};

int vector_get_in_range_with_threads (int v[], int v_sz, int sv[], int min, int max, int n_threads);

#endif
