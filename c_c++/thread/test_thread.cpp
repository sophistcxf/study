 /*************************************************************************
  > File Name: test_thread_yield.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: 二  8/14 11:34:36 2018
 ************************************************************************/

// this_thread::yield example
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::yield
#include <atomic>         // std::atomic

void count1m(int id) {
    thread_local volatile std::thread::id tid = std::this_thread::get_id();
    std::cout << tid << std::endl;
    for (int i = 0; i < 1000*1000; ++i) {
    }
}

void test1() {
  std::thread threads[10];
  std::cout << "race of 10 threads that count to 1 million:\n";
  for (int i = 0; i < 10; ++i) {
    // 设置线程函数和参数
    threads[i] = std::thread(count1m,i);
  }
  // 所有的线程统一等ready=true后，同时开始执行
  for (auto& th : threads) {
    th.join();
  }
  std::cout << '\n';
}

int main ()
{
  test1();
  return 0;
}
