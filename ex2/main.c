#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <pthread.h> 
#include <errno.h>

void * process_work(void * _arg);

int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Número de argumentos inválidos\n");
        return -1;
    }

    int nThreads = atol(argv[1]);
    pthread_t thid[nThreads];

    for (int i = 0; i < nThreads; i++){

        long v = 1e9;
        int error;

        if ((error = pthread_create(&thid[i], NULL, process_work, &v)) != 0){
            errno = error;
            perror("Erro ao criar a thread\n");
            exit(-1);
        }
    }

    for (int i = 0; i < nThreads; i++){

        int error;

        if ((error = pthread_join(thid[i], NULL)) != 0){
            errno = error;
            perror("Erro ao esperar pela thread\n");
            exit(-1);
        }
    }
    

    return 0;
}

void * process_work(void * _arg) {

    long *arg = (long *) _arg;
    long niter = *arg; 

    for (long i = 0; i < niter; i++) {
        sqrt (rand());
    }

    return NULL;
}