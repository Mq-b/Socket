#include<WinSock2.h>
#include<iostream>
#include<fstream>

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr = 0;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	std::ifstream ifs{ "E:\\常用\\TCP_IP网络编程.md", std::ios::binary | std::ios::in };
	if (!ifs.is_open()){
		std::cerr << "文件打开失败" << std::endl;
		exit(1);
	}
	char* buffer = new char[1024];
	int len = 1024;
	while (!ifs.eof()) {
		len = ifs.read(buffer, 1024).gcount();//gcount返回最近的无格式输入操作所提取的字符数，这很重要，能防止多写多度
		send(hClntSock, buffer, len, 0);
	}
	ifs.close();
	delete[] buffer;
	closesocket(hClntSock);
	closesocket(hServSock);
}