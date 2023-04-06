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
	sockaddr_in serv_adr, from_adr;
	socklen_t adr_sz = 0;
	if (argc != 3) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("UDP socket creation error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	while (1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		sendto(sock, message, strlen(message), 0, (sockaddr*)&serv_adr, sizeof(serv_adr));
		adr_sz = sizeof(from_adr);
		auto str_len = recvfrom(sock, message, BUF_SIZE, 0, (sockaddr*)&from_adr, &adr_sz);
		printf("Message from server: %s", message);
	}
}