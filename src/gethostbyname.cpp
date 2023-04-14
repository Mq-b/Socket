#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>
void error_handling(const char* message) {
	fputs(message,stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv[]) {
	if (argc != 2) {
		printf("Usage : %s <addr>\n", argv[0]);
		exit(1);
	}
	hostent* host = gethostbyname(argv[1]);//假设传入的是www.baidu.com
	if (!host)
		error_handling("gethost... error");

	printf("Official name: %s \n", host->h_name);//输出官方域名，www.a.shifen.com不一定会使用
	for (int i = 0; host->h_aliases[i]; ++i) {
		printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);//输出别名www.baidu.com
	}
	for (int i = 0; host->h_addr_list[i]; ++i) {
		printf("IP addr %d: %s \n", i + 1, inet_ntoa(*(in_addr*)host->h_addr_list[i]));//输出IP地址，可能有多个，如百度的话是
		//IP addr 1: 36.152.44.95
		//IP addr 2 : 36.152.44.96
	}
}