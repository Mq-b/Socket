#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>

#define BUF_SIZE 30
void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
void read_routine(int sock, char* buf) {
	while (1) {
		int str_len = read(sock, buf, BUF_SIZE);
		if (str_len == 0)return;
		printf("Message from server: %s", buf);
	}
}
void write_routine(int sock, char* buf) {
	while (1) {
		fgets(buf, BUF_SIZE, stdin);
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
			shutdown(sock, SHUT_WR);//关闭写，向服务端传递EOF，
			return;
		}
		write(sock, buf, strlen(buf));
	}
}

int main(int argc,char*argv[]) {
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
	}
	sockaddr_in serv_adr{};
	char buf[BUF_SIZE]{};
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	int pid = fork();
	if (pid == 0)//子进程
		write_routine(sock, buf);//除非输入q才会退出，然后关闭标准输入
	else//父进程
		read_routine(sock, buf);//读不到东西后退出，也就是子进程使用shutdown关闭了标准输入运行到49行close(sock)发送完EOF了
	close(sock);//因为前面的fork()的缘故，套接字文件描述符有两个，单独的close(sock)没有作用，是子进程调用了shutdown后再运行到这里才有作用
}