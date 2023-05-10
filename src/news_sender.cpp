#include<iostream>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
constexpr size_t BUF_SIZE{1024};
constexpr size_t TTL {64};

void error_handling(const char*message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
int main(int argc,char*argv[]){
	if(argc!=3){
		printf("Usage : %s <GroupIp> <PORT>\n",argv[0]);
		exit(1);
	}
	int send_sock = socket(PF_INET,SOCK_DGRAM,0);//多播数据通信是通过UDP完成的，因此创建UDP套接字
	sockaddr_in mul_adr{};
	mul_adr.sin_family = AF_INET;
	mul_adr.sin_addr.s_addr = inet_addr(argv[1]);//IP设置为多播地址
	mul_adr.sin_port = htons(static_cast<short>(atoi(argv[2])));

	int time_live = TTL;
	//指定套接字TTL信息 TTL的作用是限制IP数据包在计算机网络中的存在的时间。TTL的最大值是255，TTL的一个推荐值是64
	setsockopt(send_sock,IPPROTO_IP,IP_MULTICAST_TTL,(void*)&time_live,sizeof(time_live));

	FILE*fp{};
	if((fp = fopen("news.txt","r"))==nullptr)
		error_handling("fopen() error");
	char buf[BUF_SIZE]{};

	while(!feof(fp)){
		fgets(buf,BUF_SIZE,fp);//读取文件的内容写入buf
		puts(buf);
		//别忘记了，UDP是如果没有使用connect()，那么发送数据使用sendto()。发送先前从文件中读取到的buf的数据
		sendto(send_sock,buf,BUF_SIZE,0,(sockaddr*)&mul_adr,sizeof(mul_adr));
		sleep(2);//时间隔离，没特殊含义
	}
	fclose(fp);
	close(send_sock);
}
