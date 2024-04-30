#include "errors.h"
#include <stdio.h>

void fatal_error_system(char *msg){
	perror(msg);
	exit(-1);
}

void handle_error_system(int error, char *msg){

	if (error < 0){
		fatal_error_system(msg);
	}

}
