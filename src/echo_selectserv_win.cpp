#include<iostream>
#include<WinSock2.h>
#define BUF_SIZE 1024

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	WSADATA wsaData{};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	int hServSock = socket(PF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN servAdr{}, clntAdr{};
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	fd_set reads{}, cpyReads{};
	FD_ZERO(&reads);
	FD_SET(hServSock, &reads);
	TIMEVAL timeout{};
	int fdNum{};
	int fd_max{};
	char buf[BUF_SIZE]{};

	while (1) {
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
			break;//错误
		if (fdNum == 0)
			continue;//超时
		for (int i = 0; i < reads.fd_count; ++i) {
			if (FD_ISSET(reads.fd_array[i], &cpyReads)) {////检测fd在fdset集合中的状态是否变化，当检测到fd状态发生变化(读写)时返回真，否则，返回假
				if (reads.fd_array[i] == hServSock) {//连接请求
					int adrSz = sizeof(clntAdr);
					int hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
					FD_SET(hClntSock, &reads);//注册了与客户端连接的套接字文件描述符
					if (fd_max < hClntSock)
						fd_max = hClntSock;
					printf("connected client: %d \n", hClntSock);
				}
				else {
					int strlen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
					if (strlen == 0) {
						FD_CLR(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
						closesocket(cpyReads.fd_array[i]);
						printf("closed client: %zd \n", cpyReads.fd_array[i]);
					}
					else {
						send(reads.fd_array[i], buf, strlen, 0);
					}
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
}