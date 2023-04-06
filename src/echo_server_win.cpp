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
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len;

	SOCKADDR_IN serv_adr, clnt_adr;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		ErrorHandling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		ErrorHandling("bind() error");
	if (listen(serv_sock, 5) == -1)
		ErrorHandling("listen() error");

	int clnt_adr_sz = sizeof(clnt_adr);
	for (int i = 0; i < 5; ++i) {
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d \n", i + 1);
		while ((str_len = recv(clnt_sock, message, BUF_SIZE, 0)) != 0)//除非客户端的socket已经close了才会退出循环
			send(clnt_sock, message, str_len, 0);
		closesocket(clnt_sock);
	}
	closesocket(serv_sock);
}
//只有第一个客户端连接的时候可以正常的打印Connected client 1，后面的都会堵塞等着循环
//后面每次退出一个客户端就往下运行打印一次，最多运行打印五次然后关闭