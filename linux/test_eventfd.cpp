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

void* thread_cb1(void*)
{
  sleep(5);
  uint64_t v = 1;
  write(fd, &v, sizeof(v));
}

void test1()
{
  pthread_t pid;
  if (pthread_create(&pid, NULL, thread_cb1, NULL) != 0) return;
  fd = eventfd(0, 0);
  uint64_t v;
  ssize_t rlt = read(fd, &v, sizeof(v));
}


void* thread_cb2(void*)
{
  char buf[256] = "hello world";
  write(fd, buf, sizeof(buf)); 
}

/*
 * 不能代替pipe来通信
 */
void test2()
{
  pthread_t pid;
  if (pthread_create(&pid, NULL, thread_cb1, NULL) != 0) return;
  fd = eventfd(0, 0);
  char buf[256] = {0};
  ssize_t rlt = read(fd, buf, sizeof(buf));
  cout << buf << endl;
}

int main()
{
  test2();
  return 0;
}
