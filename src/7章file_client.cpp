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
	char buf[BUF_SIZE]{};
	int read_cnt = 0;
	sockaddr_in serv_adr{};
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	FILE* fp = fopen("receive.dat", "wb");
	int sd = socket(PF_INET, SOCK_STREAM, 0);
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	connect(sd, (sockaddr*)&serv_adr, sizeof(serv_adr));

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		fwrite(buf, 1, read_cnt, fp);

	puts("Received file data");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);
}