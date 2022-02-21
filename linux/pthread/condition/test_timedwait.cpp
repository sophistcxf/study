/*************************************************************************
  > File Name: test_condition.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年10月25日 星期二 14时46分30秒
 ************************************************************************/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

pthread_cond_t cond_handle;
pthread_mutex_t wait_mutex;

void* thread_cb1(void* param);
void* thread_cb2(void* param);

int main()
{
  pthread_t p2;
  pthread_create(&p2, NULL, thread_cb2, NULL);
  pthread_t p1;
  pthread_create(&p1, NULL, thread_cb1, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  return 0;
}

/**
 * 这种用法有问题，因为对wait_mutex lock了，所以thread_cb2即使到了
 * 3秒，也不会退出pthread_cond_timedwait，因为无法获得wait_mutex
 */
void* thread_cb1(void* param)
{
  int num = 0;
  sleep(1);
  pthread_mutex_lock(&wait_mutex);
  printf("thread_1 lock\n");
  sleep(5);
  printf("I'm thread_cb1, %d\n", num++);
  pthread_cond_signal(&cond_handle);
  pthread_mutex_unlock(&wait_mutex);
}

void* thread_cb2(void* param)
{
  pthread_mutex_lock(&wait_mutex);
  struct timespec ts;
  memset(&ts, 0, sizeof(ts));
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 3;
  int rlt = pthread_cond_timedwait(&cond_handle, &wait_mutex, &ts);
  if (rlt == 0) {
      printf("I'm thread_cb2, signaled\n");
  }
  else if (rlt == ETIMEDOUT) {
      printf("I'm thread_cb2, timeout\n");
  }
  else {
      printf("I'm thread_cb2, errno %d\n", rlt);
  }
  pthread_mutex_unlock(&wait_mutex);
}
