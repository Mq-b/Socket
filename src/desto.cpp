#include<iostream>
#include<fcntl.h>

int main(){
    int fd = open("data.dat",O_WRONLY | O_CREAT | O_TRUNC);//创建文件写，截断0
    if(fd==-1){
        fputs("file open error",stdout);
        return -1;
    }
    FILE*fp = fdopen(fd,"w");//将文件描述符转换为FILE*
    fputs("笑死了惹🤣🤣🤣🤣\n",fp);
    fclose(fp);
}
//打开文件查看记得给个 chmod 777，增加读写权限