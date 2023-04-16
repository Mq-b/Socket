#include<iostream>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
	sockaddr_in addr{};
	if (argc != 2) {
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}
	addr.sin_addr.s_addr = inet_addr("36.152.44.95");
	hostent* host = gethostbyaddr((char*) & addr.sin_addr, 4, AF_INET);//将IPv4地址转换为域名
	if (!host)
		error_handling("gethost... error");

	printf("Official name: %s \n", host->h_name);//打印域名
	for (int i = 0; host->h_aliases[i]; ++i)
		printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);
	printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");//打印协议类型，是IPv4还是IPv6
	for (int i = 0; host->h_addr_list[i]; ++i)
		printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(in_addr*)host->h_addr_list[i]));//打印IP
}
//一定要注意，更改/etc/hosts文件，假设我们输入的是百度的IP，那么你至少需要手动在这个文件里添加 36.152.44.95    www.baidu.com
//它不会去联网解析的