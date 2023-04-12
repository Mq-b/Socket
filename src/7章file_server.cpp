#include<iostream>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30
void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv[]) {
	int serv_sd = 0, clnt_sd = 0;
	FILE* fp;
	char buf[BUF_SIZE]{};
	int read_cnt = 0;
	sockaddr_in serv_adr {}, clnt_adr{};
	socklen_t clnt_ad_sz = 0;
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	fp = fopen("7章file_server.cpp", "rb");
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);
	serv_adr.sin_family = AF_INET;//定义是哪种地址族 AF_INET表示IPV4
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);//ip，INADDR_ANY表示本机IP
	serv_adr.sin_port = htons(atoi(argv[1]));//端口 htons函数可用于将主机字节顺序中的IP端口号转换为网络字节顺序中的IP端口号

	bind(serv_sd, (sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);

	socklen_t clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (sockaddr*)&clnt_adr, &clnt_adr_sz);
	while (1) {
		read_cnt = fread(buf, 1, BUF_SIZE, fp);
		if (read_cnt < BUF_SIZE) {
			write(clnt_sd, buf, read_cnt);
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
	}
	shutdown(clnt_sd, SHUT_WR);//断开输出流 向客户端传输了EOF，客户端知道文件传输完毕
	read(clnt_sd, buf, BUF_SIZE); //断开输出流 仍然可以使用输入流，即可以读取数据
	printf("Message from client: %s \n", buf);
	fclose(fp);
	close(clnt_sd); close(serv_sd);
}