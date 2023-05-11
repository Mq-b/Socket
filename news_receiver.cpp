#include<iostream>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
constexpr size_t BUF_SIZE{1024};

void error_handling(const char*message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
int main(int argc,char*argv[]){
    if(argc!=2){
        printf("Usage : %s <GroupIP> <PORT>\n",argv[0]);
        exit(1);
    }

    int recv_sock = socket(PF_INET,SOCK_DGRAM,0);
    sockaddr_in adr{};
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(static_cast<short>(atoi(argv[1])));
    //光播不需要ip_mreq 加入多播，所以代码会少一些
    if(bind(recv_sock,reinterpret_cast<sockaddr*>(&adr),sizeof(adr))==-1)
        error_handling("bind() error");

    char buf[BUF_SIZE]{};
    while(1){
        int str_len = recvfrom(recv_sock,buf,BUF_SIZE,0,nullptr,0);//读
        if(str_len<0)
            break;
        std::cout<<buf<<std::endl;//如果你把这行换成puts，你和发送端运行的时候会惊奇的发现，咦，接收端怎么少打印了一行 
                                  //linux下标准输出好像是行缓冲，加个flush或者输出'\n'才会打印出来
    }
    close(recv_sock);
}