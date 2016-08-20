/*************************************************************************
  > File Name: tset_fork1.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年08月20日 星期六 11时47分23秒
 ************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
using namespace std;

int main()
{
  pid_t pid;
  int i = 10;
  if ((pid = fork()) > 0) {
    i++;
    printf("I'm parent, pid is %d, i is %d\n", getpid(), i);
  } else {
    i++;
    printf("I'm child, pid is %d, i is %d\n", getpid(), i);
  }
  return 0;
}
