#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>

#define msglength 516

int main (int argc, char ** argv){
	char returnText[256];
	int ret;
	int status;
	int pid;
	
	if (argc != 3){
		int sizeofreturnText = sprintf(returnText,"error in the number of arguments expected 3, received %d\n",argc);
		write(STDOUT_FILENO,returnText,sizeofreturnText);
		exit(EXIT_FAILURE);
	}
	char * serverName = argv[1];
	char * fileName = argv[2];
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
	getaddrinfo(serverName , "1069", &hints, &serverInfo);
	int socketFd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if(socketFd <0){
		int sizeofreturnMessage = sprintf(returnText,"error opening socket, socket = %d, reason : %d\n",socketFd,errno);
		write(STDOUT_FILENO,returnText,sizeofreturnMessage);
	}
	char socketMessage[msglength];
	char * messageMode = "netascii";
	
	//RRQ message
	int sizeofsocketMessage = sprintf(socketMessage,"%c%c%s%c%s%c",'\0',1,fileName,'\0',messageMode,'\0');
	sendto(socketFd,socketMessage,sizeofsocketMessage,0,serverInfo->ai_addr,serverInfo->ai_addrlen);
	
	int last = 0;
	int lastACKbyte1 = 0;
	int lastACKbyte2 = 0;
	int lastACK = 0;
	while (1){
		sizeofsocketMessage = recvfrom(socketFd,socketMessage,msglength,0,serverInfo->ai_addr,&(serverInfo->ai_addrlen));
		write(STDOUT_FILENO,"test",8);
		
		if (socketMessage[2]*256+socketMessage[3] == lastACK+1){
			lastACKbyte1+=1;
			if (lastACKbyte1 ==256){
				lastACKbyte2+=1;
				lastACKbyte1=0;
			}
			lastACK+=1;
			sprintf(socketMessage,"%c%c%c%c",'\0',4,lastACKbyte2,lastACKbyte1);
			sendto(socketFd,socketMessage,4,0,serverInfo->ai_addr,serverInfo->ai_addrlen);
			write(STDOUT_FILENO,socketMessage+4,sizeofsocketMessage-4);
			
		}
		else {
			write(STDOUT_FILENO,socketMessage+4,sizeofsocketMessage-4);
			sendto(socketFd,socketMessage,4,0,serverInfo->ai_addr,serverInfo->ai_addrlen);
		}
		
		if (sizeofsocketMessage < msglength){
			break;
		}
	}
	
	
}

