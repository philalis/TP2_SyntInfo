#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char ** argv){
	char returnText[1024];
	int ret;
	int status;
	int pid;
	
	if (argc != 3){
		int sizeofreturnText = sprintf(returnText,"error in the number of arguments expected 3, received %d\n",argc);
		write(STDOUT_FILENO,returnText,sizeofreturnText);
		exit(EXIT_FAILURE);
	}
	char * serverName;
	char * file name;
	pid = fork();
	if(pid == 0){
		execlp("ping","ping",serverName,(char *)NULL);
	}
	else {
		wait(&status);			// to be improved considering the ping might not succeed and keep on sending
		if (WIFEXITED(status)){
			if (!WEXITSTATUS(status)){
				write(STDOUT_FILENO,"host not valid\n",16);
				exit(EXIT_FAILURE);
			}
		}
	}
	
	
}

