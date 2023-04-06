# 第一章

## linux系统下

### `server`

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(const char* message);

int main(int argc,char*argv[]) {
	int serv_sock = 0;
	int clnt_sock = 0;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = 0;
	const char message[] = "Hello World";
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(-1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);//PF_INET等价AF_INET，都是表示IPV4
	if (serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;//定义是哪种地址族 AF_INET表示IPV4
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//ip，INADDR_ANY表示本机IP
	serv_addr.sin_port = htons(atoi(argv[1]));//端口 htons函数可用于将主机字节顺序中的IP端口号转换为网络字节顺序中的IP端口号

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");

	write(clnt_sock, message, sizeof(message));//写入到客户端
	close(clnt_sock);
	close(serv_sock);
}

void error_handling(const char* message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
```

`accept`如果调用成功，那么会返回客户端的文件描述符，我们可以使用它进行通信。如果客户端没有发出通信请求，那么会一直堵塞

### `client`

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(const char* message);

int main(int argc, char* argv[]) {
	int sock = 0;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len = 0;
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(-1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error!");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;//地址族
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//IP
	serv_addr.sin_port = htons(atoi(argv[2]));//端口

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	str_len = read(sock, message, sizeof(message) - 1);
	if (str_len == -1)
		error_handling("read() error!");
	printf("Message from server: %s \n", message);
	close(sock);
}

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
```

假设你已经有了一个sockaddr_in结构体ina，你有一个IP地址"132.241.5.10" 要储存

> 在其中，你就要用到函数**`inet_addr(),`**将IP地址从 点数格式转换成无符号长整型。
>
> 使用方法如下：
> `ina.sin_addr.s_addr = inet_addr("132.241.5.10");`



## Windows下

### `server`

```cpp
#include<WinSock2.h>
#include<iostream>

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr = 0;
	const char message[] = "hello Word!";
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");
	send(hClntSock, message, sizeof(message), 0);
	closesocket(hClntSock);
	closesocket(hServSock);
}
```

### `client`

```cpp
#include<WinSock2.h>
#include<iostream>

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	char message[30];
	int strlen = 0;
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(-1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//初始化Winsock库
		ErrorHandling("WSAStartup() error!");
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	strlen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (strlen == -1)
		ErrorHandling("read() error!");
	printf("Message from server: %s \n", message);
	closesocket(hSocket);
	WSACleanup();//注销先前初始化的Winsocket库
}
```

> 1. [套接字](https://so.csdn.net/so/search?q=套接字&spm=1001.2101.3001.7020)在网络编程中的作用是什么？为什么称它为套接字？
>
>    P2，网络编程就是编写程序让两台联网的计算机相互交换数据。在我们不需要考虑物理连接的情况下，我们只需要考虑如何编写传输软件。操作系统提供了名为“套接字”，套接字是网络传输传输用的软件设备
>
>    [socket](https://so.csdn.net/so/search?q=socket&spm=1001.2101.3001.7020)英文原意是插座：我们把插头插到插座上就能从电网获得电力供给，同样，为了与远程计算机进行数据传输，需要连接到Internet,而变成中的“套接字”就是用来连接该网络的工具
>
> 2. 在服务器端创建套接字后，会依次调用listen函数和accept函数。请比较并说明两者作用
>
>    listen:将套接字转为可接受连接方式
>
>    accept:受理连接请求，**并且在没有连接请求的情况调用该函数，不会返回**。直到有连接请求为止。二者存在逻辑上的先后关系
>
> 3. Linux中，对套接字数据进行I/O时可以直接使用I/O相关函数；而在Windows中则不可以。原因为何？
>
>    Linux把套接字也看作是文件，所以可以用文件I/O相关函数；而Windows要区分套接字和文件，所以设置了特殊的函数
>
> 4. 创建套接字后一般会给它分配地址，为什么？为了完成地址分配需要调用哪些函数？
>
>    要在网络上区分来自不同机器的套接字，所以需要地址信息。分配地址是通过`bind()`函数实现
>
> 5. Linux中的文件描述符与Windows的句柄实际上非常类似。请以套接字为对象说明他们的含义。
>
>    Linux的文件描述符是为了区分指定文件而赋予文件的整数值（相当于编号）。Windows的文件描述符其实也是套接字的整数值，其目的也是区分指定套接字。
>
> 6. 底层文件I/O函数与ANSI标准定义的文件I/O函数之间有何区别？
>
>    ANSI标准定义的输入、输出函数是与操作系统（内核）无关的以C标准写成的函数；相反，底层文件I/O函数是**操作系统直接提供的**。
>
> 7. 参考本书给出的示例low_open.c和low_read.c,分别利用底层文件I/O和ANSI标准I/O编写文件复制程序。可任意指定复制程序的使用方法
