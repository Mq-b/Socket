#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
	int snd_buf{};
	int recv_buf{};
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	socklen_t len = sizeof(snd_buf);
	int state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &len);
	if (state)
		error_handling("getsockopt() error");
	len = sizeof(recv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recv_buf, &len);//注意第三个参数
	if (state)
		error_handling("getsockopt() error");

	printf("Input buffer size: %d\n", recv_buf);
	printf("Output buffer size: %d\n", snd_buf);
}