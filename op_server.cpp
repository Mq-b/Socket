#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <string.h>
#include<vector>

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int operation(char o, std::vector<int>& v) {
	if (o == '+') {
		int result = 0;
		for (const auto & i:v) {
			result += i;
		}
		return result;
	}
	else if (o == '-') {
		int result = 0;
		for (int i = 0; i < v.size() - 1; ++i) {
			result = v[i] - v[i + 1];
			v[i + 1] = result;
		}
		return v[v.size() - 1];
	}
	else if (o == '*') {
		int result = 1;
		for (const auto& i : v) {
			result *= i;
		}
		return result;
	}
	else if (o == '/') {
		int result = 0;
		for (int i = 0; i < v.size() - 1; ++i) {
			result = v[i] / v[i + 1];
			v[i + 1] = result;
		}
		return v[v.size() - 1];
	}
	else {
		return 0;
	}
}
int main(int argc, char* argv[]) {
	int serv_sock = 0;
	int clnt_sock = 0;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = 0;
	//char message[] = "请输入";
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(-1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);//PF_INET等价AF_INET，都是表示IPV4
	if (serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock == -1)
		error_handling("accept() error");

	char size{};//存储客户端要传递数字个数
	char operator_{};//存储运算符
	read(clnt_sock, &size, 1);//读取一字节，即传递数字个数
	std::vector<int>v(static_cast<size_t>(size));//vector用于存储客户端传入用于运算的整数
	for (int i = 0, tmp = 0; i < size; ++i) {//循环读取客户端传入的数值
		read(clnt_sock, &tmp, sizeof(tmp));
		v[i] = tmp;
	}
	read(clnt_sock, &operator_, 1);//读取运算符
	int value = operation(operator_, v);//运算结果
	write(clnt_sock, &value, sizeof(value));//写入到客户端

	close(clnt_sock);
	close(serv_sock);
}