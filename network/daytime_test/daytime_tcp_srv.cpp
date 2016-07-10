/*************************************************************************
  > File Name: daytime_tcp_srv.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 09 Jul 2016 09:36:43 PM PDT
 ************************************************************************/

#include <iostream>
#include <time.h>
#include <string.h>
#include <stdio.h>
using namespace std;

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define MAX_LISTENQ 8

int main(int argc, char* argv[])
{
  struct sockaddr_in servaddr, cliaddr;
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(13);
  bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
  listen(listenfd, MAX_LISTENQ);
  while (true) {
    char buff[MAXLINE] = {0};
    socklen_t len = sizeof(cliaddr);
    int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
    //printf("connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
    time_t ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    write(connfd, buff, strlen(buff));
    close(connfd);
  }
  return 0;
}
