#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	char message[30]{};
	sockaddr_in serv_adr{}, clnt_adr{};
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
	}

	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	int optlen{};
	optlen = sizeof(optlen);
	int option = true;
	//将Time-wait状态下的套接字端口号重新分配给新的套接字
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	unsigned clnt_adr_sz = sizeof(clnt_adr);
	int clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &clnt_adr_sz);
	int str_len{};
	while ((str_len = read(clnt_sock, message, sizeof(message))) != 0) {
		write(clnt_sock, message, str_len);
		write(1, message, str_len);
	}
	close(clnt_sock);
	close(serv_sock);
}