#include<iostream>
#include<fcntl.h>

int main(){
    int fd = open("data.dat",O_WRONLY | O_CREAT | O_TRUNC);
    if(fd==-1){
        fputs("file open error",stdout);
        return -1;
    }
    printf("First file desctiptor: %d\n",fd);
    FILE*fp = fdopen(fd,"w");//文件描述符转换为FILE*
    fputs("TCP/IP SOCKET PROGRAMMING\n",fp);
    printf("Second file descriptor: %d\n",fileno(fp));//再将FILE*转换为文件描述符
    fclose(fp);
}