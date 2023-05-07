#include<iostream>
#include<sys/uio.h>

int main() {
	iovec vec[2]{};
	char buf1[] = "ABCDEFG";
	char buf2[] = "1234567";
	vec[0].iov_base = buf1;
	vec[0].iov_len = 3;
	vec[1].iov_base = buf2;
	vec[1].iov_len = 4;

	ssize_t str_len = writev(1, vec, 2);//一次性向终端输出 ABC1234
	puts("");
	printf("Write bytes: %d \n", str_len);
}