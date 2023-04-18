#include<iostream>
#include<WinSock2.h>

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv[]) {
	WSADATA waData;
	if (argc != 2) {
		printf("Usage : %s <addr>\n", argv[0]);
	}
	if (WSAStartup(MAKEWORD(2, 2), &waData) != 0)
		ErrorHandling("WSAStartup() error!");

	hostent* host = gethostbyname(argv[1]);
	if (!host)
		ErrorHandling("gethost... error");

	printf("Official name: %s \n", host->h_name);
	for (int i = 0; host->h_aliases[i]; ++i)
		printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);
	printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");//打印协议类型，是IPv4还是IPv6
	for (int i = 0; host->h_addr_list[i]; ++i)
		printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(in_addr*)host->h_addr_list[i]));
}
/*PS E:\vs src\socket\socket\out\build\x64-debug> .\socket.exe www.baidu.com
Official name: www.a.shifen.com
Aliases 1: www.baidu.com
Address type: AF_INET
IP addr 1: 36.152.44.96
IP addr 2: 36.152.44.95*/
//和linux的一样