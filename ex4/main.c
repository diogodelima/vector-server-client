#include "sockets.h"
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    
    int s = un_socket_server_init("/tmp/SocketSO");

    int socketfd;

	pid_t retfork = fork();

	if (retfork == 0){

		un_socket_client_init("/tmp/SocketSO");

		return 0;
	}

	waitpid(retfork, NULL, 0);

    while((socketfd = un_socket_server_accept(s)) < 0){

    }


    return 0;
}
