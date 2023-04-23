#include<iostream>
#include<unistd.h>
#include<sys/wait.h>

int main() {
	int status{};
	pid_t pid = fork();
	if (pid == 0)return 3;//如果是子进程就直接return
	else {
		printf("Child PID: %d\n", pid);
		pid = fork();
		if (pid == 0)exit(7);//如果是子进程就直接exit
		else {
			printf("Child PID: %d\n", pid);
			wait(&status);//调用wait函数。之前终止的子进程相关信息将保存到status变量，同时相关子进程被完全销毁
			if (WIFEXITED(status))
				printf("Child send one: %d\n", WEXITSTATUS(status));
			wait(&status);//因为之前创建了2个进程，所以再次调用wait函数和宏
			if (WIFEXITED(status))
				printf("Child send two: %d\n", WEXITSTATUS(status));
			sleep(30);//为暂停父进程终止而插入的代码 此时可以查看子进程的状态
		}
	}
}