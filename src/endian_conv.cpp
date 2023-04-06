#include<stdio.h>
#ifdef  WIN32
#include <WinSock2.h>
#else
#include<arpa/inet.h>
#endif

int main() {
	unsigned short host_port = 0X1234;
	unsigned short net_port;
	unsigned long host_addr = 0X12345678;
	unsigned long net_addr;

	//结果和书上一样，不管是linux还是Windows，Intel和AMD的CPU都采用小端序标准
	net_port = htons(host_port);
	net_addr = htonl(host_addr);
	printf("%#x\n", host_port);
	printf("%#x\n", net_port);
	printf("%#x\n", host_addr);
	printf("%#x\n", net_addr);
}