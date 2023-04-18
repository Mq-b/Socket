#include<iostream>
#include<WinSock2.h>

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	WSADATA waData;
	SOCKADDR_IN addr{};
	if (argc != 2) {
		printf("Usage : %s <addr>\n", argv[0]);
	}
	if (WSAStartup(MAKEWORD(2, 2), &waData) != 0)
		ErrorHandling("WSAStartup() error!");
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	hostent* host = gethostbyaddr((char*) & addr.sin_addr, 4, AF_INET);//通过域名获取网络IP信息
	if (!host) {
		ErrorHandling("gethost... error");
	}

	printf("Official name: %s \n", host->h_name);
	for (int i = 0; host->h_aliases[i]; ++i)
		printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);
	printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");//打印协议类型，是IPv4还是IPv6
	for (int i = 0; host->h_addr_list[i]; ++i)
		printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(in_addr*)host->h_addr_list[i]));//打印IP
}
//和linux一样，需要更改hosts文件 假设添加36.152.44.95    www.baidu.com