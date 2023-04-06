#include<WinSock2.h>
#include<iostream>
#include<fstream>

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	std::ofstream ofs{ "TCP_IP.md",std::ios::binary | std::ios::out };
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	char buffer[1024]{};
	int strlen = 1024;
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(-1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//初始化Winsock库
		ErrorHandling("WSAStartup() error!");
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));//主机字节顺序转换为网络字节序

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	
	while (strlen != 0) {
		strlen = recv(hSocket, buffer, strlen, 0);
		ofs.write(buffer, strlen);
	}
	if (strlen == -1)
		ErrorHandling("read() error!");
	ofs.close();
	closesocket(hSocket);
	WSACleanup();//注销先前初始化的Winsocket库
}