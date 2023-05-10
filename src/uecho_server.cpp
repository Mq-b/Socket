#include<iostream>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 30

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char*argv[]) {
	socklen_t clnt_adr_sz = 0;
	socklen_t str_len = 0;
	char message[BUF_SIZE]{};
	struct sockaddr_in serv_adr, clnt_adr;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	int serv_sock = socket(PF_INET, SOCK_DGRAM, 0);//创建UDP套接字，看第二个参数
	if (serv_sock == -1)
		error_handling("UDP socket creation error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	while (1) {
		clnt_adr_sz = sizeof(clnt_adr);
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);//读取数据
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);//写入数据
	}
	close(serv_sock);
}