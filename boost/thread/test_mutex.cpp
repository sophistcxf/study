/*************************************************************************
  > File Name: test_mutex.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: 2016年10月20日 星期四 21时41分03秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

void wait(int seconds)
{
  boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

boost::mutex mutex;

void thread()
{
  using boost::this_thread::get_id;
  for (int i = 0; i < 5; ++i)
  {
    wait(1);
    mutex.lock();
    std::cout << "Thread " << get_id() << ": " << i << std::endl;
    mutex.unlock();
  }
}

int main()
{
  boost::thread t1(thread);
  boost::thread t2(thread);
  t1.join();
  t2.join();
}
