/*************************************************************************
  > File Name: epoll_test.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 04 May 2019 11:12:23 AM CST
 ************************************************************************/

#include <iostream>
#include <cstdlib>
using namespace std;

#include <sys/epoll.h>

/*!
 * 创建一个 TCP 服务器。客户端连接并且输入
 */
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
        if(!fork())
        {
            n = recv(connect_fd, buff, MAXLINE, 0);
            buff[n] = '\0';
            printf("recv msg from client:%s\n", buff);
            if (n <= 0)
            {
                fprintf(stderr, "recv return value is %d, errno is %d", n, errno);
                close(connect_fd);
                continue;
            }
            close(connect_fd);
            exit(0);
        }
        close(connect_fd);
    }
    close(socket_fd);
    int fd_epoll = epoll_create(10);
    std::cout << "fd_epoll " << fd_epoll << std::endl;
    if (fd_epoll < 0) {
        std::cerr << "epoll_create failed" << std::endl;
        return;
    }
    const int MAX_EVENTS = 10;
    epoll_event ev, events[MAX_EVENTS];
    int listen_sock;
    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        std::cerr << "epoll_ctl failed" << std::endl;
        return;
    }
    while (true) {
        int nfds = epoll_wait(fd_epoll, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            std::cerr << "epoll_wait failed" << std::endl;
            return;
        }
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) {
                int conn_sock = accept(listen_sock, )
            }
        }
    }
}

int main(int argc, char* argv[])
{ 
    if (argc < 2) {
        std::cerr << "Usage: exe idx" << std::endl;
        return -1;
    }
    int idx = atoi(argv[1]);
    switch (idx) {
        case 1:
            test1();
            break;
        default:
            std::cerr << "idx " << idx << " is invalid" << std::endl;
            return -1;
    }
    return 0;
}
