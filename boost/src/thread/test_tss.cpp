/*************************************************************************
  > File Name: test_tts.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: 二  8/14 14:28:55 2018
 ************************************************************************/

#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <cassert>
#include <iostream>

boost::mutex mutex;

class C
{
public:
    C(int i) : v(0), index(i)
    {
        boost::mutex::scoped_lock scoped_lock(mutex);
        std::cout << "C construct " << index << std::endl;
    }
    ~C()
    {
        boost::mutex::scoped_lock scoped_lock(mutex);
        std::cout << "C destruct " << index << std::endl;
    }
    int v;
    int index;
};

boost::thread_specific_ptr<C> value;
boost::thread_specific_ptr<time_t> tss_time;

void thread_proc(int index)
{
    if (value.get() == NULL)
    {
        value.reset(new C(index)); // initialize the thread's storage
    }
    //delete value.get(); 不能显式的 delete value，线程退出时会释放
    for (int i = 0; i < 10; ++i)
        value.get()->v++;
    boost::mutex::scoped_lock scoped_lock(mutex);
    std::cout << "thread " << index << " cancelled, value is " << value.get()->v << std::endl;
}

int main(int argc, char* argv[])
{
    boost::thread_group threads;
    for (int i = 0; i < 5; ++i)
        threads.create_thread(boost::bind(&thread_proc, i));
    threads.join_all();
}
