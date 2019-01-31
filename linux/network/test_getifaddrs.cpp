/*************************************************************************
  > File Name: get_getifaddrs.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: 四  1/31 12:15:11 2019
 ************************************************************************/

#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netpacket/packet.h>

void output_sockaddr(const struct sockaddr* addr)
{

}

void output_ifaddr(const struct ifaddrs* ifa)
{
    struct sockaddr* ifa_addr = ifa->ifa_addr;
    struct sockaddr* ifa_netmask = ifa->ifa_netmask;

    output_sockaddr(ifa_addr);
    output_sockaddr(ifa_netmask);

    /* sa_family should be consulted to determine the format of the address structure */
    int family = ifa->ifa_addr->sa_family;

    /* Display interface name and family (including symbolic
       form of the latter for the common families) */
    printf("%s  address family: %d%s\n",
            ifa->ifa_name, family,
            (family == AF_PACKET) ? " (AF_PACKET)" :
            (family == AF_INET) ?   " (AF_INET)" :
            (family == AF_INET6) ?  " (AF_INET6)" : "");

    /* For an AF_INET* interface address, display the address */

    if (family == AF_INET || family == AF_INET6) {
        char host[NI_MAXHOST];
        int s = getnameinfo(ifa->ifa_addr,
                (family == AF_INET) ? sizeof(struct sockaddr_in) :
                sizeof(struct sockaddr_in6),
                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if (s != 0) {
            printf("getnameinfo() failed: %s\n", gai_strerror(s));
            exit(EXIT_FAILURE);
        }
        printf("\taddress: <%s>\n", host);
    }
}

int main(int argc, char *argv[])
{
    struct ifaddrs *ifaddr;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /* Walk through linked list, maintaining head pointer so we
       can free list later */

    for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        output_ifaddr(ifa);
    }

    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}
