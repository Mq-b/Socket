#include<iostream>
#include<unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[]) {
	int fds1[2]{}, fds2[2]{};
	char str1[] = "Who are you?";
	char str2[] = "Thank you your message";
	char buf[BUF_SIZE]{};

	pipe(fds1), pipe(fds2);//创建两个管道
	pid_t pid = fork();
	if (pid == 0) {//子进程
		write(fds1[1], str1, sizeof(str1));//父进程write fds1
		read(fds2[0], buf, BUF_SIZE);
		printf("Child proc output: %s\n", buf);
	}
	else {//父进程
		read(fds1[0], buf, BUF_SIZE);//子进程read fds1 这样的对应
		printf("Parent proc output: %s\n", buf);
		write(fds2[1], str2, sizeof(str2));
	}
}