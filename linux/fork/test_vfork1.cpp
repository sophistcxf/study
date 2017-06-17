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
  if ((pid = vfork()) > 0) {
    i++;
    printf("I'm parent, pid is %d, i is %d\n", getpid(), i);
  } else {
    // 子进程先运行，走到调用_exit或exec，父进程才开始运行
    // 并且子进程共享了父进程的数据空间，会修改父进程空间的内容，所以子进程的i为11，父进程的i为12
    i++;
    printf("I'm child, pid is %d, i is %d\n", getpid(), i);
    // 如果子进程不调用_exit，为发生诡异的事情，见test_vfork2.cpp
    _exit(0);
  }
  return 0;
}
