#include<iostream>
#include<unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[]) {
	int fds[2]{};
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE]{};
	pipe(fds);
	pid_t pid = fork();
	if (pid == 0) {//父进程
		write(fds[1], str1, sizeof(str1));
		sleep(2);//这个延时是为了确保子进程读取了管道的数据，不然下一行会读取自己管道的数据
		read(fds[0], buf, BUF_SIZE);//如果没有上面的延时，那么子进程读取不到数据，自然无法write，这里会堵塞
		printf("Child proc output: %s\n", buf);
	}
	else {//子进程
		read(fds[0], buf, BUF_SIZE);
		printf("Parent proc output: %s \n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}
}