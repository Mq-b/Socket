#include<iostream>
#include<string.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
constexpr size_t BUF_SIZE{ 1024 };

void ErrorHandling(const char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
        exit(1);
    }

    WSADATA wsaData{};
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error");

    int recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    sockaddr_in adr{};
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(static_cast<short>(atoi(argv[2])));

    if (bind(recv_sock, reinterpret_cast<sockaddr*>(&adr), sizeof(adr)) == -1)
        ErrorHandling("bind() error");

    ip_mreq join_adr{};
    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);//初始化多组播地址
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) & join_adr, sizeof(join_adr));
    char buf[BUF_SIZE]{};
    while (1) {
        int str_len = recvfrom(recv_sock, buf, BUF_SIZE, 0, nullptr, 0);//读
        if (str_len < 0)
            break;
        std::cout << buf << std::endl;//如果你把这行换成puts，你和发送端运行的时候会惊奇的发现，咦，接收端怎么少打印了一行 
        //linux下标准输出好像是行缓冲，加个flush或者输出'\n'才会打印出来
    }
    closesocket(recv_sock);
}