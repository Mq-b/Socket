#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define BUF_SIZE 30

void read_childpro(int sig) {
	int status{};
	pid_t pid = waitpid(-1, &status, WNOHANG);//回收子进程，返回回收子进程的ID
	printf("removed proc id: %d\n", pid);
}
void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv[]) {
	if (argc != 2) {
		printf("Usage :%s <port>\n", argv[0]);
		exit(1);
	}
	sockaddr_in serv_adr{}, clnt_adr{};
	char buf[BUF_SIZE]{};
	struct sigaction act{};
	act.sa_handler = read_childpro;
	sigemptyset(&act.sa_mask);
	int state = sigaction(SIGCHLD, &act, 0);//注册子进程结束信号的处理函数
	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);//创建套接字
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	pid_t pid{};
	socklen_t adr_sz{};
	int str_len{};
	int  clnt_sock{};
	while (1) {
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (sockaddr*)&clnt_adr, &adr_sz);
		if (clnt_sock == -1)
			continue;
		else
			puts("new client connected...");
		pid = fork();
		if (pid == -1) {
			close(clnt_sock);
			continue;
		}
		if (pid == 0) {//子进程
			close(serv_sock);//关闭服务器套接字
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len);
			close(clnt_sock);//收到客户端发送的EOF后跳出循环，那么关闭和客户端的连接销毁套接字
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock);//无关的套接字文件描述符关掉
	}
}
/*fork()函数复制父进程的所有资源，但是套接字不属于进程，它是操作系统的，进程拥有的智商相应套接字的文件描述符，所以会复制文件描述符
即不同进程文件描述符指向同一个套接字*/
/*一个套接字中存在2个文件描述符的时候，只有两个文件描述符都被销毁后，才能销毁套接字。单独的销毁一个套接字描述符不会销毁套接字，所以我们64行
需要将父进程的套接字文件描述符关闭，这样子进程执行完毕后调用close(clnt_sock);才会真的销毁套接字断开连接*/