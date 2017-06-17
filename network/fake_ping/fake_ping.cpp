/*************************************************************************
  > File Name: fake_ping.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 09 Jul 2016 08:54:43 PM PDT
 ************************************************************************/

/*!
 *
 */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>      // for addrinfo
#include <arpa/inet.h>
#include <stdio.h>
using namespace std;

#include "ping.h"
#include "error.h"

extern int verbose;   // declared in ping.cpp
extern char* host;    // declared in ping.cpp
pid_t pid;
extern int datalen;
extern struct proto* pr;

struct proto proto_v4 = {proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP};
#ifdef IPV6
struct proto proto_v6 = {proc_v6, send_v6, init_v6, NULL, NULL, 0, IPPROTO_ICMP};
#endif

int main(int argc, char* argv[])
{
  int c;
  while (( c = getopt(argc, argv, "v")) != -1) {
    switch(c) {
    case 'v':
      verbose++; break;
    case '?':
      err_quit("unrecognized option %c", c);      
    }
  }
  if (optind != argc - 1)
    err_quit("usage: ping [ -v ] <hostname>");
  host = argv[optind];
  // ICMP ID fields is 16 bits, 65535
  pid = getpid() & 0xffff;
  signal(SIGALRM, sig_alrm);
  struct addrinfo ai;
  struct addrinfo *res;
  memset(&ai, 0, sizeof(struct addrinfo));
  ai.ai_flags = AI_CANONNAME;
  ai.ai_family = AF_INET;
  ai.ai_socktype = SOCK_STREAM;
  int n;
  if ( (n = getaddrinfo(host, NULL, &ai, &res)) != 0)
    err_quit("host_serv error for %s, %s: %s",
        (host == NULL) ? "(no hostname)" : host,
        "(no service name)",
        gai_strerror(n));
  char str[128] = {0};
  struct sockaddr_in* sin = (struct sockaddr_in*)res->ai_addr; 
  inet_ntop(sin->sin_family, &sin->sin_addr, str, sizeof(str));
  printf("PING %s (%s): %d data bytes\n",
         res->ai_canonname ? res->ai_canonname : str, str, datalen);

  if (res->ai_family == AF_INET) {
    pr = &proto_v4;
  }
#ifdef IPV6
  else if (res->ai_family == AF_INET6) {
  }
#endif
  else err_quit("unknown address family %d", res->ai_family);
  pr->sasend = res->ai_addr;
  pr->sarecv = (struct sockaddr*)calloc(1, res->ai_addrlen);
  if (pr->sarecv == NULL) err_sys("calloc error");
  pr->salen = res->ai_addrlen;
  readloop();
  return 0;
}
