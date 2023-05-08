#include<iostream>
#include<stdlib.h>
#include<WinSock2.h>
constexpr size_t BUF_SIZE{ 30 };

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	char buf[BUF_SIZE]{};
	WSADATA wsaData{};
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	SOCKET hAcptSock{}, hRecvSock{};
	hAcptSock = socket(PF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN recvAdr{};
	recvAdr.sin_family = AF_INET;
	recvAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	recvAdr.sin_port = htons(static_cast<short>(atoi(argv[1])));

	if (bind(hAcptSock, (SOCKADDR*)&recvAdr, sizeof(recvAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hAcptSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	SOCKADDR_IN sendAdr{};
	int sendAdrSize = sizeof(sendAdr);
	hRecvSock = accept(hAcptSock, (SOCKADDR*)&sendAdr, &sendAdrSize);
	
	fd_set read{}, except{}, readCopy{}, exceptCopy{};
	timeval timeout{};

	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET(hRecvSock, &read);
	FD_SET(hRecvSock, &except);
	int str_len{};

	while (1) {
		readCopy = read;
		exceptCopy = except;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		int result = select(0, &readCopy, 0, &exceptCopy, &timeout);//select检查readCopy是否可读，exceptCopy是检查错误，timeout是时间

		if (result > 0) {
			if (FD_ISSET(hRecvSock, &exceptCopy)) {//检测hRecvSock在exceptCopy集合中的状态是否变化，当检测到hRecvSock状态发生变化(出现MSG_OOB)时返回真，否则，返回假
				str_len = recv(hRecvSock, buf, BUF_SIZE - 1, MSG_OOB);//这个分支是MSG_OOB
				printf("Urgen message: %s \n", buf);
			}
			if (FD_ISSET(hRecvSock, &readCopy)) {
				str_len = recv(hRecvSock, buf, BUF_SIZE - 1, 0);
				if (str_len == 0) {
					break;
					closesocket(hRecvSock);
				}
				else {//这个分支就是普通的对面发送信息来了
					puts(buf);
				}
			}
		}
	}
	closesocket(hAcptSock);
	WSACleanup();
}

/*和客户端配合使用，结果是:
123
Urgen message: 403
56789
*/
/*[in] nfds
已忽略。 仅包含 nfds 参数，以便与 Berkeley 套接字兼容。

[in, out] readfds
一个可选指针，指向一组要检查的套接字是否可读。

[in, out] writefds
一个可选指针，指向要检查的一组套接字是否可写。

[in, out] exceptfds
指向一组要检查错误的套接字的可选指针。

[in] timeout
选择等待的最大时间，以 TIMEVAL 结构的形式提供。 将 超时 参数设置为 null 以阻止操作*/