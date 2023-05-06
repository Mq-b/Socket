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
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	char buf[BUF_SIZE]{};

	int acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in acpt_adr{};
	 acpt_adr.sin_family = AF_INET;
	 acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	 acpt_adr.sin_port = htons(atoi(argv[1]));

	 if (bind(acpt_sock, (sockaddr*)&acpt_adr, sizeof(acpt_adr)) == -1)
		 error_handling("bind() error");
	 listen(acpt_sock, 5);

	 sockaddr_in recv_adr{};
	 socklen_t recv_adr_sz = sizeof(recv_adr);
	 int recv_sock = accept(acpt_sock, (sockaddr*)&recv_adr, &recv_adr_sz);

	 ssize_t str_len{};
	 while (1) {
		 str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);//重点！！
		 if (str_len > 0)
			 break;
	 }
	 printf("Buffering %d bytes: %s\n", str_len, buf);

	 str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
	 printf("Read again: %s\n", buf);

	 close(acpt_sock);
	 close(recv_sock);
}

//peek_send客户端只是发送了一次数据，我们却调用的两次recv读取，因为设置了MSG_PEEK调用recv函数时，即使读取了输入
//缓冲的数据也不会被删除。我们这里是MSG_PEEK | MSG_DONTWAIT，合作使用，MSG_DONTWAIT表示的是非阻塞IO调用