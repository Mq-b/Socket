#include<iostream>
#include<stdlib.h>
#include<winsock2.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET servSock;
	char message[BUF_SIZE]{};
	int strlen = 0;
	int clntAdrSz = 0;

	SOCKADDR_IN servAdr, clntAdr;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 3), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == -1)
		ErrorHandling("UDP socket() creation error");

	memset(&servAdr, 0, sizeof(servSock));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	while (1) {
		clntAdrSz = sizeof(clntAdr);
		strlen = recvfrom(servSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
		sendto(servSock, message, strlen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));//注意第三个参数
	}
	closesocket(servSock);
	WSACleanup();
}