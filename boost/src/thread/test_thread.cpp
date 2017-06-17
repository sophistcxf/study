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

void test1();
void test2();

void thread()
{
  for (int i = 0; i < 5; ++i)
  {
    sleep(1);
    std::cout << i << '\n';
  }
}

void thread_cb_2(int i, float* f)
{
  cout << i << "\t" << *f << endl;
}

int main()
{
  test2();
}

void test1()
{
  boost::thread t(thread);
  sleep(10);
}

void test2()
{
  float* f = new float;
  *f = 10.0;
  boost::thread t(thread_cb_2, 0, f);
  t.join();
}
