/*************************************************************************
  > File Name: daytime_tcp_cli.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 09 Jul 2016 09:55:04 PM PDT
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1024

int main(int argc, char* argv[])
{
  if (argc != 2) return -1;
  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) return -1;
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(13);
  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) return -1;
  if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) return -1;
  int n = -1;
  char recvline[MAXLINE] = {0};
  while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;
    if (fputs(recvline, stdout) == EOF) return -1;
  }
  return 0;
}
