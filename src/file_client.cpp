#include<iostream>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<fstream>

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	struct sockaddr_in serv_addr;
	char filename[64]{};
	char buffer[1024]{};
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(-1);
	}
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error!");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;//地址族
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//IP
	serv_addr.sin_port = htons(atoi(argv[2]));//端口

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	printf("请输入需要的文件名: "); std::cin >> filename;
	int str_len = write(sock, filename, sizeof(filename));
	if (str_len == -1)
		error_handling("read() error!");

	std::ofstream ofs{ filename,std::ios::binary | std::ios::out };
	int strlen = 1024;
	while (strlen != 0) {
		strlen = read(sock, buffer, strlen);
		ofs.write(buffer, strlen);
	}
	if (strlen == -1)
		error_handling("read() error!");
	ofs.close();
	close(sock);
}