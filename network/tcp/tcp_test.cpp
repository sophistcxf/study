/*************************************************************************
  > File Name: tcp_test.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 04 May 2019 03:41:03 PM CST
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
using namespace std;

extern int errno;
#define DEFAULT_PORT 8002
#define MAXLINE 4096

void test1()
{
    setlinebuf(stdout);
    int    socket_fd, connect_fd;
    struct sockaddr_in servaddr;
    char    buff[4096];
    int     n;
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(DEFAULT_PORT);

    //将本地地址绑定到所创建的套接字上
    if(bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    //开始监听是否有客户端连接
    if(listen(socket_fd, 10) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("======waiting for client's request, port is %d ======\n", DEFAULT_PORT);
    int count = 0;
    while(1)
    {
        if((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1)
        {
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        printf("recv connect[%d], connect_fd is %d\n", count++, connect_fd);
        pid_t pid = fork();
        if(pid == 0)
        {
            /* 子进程从连接描述符中读取内容 */
            while ((n = recv(connect_fd, buff, MAXLINE, 0)) > 0) { 
                buff[n] = '\0';
                printf("recv msg from client:%s\n", buff);
            }
            close(connect_fd);
            exit(0);
        }
        else {
            std::cout << "create child process " << pid << std::endl;
            /* 父进程关闭掉 connect_fd，由子进程处理此连接的内容 */
            close(connect_fd);
        }
    }
    close(socket_fd);
}

int main()
{
    test1();
    return 0;
}
