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
    std::cout << "calling thread: " << std::this_thread::get_id() << std::endl;
    X x;
    auto a1 = std::async(std::launch::deferred, &X::bar, x, "world!");

    // 使用 std::launch::async，在另一个线程中执行
    auto a2 = std::async(std::launch::async, X(), 43);
    std::cout << a2.get() << '\n';
    
    // 当使用 std::launch::deferred，调用 a1.wait()，才会执行
    a1.wait();
    return 0;
}