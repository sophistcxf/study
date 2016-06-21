/*************************************************************************
  > File Name: test.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 21 Jun 2016 02:09:01 PM CST
 ************************************************************************/

#include <iostream>
#include <stdio.h>
using namespace std;
#include <pthread.h>

void* cb(void* param)
{
  int idx = 0;
  while(true) {
    cout << "cb: " << idx++ << endl;
    sleep(1);
  }
}

int main()
{
  pthread_t pid;
  int rlt = pthread_create(&pid, NULL, cb, NULL);
  if (rlt != 0) {
    cerr << "pthread_create failed" << endl;
    return -1;
  }
  pthread_detach(pid);
  cout << "start cb" << endl;
  pthread_exit(0); // thread will continue to run and process becomes zombie
  sleep(10);
  return 0;
}
