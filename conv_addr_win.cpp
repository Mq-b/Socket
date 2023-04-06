#include<iostream>
#include<WinSock2.h>

int main() {
	char strAddr[] = "203.211.218.102:9190";
	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	WSADATA wsaData;
	(void)WSAStartup(MAKEWORD(2, 2), &wsaData);

	int size = sizeof(servAddr);
	WSAStringToAddress(strAddr,AF_INET,NULL,(SOCKADDR*)&servAddr, &size);
	size = sizeof(strAddrBuf);
	WSAAddressToString((SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf,  (LPDWORD) & size);
	printf("Second conv result: %s \n", strAddrBuf);
}