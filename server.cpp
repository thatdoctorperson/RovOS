/*************************************************************************************
 *                                                                                   *
 *                             RovOS Linux Server Code                               *
 *                                        c                                          *
 *                                      Nova                                         *
 *                                                                                   *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//linux server header files
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//port number socket server is running off of
#define PORT 42

//empties a character array
void blankOut(char string[]){
	for(int i = 0; i <= 100; i++){
		string[i] = ' ';
	}
}

int main(){
	
	//declaring socket variable and srvrAddr structure
	int srvrSocket;
	struct sockaddr_in srvrAddr, cliAddr;
	socklen_t cliLen = sizeof(cliAddr);
	
	//creating socket
	srvrAddr.sin_family = AF_INET;
	srvrSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (srvrSocket < 0)
	{
		printf("Socket Creation FAILED!\n");
		return 0;
	}
	printf("Socket Created!\n");
	
	//binding socket
	srvrAddr.sin_port = htons (PORT);
	srvrAddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(srvrSocket, (struct sockaddr *)&srvrAddr, sizeof(srvrAddr))<0){
		printf("Binding Socket FAILED!\n");
		if (srvrSocket) close(srvrSocket);
		return 0;
	}
	printf("Socket Bound!\n");
	
	//listening for connections
	printf("Listening on %d...\n", PORT);
	if (listen(srvrSocket, 5)<0){
		printf("Listening on Socket FAILED!\n");
		if (srvrSocket) close(srvrSocket);
		return 0;
	}
	
	//accepting connection
	srvrSocket = accept(srvrSocket, (struct sockaddr *)&cliAddr, &cliLen);
	if (srvrSocket<0)
	{
		printf("Socket Connection FAILED!\n");
		if (srvrSocket) close(srvrSocket);
		return 0;
	}
	printf("Connection Established!\n");
	
	//receiving commands
	char buffer[101];
	int n;
	while(strcmp(buffer,"end") != 0){
		
		//clearing buffer
		blankOut(buffer);
		
		//reading command
		n = read(srvrSocket, buffer, 100);
		if(n < 0){
			printf("Reading from Socket FAILED!\n");
			if (srvrSocket) close(srvrSocket);
			return 0;
		}
		
		//replies to client
		n = write(srvrSocket, "Command Executing!", 18);
		if(n < 0){
			printf("Writing to Socket FAILED!\n");
			if (srvrSocket) close(srvrSocket);
			return 0;
		}
		
		//runs commands
		if(strcmp(buffer,"end") != 0){
			printf("Command Received: %s\n", buffer);
			system(buffer);
		//shuts down server if no more commands are coming
		}else{
			printf("Closing Program...\n");
		}
	}
	
	//closing socket and ending program
	close(srvrSocket);
	return 0;
}
