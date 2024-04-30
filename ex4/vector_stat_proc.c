#include "vector_stat_proc.h"
#include <stdio.h>

struct ListOfSons *init(int n_processes){

    struct ListOfSons *listOfSons = malloc(sizeof(struct ListOfSons));
    listOfSons->list = malloc(sizeof(struct Son) * n_processes);
    listOfSons->n_elements = 0;

    return listOfSons;
}

int vector_get_in_range (int v[], int v_sz, int sv[], int min, int max, int n_processes){

    struct ListOfSons *listOfSons = init(n_processes);

    int toRead = v_sz / n_processes;

    for (int i = 0; i < n_processes; i++){

        int index = listOfSons->n_elements++;
        int start = i * toRead;
       
        int pipefd[2];
        if (pipe(pipefd) < 0){
            perror("Erro na criação do pipe");
            return -1;
        }

        pid_t retfork = fork();

        if (retfork < 0){
            perror("Erro ao criar o processo filho.\n");
            return -1;
        }

        if (retfork == 0){

            int index = 0;
            int *subarray = malloc(0);
            close(pipefd[0]);

            for (int i = start; i < start + toRead; i++){

                int el = v[i];

                if (el >= min && el <= max){

                    subarray = realloc(subarray, sizeof(int) * (index + 1));
                    subarray[index++] = el;

                }

            }

            int numberOfBytesWritten = 0;
            while ((numberOfBytesWritten = write(pipefd[1], &subarray[numberOfBytesWritten], (sizeof(int) * index) - numberOfBytesWritten)) > 0);

            if (numberOfBytesWritten < 0){
                close(pipefd[1]);
                perror("Erro na escrita\n");
                exit(-1);
            }

			free(subarray);
            close(pipefd[1]);
            exit(0);
        }else{

            close(pipefd[1]);

            struct Son son;
            son.id = retfork;
            son.reader = pipefd[0];
            son.writer = pipefd[1];
            listOfSons->list[index] = son;
  
        }


    }

    int count = 0;

    for (int i = 0; i < listOfSons->n_elements; i++){

        struct Son son = listOfSons->list[i];
        int reader = son.reader;
        int number[BUFFER_SIZE]; int status;

        while ((status = read(reader, &number, sizeof(int) * BUFFER_SIZE)) > 0){
            for (int i = 0; i < status / sizeof(int); i++)
                sv[count++] = number[i];
        }

        if (status < 0){
            perror("Erro na leitura\n");
        }

        close(reader);
    }

    for (int i = 0; i < listOfSons->n_elements; i++){

        struct Son son = listOfSons->list[i];
        int status;
        int p = waitpid(son.id, &status, 0);

        if (p < 0 || !WIFEXITED(status))
            perror("Ocorreu algum problema ao esperar pelo processo");

    }
   
	free(listOfSons->list);
	free(listOfSons);
    return count;
}
