#ifndef VECTOR_STAT_PROC_H

#define VECTOR_STAT_PROC_H

#define LOWER_LIMIT       0
#define UPPER_LIMIT     100
#define BUFFER_SIZE     128000

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <fcntl.h>

struct Son {

    pid_t id;
    int reader;
    int writer;

};

struct ListOfSons {

    struct Son *list;
    int n_elements;

};

int vector_get_in_range (int v[], int v_sz, int sv[], int min, int max, int n_processes);

#endif
