#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handle(std::string opt,std::string message)
{
    perror(opt.c_str());
    std::cout << message << std::endl;
    exit(1);
}

int main(int argc,char * argv[])
{
    int serv_sock;
    int client_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;

    socklen_t client_addr_size;

    char message[] = "hello world";

    if(argc < 2)
    {
        std::cout <<"Usage : " <<argv[0] << " <port>" << std::endl;
        exit(1);
    }

    //基于 IPv4 地址族（PF_INET）和 TCP 协议（SOCK_STREAM）
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock < 0)
    {
        error_handle("sock","socket() error.");
    }

    memset(&serv_addr,0,sizeof(serv_addr));

    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//选择当前任意网卡
    serv_addr.sin_port = htons(atoi(argv[1]));//设置接收消息的端口号

    if(bind(serv_sock,(struct sockaddr * ) & serv_addr , sizeof(serv_addr)) < 0)
    {
        error_handle("bind","bind() error.");
    }

    if(listen(serv_sock,5) < 0)
    {
        error_handle("listen", "listen() error.");
    }

    //打印输出等待连接
    std::cout << "Waiting Client...." << std::endl;
    client_addr_size = sizeof(client_addr);
    //等待接收客户端建立连接
    client_sock = accept(serv_sock, (struct sockaddr*)&client_sock, &client_addr_size);
    if(client_sock < 0)
    {
        error_handle("accept", "accept() error.");
    }

    std::cout << "Client IP : "<< inet_ntoa(client_addr.sin_addr) << " port : " << ntohs(client_addr.sin_port) << std::endl;

    write(client_sock,message,sizeof(message));

    close(client_sock);
    close(serv_sock);

    return 0;

}