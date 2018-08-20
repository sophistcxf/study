/*************************************************************************
  > File Name: test_tls.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Mon 20 Aug 2018 04:55:06 PM CST
 ************************************************************************/

#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
using namespace std;

#include <pthread.h>


static pthread_mutex_t io_mutex;

void safe_printf(const char* format, ...)
{
    pthread_mutex_lock(&io_mutex);
    char buf[256] = {0};
    va_list vl;
    va_start(vl, format);
    vsnprintf(buf, 255, format, vl);
    va_end(vl);
    printf("%s", buf);
    pthread_mutex_unlock(&io_mutex);
}
    

class Object
{
public:
    Object(int i): index(i), value(i) 
    {
        safe_printf("Object %d construct!\n", index);
    }
    ~Object()
    {
        safe_printf("Object %d destruct!\n", index);
    }
    int index;
    int value;
};

class Test
{
public:
    virtual void test() = 0;
};

class Test1 : public Test
{
public:
    virtual void test()
    {
        static const int thread_num = 10;
        pthread_t thread_id[thread_num];
        for (size_t i = 0; i < thread_num; ++i)
        {
            Object* obj = new Object(i);
            pthread_create(&thread_id[i], NULL, Test1::thread_func, obj);
        }
        int thread_stat[thread_num];
        for (size_t i = 0; i < thread_num; ++i)
        {
            int status = pthread_join(thread_id[i], (void**)&thread_stat[i]);
            safe_printf("thread %d, join status %d, exit %d\n", i, status, thread_stat[i]);
        }
    }
private:
    static void* thread_func(void* param)
    {
        if (param == NULL)
        {
            safe_printf("param is null\n");
            return NULL;
        }
        pthread_once_t key_once = PTHREAD_ONCE_INIT;
        pthread_once(&key_once, Test1::make_key);
        void* ptr;
        if ((ptr = pthread_getspecific(Test1::key)) == NULL)
        {
            ptr = param;
            /* 不同的线程对同一个key会绑定不同的ptr */
            if (pthread_setspecific(Test1::key, ptr) != 0)
            {
                cerr << "pthread_setspecific failed" << std::endl;
            }
        }
        Object* obj = (Object*)pthread_getspecific(Test1::key);
        assert(obj == param);
        while (!test_status(obj))
        {
            safe_printf("thread %d, value %d\n", obj->index, obj->value);
            obj->value--;
            sleep(1);
        }
        return NULL;
    }
    static void make_key()
    {
        pthread_key_create(&Test1::key, Test1::destructor);
    }
    static bool test_status(const Object* obj)
    {
        return obj->value == 0;
    }
    static void destructor(void* param)
    {
        if (param == NULL)
            return;
        Object* obj = (Object*)param;
        delete obj; 
    }
private:
    static pthread_key_t key;
};

pthread_key_t Test1::key;

int main(int argc, char* argv[])
{
    pthread_mutex_init(&io_mutex, NULL);
    Test* t = new Test1();
    t->test();
    pthread_mutex_destroy(&io_mutex);
    return 0;
}
