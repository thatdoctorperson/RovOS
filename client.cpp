#include <winsock.h>
#include <string.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define PORT 42

void blankOut(char string[]);

int main(){
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	
	int thisSocket;
	struct sockaddr_in destination;

	destination.sin_family = AF_INET;
	thisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (thisSocket < 0)
	{
		cout << "Socket Creation FAILED!" << endl;
		return 0;
	}
	cout << "Socket Created!" << endl;
	
	destination.sin_port = htons (PORT);
	destination.sin_addr.s_addr = inet_addr("192.168.1.150");
	if (connect(thisSocket,(struct sockaddr *)&destination,sizeof(destination))!=0){
		cout << "No such Host!" << endl;
		if (thisSocket) closesocket(thisSocket);
		return 0;
	}
	cout << "CONNECTED!" << endl;
	
	char again = 'y';
	char buffer[101] = {0};
	int n;
	
	while(again == 'y'){
		blankOut(buffer);
		cout << "Enter command: ";
		cin.get(buffer, 25);
		cin.ignore();
		n = send(thisSocket, buffer, strlen(buffer), 0);
		if(n < 0){
			cout << "Sending to Socket FAILED!" << endl;
			if (thisSocket) closesocket(thisSocket);
			return 0;
		}
		cout << "Message Sent!" << endl;
		
		cout << "Would you like to send another command (y/n): ";
		cin >> again;
		cin.ignore();
	}
	
	char end[4] = "end";
	n = send(thisSocket, end, 4, 0);
	if(n < 0){
		cout << "Sending to Socket FAILED!" << endl;
		if (thisSocket) closesocket(thisSocket);
		return 0;
	}
	
	closesocket(thisSocket);
	return 0;
}

void blankOut(char string[]){
	for(int i = 0; i <= 100; i++){
		string[i] = ' ';
	}
}
