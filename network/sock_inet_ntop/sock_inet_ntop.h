/*************************************************************************
  > File Name: /home/sophistcxf/workspace/github/ThirdLibTest/network/get_ifi_info/sock_inet_ntop.h
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 17 Jul 2016 06:02:39 AM PDT
 ************************************************************************/

#include <iostream>
using namespace std;

#include <netinet/in.h>

char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
