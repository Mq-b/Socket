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
int main(int argc, char* argv[]) {
	char message[BUF_SIZE]{};
	sockaddr_in my_adr, your_adr;
	socklen_t adr_sz = 0;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	memset(&my_adr, 0, sizeof(my_adr));
	my_adr.sin_family = AF_INET;
	my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_adr.sin_port = htons(atoi(argv[1]));
	
	if (bind(sock, (sockaddr*)&my_adr, sizeof(my_adr)) == -1)
		error_handling("bind() error");
	for (int i = 0; i < 3; ++i) {
		sleep(5);
		adr_sz = sizeof(your_adr);
		socklen_t str_len = recvfrom(sock, message, BUF_SIZE, 0, (sockaddr*)&my_adr, &adr_sz);
		printf("Message %d: %s \n", i + 1, message);
	}
	close(sock);
}