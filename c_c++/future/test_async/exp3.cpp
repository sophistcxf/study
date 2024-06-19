#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <unistd.h>
 
std::mutex m;
 
struct X
{
    int operator()(int i)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << std::this_thread::get_id() << ":";
        std::cout << i << '\n';
        sleep(10);
        return i + 10;
    }
};

int main()
{
    std::cout << "calling thread: " << std::this_thread::get_id() << std::endl;
    X x;
    auto a2 = std::async(std::launch::async, X(), 43);

    // a2.get会等待
    std::cout << a2.get() << '\n';

    return 0;
}