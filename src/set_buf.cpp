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
	int snd_buf{ 1024 * 3 }, rcv_buf{ 1024 * 3 };
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	//设置IO缓冲为3M
	int state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, sizeof(rcv_buf));
	if (state)
		error_handling("setsockopt() error!");
	state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, sizeof(snd_buf));
	if (state)
		error_handling("setsockopt() error!");
	//获取IO缓冲
	unsigned len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &len);
	if (state)
		error_handling("setsockopt() error!");
	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, &len);
	printf("Input buffer size: %d\n", rcv_buf);//6144
	printf("Output buffer size: %d\n", snd_buf);//6144 正常现象
}