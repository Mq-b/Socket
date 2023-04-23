#include<iostream>
#include<unistd.h>

int main() {
	pid_t pid = fork();
	if (pid == 0)//子进程
		puts("Hi, I am a child process");
	else {//父进程，返回值是子进程ID，所以打印
		printf("Child process ID: %d\n", pid);
		sleep(30);//父进程延时30秒，子进程会先于父进程退出，成为僵尸进程，直到父进程结束进行回收
	}
	if (pid == 0)
		puts("End child process");
	else
		puts("End parent process");
}