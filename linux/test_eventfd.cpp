/*************************************************************************
  > File Name: test_eventfd.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Thu 14 Jul 2016 11:08:08 PM CST
 ************************************************************************/

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <sys/eventfd.h>
using namespace std;

int fd;

void* thread_cb(void*)
{
  sleep(5);
  uint64_t v = 1;
  write(fd, &v, sizeof(v));
}

int main()
{
  pthread_t pid;
  if (pthread_create(&pid, NULL, thread_cb, NULL) != 0) return -1;
  fd = eventfd(0, 0);
  uint64_t v;
  ssize_t rlt = read(fd, &v, sizeof(v));
  return 0;
}
