#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>

int main() {
	struct sockaddr_in addr1, addr2;
	char* str_ptr;
	char str_arr[20];

	addr1.sin_addr.s_addr = htonl(0X1020304);
	addr2.sin_addr.s_addr = htonl(0X1010101);

	str_ptr = inet_ntoa(addr1.sin_addr);
	strcpy(str_arr, str_ptr);
	printf("Dotted-Decimal notation1: %s \n", str_ptr);

	inet_ntoa(addr2.sin_addr);
	printf("DOtted-Decimal notation2: %s \n", str_ptr);
	printf("Dotted-Decimal notation3: %s \n", str_arr);
}
//inet_aton此函数可以把网络字节序整数型IP地址转换成我们熟悉的字符串形式 和inet_aton那些相反
//inet_nota这个函数的设计就很怪，它返回一个char*的指针，指向一块buffer，它是一直存在，不需要我们维护的
//函数调用后的返回会自动更改其buffer的内容，所以只需要接一次返回值就行