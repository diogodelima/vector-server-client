#include "vector_stat_thread.h"

void * get_vector_in_range(void * _arg){

    struct ThreadArgs *arg = (struct ThreadArgs *) _arg;
    int count = 0;

    for (int i = arg->start; i < arg->end; i++){
        
        int e = arg->v[i];

        if (e <= arg->max && e >= arg->min){
            arg->sv[count] = e;
            count++;
        }

    }

    arg->count = count;
    return NULL;
}

int vector_get_in_range_with_threads (int v[], int v_sz, int sv[], int min, int max, int n_threads){

    int count = 0;
    int k = 0;
    int size = v_sz / n_threads;
    pthread_t thid[n_threads];
    struct ThreadArgs args[n_threads];
    
    for (int i = 0; i < n_threads; i++){

        args[i].v = v;
        args[i].start = i * size;
        args[i].end = args[i].start + size;
        args[i].min = min;
        args[i].max = max;
        int *ar = malloc(sizeof(int) * size);
        args[i].sv = ar;

        int error;

        if ((error = pthread_create(&thid[i], NULL, get_vector_in_range, &args[i])) != 0){
            errno = error;
            perror("Erro ao criar a thread\n");
            exit(-1);
        }

    }

    for (int i = 0; i < n_threads; i++){

        int error;

        if ((error = pthread_join(thid[i], NULL)) != 0){
            errno = error;
            perror("Erro ao esperar pela thread\n");
            exit(-1);
        }

        struct ThreadArgs arg = args[i];
        count += arg.count;

        for (int j = 0; j < arg.count; j++){
            sv[k++] = arg.sv[j];
        }

		free(arg.sv);
    }
    
    return count;
}
