#include<iostream>
#include<unistd.h>
#include<sys/socket.h>

int main() {
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	pid_t pid = fork();
	if (pid == 0) //子进程
		printf("子进程套接字文件描述符: %d\n", sock);
	else//父进程
		printf("父进程套接字文件描述符: %d\n", sock);
}