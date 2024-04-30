#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(){

    int pipefd[2];
    int status = pipe(pipefd);

    if (status < 0){
        perror("Erro ao criar os descritores pipe\n");
        return -1;
    }

    pid_t retfork = fork();

    if (retfork < 0){
        perror("Erro ao criar o processo filho\n");
        return -1;
    }

    if (retfork == 0){

        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execlp("/usr/bin/lscpu", "lscpu", NULL);

        exit(0);
    }

    close(pipefd[1]);
    char *output = malloc(sizeof(char));
    char buffer[BUFFER_SIZE];
    int n, k = 0;

    while ((n = read(pipefd[0], buffer, sizeof(char) * BUFFER_SIZE)) > 0){

        output = realloc(output, sizeof(char) * (n + (k == 0 ? 1 : k)));
        int j = 0;
        while (n-- > 0){
            output[k++] = toupper(buffer[j++]);
        }

    }

    if (n < 0){
        perror("Erro na leitura\n");
        return -1;
	}

    close(pipefd[0]);
    printf("%s", output);

    return 0;
}
