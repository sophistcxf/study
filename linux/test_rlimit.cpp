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
#include <map>
using namespace std;

#include <sys/resource.h>
#include <errno.h>

static map<int, string> RESOURCES;
/*
 * __RLIMIT_NLIMITS is defined in bits/resources.h
 */
static bool init_resources()
{
  RESOURCES.insert(make_pair(0, "RLIMIT_CPU"));
  RESOURCES.insert(make_pair(1, "RLIMIT_FSIZE"));
  RESOURCES.insert(make_pair(2, "RLIMIT_DATA"));
  RESOURCES.insert(make_pair(3, "RLIMIT_STACK"));
  RESOURCES.insert(make_pair(4, "RLIMIT_CORE"));
  RESOURCES.insert(make_pair(5, "RLIMIT_RSS"));
  RESOURCES.insert(make_pair(6,"RLIMIT_NPROC"));
  RESOURCES.insert(make_pair(7, "RLIMIT_NOFILE"));
  RESOURCES.insert(make_pair(8, "RLIMIT_MEMLOCK"));
  RESOURCES.insert(make_pair(9, "RLIMIT_AS"));
  RESOURCES.insert(make_pair(10, "RLIMIT_SIGPENDING"));
  RESOURCES.insert(make_pair(11, "RLIMIT_MSGQUEUE"));
  RESOURCES.insert(make_pair(13, "RLIMIT_NICE"));
  RESOURCES.insert(make_pair(14, "RLIMIT_RTPRIO"));
}

static bool isInit = init_resources();

void print_rlimit(int resource)
{
  if (resource >= RESOURCES.size()) {
    fprintf(stderr, "no resource %d\n", resource);
    return;
  }
  struct rlimit fd_limit;
  if (getrlimit(resource, &fd_limit) < 0) {
    fprintf(stderr, "getrlimit(%s, &fd_limit) error: %d", RESOURCES[resource].c_str(), errno);
    return;
  }
  printf("Resource %s, cur limit: %d, max limit: %d\n", RESOURCES[resource].c_str(), fd_limit.rlim_cur, fd_limit.rlim_max); 
}

int main()
{
  for (map<int, string>::iterator iter = RESOURCES.begin(); iter != RESOURCES.end(); ++iter)
    print_rlimit(iter->first);
  return 0;
}
