#include<iostream>
#include<sys/uio.h>
constexpr size_t BUF_SIZE{ 100 };

int main() {
	iovec vec[2]{};
	char buf1[BUF_SIZE]{};
	char buf2[BUF_SIZE]{};

	vec[0].iov_base = buf1;
	vec[0].iov_len = 5;
	vec[1].iov_base = buf2;
	vec[1].iov_len = BUF_SIZE;

	ssize_t str_len = readv(0, vec, 2);//读取标准输入，所以会在这里堵塞等待输入
	printf("Read bytes: %d \n", str_len);
	printf("First message: %s\n", buf1);
	printf("Second message: %s \n", buf2);
}