#include<iostream>
#include<unistd.h>
#include<signal.h>
void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");
	alarm(2);
}
int main() {
	struct sigaction act {};
	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask);//将信号集初始化为空
	sigaction(SIGALRM, &act, 0);//注册SIGALRM信号的处理器
	alarm(2);//调用alarm函数预约2秒后发生SIGALRM信号
	for (int i = 0; i < 3; ++i) {
		puts("wait...");
		sleep(100);//为了调用信号处理器将唤醒由于调用sleep函数而进入阻塞状态的进程，所以这里是的sleep是不会等待这么久的
	}
}
//alarm()的作用是设置定时器(闹钟)。在指定seconds后，内核会给当前进程发送14）SIGALRM信号。进程收到该信号，默认动作终止。每个进程都有且只有唯一的一个定时器