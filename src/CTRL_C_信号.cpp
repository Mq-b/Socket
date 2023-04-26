#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<string.h>

void ctrl_handler(int sig){
	char buf[30]{};
	while (1) {
		printf("\n你确定要退出程序吗？Y/N \n");
		fgets(buf, 30, stdin);
		if (!strcmp(buf, "Y\n"))exit(0);
		else return;
	}
}
int main() {
	struct sigaction act {};
	act.sa_handler = ctrl_handler;
	sigaction(SIGINT, &act, 0);

	while (1) { 	
		puts("🤣🤣"); 
		sleep(1);
	}
}
//signal(SIGINT, ctrl_handler);也可行