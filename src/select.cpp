#include<iostream>
#include<unistd.h>
#include<sys/time.h>
#include<sys/select.h>
#define BUF_SIZE 30

int main() {
	fd_set reads{}, temps{};
	char buf[BUF_SIZE]{};
	timeval timeout{};
	int result{};
	FD_SET(0, &reads);//将第一位设置为1，是一位，当然，调试看起来是一位是第一个long元素是1，这是正常的

	while (1) {
		temps = reads;
		//设置等待时间
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		result = select(1, &temps, 0, 0, &timeout);//监视一个文件描述符
		if (result == -1) {
			puts("select() error!");
			break;
		}
		else if (result == 0) {
			puts("Time-out!");
		}
		else {
			if (FD_ISSET(0, &temps)) {
				int str_len = read(0, buf, BUF_SIZE);
				printf("message from console: %s\n", buf);
			}
		}
	}
}
//fd_set保有了一个long 16的数组，即8字节，16个，一字节8位，即8*8*16=1024