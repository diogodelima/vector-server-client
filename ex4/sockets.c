#include "sockets.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/*

init tcp socket, returns -1 if error or socket fd if success

*/
int tcp_socket_server_init(int serverPort){

    int socketfd;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(serverPort);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0){
		close(socketfd);
        return -1;
	}

    if ((listen(socketfd, 5)) < 0){
		close(socketfd);
        return -1;
	}

    return socketfd;
}

int tcp_socket_server_accept(int serverSocket){

    struct sockaddr client_addr;
    __u_int dim_client = sizeof(client_addr);
    int socketfd = accept(serverSocket, (struct sockaddr *) &client_addr, &dim_client);

    if (socketfd < 0)
        return -1;

    return socketfd;
}

int tcp_socket_client_init(const char *host, int port){

    int socketfd;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host);
    serv_addr.sin_port = htons(port);

    if (connect(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        return -2;

    return socketfd;
}

int un_socket_server_init(const char *serverEndPoint){

	int socketfd;

	if ((socketfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return -1;

	struct sockaddr_un serv_addr;
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, serverEndPoint);

	if (bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		close(socketfd);
		return -2;
	}

	if (listen(socketfd, 5) < 0){
		close(socketfd);
		return -3;
	}

	return socketfd;
}

int un_socket_server_accept(int serverSocket){

	struct sockaddr client_addr;
    __u_int dim_client = sizeof(client_addr);
    int socketfd = accept(serverSocket, (struct sockaddr *) &client_addr, &dim_client);

    if (socketfd < 0){
        return -1;
	}

    return socketfd;
}

int un_socket_client_init(const char *serverEndPoint){

	int socketfd;

    if ((socketfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return -1;

    struct sockaddr_un serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, serverEndPoint);

    if (connect(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        return -2;
	}

    return socketfd;
}

int readn (int fd, void *buf, size_t count){
	
	char *ptbuf = buf;
	size_t bytesToReceive = count;
	
	while (bytesToReceive > 0) {

		int nBytesRD = read(fd, ptbuf, bytesToReceive);
		if (nBytesRD <= 0) return nBytesRD;
		ptbuf += nBytesRD;
		bytesToReceive -= nBytesRD;
	
	}

	return count;
}

int writen (int fd, const void *buf, size_t count){

	int nBytesWR;
	const char *ptbuf = buf;
	size_t bytesToSend = count;

	while (bytesToSend > 0) {
	
		nBytesWR = write(fd, ptbuf, bytesToSend);
		if (nBytesWR <= 0) return nBytesWR;
		ptbuf += nBytesWR;
		bytesToSend -= nBytesWR;

	}

	return count;
}


