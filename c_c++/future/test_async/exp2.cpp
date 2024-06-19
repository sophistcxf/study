#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
 
std::mutex m;
 
struct X
{
    void bar(const std::string& str)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << std::this_thread::get_id() << ":";
        std::cout << str << '\n';
    }
 
    int operator()(int i)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << std::this_thread::get_id() << ":";
        std::cout << i << '\n';
        return i + 10;
    }
};

int main()
{
    X x;
    auto a1 = std::async(std::launch::deferred, &X::bar, x, "world!");
    a1.wait();
    a1.wait();
    return 0;
}