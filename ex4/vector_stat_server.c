#include "errors.h"
#include "sockets.h"
#include "vector_stat_proc.h"
#include "vector_stat_thread.h"
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <complex.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>

#define PORT 5000

typedef struct ArgsThread {

	int socketfd;
	int mode;

} ArgsThread;

void *handle_client(void *_args){

	ArgsThread *args = (ArgsThread *) _args;
	int mode = args->mode;
	int socketfd = args->socketfd;

	int values[3];
	int error;
	error = readn(socketfd, values, sizeof(int) * 3);
	handle_error_system(error, "Read\n");

	int v_sz = values[0];
	int min = values[1];
	int max = values[2];
	int *sv = malloc(sizeof(int) * v_sz);

	if (sv == NULL){
		printf("Erro ao alocar memória\n");
		exit(0);
	}

	int size;
	int *v = malloc(sizeof(int) * v_sz);
	error = readn(socketfd, v, sizeof(int) * v_sz);
	handle_error_system(error, "Read\n");

	if (mode == 0){
		size = vector_get_in_range(v, v_sz, sv, min, max, 4);	
	}else if (mode == 1){
		size = vector_get_in_range_with_threads(v, v_sz, sv, min, max, 4);
	}

	error = writen(socketfd, &size, sizeof(int));
	handle_error_system(error, "Write 1\n");
	error = writen(socketfd, sv, sizeof(int) * size);
	handle_error_system(error, "Write\n");

	free(args);
	free(sv);
	free(v);
	close(socketfd);
	return NULL;
}

int main(int argc, char *argv[]){

	if (argc != 2){
		printf("Número de argumentos inválido\n");
		return -1;
	}

	int mode;
	char *arg = argv[1];

	if (strcmp("-p", arg) == 0)
		mode = 0;
	else if (strcmp("-t", arg) == 0)
		mode = 1;
	else{
		printf("Argumento inválido\n");
		return -1;
	}

	int socketfd = tcp_socket_server_init(PORT);
	handle_error_system(socketfd, "TCP Server initialization\n");

	int *threads = malloc(sizeof(pthread_t));
	int i = 0;

	while (true) {
	
		int clientSocketfd = tcp_socket_server_accept(socketfd);
		handle_error_system(clientSocketfd, "Server connection accept\n");
		printf("Conexão %d aceita\n", i);
		pthread_t thread;
		ArgsThread *args = malloc(sizeof(ArgsThread));
		args->mode = mode;
		args->socketfd = clientSocketfd;
		int error;

		if ((error = pthread_create(&thread, NULL, handle_client, args)) != 0){
			errno = error;
			perror("Thread creation\n");
			exit(-1);
		}

		threads[i++] = thread;
		threads = realloc(threads, sizeof(pthread_t) * (i + 1));
	}	

	for (int j = 0; j < i; j++){

		int error;
		if ((error = pthread_join(threads[j], NULL)) != 0){
			errno = error;
			perror("Thread join\n");
			exit(-1);
		}

	}

	free(threads);
	return 0;
}
