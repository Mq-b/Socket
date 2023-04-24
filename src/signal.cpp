#include<iostream>
#include<unistd.h>
#include<signal.h>

void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");
	alarm(2);//为了每隔两秒重复产生SIGALRM信号，在信号处理器中调用alarm函数
}
void keycontrol(int sig) {
	if (sig == SIGINT)
		puts("CTRL+C pressed");
}

int main() {
	//注册SIGALRM(已到通过调用alarm函数注册的时间) SIGINT(CTRL+C)信号和相应的处理函数
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);
	for (int i{}; i < 3; ++i) {
		puts("wait...");
		sleep(100);
	}
}
//：alarm()函数的主要功能是设置信号传送闹钟，即用来设置信号SIGALRM在经过参数seconds秒数后发送给目前的进程。如果未设置信号SIGALARM的处理函数，那么alarm()默认处理终止进程

/*调用函数的主体的确是操作系统，但进程处于睡眠状态时无法调用函数。 因此，产生信号时， 为了调用信号处理器，将唤醒由于调用sleep函数而进入
阻塞状态的进程。 而且，进程一旦被唤醒， 就不会再进入睡眠状态。 即使还未到sleep函数中规定的时间也是如此。 所以，上述示例运行不到
10秒就会结束，连续输入CTRL+C则有可能1秒都不到*/