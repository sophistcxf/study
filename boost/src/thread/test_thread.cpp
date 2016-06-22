/*************************************************************************
  > File Name: test_thread.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 21 Jun 2016 03:11:55 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/thread.hpp>
#include <iostream>

void thread()
{
  for (int i = 0; i < 5; ++i)
  {
    sleep(1);
    std::cout << i << '\n';
  }
}

int main()
{
  boost::thread t(thread);
  t.join();
}
