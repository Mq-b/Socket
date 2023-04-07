#include<iostream>
#include<stdlib.h>
#include<winsock2.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET sock = 0;
	char message[BUF_SIZE]{};
	SOCKADDR_IN servAdr{};
	if (argc != 3) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 3), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));
	connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		send(sock, message, strlen(message), 0);//写 注意第三个参数，写入的字节数是我们存储的有效字符
		recv(sock, message, sizeof(message) - 1, 0);//读
		printf("Message from server: %s", message);
	}
	closesocket(sock);
	WSACleanup();
}