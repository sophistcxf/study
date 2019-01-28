#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#define DEFAULT_PORT 8002
#define MAXLINE 4096
using namespace std;

extern int errno;

int main()
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT

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
            string body = "<html><body><h1>Content from server test_301! I'm cxxxxf </h1></body></html>\n";
            const char *http_res_hdr_tmpl =
                "HTTP/1.1 301 Moved Permanently\r\n"
                "Server: cxxxxf's Server test_301 <0.1>\r\n"
                "Accept-Ranges: bytes\r\n"
                "Content-Length: %d\r\n"
                "Connection: close\r\n"
                "Location: http://127.0.0.1:8001\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n"
                "custom_header: custom\r\n\r\n";
            char con[1024] = {0};
            sprintf(con, http_res_hdr_tmpl, body.size());
            send(connect_fd, con, strlen(con), 0);
            send(connect_fd, body.c_str(), body.size(), 0);
            close(connect_fd);
            exit(0);
        }
        close(connect_fd);
    }
    close(socket_fd);
}
