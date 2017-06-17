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
    // 当子进程没有调用_exit或exec时，退出作用域时父进程的栈可能被破坏了，
    // 父进程从栈上拿到的返回值就可能是0，也可能不是0了，所以又重复了子进程的过程
    // 是这么解释吗？
    i++;
    printf("I'm child, pid is %d, i is %d\n", getpid(), i);
  }
  return 0;
}
