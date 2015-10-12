/*************************************************************************************
 *                                                                                   *
 *                            RovOS Windows Client Code                              *
 *                                       c++                                         *
 *                                      Nova                                         *
 *                                                                                   *
 *************************************************************************************/

#include <cstring>
#include <iostream>
using namespace std;
//windows server header file
#include <winsock.h>

//weird thing required on windows
#pragma comment(lib,"ws2_32.lib")

//port number socket server is running off of
#define PORT 42

//empties a character array
void blankOut(char string[]){
	for(int i = 0; i <= 100; i++){
		string[i] = ' ';
	}
}

int main(){
	//weird stuff windows requires
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	
	//declaring socket variable and srvrAddr structure
	int srvrSocket;
	struct sockaddr_in srvrAddr;

	//creating socket
	srvrAddr.sin_family = AF_INET;
	srvrSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (srvrSocket < 0)
	{
		cout << "Socket Creation FAILED!" << endl;
		return 1;
	}
	cout << "Socket Created!" << endl;
	
	//connecting to socket
	srvrAddr.sin_port = htons (PORT);
	srvrAddr.sin_addr.s_addr = inet_addr("192.168.1.150");
	if (connect(srvrSocket,(struct sockaddr *)&srvrAddr,sizeof(srvrAddr))!=0){
		cout << "No such Host!" << endl;
		if (srvrSocket) closesocket(srvrSocket);
		return 1;
	}
	cout << "CONNECTED!" << endl;
	
	//declaring variables for sending commands
	char again = 'y';
	char buffer[101] = {0};
	int n;
	
	//sends commands to linux server
	while(again == 'y'){
		
		//clearing buffer
		blankOut(buffer);
		
		//getting commands from user
		cout << "Enter command: ";
		cin.get(buffer, 25);
		cin.ignore();
		
		//sending command
		n = send(srvrSocket, buffer, strlen(buffer), 0);
		if(n < 0){
			cout << "Sending to Socket FAILED!" << endl;
			if (srvrSocket) closesocket(srvrSocket);
			return 1;
		}
		cout << "Message Sent!" << endl;
		
		//clearing buffer
		blankOut(buffer);
		
		//receving server reply
		n = recv(srvrSocket, buffer, 100, 0);
		if(n < 0){
			cout << "Reading from Socket FAILED!" << endl;
			if (srvrSocket) closesocket(srvrSocket);
			return 1;
		}
		//re,oving extra characters from server message;
		for(int i = 0; i < n; i++){
			cout << buffer[i];
		}
		cout << endl;
		
		//loops for multiple commands
		cout << "Would you like to send another command (y/n): ";
		cin >> again;
		cin.ignore();
	}
	
	//telling server to shutdown
	char end[4] = "end";
	n = send(srvrSocket, end, 4, 0);
	if(n < 0){
		cout << "Sending to Socket FAILED!" << endl;
		if (srvrSocket) closesocket(srvrSocket);
		return 1;
	}
	
	//closing socket and ending program
	closesocket(srvrSocket);
	return 0;
}
