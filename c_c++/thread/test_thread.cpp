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

std::atomic<bool> ready (false);
std::mutex g_display_mutex;

void count1m(int id) {
  while (!ready) {             // wait until main() sets ready...
    std::this_thread::yield();
  }
  std::thread::id tid = std::this_thread::get_id();
  g_display_mutex.lock();
  std::cout << "tid " << tid << " begin" << std::endl;
  g_display_mutex.unlock();
  for (volatile int i=0; i<1000000; ++i) {}
  g_display_mutex.lock();
  std::cout << id << " ";
  g_display_mutex.unlock();
}

void test1() {
  std::thread threads[10];
  std::cout << "race of 10 threads that count to 1 million:\n";
  for (int i = 0; i < 10; ++i) {
    // 设置线程函数和参数
    threads[i] = std::thread(count1m,i);
  }
  // 所有的线程统一等ready=true后，同时开始执行
  ready = true;
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
