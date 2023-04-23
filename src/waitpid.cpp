#include<iostream>
#include<sys/wait.h>
#include<unistd.h>

int main() {
	int status{};
	pid_t pid = fork();
	if (pid == 0) {//子进程
		sleep(15);
		return 24;
	}
	else {
		//参数3为WNOHANG的时候调用waitpid发现没有已退出的子进程可收集，则返回0，也就是会一直循环，直到子进程运行完毕
		while (!waitpid(-1, &status, WNOHANG)) {//-1表示和wait一样等待任意子进程终止 WNOHANG表示即使没有终止子进程也不会进入阻塞状态，而是直接返回0并退出函数
			sleep(3);
			puts("sleep 3sec.");
		}
		if (WIFEXITED(status))//验证子进程是否正确退出
			printf("Child send %d\n", WEXITSTATUS(status));//打印子进程的返回值
	}
}
//根据运行结果我们能知道waitpid不会阻塞