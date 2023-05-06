#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
constexpr size_t BUF_SIZE{ 30 };

void error_handling(const char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc,char*argv[]) {
    sockaddr_in recv_adr{};
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
        error_handling("connect() error!");

    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);
}