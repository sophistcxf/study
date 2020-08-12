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
#include <iostream>

pthread_mutex_t wait_mutex;

void* thread_cb1(void* param);
void* thread_cb2(void* param);

void output(const pthread_mutex_t& mutex)
{
    std::cout << "mutex.__data.__lock: " << mutex.__data.__lock << std::endl
              << "mutex.__data.__count: " << mutex.__data.__count << std::endl
              << "mutex.__data.__owner: " << mutex.__data.__owner << std::endl
              << "mutex.__data.__kind: " << mutex.__data.__kind << std::endl
              << std::endl;
}

int main()
{
    output(wait_mutex);
    pthread_t p2;
    pthread_create(&p2, NULL, thread_cb2, NULL);
    pthread_t p1;
    pthread_create(&p1, NULL, thread_cb1, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    return 0;
}

void* thread_cb1(void* param)
{
    pthread_mutex_lock(&wait_mutex);
    std::cout << "I'm thread_cb1" << std::endl;
    output(wait_mutex);
    pthread_mutex_unlock(&wait_mutex);
}

void* thread_cb2(void* param)
{
    pthread_mutex_lock(&wait_mutex);
    printf("I'm thread_cb2\n");
    output(wait_mutex);
    pthread_mutex_unlock(&wait_mutex);
}
