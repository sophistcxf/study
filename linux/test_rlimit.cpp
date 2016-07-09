/*************************************************************************
  > File Name: test_rlimit.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat Jul  9 15:38:32 2016
 ************************************************************************/

/*
 * test getrlimit and setrlimit 
 * what's the difference between rlim_cur and rlim_max?
 */

#include <iostream>
#include <stdio.h>
using namespace std;

#include <sys/resource.h>
#include <errno.h>

static vector<string> RESOURCES;
/*
 * __RLIMIT_NLIMITS is defined in bits/resources.h
 */
static void initResources
{
  
}

void print_rlimit(int resource)
{
  struct rlimit fd_limit;
  if (getrlimit(resource, &fd_limit) < 0) {
    fprintf(stderr, "getrlimit(%d, &fd_limit) error: %d", errno);
    return -1;
  }
  printf("file descriptor, cur limit: %d, max limit: %d\n", fd_limit.rlim_cur, fd_limit.rlim_max); 
}

int main()
{
  return 0;
}
