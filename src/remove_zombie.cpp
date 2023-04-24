#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

void read_childpro(int sig) {
	int status{};
	pid_t id = waitpid(-1, &status, WNOHANG);//回收子进程，返回回收子进程的ID
	if (WIFEXITED(status)) {
		printf("Remove proc id: %d\n", id);//打印删除的进程ID
		printf("Child send: %d\n", WEXITSTATUS(status));//打印删除的进程的返回值
	}
}
int main() {
	pid_t pid{};
	struct sigaction act {};
	act.sa_handler = read_childpro;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD, &act, 0);//注册SIGCHLD信号的处理函数，若子进程终止则调用第6行中定义的函数
	
	pid = fork();//创建进程
	if (pid == 0) { //子进程
		puts("Hi! I'm child process");
		sleep(10);
		return 12;//子进程结束返回12
	}
	else {			//父进程
		printf("Child proc id: %d\n", pid);//打印创建的第一个子进程PID
		pid = fork();//创建进程 也就是父进程创建了两个子进程
		if (pid == 0) {	//子进程
			puts("Hi! I'm child process");
			sleep(10);
			exit(24);//子进程结束返回24
		}
		else {			//父进程
			printf("Child proc id: %d\n", pid);////打印创建的第二个子进程PID
			for (int i = 0; i < 5; ++i) {
				puts("wait...");
				sleep(5);
			}
		}
	}
}