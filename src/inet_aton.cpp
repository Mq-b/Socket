#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
//和书上结果完全一模一样 inet_aton和inet_addr功能完全相同，但是使用不一样
int main() {
	const char* addr = "127.232.124.79";
	struct sockaddr_in addr_inet;
	if (!inet_aton(addr, &addr_inet.sin_addr))
		error_handling("Conversion error");
	else
		printf("Network ordered integer addr: %#x\n", addr_inet.sin_addr.s_addr);
}