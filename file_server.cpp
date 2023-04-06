#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<fstream>

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv[]) {
	int serv_sock = 0;
	int clnt_sock = 0;
	struct sockaddr_in serv_addr, clnt_addr;
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(-1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);//PF_INET等价AF_INET，都是表示IPV4
	if (serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;//定义是哪种地址族 AF_INET表示IPV4
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//ip，INADDR_ANY表示本机IP
	serv_addr.sin_port = htons(atoi(argv[1]));//端口 htons函数可用于将主机字节顺序中的IP端口号转换为网络字节顺序中的IP端口号

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	char filename[64]{};
	read(clnt_sock, filename, 64);//读取客户端传入的文件名
	std::ifstream ifs{ filename, std::ios::binary | std::ios::in };
	if (!ifs.is_open()) {
		std::cerr << "文件不存在" << std::endl;
		close(clnt_sock);
		close(serv_sock);
		exit(1);
	}
	char* buffer = new char[1024];
	int len = 1024;
	while (!ifs.eof()) {
		len = ifs.read(buffer, 1024).gcount();//gcount返回最近的无格式输入操作所提取的字符数，这很重要，能防止多写多读
		write(clnt_sock, buffer, len);
	}
	ifs.close();
	delete[] buffer;

	close(clnt_sock);
	close(serv_sock);
}