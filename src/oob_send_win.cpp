#include<iostream>
#include<stdlib.h>
#include<WinSock2.h>
constexpr size_t BUF_SIZE{ 30 };

void ErrorHandling(const char* message) {
	fputs(message,stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv []) {
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	WSADATA wsaData{};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAtartup() error!");
	SOCKET hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == -1)
		ErrorHandling("socket() error");

	SOCKADDR_IN sendAdr{};
	sendAdr.sin_family = AF_INET;
	sendAdr.sin_addr.s_addr = inet_addr(argv[1]);
	sendAdr.sin_port = htons(static_cast<short>(atoi(argv[2])));

	if (connect(hSocket, (SOCKADDR*)&sendAdr, sizeof(sendAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	
	send(hSocket, "123", 3, 0);
	send(hSocket, "4", 1, MSG_OOB);
	send(hSocket, "567", 3, 0);
	send(hSocket, "890", 3, MSG_OOB);

	closesocket(hSocket);
	WSACleanup();
}