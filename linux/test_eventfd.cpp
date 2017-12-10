/*************************************************************************
  > File Name: test_eventfd.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Thu 14 Jul 2016 11:08:08 PM CST
 ************************************************************************/

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/eventfd.h>
using namespace std;

int fd;
extern int errno;

#define CHECK_RV(rv)			\
	if (rv == -1)				\
	{							\
		std::cerr << __FUNCTION__ << " failed, errno " << errno << std::endl;	\
		return;			\
	}

void* thread_cb1(void*)
{
	// 有sleep，read时大概率按0,1,2,3,...今次读取
	for (unsigned i = 0; i < 10; ++i)
	{
		uint64_t v = i;
		write(fd, &v, sizeof(v));
		sleep(1);
	}
	// 没有sleep，读取时有可能读到的时前几个数字的和
	for (unsigned i = 0; i < 10; ++i)
	{
		uint64_t v = i;
		write(fd, &v, sizeof(v));
	}
}

void test1()
{
  pthread_t pid;
  fd = eventfd(0, EFD_CLOEXEC);
  if (pthread_create(&pid, NULL, thread_cb1, NULL) != 0) return;
  while (true)
  {
	  uint64_t v;
	  ssize_t rlt = read(fd, &v, sizeof(v));
	  CHECK_RV(rlt);
	  std::cout << v << std::endl;
  }
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
  fd = eventfd(0, EFD_CLOEXEC);
  if (pthread_create(&pid, NULL, thread_cb1, NULL) != 0) return;
  char buf[256] = {0};
  ssize_t rlt = read(fd, buf, sizeof(buf));
  CHECK_RV(rlt);
  cout << buf << endl;
}

void test3()
{
	fd = eventfd(0, 0);
	char buf[2] = {0};
	// buf大小小于8，errno设为EINVAL
	ssize_t rlt = write(fd, buf, sizeof(buf));
	CHECK_RV(rlt);
}

int main()
{
  test1();
  return 0;
}
