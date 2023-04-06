#include<stdio.h>
#include<arpa/inet.h>

//和书上结果一样 函数将IP地址字符串转换为32位整数型，转换为网络字节序
int main() {
	const char* addr1 = "1.2.3.4";
	const char* addr2 = "1.2.3.256";
	unsigned long conv_addr = inet_addr(addr1);
	if (conv_addr == INADDR_NONE)
		printf("Error occured!\n");
	else
		printf("Network ordered integer addr: %#lx\n", conv_addr);

	conv_addr = inet_addr(addr2);
	if (conv_addr == INADDR_NONE)
		printf("Error occureded\n");
	else
		printf("Network ordered integer addr: %#lx \n", conv_addr);
}