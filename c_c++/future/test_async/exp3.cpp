#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <unistd.h>
#include <chrono>
 
std::mutex m;
using namespace std::chrono_literals;
 
struct X
{
    int operator()(int i)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << std::this_thread::get_id() << ":";
        std::cout << i << '\n';
        std::this_thread::sleep_for(10s);
        return i + 10;
    }
};

void test1() {
    std::cout << "calling thread: " << std::this_thread::get_id() << std::endl;
    X x;
    auto a2 = std::async(std::launch::async, X(), 43);

    // a2.get会等待
    std::cout << a2.get() << '\n';
}

// 调用 wait_for
void test2() {
    std::cout << "calling thread: " << std::this_thread::get_id() << std::endl;
    X x;
    auto a2 = std::async(std::launch::async, X(), 43);
    std::future_status status = a2.wait_for(3s);
    if (status == std::future_status::ready) {
        std::cout << a2.get() << std::endl;
    }
    else if (status == std::future_status::timeout) {
        std::cout << "time out" << std::endl;
    }
}

int main()
{
    test2();
    return 0;
}