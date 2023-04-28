#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

constexpr size_t BUF_SIZE{ 100 };
void error_handling(const char* buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv[]) {
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in serv_adr{}, clnt_adr{};
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	fd_set reads{}, cpy_reads{};
	FD_ZERO(&reads);//初始化，可以不需要
	FD_SET(serv_sock, &reads);//将套接字文件描述符放到reads

	int fd_max = serv_sock;
	timeval timeout{};
	int str_len{}, fd_num{};
	char buf[BUF_SIZE]{};
	while (1) {
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;
		printf("max: %d", fd_max);
		for (int i = 0; i < fd_max + 1; i++) {
			if (FD_ISSET(i, &cpy_reads)) {//检测fd在fdset集合中的状态是否变化，当检测到fd状态发生变化(读写)时返回真，否则，返回假
				if (i == serv_sock) {//连接请求
					unsigned adr_sz = sizeof(clnt_adr);
					int clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);//注册了与客户端连接的套接字文件描述符
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;//改变fd_max
					printf("connected client: %d\n", clnt_sock);
				}
				else {//读取日志
					str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0) {//关闭
						FD_CLR(i, &reads);
						close(i);
						printf("close client: %d\n", i);
					}
					else {
						write(i, buf, str_len);
					}
				}
			}
		}
	}
}