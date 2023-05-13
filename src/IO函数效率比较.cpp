#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<chrono>
constexpr size_t BUF_SIZE{3};

std::chrono::duration<double> timer(void(f)()) {
	auto t1{ std::chrono::steady_clock::now() };
	f();
	auto t2{ std::chrono::steady_clock::now() };
	std::chrono::duration<double> secs{ t2 - t1 };
	return secs;
}

void syscpy(){
    char buf[BUF_SIZE]{};
    int fd1 = open("视频181MB.mp4",O_RDONLY);
    int fd2 = open("cpy.mp4",O_WRONLY | O_CREAT | O_TRUNC);
    
    int len {};
    while((len = read(fd1,buf,sizeof(buf)))>0){
        write(fd2,buf,len);
    }
    close(fd1);
    close(fd2);
}
void stdcpy(){
    FILE*fp1{},*fp2{};
    char buf[BUF_SIZE]{};
    fp1 = fopen("视频181MB.mp4","r");
    fp2 = fopen("cpy2.mp4","w");
    while(fgets(buf,BUF_SIZE,fp1)!=nullptr)
        fputs(buf,fp2);
    fclose(fp1);
    fclose(fp2);
}

int main(){
    auto t = timer(syscpy);
    std::cout<<"系统IO函数耗时: "<<t.count() <<"s\n";//13s
    auto t2 = timer(stdcpy);
    std::cout<<"标准IO函数耗时: "<<t2.count() <<"s\n";//3s
}