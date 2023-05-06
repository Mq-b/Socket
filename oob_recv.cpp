#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
constexpr size_t BUF_SIZE{ 30 };

int acpt_sock{};
int recv_sock{};

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
void urg_handling(int signo) {
	char buf[BUF_SIZE]{};
	int str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);//接受紧急消息MSG_OOB
	printf("Urgent message: %s\n", buf);
}

int main(int argc,char*argv[]) {
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	struct sigaction act {};
	char buf[BUF_SIZE]{};
	act.sa_handler = urg_handling;//注册信号处理函数
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	int acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in recv_adr{}, serv_adr{};
	recv_adr.sin_family = AF_INET;
	recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(acpt_sock, (sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
		error_handling("bind() errpr");
	listen(acpt_sock, 5);
	socklen_t serv_adr_sz = sizeof(serv_adr);
	recv_sock = accept(acpt_sock,(sockaddr*)&recv_adr,&serv_adr_sz);
	fcntl(recv_sock, F_SETOWN, getpid());//将文件描述符指向的套接字拥有者（F_SETOWN）改为把getpid()函数返回值用作ID的进程，即当前进程
	int state = sigaction(SIGURG, &act, nullptr);//当收到MSG_OOB紧急信息的时候，会产生SIGURG信号，注册处理函数

	size_t str_len{};
	while ((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0) {
		if (str_len == -1)
			continue;
		puts(buf);
	}
	close(recv_sock);
	close(acpt_sock);
}