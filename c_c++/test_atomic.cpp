/*************************************************************************
  > File Name: test_atomic.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Tue 14 Aug 2018 11:23:14 AM CST
 ************************************************************************/

// constructing atomics
#include <iostream>       // std::cout
#include <atomic>         // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <thread>         // std::thread, std::this_thread::yield
#include <vector>         // std::vector

std::atomic<bool> ready (false);
std::atomic_flag winner = ATOMIC_FLAG_INIT;

void count1m (int id) {
    while (!ready)
    { 
        std::this_thread::yield();
    }      // wait for the ready signal
    for (volatile int i=0; i<1000000; ++i) 
    {}          // go!, count to 1 million
    if (!winner.test_and_set()) 
    { 
        std::cout << "thread #" << id << " won!\n"; 
    }
};

int main ()
{
    std::vector<std::thread> threads;
    std::cout << "spawning 10 threads that count to 1 million...\n";
    for (int i=1; i<=10; ++i)
        threads.push_back(std::thread(count1m,i));
    ready = true;
    for (auto& th : threads)
        th.join();

    return 0;
}
