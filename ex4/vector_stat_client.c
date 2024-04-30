#include "errors.h"
#include "sockets.h"
#include "vector_stat_thread.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#define HOST "127.0.0.1"
#define PORT 5000
#define LOWER_LIMIT 0
#define UPPER_LIMIT 100

typedef struct ArgsThread {

	int socketfd;
	int *v;
	int v_sz;
	int min;
	int max;

}ArgsThread;

int get_random (int min, int max) 
{
    return rand() % (max - min + 1) + min;
}

void vector_init_rand (int v[], long dim, int min, int max)
{
    for (long i = 0; i < dim; i++) {
        v[i] = get_random(min, max);
    }
}

int test(int *v, int v_sz, int *v_server, int size_server){

	int *sv = malloc(sizeof(int) * v_sz);

    if (sv == NULL) {
        fprintf(stderr, "Erro malloc\n");
        return -1;
    }

	int count = vector_get_in_range_with_threads(v, v_sz, sv, 50, 100, 4);
	
	if (count != size_server)
		return -1;

	int status = memcmp(v_server, sv, size_server);
	free(sv);
	return status;
}

void *connection(void *_args){

	ArgsThread *args = (ArgsThread *) _args;
	int socketfd = args->socketfd;

	int *values = args->v;
	int v_sz = args->v_sz;
	int min = args->min;
	int max = args->max;

	int error;
	error = writen(socketfd, &v_sz, sizeof(int));
	handle_error_system(error, "Write\n");
	error = writen(socketfd, &min, sizeof(int));
	handle_error_system(error, "Write\n");
	error = writen(socketfd, &max, sizeof(int));
	handle_error_system(error, "Write\n");
	error = writen(socketfd, values, sizeof(int) * v_sz);
	handle_error_system(error, "Write\n");

	int size;

	error = readn(socketfd, &size, sizeof(int));
	handle_error_system(error, "Read\n");

	int *sv = malloc(sizeof(int) * size);
	error = readn(socketfd, sv, sizeof(int) * size);
	handle_error_system(error, "Read\n");

	int testNum = test(values, v_sz, sv, size);
	printf("TESTE=%d\n", testNum);

	free(args);
	free(sv);

	close(socketfd);
	return NULL;
}

int main(int argc, char *argv[]){

	if (argc != 3){
        printf("ERRO! ./vector_stat_proc <vector dimension> <connections>");
        return -1;
    }

	int min = 50;
	int max = 100;
    int v_sz = atoi(argv[1]);
	int connections = atoi(argv[2]);
    int *values = malloc(sizeof(int) * v_sz);

    if (values == NULL) {
        fprintf(stderr, "Erro malloc\n");
        return -1;
    }

    vector_init_rand(values, v_sz, LOWER_LIMIT, UPPER_LIMIT);
	pthread_t threads[connections];
	int i = 0;

	for (; i < connections; i++){

		int socketfd = tcp_socket_client_init(HOST, PORT);
		handle_error_system(socketfd, "Client connection\n");
		ArgsThread *args = malloc(sizeof(ArgsThread));
		args->socketfd = socketfd;
		args->min = min;
		args->max = max;
		args->v_sz = v_sz;
		args->v = values;
		pthread_t thread;
		int error;

		if ((error = pthread_create(&thread, NULL, connection, args)) != 0){
			errno = error;
			perror("Thread creation\n");
			exit(-1);
		}
		
		threads[i] = thread;
	}

	for (int j = 0; j < i; j++){

		int error;
		if ((error = pthread_join(threads[j], NULL)) != 0){
			errno = error;
			perror("Thread join\n");
			exit(-1);
		}

	}

	free(values);

	return 0;
}
