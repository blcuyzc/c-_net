#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//出错调用函数
void error_handle(std::string opt, std::string message)
{
    //根据errno值获取失败原因并打印到终端
    perror(opt.c_str());
    std::cout << message << std::endl;
    exit(1);
}

int main(int argc,char * argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[64];
    int str_len;

    if(argc < 3)
    {
        std::cout << "Usage : " << argv[0] << " <IP> <port>" << std::endl;
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        error_handle("socket", "socket() error.");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        error_handle("connect", "connect() error.");
    }

    str_len = read(sock,message,sizeof(message) - 1);
    if(str_len < 0)
    {
        error_handle("read", "read() error.");
    }

    //将读取到的输出打印出来
    std::cout << "Recv Message : " << message << std::endl;

    //关闭socket 套接字
    close(sock);
    return 0;
}