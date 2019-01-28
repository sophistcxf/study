#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include <string>
#include <sstream>
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include <unistd.h>
#include<netinet/in.h>  
#define DEFAULT_PORT 8000  
#define MAXLINE 4096  
using namespace std;

int main()
{
    int    socket_fd, connect_fd;  
    struct sockaddr_in     servaddr;  
    char    buff[4096];  
    int     n;  
    //初始化Socket  
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }  
    //初始化  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。  
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT  

    //将本地地址绑定到所创建的套接字上  
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){  
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }  
    //开始监听是否有客户端连接  
    if( listen(socket_fd, 10) == -1){  
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }  
    printf("======waiting for client's request======\n");  
    while(1){  
        //阻塞直到有客户端连接，不然多浪费CPU资源。  
        if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){  
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);  
            continue;  
        }  
        //接受客户端传过来的数据  
        n = recv(connect_fd, buff, MAXLINE, 0);  
        //向客户端发送回应数据  
        if(!fork()){ /*紫禁城*/  
            string body = "<html><body><h1>Content from server! I'm cxxxxf </h1></body></html>\n";
            char header[255] = {0};
            const char *http_res_hdr_tmpl = "HTTP/1.1 302 Moved Temporarily\r\nServer: cxxxxf's Server <0.1>\r\n"
    "Accept-Ranges: bytes\r\nContent-Length: %d\r\nConnection: close\r\n"
    "Location: http://www.baidu.com\r\n"
    "Content-Type: text/html; charset=UTF-8\r\ncustom_header: custom\r\n\r\n";
            char con[1024] = {0};
            sprintf(con, http_res_hdr_tmpl, body.size());
            send(connect_fd, con, strlen(con), 0);
            send(connect_fd, body.c_str(), body.size(), 0); 
            close(connect_fd);  
            exit(0);  
        }  
        buff[n] = '\0';  
        printf("recv msg from client: %s\n", buff);  
        close(connect_fd);  
    }  
    close(socket_fd);   
}
