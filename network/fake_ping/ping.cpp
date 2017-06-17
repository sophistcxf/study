/*************************************************************************
  > File Name: ping.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sun 10 Jul 2016 12:22:18 AM PDT
 ************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <arpa/inet.h>  // for inet_ntop
#include <stdio.h>
using namespace std;

#include "ping.h"
#include "error.h"

			/* globals */
char	 sendbuf[BUFSIZE];

int		 datalen = 56;  	/* # bytes of data following ICMP header */
char	*host;
int		 nsent;				/* add 1 for each sendto() */
extern pid_t	 pid;				/* our PID */
int		 sockfd;
int		 verbose;
struct proto* pr;

void readloop()
{
  // 创建原始套接字，并指定协议为ICMP
  // 在SOCK_RAW下，是否还有别的协议？
  sockfd = socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
  if (sockfd < 0) {
    err_quit("create socket error, family=%d, type=%d, protocol=%d", pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
  }
  setuid(getuid()); // don't need special permissions any more
  if (pr->finit)
    (*(pr->finit))();

  int size = 60 * 1024;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)); 
  sig_alrm(SIGALRM);

  char recvbuf[BUFSIZE] = {0};
  struct iovec iov;
  iov.iov_base = recvbuf; 
  iov.iov_len = sizeof(recvbuf);
  
  char controlbuf[BUFSIZE] = {0};
  struct msghdr msg;
  msg.msg_name = pr->sarecv;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = controlbuf;
  while (true) {
    msg.msg_namelen = pr->salen;
    msg.msg_controllen = sizeof(controlbuf);
    int n = recvmsg(sockfd, &msg, 0);
    if (n < 0) {
      if (errno == EINTR) continue;
      else err_sys("recvmsg error");
    }
    struct timeval tval;
    gettimeofday(&tval, NULL);
    (*pr->fproc)(recvbuf, n, &msg, &tval);
  }
}

/*
 * 自行解析从数据链路层收到的ip报文
 */
void proc_v4(char* ptr, ssize_t len, struct msghdr* msg, struct timeval* tvrecv)
{
  struct ip* ip = (struct ip*)ptr;
  // head length * 4
  int hlen1 = ip->ip_hl << 2;
  // protocol is not icmp
  if (ip->ip_p != IPPROTO_ICMP) return;
  // icmp point to start of icmp header
  struct icmp* icmp = (struct icmp*)(ptr + hlen1);
  // if icmp's length < 8
  int icmplen;
  if ((icmplen = len - hlen1) < 8) return;
  assert(pr->sarecv->sa_family == AF_INET);
  struct sockaddr_in* sin = (struct sockaddr_in*)pr->sarecv;
  char str[128] = {0};
  if (inet_ntop(sin->sin_family, &sin->sin_addr, str, sizeof(str)) == NULL)
    err_sys("inet_ntop error");
  if (icmp->icmp_type == ICMP_ECHOREPLY) {
    if (icmp->icmp_id != pid) return;   // not a response to our ECHO_REQUEST
    if (icmplen < 16) return;           // not enough data to use
    struct timeval* tvsend = (struct timeval*)icmp->icmp_data;
    tv_sub(tvrecv, tvsend);
    double rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
    printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3fms\n",
           icmplen, str, icmp->icmp_seq, ip->ip_ttl, rtt); 
  } else if (verbose) {
    printf("  %d bytes from %s: type = %d, code = %d\n",
      icmplen, str, icmp->icmp_type, icmp->icmp_code);
  }
}

void sig_alrm(int signo)
{
  (*pr->fsend)();
  alarm(1);
  return;
}

void tv_sub(struct timeval* out, struct timeval* in)
{
  if ((out->tv_usec -= in->tv_usec) < 0) {
    --out->tv_sec;
    out->tv_usec += (1 << 6);
  }
  out->tv_sec -= in->tv_sec;
}

void init_v6()
{
}

void send_v4()
{
  struct icmp* icmp;
  icmp = (struct icmp*)sendbuf;
  icmp->icmp_type = ICMP_ECHO;
  icmp->icmp_code = 0;
  icmp->icmp_id = pid;
  icmp->icmp_seq = nsent++;
  memset(icmp->icmp_data, 0xa5, datalen);
  gettimeofday((struct timeval*)icmp->icmp_data, NULL);
  int len = 8 + datalen;
  icmp->icmp_cksum = 0;
  icmp->icmp_cksum = in_cksum((unsigned short*)icmp, len);
  if (sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen) < 0)
    err_sys("sendto failed, errno: %d", errno);
}

unsigned short in_cksum(unsigned short* addr, int len)
{
  unsigned short* w = addr;
  unsigned short answer = 0;
  unsigned int sum = 0;
  int nleft = len;
  while (nleft > 1) {
    sum += *w++;
    nleft -= 2;
  }
  if (nleft == 1) {
    *(unsigned char*)(&answer) = *(unsigned char*)w;
    sum += answer;
  }

  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  answer = ~sum;
  return answer;
}
