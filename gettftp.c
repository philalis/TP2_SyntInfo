#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

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
	char * fileName;
	/*pid = fork();
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
	}*/
	struct addrinfo hints = {0};
	struct addrinfo * serverInfo;
	hints.ai_protocol = 17;
	getaddrinfo(argv[1] , NULL, &hints, &serverInfo);
	int socketFd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if(socketFd <0){
		int sizeofreturnMessage = sprintf(returnText,"error opening socket, socket = %d, reason : %d\n",socketFd,errno);
		write(STDOUT_FILENO,returnText,sizeofreturnMessage);
	}
	char socketMessage[1024];
	char * messageMode ="netascii";
	int sizeofsocketMessage = sprintf(socketMessage,"%c%c%s%c%s%c",'\0',1,fileName,'\0',messageMode,'\0');
	sendto(socketFd,socketMessage,sizeofsocketMessage,0,serverInfo->ai_addr,serverInfo->ai_addrlen);
	
	while(1){//field of 512 bytes, data =3, receiving moment incoming.

	}
	
	
}

