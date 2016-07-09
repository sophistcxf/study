/*************************************************************************
  > File Name: test_rlimit.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat Jul  9 15:38:32 2016
 ************************************************************************/

/*
 * test getrlimit and setrlimit 
 */

#include <iostream>
#include <stdio.h>
using namespace std;

#include <sys/resource.h>

extern int errno;

int main()
{
  struct rlimit fd_limit;
  if (getrlimit(RLIMIT_NOFILE, &fd_limit) < 0) {
    fprintf(stderr, "getrlimit(RLIMIT_NOFILE, &fd_limit) error: %d", errno);
    return -1;
  }
  printf("file descriptor, cur limit: %d, max limit: %d\n", fd_limit.rlim_cur, fd_limit.rlim_max); 
  return 0;
}
