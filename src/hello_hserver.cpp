#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(const char* message);

int main(int argc,char*argv[]) {
	int serv_sock = 0;
	int clnt_sock = 0;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = 0;
	const char message[] = "Hello World";
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(-1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);//PF_INET等价AF_INET，都是表示IPV4
	if (serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;//定义是哪种地址族 AF_INET表示IPV4
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//ip，INADDR_ANY表示本机IP
	serv_addr.sin_port = htons(atoi(argv[1]));//端口 htons函数可用于将主机字节顺序中的IP端口号转换为网络字节顺序中的IP端口号

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	write(clnt_sock, message, sizeof(message));//写入到客户端
	close(clnt_sock);
	close(serv_sock);
}

void error_handling(const char* message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}