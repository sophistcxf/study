/*************************************************************************
  > File Name: test_inet_aton.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: 一  1/28 19:18:24 2019
 ************************************************************************/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /* in_addr 只有一个成员变量，unsigned long s_addr */
    struct in_addr addr;

    if (argc != 2) {
        fprintf(stderr, "%s <dotted-address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (inet_aton(argv[1], &addr) == 0) {
        perror("inet_aton");
        exit(EXIT_FAILURE);
    }

    printf("%d\n", addr.s_addr);
    printf("%s\n", inet_ntoa(addr));
    exit(EXIT_SUCCESS);
}
