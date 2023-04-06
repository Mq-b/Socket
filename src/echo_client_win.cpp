#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock.h>

#define BUF_SIZE 1024
void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	WSADATA wsaData;
	int sock = 0;
	char message[BUF_SIZE]{};
	int str_len = 0;
	SOCKADDR_IN serv_adr;
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		ErrorHandling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		ErrorHandling("connect() error!");
	else
		puts("Connected.........");
	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		send(sock, message, strlen(message), 0);
		str_len = recv(sock, message, BUF_SIZE - 1, 0);
		printf("Message from server: %s", message);
	}
	closesocket(sock);
}