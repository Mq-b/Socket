#include<iostream>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	int sock = 0;
	struct sockaddr_in serv_addr;
	int str_len = 0;
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(-1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error!");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;//地址族
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//IP
	serv_addr.sin_port = htons(atoi(argv[2]));//端口

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	printf("Connected.........\n");
	int count = 0;
	printf("Operand count: ");std::cin >> count;
	char t = static_cast<char>(count);
	write(sock, &t, 1);//写入服务器要传递数字个数
	int* Buffer = new int[count] {};
	for (int i = 0, t = 0; i < count; ++i) {
		printf("Operand %d: ", i + 1); std::cin >> t;
		Buffer[i] = t;
	}
	char Operator{};
	int result = 0;
	printf("Operator: "); std::cin >> Operator;
	write(sock, Buffer, count * 4);
	write(sock, &Operator, 1);
	str_len = read(sock, &result, sizeof(result));
	if (str_len == -1)
		error_handling("read() error!");
	printf("Operation result: %d\n", result);
	close(sock);
	delete[] Buffer;
}