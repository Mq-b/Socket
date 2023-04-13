#include<winsock2.h>
#include<iostream>
#define BUF_SIZE 30

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hServSock{}, hClntSock{};
	char buf[BUF_SIZE]{};
	int readCnt{};

	SOCKADDR_IN servAdr{}, clntAdr{};
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	FILE* fp = fopen("7章file_server_win.cpp","rb");
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1])); 

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);
	int clntAdrSz = sizeof(clntAdr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

	while (1) {
		readCnt = fread(buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE) {
			send(hClntSock, buf, readCnt, 0);
		}
		send(hClntSock, buf, BUF_SIZE, 0);
	}
	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, buf, BUF_SIZE, 0);
	printf("Message from client: %s \n", buf);

	fclose(fp);
	closesocket(hClntSock);
	WSACleanup();
}