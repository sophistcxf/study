/*************************************************************************
  > File Name: test___thread.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 14 Aug 2018 10:23:06 AM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;
const int i=5;
__thread int var=i;//两种方式效果一样

void* worker1(void* arg);
void* worker2(void* arg);
int main(){
    pthread_t pid1,pid2;
    pthread_create(&pid1,NULL,worker1,NULL);
    pthread_create(&pid2,NULL,worker2,NULL);
    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);
    return 0;
}
void* worker1(void* arg){
    cout<<++var<<endl;//输出 6
    return NULL;
}

void* worker2(void* arg){
    sleep(1);//等待线程1改变var值，验证是否影响线程2
    cout<<++var<<endl;//也输出6
    return NULL;
}

