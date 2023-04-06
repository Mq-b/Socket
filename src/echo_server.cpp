#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv[]) {
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len;

	struct sockaddr_in serv_adr, clnt_adr;
	
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	socklen_t clnt_adr_sz=sizeof(clnt_adr);
	for (int i = 0; i < 5; ++i) {
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);
		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)//除非客户端的socket已经close了才会退出循环
			write(clnt_sock, message, str_len);
		close(clnt_sock);
	}
	close(serv_sock);
}
//只有第一个客户端连接的时候可以正常的打印Connected client 1，后面的都会堵塞等着循环
//后面每次退出一个客户端就往下运行打印一次，最多运行打印五次然后关闭