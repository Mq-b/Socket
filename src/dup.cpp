#include<stdio.h>
#include<unistd.h>

int main(){
    char str1[]="Hi~\n";
    char str2[]="It`s nice day~\n";

    int cdf1 = dup(1);//复制文件描述符
    int cdf2 = dup2(cdf1,7);//复制文件描述符并指定值为7
    
    printf("fd1=%d，fd2=%d\n",cdf1,cdf2);//3 7
    write(cdf1,str1,sizeof(str1));
    write(cdf2,str2,sizeof(str2));

    close(cdf1);
    close(cdf2);
    write(1,str1,sizeof(str1));
    close(1);
    write(1,str2,sizeof(str2));//无法输出，所有文件描述符都被关闭
}